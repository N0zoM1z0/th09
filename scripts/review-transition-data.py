#!/usr/bin/env python3
"""Remove four IDA pseudo-functions embedded in TH09 transition records."""
from __future__ import annotations

import argparse
import csv
import importlib.util
import json
from pathlib import Path
import sys


ROOT = Path(__file__).resolve().parents[1]
FUNCTIONS = ROOT / "config" / "functions.csv"
ORIGINS = ROOT / "config" / "function-origins.csv"
EVIDENCE_ID = "transition-record-data-review-2026-09-19"
RECORDS = {
    "0x0044E1C4": (0x0044E1C0, "10534900c20000000cc2008000000000"),
    "0x0044E1D4": (0x0044E1D0, "10534900c30000000cc3008000000000"),
    "0x0044E244": (0x0044E240, "10534900ca0000000cca008000000000"),
    "0x0044E254": (0x0044E250, "10534900cb0000000ccb008000000000"),
}


def load_script(filename: str, module_name: str):
    path = ROOT / "scripts" / filename
    spec = importlib.util.spec_from_file_location(module_name, path)
    if spec is None or spec.loader is None:
        raise ValueError(f"cannot load {path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def read_rows(path: Path) -> dict[str, dict[str, str]]:
    with path.open(newline="", encoding="utf-8") as stream:
        return {row["address"]: row for row in csv.DictReader(stream)}


def review(write: bool) -> dict[str, object]:
    target = load_script("review-static-init-tail.py", "th09_transition_data_target")
    target.verify_target_tail()
    data = target.TARGET.read_bytes()
    _, sections = target.parse_image(data)
    for address, (record_address, expected_hex) in RECORDS.items():
        actual = target.read_va(data, sections, record_address, 16)
        if actual != bytes.fromhex(expected_hex):
            raise ValueError(f"transition record changed around {address}")

    function_rows = read_rows(FUNCTIONS)
    origin_rows = read_rows(ORIGINS)
    pending: set[str] = set()
    already_applied: set[str] = set()
    for address in RECORDS:
        function = function_rows[address]
        origin = origin_rows[address]
        if function["status"] == "unclassified" and origin["origin"] == "unknown":
            pending.add(address)
        elif (
            function["status"] == "excluded"
            and function["module"] == "Data"
            and function["owner"] == "data"
            and origin["origin"] == "data"
            and origin["subsystem"] == "Data"
            and origin["disposition"] == "exclude"
            and origin["evidence_id"] == EVIDENCE_ID
        ):
            already_applied.add(address)
        else:
            raise ValueError(f"unexpected transition-record state at {address}")

    updater = load_script(
        "apply-runtime-origin-review.py", "th09_transition_data_ledger_update"
    )

    def mutate_origin(row: dict[str, str]) -> None:
        row["origin"] = "data"
        row["subsystem"] = "Data"
        row["disposition"] = "exclude"
        row["confidence"] = "high"
        row["evidence_id"] = EVIDENCE_ID

    def mutate_function(row: dict[str, str]) -> None:
        row["module"] = "Data"
        row["status"] = "excluded"
        row["match_percent"] = "0.00"
        row["is_thunk"] = "false"
        row["owner"] = "data"
        row["evidence"] = (
            "Target bytes place the alleged entry at offset four of a complete "
            "16-byte transition record; the record index byte decodes accidentally "
            "as an x86 ret/retf opcode."
        )
        row["notes"] = (
            "IDA pseudo-function inside data, not a code boundary; excluded from "
            "the authored denominator without source or exactness credit."
        )

    origin_count = updater.rewrite_selected_rows(
        ORIGINS, pending, mutate_origin, write
    )
    function_count = updater.rewrite_selected_rows(
        FUNCTIONS, pending, mutate_function, write
    )
    if origin_count != function_count or origin_count != len(pending):
        raise ValueError("incomplete transition-data ledger update")
    return {
        "write": write,
        "updated_candidates": function_count,
        "already_applied": len(already_applied),
        "reviewed_records": len(RECORDS),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--apply", action="store_true", help="update reviewed ledgers")
    args = parser.parse_args()
    print(json.dumps(review(args.apply), indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError) as exc:
        print(f"error: transition-data review failed: {exc}", file=sys.stderr)
        raise SystemExit(1)
