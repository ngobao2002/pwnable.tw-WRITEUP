#!/usr/bin/env python3
import argparse
import json
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

import grpc
from google.protobuf import descriptor_pool
from google.protobuf import json_format
from google.protobuf import message_factory


def extract_file_descriptor_proto(jar_path: Path) -> bytes:
    helper_source = """
import java.util.Base64;
public class DumpRobotDescriptor {
  public static void main(String[] args) throws Exception {
    var fd = ctf.hitcon.robot.aG.a();
    var bytes = fd.toProto().toByteArray();
    System.out.print(Base64.getEncoder().encodeToString(bytes));
  }
}
""".strip()

    with tempfile.TemporaryDirectory() as tmpdir:
        src = Path(tmpdir) / "DumpRobotDescriptor.java"
        src.write_text(helper_source)
        cmd = [
            "java",
            "--class-path",
            str(jar_path),
            str(src),
        ]
        proc = subprocess.run(cmd, check=True, capture_output=True, text=True)
    return __import__("base64").b64decode(proc.stdout.strip())


def build_dynamic_types(fd_bytes: bytes):
    pool = descriptor_pool.DescriptorPool()
    pool.AddSerializedFile(fd_bytes)

    def cls(name: str):
        return message_factory.GetMessageClass(pool.FindMessageTypeByName(name))

    enums = {
        "robot.Type": pool.FindEnumTypeByName("robot.Type"),
        "robot.Ord": pool.FindEnumTypeByName("robot.Ord"),
    }
    return pool, cls, enums


def enum_number(enum_desc, value):
    if isinstance(value, int):
        return value
    return enum_desc.values_by_name[value].number


def fill_message(msg, data, enums):
    for field_name, value in data.items():
        field = msg.DESCRIPTOR.fields_by_name[field_name]
        if field.label == field.LABEL_REPEATED:
            container = getattr(msg, field_name)
            if field.cpp_type == field.CPPTYPE_MESSAGE:
                for item in value:
                    sub = container.add()
                    fill_message(sub, item, enums)
            else:
                if field.enum_type is not None:
                    for item in value:
                        container.append(enum_number(field.enum_type, item))
                else:
                    container.extend(value)
            continue

        if field.cpp_type == field.CPPTYPE_MESSAGE:
            sub = getattr(msg, field_name)
            if isinstance(value, dict) and not value:
                sub.SetInParent()
                continue
            fill_message(sub, value, enums)
            continue

        if field.enum_type is not None:
            setattr(msg, field_name, enum_number(field.enum_type, value))
            continue

        setattr(msg, field_name, value)


def make_code_request(cls, enums, token: str, instructions):
    req = cls("robot.CommandRequest")()
    code = cls("robot.Code")()
    code.token = token

    for item in instructions:
        if len(item) != 1:
            raise ValueError(f"instruction must contain exactly one opcode: {item}")
        opcode_name, opcode_body = next(iter(item.items()))
        inst = code.instructions.add()
        sub = getattr(inst, opcode_name)
        if isinstance(opcode_body, dict) and not opcode_body:
            sub.SetInParent()
            continue
        fill_message(sub, opcode_body or {}, enums)

    req.code.CopyFrom(code)
    return req


def make_message_request(cls, text: str):
    req = cls("robot.CommandRequest")()
    req.message = text
    return req


def send_boxed_with_java_opts(jar_path: Path, requests, java_opts):
    proc = subprocess.Popen(
        ["java", *java_opts, "-jar", str(jar_path), "--box"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    assert proc.stdin is not None
    assert proc.stdout is not None

    try:
        for req in requests:
            payload = req.SerializeToString()
            proc.stdin.write(struct.pack(">I", len(payload)))
            proc.stdin.write(payload)
            proc.stdin.flush()

            header = proc.stdout.read(4)
            if len(header) != 4:
                raise RuntimeError("boxed process closed before response header")
            (size,) = struct.unpack(">I", header)
            body = proc.stdout.read(size)
            if len(body) != size:
                raise RuntimeError("boxed process returned truncated response")

            yield body.decode()
    finally:
        proc.kill()
        proc.wait(timeout=2)


def send_grpc(target: str, requests, response_cls):
    with grpc.insecure_channel(target) as channel:
        rpc = channel.stream_stream(
            "/robot.Robot/Send",
            request_serializer=lambda m: m.SerializeToString(),
            response_deserializer=lambda b: response_cls.FromString(b),
        )
        yield from rpc(iter(requests))


def load_instructions(path: Path):
    data = json.loads(path.read_text())
    if not isinstance(data, list):
        raise ValueError("instruction JSON must be a list")
    return data


def main():
    parser = argparse.ArgumentParser(
        description="Drive the HITCON robot VM flow using dynamic protobuf types from robot.jar."
    )
    parser.add_argument("--jar", default="robot.jar", help="path to robot.jar")
    parser.add_argument("--mode", choices=["box", "grpc"], default="box")
    parser.add_argument("--target", default="127.0.0.1:50051", help="gRPC target")
    parser.add_argument(
        "--java-opt",
        action="append",
        default=[],
        help="extra option passed to the local java process in --mode box",
    )
    parser.add_argument("--token", default="", help="Code.token value")
    parser.add_argument(
        "--instructions",
        type=Path,
        required=True,
        help="JSON file containing a list of Instruction oneof objects",
    )
    parser.add_argument(
        "--chat",
        default="ping",
        help="message to send after uploading code, to trigger MyRobot.chat()",
    )
    parser.add_argument(
        "--dump-json",
        action="store_true",
        help="print protobuf responses as JSON",
    )
    args = parser.parse_args()

    jar_path = Path(args.jar).resolve()
    fd_bytes = extract_file_descriptor_proto(jar_path)
    _, cls, enums = build_dynamic_types(fd_bytes)

    instructions = load_instructions(args.instructions)
    update_req = make_code_request(cls, enums, args.token, instructions)
    chat_req = make_message_request(cls, args.chat)
    response_cls = cls("robot.CommandResponse")

    if args.mode == "box":
        responses = send_boxed_with_java_opts(jar_path, [update_req, chat_req], args.java_opt)
    else:
        responses = send_grpc(args.target, [update_req, chat_req], response_cls)

    for idx, resp in enumerate(responses, 1):
        if args.mode == "box":
            print(f"[response {idx}] {resp}")
            continue
        if args.dump_json:
            print(f"[response {idx}] {json_format.MessageToJson(resp).strip()}")
        else:
            print(f"[response {idx}] {resp.response}")


if __name__ == "__main__":
    main()
