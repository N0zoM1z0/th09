#!/usr/bin/env python3
"""Validate maintained TH09 documentation against repository state."""
from __future__ import annotations

from collections import Counter
from pathlib import Path
import re
import sys

from progress import measures, rows


ROOT = Path(__file__).resolve().parents[1]
DOCS = ROOT / "docs"
KNOWLEDGE = DOCS / "KNOWLEDGE_BASE.md"
HANDOFF = DOCS / "RE_HANDOFF.md"
FRONTIER = DOCS / "SMALL_FUNCTION_FRONTIER.md"
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


def validate_knowledge_base() -> None:
    ids: list[str] = []
    forbidden_states = {"superseded", "historical / superseded-codegen-diagnostic"}
    knowledge = KNOWLEDGE.read_text(encoding="utf-8")
    for line_number, line in enumerate(
        knowledge.splitlines(), 1
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
    packet_ids = re.findall(r"^## Packet (\d+[A-Z]?)\b", knowledge, re.MULTILINE)
    duplicate_packets = sorted(
        identifier for identifier, count in Counter(packet_ids).items() if count > 1
    )
    if duplicate_packets:
        raise ValueError(f"duplicate knowledge packets: {', '.join(duplicate_packets)}")


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
        "Source-present non-exact functions": actual["source_nonexact"],
        "Source-present non-exact bytes": actual["source_nonexact_bytes"],
        "Authored without maintained source": actual["without_source"],
        "Canonical exact authored bytes": actual["exact_bytes"],
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


def validate_frontier() -> None:
    text = FRONTIER.read_text(encoding="utf-8")
    counts = re.search(
        r"yields \*\*(\d+) functions\*\*.*?"
        r"of which \*\*(\d+) are at most\s+128 bytes\*\*.*?"
        r"canonical total is \*\*(\d+) exact functions\*\*",
        text,
        re.DOTALL,
    )
    if counts is None:
        raise ValueError("short-function frontier summary is missing")

    origins = {row["address"]: row["disposition"] for row in rows("function-origins.csv")}
    exact = {row["address"] for row in rows("matches.csv")}
    expected = sorted(
        (row["address"], row["size"], row["module"], row["proposed_name"])
        for row in rows("functions.csv")
        if origins[row["address"]] == "authored"
        and row["source_file"]
        and row["address"] not in exact
        and int(row["size"], 0) <= 256
    )
    observed = []
    in_table = False
    for line in text.splitlines():
        if line.startswith("| Target address |"):
            in_table = True
            continue
        if not in_table or line.startswith("| ---"):
            continue
        if not line.startswith("| "):
            break
        address, size, module, name = table_cells(line)
        observed.append((address, size, module, name))
    if sorted(observed) != expected:
        raise ValueError("short-function frontier rows differ from live ledgers")
    summary = (len(expected), sum(int(row[1], 0) <= 128 for row in expected), len(exact))
    if tuple(map(int, counts.groups())) != summary:
        raise ValueError("short-function frontier counts differ from live ledgers")


def validate_live_guidance() -> None:
    actual = measures()
    knowledge = KNOWLEDGE.read_text(encoding="utf-8")
    inventory_rows = [
        line for line in knowledge.splitlines() if line.startswith("| INVENTORY-001 |")
    ]
    if len(inventory_rows) != 1:
        raise ValueError("knowledge base must contain exactly one INVENTORY-001 row")
    if actual["pending"] == 0 and "Every currently tracked candidate" not in inventory_rows[0]:
        raise ValueError("INVENTORY-001 no longer records complete boundary/origin review")
    if actual["pending"] != 0 and "Every currently tracked candidate" in inventory_rows[0]:
        raise ValueError("INVENTORY-001 claims complete review while candidates are pending")

    readme = README.read_text(encoding="utf-8")
    if "export TH09_TARGET_PATH" in readme:
        raise ValueError("README must not route normal work through TH09_TARGET_PATH")
    if "resources/th09.exe" not in readme:
        raise ValueError("README must name the canonical ignored target path")
    if "Factory/Web" not in readme or "th09-ida" not in readme:
        raise ValueError("README must distinguish Factory/Web IDA routing")


def validate_local_links() -> None:
    paths = [README, *sorted(DOCS.glob("*.md"))]
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
        validate_knowledge_base()
        validate_handoff_totals()
        validate_frontier()
        validate_live_guidance()
        validate_local_links()
    except (OSError, ValueError) as exc:
        print(f"error: documentation validation failed: {exc}", file=sys.stderr)
        return 1
    print("documentation OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
