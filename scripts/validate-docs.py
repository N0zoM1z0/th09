#!/usr/bin/env python3
"""Validate maintained TH09 documentation against repository state."""
from __future__ import annotations

from pathlib import Path
import re
import sys

from progress import rows


ROOT = Path(__file__).resolve().parents[1]
DOCS = ROOT / "docs"
HANDOFF = DOCS / "RE_HANDOFF.md"
README = ROOT / "README.md"
TABLE_SPLIT = re.compile(r"(?<!\\)\|")
MARKDOWN_LINK = re.compile(r"\[[^\]]*\]\(([^)]+)\)")
FENCE = re.compile(r"^ {0,3}(?P<marker>`{3,}|~{3,})(?P<rest>.*)$")
CODE_SPAN = re.compile(r"(?<!`)(`+)(?!`)(.*?)\1(?!`)", re.DOTALL)


def table_cells(line: str) -> list[str]:
    return [cell.strip() for cell in TABLE_SPLIT.split(line.strip().strip("|"))]


def prose_link_targets(text: str) -> list[str]:
    """Find Markdown links without treating code examples as links."""
    prose: list[str] = []
    fence_char = ""
    fence_length = 0
    for line in text.splitlines():
        marker = FENCE.match(line)
        if fence_char:
            if (
                marker
                and marker["marker"][0] == fence_char
                and len(marker["marker"]) >= fence_length
                and not marker["rest"].strip()
            ):
                fence_char = ""
            continue
        if marker:
            fence_char = marker["marker"][0]
            fence_length = len(marker["marker"])
            continue
        prose.append(line)
    return MARKDOWN_LINK.findall(CODE_SPAN.sub("", "\n".join(prose)))


def validate_active_frontiers(
    text: str,
    functions: list[dict[str, str]],
    exact_addresses: set[str],
) -> None:
    """Keep closed or unmapped owners out of the current handoff frontier."""
    by_address = {row["address"].lower(): row for row in functions}
    exact = {address.lower() for address in exact_addresses}
    seen: set[str] = set()
    in_table = False
    for line in text.splitlines():
        if line == "| Owner | Target address | Current comparison | Remaining work |":
            if in_table or seen:
                raise ValueError("duplicate active-frontier table")
            in_table = True
            continue
        if not in_table:
            continue
        if not line.startswith("| "):
            in_table = False
            continue
        if line.startswith("| ---"):
            continue
        cells = table_cells(line)
        if len(cells) != 4:
            raise ValueError("active frontier must have four cells")
        owner, addresses, comparison, remaining = cells
        if not owner or not comparison or not remaining or not re.fullmatch(
            r"0x[0-9A-Fa-f]{8}(?: / 0x[0-9A-Fa-f]{8})*", addresses
        ):
            raise ValueError(f"incomplete active frontier: {owner}")
        for address in addresses.lower().split(" / "):
            if address in seen:
                raise ValueError(f"duplicate active frontier: {address}")
            seen.add(address)
            if address in exact:
                raise ValueError(f"exact owner remains in active frontier: {owner} {address}")
            row = by_address.get(address)
            if row is None or row["owner"] != "authored" or not row["source_file"]:
                raise ValueError(f"active frontier lacks maintained authored source: {address}")
    if not seen:
        raise ValueError("active-frontier table is missing or empty")


def validate_local_links() -> None:
    paths = [README, ROOT / "AGENTS.md", ROOT / "THIRD_PARTY_NOTICES.md",
             *sorted(DOCS.glob("*.md"))]
    for path in paths:
        text = path.read_text(encoding="utf-8")
        for raw_target in prose_link_targets(text):
            target = raw_target.strip().strip("<>").split("#", 1)[0]
            if not target or re.match(r"^[a-z][a-z0-9+.-]*:", target, re.I):
                continue
            candidate = (path.parent / target).resolve()
            if not candidate.exists():
                raise ValueError(
                    f"broken local link in {path.relative_to(ROOT)}: {raw_target}"
                )


def main() -> int:
    try:
        validate_active_frontiers(
            HANDOFF.read_text(encoding="utf-8"),
            rows("functions.csv"),
            {row["address"] for row in rows("matches.csv")},
        )
        validate_local_links()
    except (OSError, ValueError) as exc:
        print(f"error: documentation validation failed: {exc}", file=sys.stderr)
        return 1
    print("documentation OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
