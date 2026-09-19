#!/usr/bin/env python3
"""Validate the small, target-bound TH09 reconstruction control plane."""

from __future__ import annotations

import argparse
import csv
from pathlib import Path
import subprocess
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]
CONFIG = ROOT / "config"


def rows(name: str) -> list[dict[str, str]]:
    with (CONFIG / name).open(newline="", encoding="utf-8") as stream:
        reader = csv.DictReader(stream)
        result = []
        for line_number, row in enumerate(reader, 2):
            if None in row or any(value is None for value in row.values()):
                raise ValueError(f"malformed {name} row at line {line_number}")
            result.append(row)
        return result


def one_column(name: str) -> list[str]:
    with (CONFIG / name).open(newline="", encoding="utf-8") as stream:
        result = []
        for line_number, row in enumerate(csv.reader(stream), 1):
            if len(row) != 1 or not row[0]:
                raise ValueError(f"malformed {name} row at line {line_number}")
            result.append(row[0])
        return result


def validate() -> dict[str, int]:
    with (CONFIG / "target.toml").open("rb") as stream:
        target_manifest = tomllib.load(stream)
    target = target_manifest["target"]
    pe = target_manifest["pe"]
    with (CONFIG / "match-units.toml").open("rb") as stream:
        units = tomllib.load(stream)
    with (CONFIG / "build.toml").open("rb") as stream:
        build = tomllib.load(stream)
    for label, digest in (
        ("match-units", units.get("target_sha256")),
        ("build", build.get("target_sha256")),
    ):
        if digest != target["sha256"]:
            raise ValueError(f"{label} target SHA-256 mismatch")
    functions = rows("functions.csv")
    origins = rows("function-origins.csv")
    mappings = rows("reccmp-functions.csv")
    matches = rows("matches.csv")
    implemented = set(one_column("implemented.csv"))
    starts: list[int] = []
    text_start, text_end = int(pe["text_start"], 0), int(pe["text_end"], 0)
    for row in functions:
        address, size, end = int(row["address"], 0), int(row["size"], 0), int(row["span_end"], 0)
        if size <= 0 or end != address + size - 1:
            raise ValueError(f"invalid function extent at {row['address']}")
        if address < text_start or end > text_end:
            raise ValueError(f"function extent leaves .text at {row['address']}")
        starts.append(address)
    mapped_sources = {row["source_file"] for row in functions if row["source_file"]}
    missing_sources = sorted(path for path in mapped_sources if not (ROOT / path).is_file())
    if missing_sources:
        raise ValueError(f"mapped source files do not exist: {', '.join(missing_sources)}")
    source_units = {
        path.relative_to(ROOT).as_posix()
        for path in (ROOT / "src").rglob("*.cpp")
    }
    unmapped_sources = sorted(source_units - mapped_sources)
    if unmapped_sources:
        raise ValueError(f"source files lack function mappings: {', '.join(unmapped_sources)}")
    if starts != sorted(set(starts)):
        raise ValueError("function addresses must be unique and sorted")
    function_addresses = {row["address"] for row in functions}
    origin_addresses = {row["address"] for row in origins}
    if origin_addresses != function_addresses or len(origins) != len(functions):
        raise ValueError("origin ledger must cover each function exactly once")
    allowed_origins = {"unknown", "authored", "authored_game", "compiler", "compiler_generated", "library", "third_party", "import_thunk", "data", "padding"}
    allowed_dispositions = {"review", "authored", "exclude"}
    for row in origins:
        if row["origin"] not in allowed_origins or row["disposition"] not in allowed_dispositions:
            raise ValueError(f"invalid origin state at {row['address']}")
    mapping_addresses = {row["address"] for row in mappings}
    match_addresses = {row["address"] for row in matches}
    if not mapping_addresses.issubset(function_addresses) or not match_addresses.issubset(function_addresses):
        raise ValueError("mapping or match ledger references an unknown address")
    mapped_names = {row["name"] for row in mappings}
    if not implemented.issubset(mapped_names):
        raise ValueError("implemented.csv contains an unmapped source name")
    configured_units = units.get("units", {})
    if not isinstance(configured_units, dict):
        raise ValueError("match units must be a table")
    for name, unit in configured_units.items():
        source = unit.get("source")
        if not isinstance(source, str) or not (ROOT / source).is_file():
            raise ValueError(f"match unit {name!r} has a missing source file")
    for row in matches:
        if row["unit"] not in configured_units or row["match_percent"] != "100.00":
            raise ValueError(f"unreplayable exact claim at {row['address']}")
    if build.get("schema_version") != 1:
        raise ValueError("unsupported build manifest schema")
    acceptance = build.get("acceptance", {})
    if acceptance.get("whole_build_closed") and not build.get("graph", {}).get("sources"):
        raise ValueError("whole-build closure cannot be true for an empty graph")
    return {"functions": len(functions), "mappings": len(mappings), "matches": len(matches)}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--require-target", action="store_true")
    args = parser.parse_args()
    try:
        counts = validate()
        if args.require_target:
            subprocess.run([sys.executable, "scripts/verify-target.py"], cwd=ROOT, check=True)
    except (OSError, KeyError, TypeError, ValueError, tomllib.TOMLDecodeError, subprocess.CalledProcessError) as exc:
        print(f"error: tracking validation failed: {exc}", file=sys.stderr)
        return 1
    print(
        f"tracking OK: {counts['functions']} candidates, "
        f"{counts['mappings']} mappings, {counts['matches']} exact"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
