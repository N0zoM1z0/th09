#!/usr/bin/env python3
"""Apply reproducible origin reviews within the TH09 game-code linker band."""
from __future__ import annotations

import argparse
import csv
import importlib.util
import json
from pathlib import Path
import struct
import sys


ROOT = Path(__file__).resolve().parents[1]
FUNCTIONS = ROOT / "config" / "functions.csv"
ORIGINS = ROOT / "config" / "function-origins.csv"
COMPILER_EVIDENCE_ID = "vc71-generated-helper-review-2026-09-19"
VECTOR_CONSTRUCTOR = "0x00401470"
SCALAR_DELETING_DESTRUCTORS = {
    "0x0042CC30",
    "0x0042CC60",
    "0x00431680",
    "0x00440C20",
    "0x00440C50",
}
COMPILER_GENERATED = {VECTOR_CONSTRUCTOR, *SCALAR_DELETING_DESTRUCTORS}


def load_script(filename: str, module_name: str):
    path = ROOT / "scripts" / filename
    spec = importlib.util.spec_from_file_location(module_name, path)
    if spec is None or spec.loader is None:
        raise ValueError(f"cannot load {path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def read_rows(path: Path) -> list[dict[str, str]]:
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream))


def rel32_target(address: int, body: bytes, displacement_offset: int) -> int:
    displacement = struct.unpack_from("<i", body, displacement_offset)[0]
    return address + displacement_offset + 4 + displacement


def verify_compiler_bodies() -> None:
    tail = load_script("review-static-init-tail.py", "th09_game_origin_target")
    tail.verify_target_tail()
    data = tail.TARGET.read_bytes()
    _, sections = tail.parse_image(data)

    vector_body = tail.read_va(data, sections, int(VECTOR_CONSTRUCTOR, 0), 38)
    expected_vector = bytes.fromhex(
        "558bec8b4510487819538b5d14568b7508578d78018bceffd303750c4f75f65f5e5b5dc21000"
    )
    if vector_body != expected_vector:
        raise ValueError("VC7.1 vector-constructor helper body changed")

    fixed_template = bytearray.fromhex(
        "558bec568bf1e800000000f6450801740956e80000000083c4048bc65e5dc20400"
    )
    for address_text in SCALAR_DELETING_DESTRUCTORS:
        address = int(address_text, 0)
        body = bytearray(tail.read_va(data, sections, address, 33))
        first_target = rel32_target(address, body, 7)
        second_target = rel32_target(address, body, 19)
        body[7:11] = b"\0" * 4
        body[19:23] = b"\0" * 4
        if body != fixed_template:
            raise ValueError(f"deleting-destructor shape changed at {address_text}")
        if not (0x00401000 <= first_target < 0x0044E000):
            raise ValueError(f"ordinary destructor leaves game band at {address_text}")
        if second_target != 0x0047B249:
            raise ValueError(f"deleting destructor no longer calls j__free at {address_text}")


def review_compiler(write: bool) -> dict[str, object]:
    verify_compiler_bodies()
    ledger_update = load_script(
        "apply-runtime-origin-review.py", "th09_game_origin_ledger_update"
    )
    function_rows = {row["address"]: row for row in read_rows(FUNCTIONS)}
    origin_rows = {row["address"]: row for row in read_rows(ORIGINS)}
    pending: set[str] = set()
    already_applied: set[str] = set()
    for address in COMPILER_GENERATED:
        function = function_rows[address]
        origin = origin_rows[address]
        if function["status"] == "unclassified" and origin["origin"] == "unknown":
            pending.add(address)
        elif (
            function["status"] == "excluded"
            and function["module"] == "Compiler"
            and function["owner"] == "compiler_generated"
            and origin["origin"] == "compiler_generated"
            and origin["subsystem"] == "Compiler"
            and origin["disposition"] == "exclude"
            and origin["evidence_id"] == COMPILER_EVIDENCE_ID
        ):
            already_applied.add(address)
        else:
            raise ValueError(f"unexpected compiler-helper ledger state at {address}")

    def mutate_origin(row: dict[str, str]) -> None:
        row["origin"] = "compiler_generated"
        row["subsystem"] = "Compiler"
        row["disposition"] = "exclude"
        row["confidence"] = "high"
        row["evidence_id"] = COMPILER_EVIDENCE_ID

    def mutate_function(row: dict[str, str]) -> None:
        row["module"] = "Compiler"
        row["status"] = "excluded"
        row["match_percent"] = "0.00"
        row["is_thunk"] = "false"
        row["owner"] = "compiler_generated"
        if row["address"] == VECTOR_CONSTRUCTOR:
            row["evidence"] = (
                "Target body and many repository VC7.1 relocations identify the "
                "four-argument ??_H vector-constructor iterator helper."
            )
            row["notes"] = (
                "Compiler-emitted array construction helper; excluded from the "
                "authored denominator without exactness credit."
            )
        else:
            row["evidence"] = (
                "Complete 33-byte extent matches the reviewed VC7.1 scalar deleting-"
                "destructor template after masking two rel32 fields; calls a game "
                "destructor then exact j__free."
            )
            row["notes"] = (
                "Compiler-generated scalar deleting destructor; excluded from the "
                "authored denominator without exactness credit."
            )

    origin_count = ledger_update.rewrite_selected_rows(
        ORIGINS, pending, mutate_origin, write
    )
    function_count = ledger_update.rewrite_selected_rows(
        FUNCTIONS, pending, mutate_function, write
    )
    if origin_count != function_count or origin_count != len(pending):
        raise ValueError("incomplete compiler-helper ledger update")
    return {
        "write": write,
        "updated_candidates": function_count,
        "already_applied": len(already_applied),
        "vector_constructor_helpers": 1,
        "scalar_deleting_destructors": len(SCALAR_DELETING_DESTRUCTORS),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--apply", action="store_true", help="update reviewed ledgers")
    parser.add_argument("--group", choices=("compiler",), default="compiler")
    args = parser.parse_args()
    print(json.dumps(review_compiler(args.apply), indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, struct.error) as exc:
        print(f"error: game-origin review failed: {exc}", file=sys.stderr)
        raise SystemExit(1)
