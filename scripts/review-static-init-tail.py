#!/usr/bin/env python3
"""Review and apply the target-derived VC7.1 global-init tail inventory."""
from __future__ import annotations

import argparse
import csv
import hashlib
import io
import json
from pathlib import Path
import struct
import sys
from typing import Callable


ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "resources" / "th09.exe"
FUNCTIONS = ROOT / "config" / "functions.csv"
ORIGINS = ROOT / "config" / "function-origins.csv"
EXPECTED_SHA256 = "10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822"
EVIDENCE_ID = "vc71-global-init-tail-review-2026-09-19"
ATEXIT = 0x0047C323

INITIALIZERS = {
    0x0048D7B0: 10,
    0x0048D7BA: 22,
    0x0048D7D0: 22,
    0x0048D7F0: 10,
    0x0048D800: 23,
    0x0048D820: 10,
    0x0048D830: 10,
    0x0048D840: 22,
    0x0048D860: 22,
    0x0048D880: 23,
    0x0048D8A0: 22,
    0x0048D8C0: 22,
    0x0048D8E0: 10,
    0x0048D8F0: 10,
    0x0048D900: 23,
    0x0048D920: 36,
    0x0048D950: 10,
    0x0048D960: 22,
    0x0048D980: 22,
    0x0048D9A0: 22,
    0x0048D9C0: 20,
    0x0048D9E0: 20,
    0x0048DA00: 20,
    0x0048DA20: 10,
}
TERMINATORS = {
    0x0048DA30: 10,
    0x0048DA3A: 10,
    0x0048DA50: 10,
    0x0048DA60: 10,
    0x0048DA70: 1,
    0x0048DA80: 10,
    0x0048DA90: 20,
    0x0048DAB0: 10,
    0x0048DAC0: 10,
    0x0048DAD0: 10,
}
REGISTRATIONS = {
    0x0048D7BA: 0x0048DA30,
    0x0048D7D0: 0x0048DA3A,
    0x0048D840: 0x0048DA80,
    0x0048D860: 0x0048DA60,
    0x0048D8A0: 0x0048DA50,
    0x0048D8C0: 0x0048DA70,
    0x0048D920: 0x0048DA90,
    0x0048D960: 0x0048DAB0,
    0x0048D980: 0x0048DAC0,
    0x0048D9A0: 0x0048DAD0,
}


def canonical(address: int) -> str:
    return f"0x{address:08X}"


def parse_image(data: bytes) -> tuple[int, dict[str, tuple[int, int, int, int]]]:
    if data[:2] != b"MZ":
        raise ValueError("target is not an MZ image")
    pe_offset = struct.unpack_from("<I", data, 0x3C)[0]
    if data[pe_offset : pe_offset + 4] != b"PE\0\0":
        raise ValueError("target has no PE signature")
    machine, section_count, _, _, _, optional_size = struct.unpack_from(
        "<HHIIIH", data, pe_offset + 4
    )
    if machine != 0x14C:
        raise ValueError(f"expected i386 PE, got machine 0x{machine:04X}")
    optional = pe_offset + 24
    if struct.unpack_from("<H", data, optional)[0] != 0x10B:
        raise ValueError("expected PE32 optional header")
    image_base = struct.unpack_from("<I", data, optional + 28)[0]
    sections: dict[str, tuple[int, int, int, int]] = {}
    table = optional + optional_size
    for index in range(section_count):
        offset = table + index * 40
        name = data[offset : offset + 8].split(b"\0", 1)[0].decode("ascii")
        virtual_size, virtual_address, raw_size, raw_offset = struct.unpack_from(
            "<IIII", data, offset + 8
        )
        sections[name] = (
            image_base + virtual_address,
            virtual_size,
            raw_size,
            raw_offset,
        )
    return image_base, sections


def read_va(
    data: bytes,
    sections: dict[str, tuple[int, int, int, int]],
    address: int,
    size: int,
) -> bytes:
    for start, _virtual_size, raw_size, raw_offset in sections.values():
        if start <= address and address + size <= start + raw_size:
            offset = raw_offset + address - start
            return data[offset : offset + size]
    raise ValueError(f"VA range is not file-backed: {canonical(address)}+{size}")


def relative_call_target(address: int, instruction: bytes) -> int:
    if len(instruction) != 5 or instruction[0] != 0xE8:
        raise ValueError(f"expected rel32 call at {canonical(address)}")
    displacement = struct.unpack_from("<i", instruction, 1)[0]
    return address + 5 + displacement


def verify_target_tail() -> None:
    data = TARGET.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    if digest != EXPECTED_SHA256:
        raise ValueError(f"wrong target SHA-256: {digest}")
    _, sections = parse_image(data)
    if sections.get(".text", (0, 0, 0, 0))[:2] != (0x00401000, 0x0008CADA):
        raise ValueError("unexpected .text extent")

    table_values = [0, 0x00488291, *INITIALIZERS, 0, 0]
    table = read_va(data, sections, 0x004A0000, len(table_values) * 4)
    actual_values = list(struct.unpack(f"<{len(table_values)}I", table))
    if actual_values != table_values:
        raise ValueError("global initializer pointer table changed")

    ordered = [*INITIALIZERS.items(), *TERMINATORS.items()]
    for index, (address, size) in enumerate(ordered):
        body = read_va(data, sections, address, size)
        if size == 10:
            if body[0] != 0xB9 or body[5] != 0xE9:
                raise ValueError(f"unexpected tail wrapper at {canonical(address)}")
        elif address == 0x0048DA70:
            if body != b"\xC3":
                raise ValueError("null terminator wrapper changed")
        elif body[-1] != 0xC3:
            raise ValueError(f"wrapper has no terminal ret at {canonical(address)}")

        end = address + size
        next_address = ordered[index + 1][0] if index + 1 < len(ordered) else 0x0048DADA
        padding = read_va(data, sections, end, next_address - end)
        if padding != b"\xCC" * len(padding):
            raise ValueError(f"non-CC boundary gap after {canonical(address)}")

    for initializer, terminator in REGISTRATIONS.items():
        body = read_va(data, sections, initializer, INITIALIZERS[initializer])
        marker = b"\x68" + struct.pack("<I", terminator)
        offset = body.find(marker)
        if offset < 0 or body.find(marker, offset + 1) >= 0:
            raise ValueError(f"missing unique terminator registration in {canonical(initializer)}")
        call_address = initializer + offset + len(marker)
        call = body[offset + len(marker) : offset + len(marker) + 5]
        if relative_call_target(call_address, call) != ATEXIT:
            raise ValueError(f"terminator does not register through _atexit at {canonical(initializer)}")


def read_rows(path: Path) -> list[dict[str, str]]:
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream))


def render_row(fields: list[str], row: dict[str, str]) -> str:
    output = io.StringIO(newline="")
    csv.DictWriter(output, fieldnames=fields, lineterminator="\n").writerow(row)
    return output.getvalue()


def rewrite_rows(
    path: Path,
    additions: dict[str, dict[str, str]],
    updates: set[str],
    mutate: Callable[[dict[str, str]], None],
    write: bool,
) -> tuple[int, int]:
    lines = path.read_text(encoding="utf-8").splitlines(keepends=True)
    fields = next(csv.reader([lines[0]]))
    remaining = dict(additions)
    output = [lines[0]]
    updated = 0
    inserted = 0
    for line in lines[1:]:
        values = next(csv.reader([line]))
        address = values[0]
        for new_address in sorted(
            (candidate for candidate in remaining if int(candidate, 0) < int(address, 0)),
            key=lambda candidate: int(candidate, 0),
        ):
            output.append(render_row(fields, remaining.pop(new_address)))
            inserted += 1
        if address not in updates:
            output.append(line)
            continue
        if len(values) != len(fields):
            raise ValueError(f"selected row has malformed field count: {address}")
        row = dict(zip(fields, values))
        mutate(row)
        output.append(render_row(fields, row))
        updated += 1
    for new_address in sorted(remaining, key=lambda candidate: int(candidate, 0)):
        output.append(render_row(fields, remaining[new_address]))
        inserted += 1
    if write:
        temporary = path.with_name(path.name + ".tmp")
        temporary.write_text("".join(output), encoding="utf-8")
        temporary.replace(path)
    return inserted, updated


def review(write: bool) -> dict[str, object]:
    verify_target_tail()
    extents = INITIALIZERS | TERMINATORS
    selected = {canonical(address) for address in extents}
    function_rows = {row["address"]: row for row in read_rows(FUNCTIONS)}
    origin_rows = {row["address"]: row for row in read_rows(ORIGINS)}
    if set(function_rows) != set(origin_rows):
        raise ValueError("function and origin ledgers disagree before update")

    missing = selected - set(function_rows)
    existing = selected & set(function_rows)
    updates: set[str] = set()
    already_applied: set[str] = set()
    for address in existing:
        function = function_rows[address]
        origin = origin_rows[address]
        if function["status"] == "unclassified" and origin["origin"] == "unknown":
            updates.add(address)
        elif (
            function["status"] == "excluded"
            and function["module"] == "Compiler"
            and function["owner"] == "compiler_generated"
            and origin["origin"] == "compiler_generated"
            and origin["subsystem"] == "Compiler"
            and origin["disposition"] == "exclude"
            and origin["evidence_id"] == EVIDENCE_ID
        ):
            already_applied.add(address)
        else:
            raise ValueError(f"unexpected existing ledger state at {address}")

    def kind(address: str) -> str:
        return "initializer" if int(address, 0) in INITIALIZERS else "terminator"

    function_additions: dict[str, dict[str, str]] = {}
    origin_additions: dict[str, dict[str, str]] = {}
    function_fields = next(csv.reader([FUNCTIONS.read_text(encoding="utf-8").splitlines()[0]]))
    origin_fields = next(csv.reader([ORIGINS.read_text(encoding="utf-8").splitlines()[0]]))
    for address in missing:
        numeric = int(address, 0)
        size = extents[numeric]
        function = {field: "" for field in function_fields}
        function.update(
            {
                "address": address,
                "size": str(size),
                "span_end": canonical(numeric + size - 1),
                "module": "Compiler",
                "status": "excluded",
                "match_percent": "0.00",
                "is_thunk": "false",
                "evidence": (
                    f"Target-derived VC7.1 global {kind(address)} wrapper; exact "
                    "extent proven by the initializer table, control terminator, "
                    "and CC boundary padding."
                ),
                "owner": "compiler_generated",
                "notes": (
                    "Absent from the initial IDA function inventory; added from "
                    "the hash-attested target's complete global-init tail review."
                ),
            }
        )
        function_additions[address] = function
        origin = {field: "" for field in origin_fields}
        origin.update(
            {
                "address": address,
                "origin": "compiler_generated",
                "subsystem": "Compiler",
                "disposition": "exclude",
                "confidence": "high",
                "evidence_id": EVIDENCE_ID,
            }
        )
        origin_additions[address] = origin

    def mutate_function(row: dict[str, str]) -> None:
        row["module"] = "Compiler"
        row["status"] = "excluded"
        row["match_percent"] = "0.00"
        row["is_thunk"] = "false"
        row["evidence"] = (
            f"Target-derived VC7.1 global {kind(row['address'])} wrapper; exact "
            "extent proven by the initializer table, control terminator, and CC "
            "boundary padding."
        )
        row["owner"] = "compiler_generated"
        row["notes"] = (
            "Compiler-synthesized global object wrapper; excluded from the authored "
            "denominator without granting source-presence or exactness credit."
        )

    def mutate_origin(row: dict[str, str]) -> None:
        row["origin"] = "compiler_generated"
        row["subsystem"] = "Compiler"
        row["disposition"] = "exclude"
        row["confidence"] = "high"
        row["evidence_id"] = EVIDENCE_ID

    function_counts = rewrite_rows(
        FUNCTIONS, function_additions, updates, mutate_function, write
    )
    origin_counts = rewrite_rows(ORIGINS, origin_additions, updates, mutate_origin, write)
    if function_counts != origin_counts:
        raise ValueError("function/origin update counts disagree")
    return {
        "write": write,
        "initializers": len(INITIALIZERS),
        "terminators": len(TERMINATORS),
        "inserted_candidates": function_counts[0],
        "updated_candidates": function_counts[1],
        "already_applied": len(already_applied),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--apply", action="store_true", help="atomically update ledgers")
    args = parser.parse_args()
    print(json.dumps(review(args.apply), indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, struct.error) as exc:
        print(f"error: static-init tail review failed: {exc}", file=sys.stderr)
        raise SystemExit(1)
