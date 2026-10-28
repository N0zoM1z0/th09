#!/usr/bin/env python3
"""Validate maintained TH09 documentation against repository state."""
from __future__ import annotations

from collections import Counter
from pathlib import Path
import re
import sys
import tomllib

from progress import measures, rows


ROOT = Path(__file__).resolve().parents[1]
AUTHORED_EXACT_GOAL_PERCENT = 98
DOCS = ROOT / "docs"
KNOWLEDGE = DOCS / "KNOWLEDGE_BASE.md"
HANDOFF = DOCS / "RE_HANDOFF.md"
RECEIPTS_ARCHIVE = DOCS / "RE_ACCEPTED_RECEIPTS.md"
FRONTIER = DOCS / "SMALL_FUNCTION_FRONTIER.md"
README = ROOT / "README.md"
TABLE_SPLIT = re.compile(r"(?<!\\)\|")
MARKDOWN_LINK = re.compile(r"\[[^\]]*\]\(([^)]+)\)")
FENCE = re.compile(r"^ {0,3}(?P<marker>`{3,}|~{3,})(?P<rest>.*)$")
CODE_SPAN = re.compile(r"(?<!`)(`+)(?!`)(.*?)\1(?!`)", re.DOTALL)
PRIMARY_ADDRESS = re.compile(r"0x[0-9A-Fa-f]{8}")
STALE_EXACT_STATE = re.compile(
    r"non[- ]?exact|near[- ]?match|codegen-blocked|persistent-nonexact|hard-frontier",
    re.IGNORECASE,
)


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
    exact_addresses = {
        row["address"].lower() for row in rows("matches.csv")
    }
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
        primary_address = PRIMARY_ADDRESS.search(fact)
        if (
            primary_address is not None
            and primary_address.group(0).lower() in exact_addresses
            and STALE_EXACT_STATE.search(state)
        ):
            raise ValueError(
                "stale knowledge state for exact primary function at "
                f"line {line_number}: {identifier} ({state})"
            )
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
        if cells[0] in observed:
            raise ValueError(f"duplicate handoff total: {cells[0]}")
        observed[cells[0]] = int(cells[1].replace(",", ""))
    if observed != expected:
        raise ValueError(f"handoff totals differ from ledgers: {observed} != {expected}")


def validate_accepted_owner_table(
    text: str, match_rows: list[dict[str, str]], units: dict[str, dict]
) -> None:
    """Keep the historical accepted-owner archive tied to live manifests.

    Receipt acceptance itself is established by the separate Factory Oracle;
    this validates only local owner identity, bytes, fields and receipt syntax.
    """
    section = "## Closed codegen landmarks and accepted receipts"
    if text.count(section) != 1:
        raise ValueError("accepted-receipt archive section is missing or duplicated")
    body = text.split(section, 1)[1].split("\n## ", 1)[0]
    if re.search(r"\b(?:receipt pending|acceptance pending)\b", body, re.I):
        raise ValueError("accepted-owner section still claims a pending receipt")

    by_address = {row["address"].lower(): row for row in match_rows}
    seen: set[str] = set()
    active = False
    for line in body.splitlines():
        if line.startswith("| Exact owner |"):
            if active:
                raise ValueError("duplicate accepted-owner table")
            active = True
            continue
        if not active:
            continue
        if not line.startswith("| "):
            if seen:
                break
            continue
        if line.startswith("| ---"):
            continue
        cells = table_cells(line)
        if len(cells) != 4:
            raise ValueError("accepted-owner row requires four cells")
        owner, description, unit_cell, receipt_cell = cells
        address = re.search(r"`(0x[0-9A-Fa-f]{8})`", description)
        sizes = re.search(r"\*\*([\d,]+)/([\d,]+)\*\*", description)
        fields = re.search(r"(\d+) fields", description)
        unit_name = re.fullmatch(r"`([a-z0-9-]+)`", unit_cell)
        commit = re.search(r"`([0-9a-f]{7,40})`", receipt_cell)
        receipt = re.search(r"`receipt:([0-9a-f]{64})`", receipt_cell)
        if not all((address, sizes, fields, unit_name, commit, receipt)):
            raise ValueError(f"accepted-owner evidence is incomplete: {owner}")
        canonical_name = unit_name.group(1)
        if canonical_name in seen:
            raise ValueError(f"duplicate accepted exact unit: {canonical_name}")
        seen.add(canonical_name)
        unit = units.get(canonical_name)
        match = by_address.get(address.group(1).lower())
        if unit is None or match is None:
            raise ValueError(f"accepted owner lacks canonical exact ledger: {canonical_name}")
        expected_size = int(unit["size"])
        if not (
            int(unit["target_address"]) == int(address.group(1), 16)
            and unit["functions"] == [owner.strip("`")]
            and match["name"] == owner.strip("`")
            and match["unit"] == canonical_name
            and match["status"] == "matching"
            and int(sizes.group(1).replace(",", "")) == expected_size
            and int(sizes.group(2).replace(",", "")) == expected_size
            and int(fields.group(1)) == len(unit["relocations"])
        ):
            raise ValueError(f"accepted owner differs from live manifest: {canonical_name}")
    if not seen:
        raise ValueError("accepted-receipt archive requires reviewed owner rows")


def validate_handoff_derived_summary(text: str, actual: dict[str, int]) -> None:
    """Do not let prose silently retain yesterday's exactness snapshot.

    The live totals table is checked separately; this checks the percentage,
    current campaign goal/gap, and restart-summary copy of those same numbers.
    Historical packet counters belong only in KNOWLEDGE_BASE/Git history.
    """
    reviewed_bytes = actual["authored_bytes"]
    exact_bytes = actual["exact_bytes"]
    if reviewed_bytes <= 0 or not 0 <= exact_bytes <= reviewed_bytes:
        raise ValueError("invalid authored-byte denominator in live ledger")

    ratio = (
        f"**{exact_bytes:,} / {reviewed_bytes:,} reviewed-authored bytes = "
        f"{100 * exact_bytes / reviewed_bytes:.4f}% exact**"
    )
    if text.count(ratio) != 1:
        raise ValueError("handoff exact-byte percentage differs from live ledgers")

    # Integer ceiling of the current goal share, without floating-point rounding.
    target_bytes = (AUTHORED_EXACT_GOAL_PERCENT * reviewed_bytes + 99) // 100
    remaining = max(0, target_bytes - exact_bytes)
    goal = re.search(r"To reach\s+(\d+)%", text)
    if goal is None or int(goal.group(1)) != AUTHORED_EXACT_GOAL_PERCENT:
        raise ValueError("handoff authored-byte goal differs from current campaign")
    gap = re.search(r"(?:another|remaining) \*\*([\d,]+)\s+bytes\*\*", text)
    if gap is None or int(gap.group(1).replace(",", "")) != remaining:
        raise ValueError(
            f"handoff {AUTHORED_EXACT_GOAL_PERCENT}% gap differs from live ledgers"
        )

    snapshot = (
        f"**{actual['implemented']} source-present / "
        f"{actual['matches']} exact / {actual['source_nonexact']} non-exact**"
    )
    if text.count(snapshot) != 1:
        raise ValueError("handoff restart snapshot differs from live ledgers")


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


def validate_frontier() -> None:
    text = FRONTIER.read_text(encoding="utf-8")
    counts = re.search(
        r"yields \*\*(\d+) functions\*\*.*?"
        r"of which \*\*(\d+) (?:are|is) at most\s+128 bytes\*\*.*?"
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
        validate_handoff_derived_summary(
            HANDOFF.read_text(encoding="utf-8"), measures()
        )
        with (ROOT / "config" / "match-units.toml").open("rb") as handle:
            match_units = tomllib.load(handle)["units"]
        validate_accepted_owner_table(
            RECEIPTS_ARCHIVE.read_text(encoding="utf-8"),
            rows("matches.csv"), match_units
        )
        validate_active_frontiers(
            HANDOFF.read_text(encoding="utf-8"),
            rows("functions.csv"),
            {row["address"] for row in rows("matches.csv")},
        )
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
