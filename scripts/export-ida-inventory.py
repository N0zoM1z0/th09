#!/usr/bin/env python3
"""Initialize provisional TH09 function ledgers from the attested IDA database."""

from __future__ import annotations

import argparse
import asyncio
import csv
import json
from pathlib import Path
from typing import Any

from ida_mcp_client import DEFAULT_SERVER, IdaMcpError, call_json, open_session, parse_int, require_target


ROOT = Path(__file__).resolve().parents[1]
ANALYSIS = ROOT / ".analysis" / "ida-functions.json"
FUNCTIONS = ROOT / "config" / "functions.csv"
ORIGINS = ROOT / "config" / "function-origins.csv"
RECCMP = ROOT / "config" / "reccmp-functions.csv"
FUNCTION_FIELDS = [
    "address", "size", "span_end", "current_name", "proposed_name", "module",
    "status", "match_percent", "calling_convention", "signature", "is_thunk",
    "source_file", "evidence", "owner", "notes",
]
ORIGIN_FIELDS = ["address", "origin", "subsystem", "disposition", "confidence", "evidence_id"]


def canonical(value: int) -> str:
    return f"0x{value:08X}"


def normalize_page(payload: Any) -> tuple[list[dict[str, Any]], int | None]:
    if isinstance(payload, list):
        return payload, None
    if not isinstance(payload, dict) or not isinstance(payload.get("data"), list):
        raise IdaMcpError("tool_error", f"unexpected list_functions result: {payload!r}")
    next_offset = payload.get("next_offset")
    return payload["data"], None if next_offset is None else parse_int(next_offset)


async def read_inventory(server: str) -> tuple[dict[str, Any], list[dict[str, Any]]]:
    async with open_session(server) as (session, _initialized):
        metadata = await require_target(session)
        rows: list[dict[str, Any]] = []
        offset = 0
        while True:
            page, next_offset = normalize_page(
                await call_json(session, "list_functions", {"offset": offset, "count": 1000})
            )
            rows.extend(page)
            if next_offset is None:
                break
            if next_offset <= offset:
                raise IdaMcpError("tool_error", "IDA function pagination did not advance")
            offset = next_offset
        return metadata, rows


def validate_rows(raw_rows: list[dict[str, Any]]) -> list[dict[str, Any]]:
    rows = []
    seen: set[int] = set()
    for raw in raw_rows:
        address, size, name = parse_int(raw.get("address")), parse_int(raw.get("size")), str(raw.get("name", ""))
        if address in seen or size <= 0 or not name:
            raise ValueError(f"invalid or duplicate IDA function row: {raw!r}")
        seen.add(address)
        rows.append({"address": address, "size": size, "name": name})
    rows.sort(key=lambda row: row["address"])
    return rows


def require_empty_ledger() -> None:
    with FUNCTIONS.open(newline="", encoding="utf-8") as stream:
        if any(csv.DictReader(stream)):
            raise ValueError(
                "functions.csv is not empty; initialization will not overwrite reviewed state"
            )


def write_inventory(metadata: dict[str, Any], rows: list[dict[str, Any]]) -> None:
    require_empty_ledger()
    ANALYSIS.parent.mkdir(parents=True, exist_ok=True)
    ANALYSIS.write_text(
        json.dumps({"authority": "provisional-semantic-analysis", "target": metadata, "functions": rows}, indent=2) + "\n",
        encoding="utf-8",
    )
    with FUNCTIONS.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=FUNCTION_FIELDS, lineterminator="\n")
        writer.writeheader()
        for row in rows:
            writer.writerow(
                {
                    "address": canonical(row["address"]),
                    "size": row["size"],
                    "span_end": canonical(row["address"] + row["size"] - 1),
                    "current_name": row["name"],
                    "status": "unclassified",
                    "match_percent": "0.00",
                    "is_thunk": "false",
                    "evidence": "Attested IDA 1.50a initial candidate; boundary unreviewed",
                }
            )
    with ORIGINS.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=ORIGIN_FIELDS, lineterminator="\n")
        writer.writeheader()
        for row in rows:
            writer.writerow(
                {
                    "address": canonical(row["address"]),
                    "origin": "unknown",
                    "disposition": "review",
                    "confidence": "unknown",
                    "evidence_id": "ida-1.50a-initial-inventory",
                }
            )
    RECCMP.write_text("name,address,type\n", encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--server", default=DEFAULT_SERVER)
    parser.add_argument("--initialize", action="store_true")
    args = parser.parse_args()
    if not args.initialize:
        parser.error("refusing to write ledgers without --initialize")
    try:
        metadata, raw_rows = asyncio.run(read_inventory(args.server))
        rows = validate_rows(raw_rows)
        write_inventory(metadata, rows)
    except (IdaMcpError, OSError, TypeError, ValueError) as exc:
        print(f"error: IDA inventory initialization failed: {exc}")
        return 1
    print(f"initialized {len(rows)} provisional IDA function candidates")
    print(f"private snapshot: {ANALYSIS.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
