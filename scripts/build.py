#!/usr/bin/env python3
"""Check the TH09 whole-build graph or an executable's runtime version identity."""

from __future__ import annotations

import argparse
import hashlib
from pathlib import Path
import struct
import sys
import tomllib

from target_identity import load_manifest, parse_pe


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


def runtime_executable_identity(image: bytes) -> tuple[int, int]:
    # ComputeExecutableChecksum sums all complete DWORDs except the last.
    size = len(image)
    end = max(0, size // 4 - 1) * 4
    checksum = sum(word for (word,) in struct.iter_unpack("<I", memoryview(image)[:end]))
    return size, checksum & 0xFFFFFFFF


def check_runtime_versions(executables: list[Path]) -> int:
    game_data = load_manifest()["game_data"]
    archive = ROOT / "resources" / game_data["filename"]
    with archive.open("rb") as stream:
        if hashlib.file_digest(stream, "sha256").hexdigest() != game_data["sha256"]:
            raise ValueError(f"{archive} differs from the archive bound to the version record")

    version = game_data["runtime_version"]
    expected = version["executable_size"], version["executable_checksum"]
    print(f'{version["member"]} record {version["identifier"]} expects '
          f'{expected[0]} bytes and checksum {expected[1]}')
    accepted = True
    for path in executables:
        try:
            image = path.read_bytes()
            if parse_pe(image)["machine_id"] != 0x14C:
                raise ValueError("expected an i386 PE32 executable")
        except (OSError, ValueError, struct.error) as exc:
            print(f"{path} could not be checked: {exc}", file=sys.stderr)
            accepted = False
            continue
        size, checksum = runtime_executable_identity(image)
        match = (size, checksum) == expected
        status = "accepted" if match else "rejected"
        print(f"{path} {status} ({size} bytes, checksum {checksum})")
        accepted &= match
    return 0 if accepted else 1


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    action = parser.add_mutually_exclusive_group()
    action.add_argument("--check", action="store_true")
    action.add_argument("--runtime-version", nargs="+", metavar="EXE", type=Path,
                        help="check executables against the stock archive's periodic version record")
    args = parser.parse_args()
    if args.runtime_version:
        try:
            return check_runtime_versions(args.runtime_version)
        except (OSError, KeyError, ValueError) as exc:
            print(f"runtime version check failed: {exc}", file=sys.stderr)
            return 1
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
