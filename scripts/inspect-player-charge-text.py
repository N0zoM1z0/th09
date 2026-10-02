#!/usr/bin/env python3
"""Check three SHT text-address call regions; never whole-owner exact credit."""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
OWNER = "?UpdateChargeAttack@PlayerChargeAttackView@@QAEHXZ"
START_ATTACK = "?StartAttack@CardAttackStartView@@QAEXHHHPBD@Z"
# Independently reviewed target SHT base-load through StartAttack call.
REGIONS = ((0xAC, 0x41F999, 0x41F9B1),
           (0x6C, 0x41FB55, 0x41FB6B),
           (0x2C, 0x41FBA1, 0x41FBB7))


def replay_region(code, relocations, row, start, call):
    size = call + 5 - start
    offset = row["offset"] - 1 - (call - start)
    if offset < 0 or offset + size > len(code):
        raise ValueError("text-address region lies outside the candidate owner")
    fields = [r for r in relocations if offset <= r["offset"] < offset + size]
    if fields != [row] or row["type"] != "REL32" or row["addend"] != 0:
        raise ValueError("unexpected relocation in text-address region")
    region = bytearray(code[offset:offset + size])
    if region[-5] != 0xE8:
        raise ValueError("expected an immediate StartAttack call")
    struct.pack_into("<i", region, size - 4, 0x403E50 - (call + 5))
    return offset, region


def inspect(path):
    spec = importlib.util.spec_from_file_location("charge_coff", ROOT / "scripts/compare-coff-function.py")
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    target = coff.verified_target()
    code, relocations = coff.object_function(path, OWNER)
    calls = [r for r in relocations if r["symbol"] == START_ATTACK]
    if len(calls) != 3:
        raise ValueError("expected exactly three StartAttack relocations")
    results = []
    for row, (member, start, call) in zip(calls, REGIONS):
        offset, replay = replay_region(code, relocations, row, start, call)
        expected = coff.pe_bytes_at(target, start, len(replay))
        results.append({"sht_text_offset": hex(member), "target_start": hex(start),
                        "candidate_offset": hex(offset), "bytes": len(replay),
                        "region_agrees": replay == expected})
    return {"diagnostic_only": True, "whole_owner_exact_credit": "none",
            "target_code_bytes": 1210, "candidate_code_bytes": len(code),
            "relocations": len(relocations),
            "raw_function_sha256": hashlib.sha256(code).hexdigest(), "regions": results}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("object", type=Path)
    args = parser.parse_args()
    try:
        result = inspect(args.object)
        print(json.dumps(result, indent=2))
        return 0 if all(r["region_agrees"] for r in result["regions"]) else 1
    except (OSError, ValueError, struct.error) as exc:
        print("error: " + str(exc), file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
