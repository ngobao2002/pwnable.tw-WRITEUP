#!/usr/bin/env python3
import struct
import subprocess
import sys
from pathlib import Path

from vm_flow import (
    build_dynamic_types,
    extract_file_descriptor_proto,
    load_instructions,
    make_code_request,
    make_message_request,
)


def main():
    jar = Path(sys.argv[1]).resolve() if len(sys.argv) > 1 else Path("robot.jar").resolve()
    instructions = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("sample_exploit2.json")
    chat = sys.argv[3] if len(sys.argv) > 3 else "x"

    _, cls, enums = build_dynamic_types(extract_file_descriptor_proto(jar))
    reqs = [
        make_code_request(cls, enums, "", load_instructions(instructions)),
        make_message_request(cls, chat),
    ]

    proc = subprocess.Popen(
        ["java", "-XX:-UsePerfData", "-jar", str(jar), "--box"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    assert proc.stdin is not None
    assert proc.stdout is not None

    for idx, req in enumerate(reqs, 1):
        payload = req.SerializeToString()
        proc.stdin.write(struct.pack(">I", len(payload)))
        proc.stdin.write(payload)
        proc.stdin.flush()

        header = proc.stdout.read(4)
        if len(header) != 4:
            print(f"no response header for request {idx}")
            break
        (size,) = struct.unpack(">I", header)
        body = proc.stdout.read(size)
        print(f"response {idx}: {body.decode('utf-8', 'replace')}")

    proc.stdin.close()
    proc.wait(timeout=2)
    stderr = proc.stderr.read().decode("utf-8", "replace")
    if stderr:
        print("stderr>>>")
        print(stderr)

    if proc.poll() is None:
        proc.kill()
        proc.wait(timeout=2)


if __name__ == "__main__":
    main()
