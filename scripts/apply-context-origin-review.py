#!/usr/bin/env python3
"""Apply target-context reviews for short runtime-shaped TH09 candidates."""
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
EVIDENCE_ID = "target-context-runtime-origin-review-2026-09-19"

D3DX8_CONTEXT = {
    "0x0044E823", "0x0044EB2D", "0x0044EBB7", "0x0044F126",
    "0x0044F36F", "0x0044F632", "0x0044FAF5", "0x0044FB91",
    "0x0044FC2E", "0x0044FDDF", "0x0045049E", "0x0045058A",
    "0x00450640", "0x004506FA", "0x0045077E", "0x00450843",
    "0x00450A85", "0x00451F68", "0x00453C49", "0x00458AA2",
    "0x00458EBA", "0x0045922D", "0x0045DBE1", "0x0045DC36",
    "0x0045E430", "0x0045EEB3", "0x0046310C", "0x0046C8F2",
}
CRT_CONTEXT = {"0x0047B249", "0x0047B7EA", "0x004889B8"}


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


def review(write: bool) -> dict[str, object]:
    runtime_audit = load_script(
        "audit-runtime-origins.py", "th09_context_runtime_origin_audit"
    )
    ledger_update = load_script(
        "apply-runtime-origin-review.py", "th09_context_ledger_update"
    )
    report = runtime_audit.audit(5)
    weak = {
        str(row["address_text"]): row
        for row in report["findings"]
        if row["evidence"] == "pinned_coff_shape_in_library_region"
    }
    selected = D3DX8_CONTEXT | CRT_CONTEXT
    function_rows = read_rows(FUNCTIONS)
    indexes = {row["address"]: index for index, row in enumerate(function_rows)}
    origin_rows = {row["address"]: row for row in read_rows(ORIGINS)}
    pending = selected & set(weak)
    already_applied = selected - pending
    for address in already_applied:
        function = function_rows[indexes[address]]
        origin = origin_rows[address]
        expected = "D3DX8" if address in D3DX8_CONTEXT else "CRT"
        if not (
            function["status"] == "excluded"
            and function["module"] == expected
            and function["owner"] == "library"
            and origin["origin"] == "library"
            and origin["subsystem"] == expected
            and origin["disposition"] == "exclude"
            and origin["evidence_id"] == EVIDENCE_ID
        ):
            raise ValueError(f"{address} is neither pending nor already reviewed")

    for address in pending:
        index = indexes[address]
        row = function_rows[index]
        if row["status"] != "unclassified":
            raise ValueError(f"{address} is no longer pending: {row['status']}")
        previous = function_rows[index - 1]
        following = function_rows[index + 1]
        expected = "D3DX8" if address in D3DX8_CONTEXT else "CRT"
        neighbors = (previous, following)
        if any(neighbor["status"] != "excluded" for neighbor in neighbors):
            raise ValueError(f"{address} no longer lies between reviewed exclusions")
        if not any(neighbor["module"] == expected for neighbor in neighbors):
            raise ValueError(f"{address} lost its reviewed {expected} linker context")

    def subsystem(address: str) -> str:
        return "D3DX8" if address in D3DX8_CONTEXT else "CRT"

    def mutate_origin(row: dict[str, str]) -> None:
        if row["origin"] != "unknown" or row["disposition"] != "review":
            raise ValueError(f"unexpected origin state for {row['address']}")
        row["origin"] = "library"
        row["subsystem"] = subsystem(row["address"])
        row["disposition"] = "exclude"
        row["confidence"] = "high"
        row["evidence_id"] = EVIDENCE_ID

    def mutate_function(row: dict[str, str]) -> None:
        finding = weak[row["address"]]
        row["module"] = subsystem(row["address"])
        row["status"] = "excluded"
        row["match_percent"] = "0.00"
        row["is_thunk"] = "false"
        row["evidence"] = (
            "Target-context runtime review: complete IDA extent has a pinned "
            f"COFF relocation shape ({finding['size']} bytes), and both adjacent "
            "candidates are already reviewed runtime exclusions."
        )
        row["owner"] = "library"
        row["notes"] = (
            f"Reviewed inside the contiguous {subsystem(row['address'])} linker "
            "run; short shape evidence is accepted only with target placement and "
            "callgraph context, and grants no exactness credit."
        )

    updated_origins = ledger_update.rewrite_selected_rows(
        ORIGINS, pending, mutate_origin, write
    )
    updated_functions = ledger_update.rewrite_selected_rows(
        FUNCTIONS, pending, mutate_function, write
    )
    if updated_origins != len(pending) or updated_functions != len(pending):
        raise ValueError("incomplete contextual origin update")
    return {
        "write": write,
        "updated_origins": updated_origins,
        "updated_functions": updated_functions,
        "already_applied": len(already_applied),
        "subsystems": {"D3DX8": len(D3DX8_CONTEXT), "CRT": len(CRT_CONTEXT)},
        "retained_weak_unknown": len(set(weak) - selected),
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
    except (OSError, ValueError) as exc:
        print(f"error: context-origin review failed: {exc}", file=sys.stderr)
        raise SystemExit(1)
