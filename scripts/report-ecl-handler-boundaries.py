#!/usr/bin/env python3
"""Diagnostic RunEcl opcode-handler entry comparison against verified TH09.

This is structural evidence only.  It does not grant exactness or partial byte
credit to RunEcl.
"""

import argparse
import importlib.util
import json
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
RUN_ECL_SYMBOL = "?RunEcl@EclManager@@QAEHPAUEnemyView@@@Z"
RUN_ECL_TARGET = 0x004086C0
OPCODE_TABLE_TARGET = 0x0040C0A0
OPCODE_COUNT = 187
EASING_TABLE_COUNT = 6
COMBINED_TABLE_COUNT = EASING_TABLE_COUNT + OPCODE_COUNT


def load_compare_module():
    path = ROOT / "scripts" / "compare-coff-function.py"
    spec = importlib.util.spec_from_file_location("coff_compare", path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def contiguous_dir32_runs(relocations):
    runs = []
    current = []
    for row in relocations:
        offset = int(row["offset"])
        if (
            row["type"] == "DIR32"
            and (not current or offset == int(current[-1]["offset"]) + 4)
        ):
            current.append(row)
            continue
        if current:
            runs.append(current)
        current = [row] if row["type"] == "DIR32" else []
    if current:
        runs.append(current)
    return runs


def candidate_opcode_entries(compare, object_path):
    _, relocations = compare.object_function(
        object_path, RUN_ECL_SYMBOL, include_symbol_locations=True
    )
    runs = [
        run for run in contiguous_dir32_runs(relocations)
        if len(run) == COMBINED_TABLE_COUNT
    ]
    if len(runs) != 1:
        sizes = sorted(len(run) for run in contiguous_dir32_runs(relocations))
        raise RuntimeError(
            "expected one 193-entry RunEcl compiler table; "
            f"found {len(runs)} candidates, DIR32 run sizes={sizes}"
        )

    table = runs[0]
    owner_value = int(table[0]["owner_value"])
    result = []
    for row in table[EASING_TABLE_COUNT:]:
        result.append(
            int(row["symbol_value"]) - owner_value + int(row["addend"])
        )
    if len(result) != OPCODE_COUNT:
        raise RuntimeError(f"expected {OPCODE_COUNT} opcode entries")
    return result


def target_opcode_entries(compare):
    raw = compare.pe_bytes_at(
        compare.verified_target(), OPCODE_TABLE_TARGET, OPCODE_COUNT * 4
    )
    return [
        address - RUN_ECL_TARGET
        for address in struct.unpack(f"<{OPCODE_COUNT}I", raw)
    ]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "object",
        nargs="?",
        type=Path,
        default=ROOT / "build" / "matching" / "EclManager.obj",
    )
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()

    compare = load_compare_module()
    candidate = candidate_opcode_entries(compare, args.object)
    target = target_opcode_entries(compare)

    rows = []
    for opcode, (candidate_offset, target_offset) in enumerate(
        zip(candidate, target), start=1
    ):
        if candidate_offset != target_offset:
            rows.append({
                "opcode": opcode,
                "candidate_offset": candidate_offset,
                "target_offset": target_offset,
                "delta": candidate_offset - target_offset,
            })

    payload = {
        "diagnostic_only": True,
        "exactness_credit": "none",
        "object": str(args.object),
        "opcode_entries": OPCODE_COUNT,
        "matching_entries": OPCODE_COUNT - len(rows),
        "mismatching_entries": len(rows),
        "mismatches": rows,
    }

    if args.json:
        print(json.dumps(payload, indent=2))
        return

    print(
        "RunEcl opcode entry offsets: "
        f"{payload['matching_entries']}/{OPCODE_COUNT} match target"
    )
    if not rows:
        print("all handler entries align (whole-owner exactness still unproven)")
        return

    print("opcode  candidate  target     delta")
    for row in rows:
        print(
            f"{row['opcode']:>6}  "
            f"0x{row['candidate_offset']:04X}    "
            f"0x{row['target_offset']:04X}    "
            f"{row['delta']:+d}"
        )


if __name__ == "__main__":
    main()
