#!/usr/bin/env python3
"""Validate or report the deliberately open TH09 whole-build graph."""

from __future__ import annotations

import argparse
from pathlib import Path
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]


def load() -> dict:
    with (ROOT / "config" / "build.toml").open("rb") as stream:
        return tomllib.load(stream)


def validate(document: dict) -> None:
    if document.get("schema_version") != 1:
        raise ValueError("unsupported build schema")
    graph = document.get("graph", {})
    for field in ("sources", "libraries", "resources"):
        if not isinstance(graph.get(field), list):
            raise ValueError(f"build graph {field} must be a list")
    acceptance = document.get("acceptance", {})
    if acceptance.get("whole_build_closed") and not graph["sources"]:
        raise ValueError("closed whole build requires at least one translation unit")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    try:
        document = load()
        validate(document)
    except (OSError, ValueError, tomllib.TOMLDecodeError) as exc:
        print(f"error: invalid build graph: {exc}", file=sys.stderr)
        return 1
    if args.check:
        print("build graph OK: skeleton is target-bound and explicitly open")
        return 0
    if document["product"]["status"] != "configured":
        print(
            "whole build unavailable: compile flags, TU partition, libraries, resources, "
            "and link order remain unknown",
            file=sys.stderr,
        )
        return 2
    print("error: configured build execution is not implemented", file=sys.stderr)
    return 2


if __name__ == "__main__":
    raise SystemExit(main())
