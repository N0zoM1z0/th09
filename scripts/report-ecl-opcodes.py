#!/usr/bin/env python3
"""Decode the TH09 v1.50a RunEcl switch tables after target attestation."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
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
CONTROL_OPCODE_MIN = 1
CONTROL_OPCODE_MAX = 53
EXPECTED_CONTROL_DEFAULT_OPCODES = (3,)
ROOT = Path(__file__).resolve().parents[1]
OPCODE_HEADER = ROOT / "src" / "EclOpcodes.hpp"
CONTROL_SOURCE = ROOT / "src" / "EclRunControl.inl"
ENUM_ENTRY_RE = re.compile(
    r"^\s*(TH09_ECL_OPCODE_[A-Z0-9_]+)\s*=\s*(\d+),?\s*$",
    re.MULTILINE,
)
CASE_LABEL_RE = re.compile(
    r"^\s*case\s+(TH09_ECL_OPCODE_[A-Z0-9_]+)\s*:",
    re.MULTILINE,
)


def decode_table(image: bytes, address: int, count: int) -> tuple[bytes, tuple[int, ...]]:
    data = pe_bytes_at(image, address, count * 4)
    entries = struct.unpack(f"<{count}I", data)
    return data, entries


def audit_control_source() -> dict[str, object]:
    header_text = OPCODE_HEADER.read_text(encoding="utf-8")
    source_text = CONTROL_SOURCE.read_text(encoding="utf-8")
    enum_match = re.search(
        r"enum\s+Th09EclOpcode\s*\{(.*?)\};", header_text, re.DOTALL
    )
    if enum_match is None:
        raise ValueError("missing Th09EclOpcode enum")
    enum_values = {
        name: int(value)
        for name, value in ENUM_ENTRY_RE.findall(enum_match.group(1))
    }
    family_entries = {
        name: value
        for name, value in enum_values.items()
        if CONTROL_OPCODE_MIN <= value <= CONTROL_OPCODE_MAX
    }
    observed_values = sorted(family_entries.values())
    expected_values = list(range(CONTROL_OPCODE_MIN, CONTROL_OPCODE_MAX + 1))
    if observed_values != expected_values:
        raise ValueError(
            "control enum does not cover every opcode 1-53 exactly once"
        )

    case_labels = CASE_LABEL_RE.findall(source_text)
    duplicate_labels = sorted(
        name for name in set(case_labels) if case_labels.count(name) != 1
    )
    missing_labels = sorted(set(family_entries) - set(case_labels))
    out_of_family_labels = sorted(
        name
        for name in case_labels
        if name not in family_entries
    )
    if duplicate_labels or missing_labels or out_of_family_labels:
        problems = []
        if duplicate_labels:
            problems.append("duplicate cases " + ",".join(duplicate_labels))
        if missing_labels:
            problems.append("missing cases " + ",".join(missing_labels))
        if out_of_family_labels:
            problems.append(
                "out-of-family cases " + ",".join(out_of_family_labels)
            )
        raise ValueError("control source coverage: " + "; ".join(problems))

    return {
        "source": str(CONTROL_SOURCE.relative_to(ROOT)),
        "opcode_min": CONTROL_OPCODE_MIN,
        "opcode_max": CONTROL_OPCODE_MAX,
        "case_count": len(case_labels),
    }


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
        control_source = audit_control_source()
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
    control_default_opcodes = tuple(
        opcode
        for opcode, destination in enumerate(
            opcode_entries[:CONTROL_OPCODE_MAX], start=CONTROL_OPCODE_MIN
        )
        if destination == DEFAULT_HANDLER
    )
    if control_default_opcodes != EXPECTED_CONTROL_DEFAULT_OPCODES:
        table_problems.append(
            "control default slots "
            + ",".join(f"{value:02X}" for value in control_default_opcodes)
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
        "control_family": {
            **control_source,
            "active_count": CONTROL_OPCODE_MAX - len(control_default_opcodes),
            "default_opcodes": [
                f"0x{value:02X}" for value in control_default_opcodes
            ],
            "destinations": [
                f"0x{value:08X}"
                for value in opcode_entries[:CONTROL_OPCODE_MAX]
            ],
            "claim": "complete lexical family coverage; RunEcl source/exactness remain open",
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
        print(
            "control family: opcodes 1-53, "
            f"{CONTROL_OPCODE_MAX - len(control_default_opcodes)} active, "
            f"{control_source['case_count']} source cases"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
