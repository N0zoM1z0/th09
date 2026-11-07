#!/usr/bin/env python3
"""Original-bound complete 23x5 Etama ANM sprite-script table audit.

The 832-byte AddedCallback code uses the actual VC7.1 .rdata definition.
Reconstruct and check all 460 table bytes *and* all seven original pointer
operands, not only symbol spelling or the function's 4-byte relocations.
This verifies one source/data owner; it does not grant any new exact credit.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
TARGET_CALLER = 0x00413E40
CALLER_PHYSICAL_SIZE = 832
TABLE_VA = 0x0048E628
ROWS, COLUMNS, WORD_BYTES = 23, 5, 4
TABLE_SIZE = ROWS * COLUMNS * WORD_BYTES
TABLE_SHA256 = '0879b881ea6bbb3f36369729f6ddc99e83aa56eb95df941c0673ea31024df577'
SYMBOL = '?AddedCallback@EtamaController@@SIHPAU1@@Z'
TABLE_SYMBOL = '_kBulletSpriteScripts'
TABLE_FIELD_SITES = (
    (0x52, 0), (0x6B, 4), (0x84, 8), (0x97, 12), (0xB0, 16),
    (0x170, 0), (0x1DC, 0),
)


def load(name: str, filename: str):
    path = ROOT / 'scripts' / filename
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise ValueError('missing target-bound COFF reader: ' + filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def verify_references(fields: list[dict], image, coff) -> list[dict]:
    """Demand seven actual source table definitions with original pointer roles."""
    relevant = [row for row in fields if row['symbol'] == TABLE_SYMBOL]
    if len(relevant) != len(TABLE_FIELD_SITES):
        raise ValueError('Etama sprite table use count differs from the original')
    definition = {(r.get('symbol_section'), r.get('symbol_value')) for r in relevant}
    if (len(definition) != 1 or
            any(type(section) is not int or section <= 0 or
                type(value) is not int or value < 0 for section, value in definition)):
        raise ValueError('Etama table lacks a single initialized native definition')
    for row, (offset, addend) in zip(relevant, TABLE_FIELD_SITES):
        if (row.get('offset') != offset or row.get('type') != 'DIR32' or
                row.get('addend') != addend or
                row.get('symbol') != TABLE_SYMBOL):
            raise ValueError('Etama sprite table source COFF reference changed')
        original_field = coff.pe_bytes_at(image, TARGET_CALLER + offset, 4)
        if int.from_bytes(original_field, 'little') != TABLE_VA + addend:
            raise ValueError('Etama sprite table original operand changed')
    return relevant


def verify_payload(original: bytes, source: bytes, *, original_hash: str = TABLE_SHA256) -> None:
    """Check the whole table; a matching COFF label never suffices."""
    if (len(original) != TABLE_SIZE or len(source) != TABLE_SIZE or
            hashlib.sha256(original).hexdigest() != original_hash or
            source != original):
        raise ValueError('Etama 460-byte original sprite-script table differs')


def inspect(object_path: Path) -> dict:
    coff = load('etama_original_coff', 'compare-coff-function.py')
    literal = load('etama_original_literals', 'coff_literal_definitions.py')
    image = coff.verified_target()
    source_code, fields = coff.object_function(
        object_path, SYMBOL, include_symbol_locations=True)
    if len(source_code) != CALLER_PHYSICAL_SIZE:
        raise ValueError('Etama native AddedCallback physical extent changed')
    relevant = verify_references(fields, image, coff)
    source_table = literal.literal_payload(object_path.read_bytes(), relevant[0], TABLE_SIZE)
    original_table = coff.pe_bytes_at(image, TABLE_VA, TABLE_SIZE)
    verify_payload(original_table, source_table)
    return {
        'diagnostic_only': True,
        'exactness_credit': 'none; table-data evidence for already exact caller',
        'target_table_va': f'0x{TABLE_VA:08X}',
        'table_bytes': TABLE_SIZE,
        'rows': ROWS, 'columns': COLUMNS, 'element_bytes': WORD_BYTES,
        'references': [(r['offset'], r['type'], r['addend']) for r in relevant],
        'table_sha256': hashlib.sha256(source_table).hexdigest(),
        'table_original_equal': True,
        'object_sha256': hashlib.sha256(object_path.read_bytes()).hexdigest(),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', nargs='?', type=Path,
                        default=ROOT / 'build/matching/EtamaControllerDraw.obj')
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        report = inspect(args.object)
    except (OSError, KeyError, TypeError, ValueError, IndexError) as exc:
        print('Etama sprite-script table audit rejected: ' + str(exc), file=sys.stderr)
        return 1
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print(f"Etama sprite-script table original-equal: {report['table_bytes']} "
              f"bytes; {len(report['references'])} bound original field operands")
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
