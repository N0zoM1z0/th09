#!/usr/bin/env python3
"""Run the small target-independent validation used by public CI."""

from __future__ import annotations

import os
from pathlib import Path
import subprocess
import sys


ROOT = Path(__file__).resolve().parents[1]


def tracked(pattern: str) -> list[str]:
    output = subprocess.run(
        ["git", "ls-files", "--cached", "--others", "--exclude-standard", "-z", pattern],
        cwd=ROOT,
        check=True,
        stdout=subprocess.PIPE,
    ).stdout
    return [raw.decode() for raw in output.split(b"\0") if raw and (ROOT / raw.decode()).is_file()]


def run(label: str, command: list[str]) -> None:
    print(f"\n==> {label}", flush=True)
    environment = os.environ.copy()
    environment["PYTHONPYCACHEPREFIX"] = str(ROOT / "build" / "pycache")
    subprocess.run(command, cwd=ROOT, env=environment, check=True)


def validate_public_tree() -> None:
    forbidden = {".exe", ".dll", ".dat", ".gpr", ".i64", ".id0", ".id1", ".id2", ".nam", ".til"}
    for relative in tracked("*"):
        if Path(relative).suffix.lower() in forbidden:
            raise ValueError(f"private binary or database is trackable: {relative}")


def main() -> int:
    try:
        validate_public_tree()
        run("Compile workflow Python", [sys.executable, "-m", "py_compile", *tracked("scripts/*.py")])
        run(
            "Run target-independent tests",
            [sys.executable, "-m", "unittest", "discover", "-s", "tests"],
        )
        run("Validate ledgers", [sys.executable, "scripts/validate-tracking.py"])
        run("Validate open whole-build graph", [sys.executable, "scripts/build.py", "--check"])
        run("Check generated progress", [sys.executable, "scripts/progress.py", "--check"])
        run("Check whitespace", ["git", "diff", "--check"])
    except (OSError, ValueError, subprocess.CalledProcessError) as exc:
        print(f"error: CI validation failed: {exc}", file=sys.stderr)
        return 1
    print("\nTH09 target-independent checks passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
