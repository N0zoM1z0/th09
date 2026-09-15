#!/usr/bin/env python3
"""Decode the TH09 v1.50a RunEcl switch tables after target attestation."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import struct
import sys

from target_identity import pe_bytes_at, resolve_target, verify_target


EASING_TABLE_ADDRESS = 0x0040C088
EASING_TABLE_COUNT = 6
EASING_TABLE_SHA256 = "c78820f6521e722fe6ddb17af45bebd6b88ad76de63bbd0be5a00c4b1077b3f6"
OPCODE_TABLE_ADDRESS = 0x0040C0A0
OPCODE_TABLE_COUNT = 187
OPCODE_TABLE_SHA256 = "516ce17e4f4e2e2fad8ecb6b96e375fd7fc5c64f558fe911b5794af67ccb8de9"
DEFAULT_HANDLER = 0x0040B410
EXPECTED_DEFAULT_OPCODES = (
    0x03,
    0x53,
    0x54,
    0x55,
    0x5A,
    0x5B,
    0x5C,
    0x7A,
    0x7B,
    0x8D,
    0x8E,
    0x9E,
    0xA4,
    0xA8,
    0xAE,
    0xB0,
    0xB3,
    0xB4,
    0xB5,
    0xB8,
)


def decode_table(image: bytes, address: int, count: int) -> tuple[bytes, tuple[int, ...]]:
    data = pe_bytes_at(image, address, count * 4)
    entries = struct.unpack(f"<{count}I", data)
    return data, entries


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("executable", nargs="?", type=Path)
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()

    target = resolve_target(args.executable)
    if not target.is_file():
        print(f"missing target: {target}", file=sys.stderr)
        return 1

    try:
        observed, problems = verify_target(target)
        if problems:
            raise ValueError("; ".join(problems))
        image = target.read_bytes()
        easing_data, easing_entries = decode_table(
            image, EASING_TABLE_ADDRESS, EASING_TABLE_COUNT
        )
        opcode_data, opcode_entries = decode_table(
            image, OPCODE_TABLE_ADDRESS, OPCODE_TABLE_COUNT
        )
    except (OSError, KeyError, TypeError, ValueError, struct.error) as exc:
        print(f"invalid target or ECL tables: {exc}", file=sys.stderr)
        return 2

    easing_hash = hashlib.sha256(easing_data).hexdigest()
    opcode_hash = hashlib.sha256(opcode_data).hexdigest()
    default_opcodes = tuple(
        opcode
        for opcode, destination in enumerate(opcode_entries, start=1)
        if destination == DEFAULT_HANDLER
    )
    table_problems = []
    if easing_hash != EASING_TABLE_SHA256:
        table_problems.append(f"easing table hash {easing_hash}")
    if opcode_hash != OPCODE_TABLE_SHA256:
        table_problems.append(f"opcode table hash {opcode_hash}")
    if default_opcodes != EXPECTED_DEFAULT_OPCODES:
        table_problems.append(
            "default slots " + ",".join(f"{value:02X}" for value in default_opcodes)
        )
    if table_problems:
        print("ECL table audit mismatch: " + "; ".join(table_problems), file=sys.stderr)
        return 3

    report = {
        "target": str(target),
        "target_sha256": observed["sha256"],
        "easing_table": {
            "address": f"0x{EASING_TABLE_ADDRESS:08X}",
            "count": len(easing_entries),
            "sha256": easing_hash,
            "destinations": [f"0x{value:08X}" for value in easing_entries],
        },
        "opcode_table": {
            "address": f"0x{OPCODE_TABLE_ADDRESS:08X}",
            "count": len(opcode_entries),
            "sha256": opcode_hash,
            "active_count": len(opcode_entries) - len(default_opcodes),
            "default_destination": f"0x{DEFAULT_HANDLER:08X}",
            "default_opcodes": [f"0x{value:02X}" for value in default_opcodes],
            "destinations": [f"0x{value:08X}" for value in opcode_entries],
        },
    }

    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print(f"target OK: {target}")
        print(
            f"easing table: 0x{EASING_TABLE_ADDRESS:08X}, "
            f"{len(easing_entries)} entries, sha256 {easing_hash}"
        )
        print(
            f"opcode table: 0x{OPCODE_TABLE_ADDRESS:08X}, "
            f"{len(opcode_entries)} entries, "
            f"{len(opcode_entries) - len(default_opcodes)} active"
        )
        print(
            "default slots: "
            + " ".join(f"{value:02X}" for value in default_opcodes)
            + f" -> 0x{DEFAULT_HANDLER:08X}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
