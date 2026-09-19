"""Line-preserving helpers for TH09 tracking CSV files."""
from __future__ import annotations

import csv
import io
from pathlib import Path
from typing import Callable


def read_rows(path: Path) -> list[dict[str, str]]:
    with path.open(newline="", encoding="utf-8") as stream:
        reader = csv.DictReader(stream)
        result = []
        for line_number, row in enumerate(reader, 2):
            if None in row or any(value is None for value in row.values()):
                raise ValueError(f"malformed CSV row in {path} at line {line_number}")
            result.append(row)
        return result


def rows_by_address(path: Path) -> dict[str, dict[str, str]]:
    rows = read_rows(path)
    result = {row["address"]: row for row in rows}
    if len(result) != len(rows):
        raise ValueError(f"duplicate addresses in {path}")
    return result


def render_row(fields: list[str], row: dict[str, str]) -> str:
    output = io.StringIO(newline="")
    writer = csv.DictWriter(output, fieldnames=fields, lineterminator="\n")
    writer.writerow(row)
    return output.getvalue()


def rewrite_selected_rows(
    path: Path,
    selected: set[str],
    mutate: Callable[[dict[str, str]], None],
    write: bool,
) -> int:
    """Mutate selected address rows without normalizing untouched CSV lines."""
    lines = path.read_text(encoding="utf-8").splitlines(keepends=True)
    if not lines:
        raise ValueError(f"empty CSV: {path}")
    fields = next(csv.reader([lines[0]]))
    output = [lines[0]]
    updated = 0
    seen: set[str] = set()
    for line in lines[1:]:
        values = next(csv.reader([line]))
        address = values[0] if values else ""
        if address not in selected:
            output.append(line)
            continue
        if len(values) != len(fields):
            raise ValueError(
                f"selected row has {len(values)} values for {len(fields)} fields: {address}"
            )
        row = dict(zip(fields, values))
        mutate(row)
        output.append(render_row(fields, row))
        updated += 1
        seen.add(address)
    missing = selected - seen
    if missing:
        raise ValueError(f"selected addresses missing from {path.name}: {sorted(missing)}")
    if write:
        temporary = path.with_name(path.name + ".tmp")
        temporary.write_text("".join(output), encoding="utf-8")
        temporary.replace(path)
    return updated
