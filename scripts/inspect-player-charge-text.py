#!/usr/bin/env python3
"""Check charge SHT text and reload regions; never whole-owner exact credit."""
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
    # The final input-gate read follows all initial timer branches. Bind its
    # independently known GameManager +0xE8 field; compare the complete side
    # reload, gate reload and byte comparison against the target window.
    gates = [r for r in relocations
             if r["symbol"] == "?g_GameManager@@3UGameManagerReplayView@@A"]
    if len(gates) != 3:
        raise ValueError("expected three input-gate accesses")
    gate = gates[-1]
    begin = gate["offset"] - 5
    if (gate["type"] != "DIR32" or gate["addend"] != 0xE8 or
            begin < 0 or begin + 17 > len(code)):
        raise ValueError("unexpected post-timer gate relocation or extent")
    fields = [r for r in relocations if begin <= r["offset"] < begin + 17]
    gate_region = bytearray(code[begin:begin + 17])
    gate_agrees = False
    if fields == [gate]:
        struct.pack_into("<I", gate_region, 5, 0x4A7D90 + 0xE8)
        gate_agrees = gate_region == coff.pe_bytes_at(target, 0x41F914, 17)

    # No spill is allowed between this conversion and the SHT-duration compare.
    # The nine-byte target window is the post-call SHT load plus FCOMP, so a
    # pre-call snapshot or an added float rounding store fails independently.
    # Include and independently bind the preceding five-byte conversion call.
    conversions = [r for r in relocations
                   if r["symbol"] == "??BZunTimer@@QAEMXZ"]
    if len(conversions) != 1:
        raise ValueError("expected one timer float conversion")
    conversion = conversions[0]
    at = conversion["offset"]
    if (conversion["type"] != "REL32" or conversion["addend"] != 0 or
            at < 1 or at + 4 > len(code) or code[at - 1] != 0xE8):
        raise ValueError("unexpected timer conversion call encoding")
    after = at + 4
    if after + 9 > len(code):
        raise ValueError("missing post-conversion duration region")
    duration_region = bytearray(code[at - 1:after + 9])
    fields = [r for r in relocations if at - 1 <= r["offset"] < after + 9]
    duration_agrees = False
    if fields == [conversion]:
        struct.pack_into("<i", duration_region, 1, 0x4014F0 - 0x41FC52)
        duration_agrees = duration_region == coff.pe_bytes_at(target, 0x41FC4D, 14)
    results.extend([
        {"region": "post_timer_side_reload", "candidate_offset": hex(begin),
         "bytes": 17, "region_agrees": gate_agrees},
        {"region": "post_conversion_sht_reload", "candidate_offset": hex(at - 1),
         "bytes": 14, "region_agrees": duration_agrees},
    ])
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
