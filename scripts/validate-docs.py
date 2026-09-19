#!/usr/bin/env python3
"""Validate maintained TH09 documentation against repository state."""
from __future__ import annotations

from collections import Counter
from pathlib import Path
import re
import sys

from progress import measures


ROOT = Path(__file__).resolve().parents[1]
DOCS = ROOT / "docs"
KNOWLEDGE = DOCS / "KNOWLEDGE_BASE.md"
HANDOFF = DOCS / "RE_HANDOFF.md"
TABLE_SPLIT = re.compile(r"(?<!\\)\|")
MARKDOWN_LINK = re.compile(r"\[[^\]]*\]\(([^)]+)\)")


def table_cells(line: str) -> list[str]:
    return [cell.strip() for cell in TABLE_SPLIT.split(line.strip().strip("|"))]


def validate_knowledge_base() -> None:
    ids: list[str] = []
    forbidden_states = {"superseded", "historical / superseded-codegen-diagnostic"}
    for line_number, line in enumerate(
        KNOWLEDGE.read_text(encoding="utf-8").splitlines(), 1
    ):
        if not line.startswith("| ") or line.startswith(("| ID ", "| ---")):
            continue
        cells = table_cells(line)
        if len(cells) != 4:
            raise ValueError(
                f"knowledge row {line_number} has {len(cells)} unescaped cells"
            )
        identifier, state, fact, evidence = cells
        if not re.fullmatch(r"[A-Z][A-Z0-9-]*", identifier):
            raise ValueError(f"invalid knowledge ID at line {line_number}: {identifier}")
        if state in forbidden_states:
            raise ValueError(
                f"obsolete knowledge state at line {line_number}: {identifier}"
            )
        if not fact or not evidence:
            raise ValueError(f"incomplete knowledge row at line {line_number}: {identifier}")
        ids.append(identifier)
    duplicates = sorted(identifier for identifier, count in Counter(ids).items() if count > 1)
    if duplicates:
        raise ValueError(f"duplicate knowledge IDs: {', '.join(duplicates)}")


def validate_handoff_totals() -> None:
    actual = measures()
    expected = {
        "Function candidates": actual["functions"],
        "Boundary/origin unreviewed": actual["pending"],
        "Reviewed but origin-unresolved": actual["unresolved"],
        "Confirmed authored": actual["authored"],
        "Classified exclusions": actual["excluded"],
        "Source-present authored mappings": actual["implemented"],
        "Canonical exact functions": actual["matches"],
    }
    observed: dict[str, int] = {}
    for line in HANDOFF.read_text(encoding="utf-8").splitlines():
        if not line.startswith("| ") or line.startswith(("| Measure ", "| ---")):
            continue
        cells = table_cells(line)
        if len(cells) != 2 or cells[0] not in expected:
            continue
        observed[cells[0]] = int(cells[1].replace(",", ""))
    if observed != expected:
        raise ValueError(f"handoff totals differ from ledgers: {observed} != {expected}")


def validate_local_links() -> None:
    paths = [ROOT / "README.md", *sorted(DOCS.glob("*.md"))]
    for path in paths:
        text = path.read_text(encoding="utf-8")
        for raw_target in MARKDOWN_LINK.findall(text):
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
        validate_knowledge_base()
        validate_handoff_totals()
        validate_local_links()
    except (OSError, ValueError) as exc:
        print(f"error: documentation validation failed: {exc}", file=sys.stderr)
        return 1
    print("documentation OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
