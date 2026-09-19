#!/usr/bin/env python3
"""Add game functions proven by the target's global-init wrapper graph."""
from __future__ import annotations

import argparse
import csv
import importlib.util
import json
from pathlib import Path
import struct
import sys

from tracking_csv import rows_by_address


ROOT = Path(__file__).resolve().parents[1]
FUNCTIONS = ROOT / "config" / "functions.csv"
ORIGINS = ROOT / "config" / "function-origins.csv"
EVIDENCE_ID = "static-init-target-boundary-review-2026-09-19"

TARGETS = {
    0x0042B0E0: {
        "size": 44,
        "wrapper": 0x0048DA60,
        "origin": "authored_game",
        "subsystem": "ZunMemory",
        "rationale": "Explicitly walks and frees non-null registry allocations; an implicit destructor cannot synthesize that ownership policy.",
    },
    0x0042D290: {
        "size": 18,
        "wrapper": 0x0048D8E0,
        "origin": "authored_game",
        "subsystem": "Game",
        "rationale": "Explicitly clears the complete 0x44-byte object; VC7.1 does not zero-initialize POD members in an implicit default constructor.",
    },
    0x00435DC0: {
        "size": 227,
        "wrapper": 0x0048D950,
        "origin": "unknown",
        "subsystem": "",
        "rationale": "Boundary and constructor role are proven, but the member-only construction sequence does not distinguish an explicit from an implicit special member.",
    },
    0x0043E470: {
        "size": 34,
        "wrapper": 0x0048DA20,
        "origin": "authored_game",
        "subsystem": "GameManager",
        "rationale": "Explicitly clears the object and then overwrites a selected 0x200-byte member region with -1; that policy is not an implicit default-constructor effect.",
    },
}


def load_tail_module():
    path = ROOT / "scripts" / "review-static-init-tail.py"
    spec = importlib.util.spec_from_file_location("th09_static_init_tail", path)
    if spec is None or spec.loader is None:
        raise ValueError(f"cannot load {path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def review(write: bool) -> dict[str, object]:
    tail = load_tail_module()
    tail.verify_target_tail()
    data = tail.TARGET.read_bytes()
    _, sections = tail.parse_image(data)

    next_starts = {
        0x0042B0E0: 0x0042B110,
        0x0042D290: 0x0042D2B0,
        0x00435DC0: 0x00435EB0,
        0x0043E470: 0x0043E4A0,
    }
    for address, review_row in TARGETS.items():
        wrapper = int(review_row["wrapper"])
        wrapper_body = tail.read_va(data, sections, wrapper, 10)
        if wrapper_body[:1] != b"\xB9" or wrapper_body[5:6] != b"\xE9":
            raise ValueError(f"unexpected static wrapper form at {tail.canonical(wrapper)}")
        displacement = struct.unpack_from("<i", wrapper_body, 6)[0]
        if wrapper + 10 + displacement != address:
            raise ValueError(f"static wrapper target changed at {tail.canonical(wrapper)}")
        size = int(review_row["size"])
        body = tail.read_va(data, sections, address, size)
        if body[-1] != 0xC3:
            raise ValueError(f"target function has no terminal ret at {tail.canonical(address)}")
        gap = tail.read_va(data, sections, address + size, next_starts[address] - address - size)
        if gap != b"\xCC" * len(gap):
            raise ValueError(f"non-CC boundary gap after {tail.canonical(address)}")

    selected = {tail.canonical(address) for address in TARGETS}
    function_rows = rows_by_address(FUNCTIONS)
    origin_rows = rows_by_address(ORIGINS)
    if set(function_rows) != set(origin_rows):
        raise ValueError("function and origin ledgers disagree before update")
    existing = selected & set(function_rows)
    for address in existing:
        review_row = TARGETS[int(address, 0)]
        function = function_rows[address]
        origin = origin_rows[address]
        expected_origin = str(review_row["origin"])
        size = int(review_row["size"])
        if (
            int(function["size"], 0) != size
            or int(function["span_end"], 0) != int(address, 0) + size - 1
        ):
            raise ValueError(f"existing target-derived boundary changed at {address}")
        if expected_origin == "authored_game" and (
            origin["origin"] != expected_origin
            or origin["disposition"] != "authored"
            or function["owner"] != "authored"
        ):
            raise ValueError(f"established authored origin changed at {address}")

    missing = selected - set(function_rows)
    function_fields = next(csv.reader([FUNCTIONS.read_text(encoding="utf-8").splitlines()[0]]))
    origin_fields = next(csv.reader([ORIGINS.read_text(encoding="utf-8").splitlines()[0]]))
    function_additions: dict[str, dict[str, str]] = {}
    origin_additions: dict[str, dict[str, str]] = {}
    for address in missing:
        numeric = int(address, 0)
        review_row = TARGETS[numeric]
        size = int(review_row["size"])
        origin_name = str(review_row["origin"])
        function = {field: "" for field in function_fields}
        function.update(
            {
                "address": address,
                "size": str(size),
                "span_end": tail.canonical(numeric + size - 1),
                "module": str(review_row["subsystem"]),
                "status": "unclassified",
                "match_percent": "0.00",
                "is_thunk": "false",
                "evidence": (
                    "Target global-init graph supplies the entry; terminal ret and "
                    "following CC gap prove the complete logical extent."
                ),
                "owner": "authored" if origin_name == "authored_game" else "",
                "notes": str(review_row["rationale"]),
            }
        )
        function_additions[address] = function
        origin = {field: "" for field in origin_fields}
        origin.update(
            {
                "address": address,
                "origin": origin_name,
                "subsystem": str(review_row["subsystem"]),
                "disposition": "authored" if origin_name == "authored_game" else "review",
                "confidence": "high" if origin_name == "authored_game" else "unknown",
                "evidence_id": EVIDENCE_ID,
            }
        )
        origin_additions[address] = origin

    function_counts = tail.rewrite_rows(
        FUNCTIONS, function_additions, set(), lambda _row: None, write
    )
    origin_counts = tail.rewrite_rows(
        ORIGINS, origin_additions, set(), lambda _row: None, write
    )
    if function_counts != origin_counts:
        raise ValueError("function/origin insertion counts disagree")
    return {
        "write": write,
        "inserted_candidates": function_counts[0],
        "already_applied": len(existing),
        "authored": sum(row["origin"] == "authored_game" for row in TARGETS.values()),
        "retained_origin_unknown": sum(row["origin"] == "unknown" for row in TARGETS.values()),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--apply", action="store_true", help="insert reviewed candidates")
    args = parser.parse_args()
    print(json.dumps(review(args.apply), indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, struct.error) as exc:
        print(f"error: static-init target review failed: {exc}", file=sys.stderr)
        raise SystemExit(1)
