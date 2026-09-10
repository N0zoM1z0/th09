#!/usr/bin/env python3
"""Fail-closed identity and PE-structure check for TH09 v1.50a."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import sys

from target_identity import resolve_target, verify_target


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("executable", nargs="?", type=Path)
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    path = resolve_target(args.executable)
    if not path.is_file():
        print(f"missing target: {path}", file=sys.stderr)
        print(
            "pass a path, set TH09_TARGET_PATH, or provide resources/th09.exe",
            file=sys.stderr,
        )
        return 1
    try:
        observed, problems = verify_target(path)
    except (OSError, KeyError, TypeError, ValueError) as exc:
        print(f"invalid target or manifest: {exc}", file=sys.stderr)
        return 2
    if args.json:
        print(json.dumps({"ok": not problems, "observed": observed, "problems": problems}, indent=2))
    elif problems:
        print(f"unsupported executable: {path}", file=sys.stderr)
        for problem in problems:
            print(f"  {problem}", file=sys.stderr)
    else:
        print(f"target OK: {path}")
        print(f"sha256: {observed['sha256']}")
        print(f"md5:    {observed['md5']}")
        print("PE:     i386, 5 sections, image base 0x00400000, entry 0x0047D45F")
    return 1 if problems else 0


if __name__ == "__main__":
    raise SystemExit(main())
