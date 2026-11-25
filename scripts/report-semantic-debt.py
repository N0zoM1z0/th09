#!/usr/bin/env python3
"""Report heuristic TH09 source locations with semantic or readability debt.

This is a read-only work selector, not a semantic-completion metric. A match
can be legitimate serialization, wire decoding, ABI glue, or an intentional
unknown field. Always verify TH09 target evidence before changing source.

Examples:
  python3 -B scripts/report-semantic-debt.py
  python3 -B scripts/report-semantic-debt.py --category numeric-case --top 25
  python3 -B scripts/report-semantic-debt.py --path src/EclOperands.cpp --details
  python3 -B scripts/report-semantic-debt.py --json
"""

from __future__ import annotations

import argparse
from collections import Counter
from dataclasses import asdict, dataclass
import json
from pathlib import Path
import re
import sys


ROOT = Path(__file__).resolve().parents[1]
SOURCE_SUFFIXES = {".c", ".cc", ".cpp", ".h", ".hpp", ".inl"}
CATEGORIES = (
    "numeric-case",
    "numeric-sound-id",
    "numeric-effect-id",
    "view-type",
    "reinterpret-cast",
    "anonymous-identifier",
    "hex-offset-field",
    "raw-target-address",
)

NUMERIC_CASE = re.compile(r"\bcase\s+[+-]?(?:0[xX][0-9A-Fa-f]+|\d+)\s*:")
VIEW_TYPE_MULTILINE = re.compile(
    r"\b(?:struct|class)\s+[A-Za-z_]\w*View\b(?:\s*:\s*[^{;]+)?\s*\{"
)
NUMERIC_EFFECT_MULTILINE = re.compile(
    r"\bSpawnEffect(?:WithVelocity|InFixedSlot)?\s*\(\s*[+-]?(?:0[xX][0-9A-Fa-f]+|\d+)\b"
)
NUMERIC_SOUND_MULTILINE = re.compile(
    r"\bPlaySound(?:Positioned)?ByIdx\s*\(\s*[+-]?(?:0[xX][0-9A-Fa-f]+|\d+)\b"
)
REINTERPRET_CAST = re.compile(r"\breinterpret_cast\s*<")
ANONYMOUS_IDENTIFIER = re.compile(
    r"\b(?:unknown|unk|pad|padding|gap|unused|reserved|raw|field)"
    r"[A-Za-z0-9_]*[0-9A-Fa-f]{2,}\b",
    re.IGNORECASE,
)
HEX_OFFSET_FIELD = re.compile(
    r"\b[a-z][A-Za-z0-9]*?(?:[0-9][0-9A-F]{2,4}|[A-F][0-9A-F]{1,4})\b"
)
HEX_OFFSET_IGNORE = {
    "i386", "d3d8", "d3dx8", "utf8", "sha256", "md5", "win32", "x86", "x87",
    "sse2", "mmx", "int32", "uint32", "int16", "uint16", "int8", "uint8",
    "float3", "float2", "float4", "vec2", "vec3", "vec4",
}
RAW_TARGET_ADDRESS = re.compile(r"\b0x004[0-9A-Fa-f]{5}\b")


@dataclass(frozen=True)
class Finding:
    category: str
    path: str
    line: int
    match: str


def source_paths(inputs: list[str]) -> list[Path]:
    paths: set[Path] = set()
    for raw in inputs:
        candidate = (ROOT / raw).resolve()
        if not candidate.is_relative_to(ROOT):
            raise ValueError(f"path escapes repository: {raw}")
        if not candidate.exists():
            raise ValueError(f"path does not exist: {raw}")
        discovered = [candidate] if candidate.is_file() else candidate.rglob("*")
        for path in discovered:
            if not path.is_file() or path.suffix.lower() not in SOURCE_SUFFIXES:
                continue
            paths.add(path)
    return sorted(paths)


def strip_line_comment(line: str) -> str:
    idx = line.find("//")
    return line if idx == -1 else line[:idx]


def line_findings(path: Path, line_number: int, raw_line: str) -> list[Finding]:
    relative = path.relative_to(ROOT).as_posix()
    code = strip_line_comment(raw_line)
    findings: list[Finding] = []
    simple_patterns = (
        ("numeric-case", NUMERIC_CASE),
        ("reinterpret-cast", REINTERPRET_CAST),
        ("anonymous-identifier", ANONYMOUS_IDENTIFIER),
        ("raw-target-address", RAW_TARGET_ADDRESS),
    )
    for category, pattern in simple_patterns:
        for match in pattern.finditer(code):
            findings.append(
                Finding(
                    category=category,
                    path=relative,
                    line=line_number,
                    match=match.group(0).strip(),
                )
            )
    for match in HEX_OFFSET_FIELD.finditer(code):
        token = match.group(0)
        if token.lower() in HEX_OFFSET_IGNORE:
            continue
        if ANONYMOUS_IDENTIFIER.fullmatch(token):
            continue
        findings.append(
            Finding(
                category="hex-offset-field",
                path=relative,
                line=line_number,
                match=token,
            )
        )
    return findings


def strip_all_comments(text: str) -> str:
    """Blank // and /* */ comments while preserving line numbers."""
    def blank(match: re.Match[str]) -> str:
        return "".join("\n" if ch == "\n" else " " for ch in match.group(0))
    return re.sub(r"//[^\n]*|/\*.*?\*/", blank, text, flags=re.DOTALL)


def scan(paths: list[Path]) -> list[Finding]:
    findings: list[Finding] = []
    multiline_patterns = (
        ("view-type", VIEW_TYPE_MULTILINE),
        ("numeric-sound-id", NUMERIC_SOUND_MULTILINE),
        ("numeric-effect-id", NUMERIC_EFFECT_MULTILINE),
    )
    for path in paths:
        relative = path.relative_to(ROOT).as_posix()
        text = path.read_text(encoding="utf-8", errors="replace")
        code_text = strip_all_comments(text)
        for category, pattern in multiline_patterns:
            for match in pattern.finditer(code_text):
                line_number = code_text.count("\n", 0, match.start()) + 1
                snippet = " ".join(match.group(0).split())[:80]
                findings.append(
                    Finding(
                        category=category,
                        path=relative,
                        line=line_number,
                        match=snippet,
                    )
                )
        for line_number, line in enumerate(code_text.splitlines(), start=1):
            findings.extend(line_findings(path, line_number, line))
    return findings


def report(paths: list[Path], findings: list[Finding]) -> dict[str, object]:
    by_category = Counter(item.category for item in findings)
    by_file = Counter(item.path for item in findings)
    return {
        "schema_version": 1,
        "scope": {
            "files_scanned": len(paths),
            "note": "Heuristic routing only; candidate counts are not semantic progress.",
        },
        "category_counts": {name: by_category.get(name, 0) for name in CATEGORIES},
        "file_counts": dict(sorted(by_file.items(), key=lambda item: (-item[1], item[0]))),
        "findings": [asdict(item) for item in findings],
    }


def print_text(data: dict[str, object], top: int, details: bool) -> None:
    scope = data["scope"]
    assert isinstance(scope, dict)
    print("TH09 semantic-debt candidates (heuristic; not a progress claim)")
    print(f"files scanned: {scope['files_scanned']}")
    print("categories:")
    category_counts = data["category_counts"]
    assert isinstance(category_counts, dict)
    for category in CATEGORIES:
        print(f"  {category:22s}: {category_counts[category]:6d}")

    file_counts = data["file_counts"]
    assert isinstance(file_counts, dict)
    print(f"top files (maximum {top}):")
    for path, count in list(file_counts.items())[:top]:
        print(f"  {count:5d}  {path}")

    if details:
        print("findings:")
        findings = data["findings"]
        assert isinstance(findings, list)
        for item in findings:
            assert isinstance(item, dict)
            print(
                f"  {item['path']}:{item['line']}: "
                f"{item['category']}: {item['match']}"
            )


def main() -> int:
    parser = argparse.ArgumentParser(
        description=__doc__,
        epilog=(
            "Candidate counts deliberately never fail CI and must not be used as "
            "semantic completion percentages."
        ),
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument(
        "--path",
        action="append",
        default=[],
        metavar="PATH",
        help="repository-relative source file or directory (repeatable; default: src)",
    )
    parser.add_argument(
        "--category",
        action="append",
        choices=CATEGORIES,
        help="keep only this category (repeatable)",
    )
    parser.add_argument("--details", action="store_true", help="print every finding")
    parser.add_argument("--json", action="store_true", help="emit the complete report as JSON")
    parser.add_argument(
        "--top",
        type=int,
        default=20,
        metavar="N",
        help="maximum files in the text summary (default: 20)",
    )
    args = parser.parse_args()

    if args.top < 1:
        parser.error("--top must be at least 1")

    try:
        paths = source_paths(args.path or ["src"])
        findings = scan(paths)
    except (OSError, ValueError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1

    if args.category:
        selected = set(args.category)
        findings = [item for item in findings if item.category in selected]
    data = report(paths, findings)
    if args.json:
        json.dump(data, sys.stdout, indent=2, sort_keys=True)
        print()
    else:
        print_text(data, args.top, args.details)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
