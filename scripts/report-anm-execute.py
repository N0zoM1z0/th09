#!/usr/bin/env python3
"""Audit the TH09 ANM interpreter source, target tables, and exact unit."""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "src" / "AnmManagerExecuteScript.cpp"
UNITS = ROOT / "config" / "match-units.toml"
COMPARE = ROOT / "scripts" / "compare-coff-function.py"

TARGET_START = 0x00436F30
TARGET_CODE_SIZE = 0x248E
TARGET_TABLE_START = 0x004393C0
TARGET_PHYSICAL_SIZE = 0x2630
UNIT_NAME = "anm-manager-execute-script"

TABLES = (
    {
        "name": "opcode",
        "address": 0x004393C0,
        "count": 91,
        "sha256": "f55403d56a7f3df37ace4088d3a76270dfec454a03de3074df61b0c17804bb8d",
    },
    {
        "name": "easing",
        "address": 0x0043952C,
        "count": 6,
        "sha256": "415679e0d30e3f4c12e72d2ba0500de20e6bad7d0bab24b0d811bef2f7779a5d",
    },
    {
        "name": "interpolation-field",
        "address": 0x00439544,
        "count": 7,
        "sha256": "7d2cfc8f42eb172c0c2bfe7a1b49c67372cbfc38927f02b19f4f1dbcef6c97ca",
    },
)


def load_compare_module():
    spec = importlib.util.spec_from_file_location("th09_compare_coff", COMPARE)
    if spec is None or spec.loader is None:
        raise ValueError("cannot load compare-coff-function.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def audit_source() -> dict[str, object]:
    text = SOURCE.read_text(encoding="utf-8")
    enum_match = re.search(r"enum AnmOpcode\s*\{(?P<body>.*?)\n\};", text, re.DOTALL)
    if enum_match is None:
        raise ValueError("AnmOpcode enum is missing")
    entries = re.findall(r"^\s*(AnmOpcode_[A-Za-z0-9_]+)\s*=\s*(-?\d+)\s*,?\s*$", enum_match.group("body"), re.MULTILINE)
    values = {name: int(value) for name, value in entries}
    if len(values) != 91 or sorted(values.values()) != list(range(-1, 90)):
        raise ValueError("AnmOpcode must cover every wire value from -1 through 89 exactly once")

    cases = set(re.findall(r"case\s+(AnmOpcode_[A-Za-z0-9_]+)\s*:", text))
    intentionally_defaulted = {"AnmOpcode_Nop", "AnmOpcode_InterruptLabel"}
    expected_cases = set(values) - intentionally_defaulted
    if cases != expected_cases:
        raise ValueError(
            "opcode case coverage differs: "
            f"missing={sorted(expected_cases - cases)!r} extra={sorted(cases - expected_cases)!r}"
        )
    return {
        "enum_entries": len(values),
        "explicit_cases": len(cases),
        "defaulted_opcodes": sorted(intentionally_defaulted),
    }


def audit_target(compare_module) -> dict[str, object]:
    target = compare_module.verified_target()
    logical = compare_module.pe_bytes_at(target, TARGET_START, TARGET_CODE_SIZE)
    physical = compare_module.pe_bytes_at(target, TARGET_START, TARGET_PHYSICAL_SIZE)
    alignment_size = TARGET_TABLE_START - (TARGET_START + TARGET_CODE_SIZE)
    if alignment_size != 2:
        raise ValueError(f"unexpected code/table alignment size {alignment_size}")

    rows = []
    table_bytes = 0
    for table in TABLES:
        raw = compare_module.pe_bytes_at(target, table["address"], table["count"] * 4)
        digest = hashlib.sha256(raw).hexdigest()
        if digest != table["sha256"]:
            raise ValueError(f"{table['name']} table hash mismatch: {digest}")
        rows.append(
            {
                "name": table["name"],
                "address": f"0x{table['address']:08X}",
                "count": table["count"],
                "bytes": len(raw),
                "sha256": digest,
            }
        )
        table_bytes += len(raw)
    if table_bytes != 416 or TARGET_CODE_SIZE + alignment_size + table_bytes != TARGET_PHYSICAL_SIZE:
        raise ValueError("ANM code/alignment/table geometry differs from the reviewed extent")

    return {
        "logical_code_bytes": len(logical),
        "alignment_bytes": alignment_size,
        "compiler_table_bytes": table_bytes,
        "physical_bytes": len(physical),
        "logical_sha256": hashlib.sha256(logical).hexdigest(),
        "physical_sha256": hashlib.sha256(physical).hexdigest(),
        "tables": rows,
    }


def audit_unit(compare_module, require_exact: bool) -> dict[str, object]:
    with UNITS.open("rb") as stream:
        units = tomllib.load(stream)["units"]
    unit = units.get(UNIT_NAME)
    if not isinstance(unit, dict):
        raise ValueError(f"missing canonical unit {UNIT_NAME}")
    if int(unit["size"]) != TARGET_CODE_SIZE or int(unit.get("compare_size", 0)) != TARGET_PHYSICAL_SIZE:
        raise ValueError("canonical unit does not preserve logical and physical ownership extents")
    if "/Ob0" not in unit["profile"]:
        raise ValueError("canonical unit lost the reviewed /Ob0 translation-unit profile")

    object_path = ROOT / str(unit["object"])
    if not object_path.is_file():
        if require_exact:
            raise ValueError(f"missing built canonical object: {object_path.relative_to(ROOT)}")
        return {"result": "not-built", "object": str(object_path.relative_to(ROOT))}

    result = compare_module.compare_unit(UNIT_NAME)
    if result["result"] != "exact":
        raise ValueError(f"canonical unit comparison returned {result['result']}")
    relocations = result["relocations"]
    table_ranges = (
        (0x2490, 0x25FC, 91),
        (0x25FC, 0x2614, 6),
        (0x2614, 0x2630, 7),
    )
    table_counts = []
    for start, end, expected in table_ranges:
        count = sum(start <= int(row["offset"], 0) < end for row in relocations)
        if count != expected:
            raise ValueError(f"compiler table relocation count differs at {start:#x}: {count}/{expected}")
        table_counts.append(count)
    if len(relocations) != 368:
        raise ValueError(f"canonical relocation count differs: {len(relocations)}/368")
    return {
        "result": "exact",
        "object": result["object"],
        "logical_match": f"{result['matched_bytes']}/{result['size']}",
        "physical_match": f"{result['matched_compared_bytes']}/{result['compared_size']}",
        "relocations": len(relocations),
        "compiler_table_relocations": table_counts,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--json", action="store_true")
    parser.add_argument("--require-exact", action="store_true")
    args = parser.parse_args()
    try:
        compare_module = load_compare_module()
        report = {
            "result": "ok",
            "source": audit_source(),
            "target": audit_target(compare_module),
            "canonical_unit": audit_unit(compare_module, args.require_exact),
        }
    except (KeyError, OSError, TypeError, ValueError, tomllib.TOMLDecodeError) as exc:
        report = {"result": "error", "error": str(exc)}

    if args.json:
        print(json.dumps(report, indent=2))
    elif report["result"] == "error":
        print(f"error: {report['error']}", file=sys.stderr)
    else:
        source = report["source"]
        target = report["target"]
        unit = report["canonical_unit"]
        print(
            "ANM ExecuteScript coverage OK: "
            f"{source['enum_entries']} wire opcodes, {source['explicit_cases']} explicit cases"
        )
        print(
            "target geometry OK: "
            f"{target['logical_code_bytes']} code + {target['alignment_bytes']} alignment + "
            f"{target['compiler_table_bytes']} tables = {target['physical_bytes']} bytes"
        )
        if unit["result"] == "exact":
            print(
                "canonical Oracle exact: "
                f"logical {unit['logical_match']}, physical {unit['physical_match']}, "
                f"{unit['relocations']} relocations"
            )
        else:
            print(f"canonical Oracle: {unit['result']} ({unit['object']})")
    return 0 if report["result"] == "ok" else 1


if __name__ == "__main__":
    raise SystemExit(main())
