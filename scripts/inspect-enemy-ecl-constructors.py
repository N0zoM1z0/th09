#!/usr/bin/env python3
"""Original-bound, non-crediting native Enemy ECL constructor-chain audit.

These four compiler-emitted-looking special members at 0x40FCA0 / 0x412090
and 0x4120C0 / 0x4120D0 have unknown authored-vs-implicit origin. Reproducing them exactly from C++
does not change their unclassified ledger status or grant authored credit.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
CHAIN = (
    {
        'name': 'EnemyEclTimelineStateView',
        'base': 0x0040FCA0,
        'size': 28,
        'symbol': '??0EnemyEclTimelineStateView@@QAE@XZ',
        'sha256': '4ecf23defc739298567914e4633047ca2b26210481c071441085e7c246fb32ae',
        'fields': (
            (0x02, 'DIR32', '??0Float3@@QAE@XZ', 0x004343D0),
            (0x14, 'REL32', '??_H@YGXPAXIHP6EPAX0@Z@Z', 0x00401470),
        ),
    },
    {
        'name': 'EnemyEclManagerView',
        'base': 0x00412090,
        'size': 15,
        'symbol': '??0EnemyEclManagerView@@QAE@XZ',
        'sha256': 'e365ff772cfca0d89f78089c221e4a9d74e88052bb3a435843feabed16b97eaa',
        'fields': (
            (0x07, 'REL32', '??0EnemyEclTimelineStateView@@QAE@XZ', 0x0040FCA0),
        ),
    },
    {
        'name': 'Th09EclInterpolationSlotView',
        'base': 0x004120C0,
        'size': 15,
        'symbol': '??0Th09EclInterpolationSlotView@@QAE@XZ',
        'sha256': '3c2c3023df47aafdf493182ccc17b61a4e32ad456b4332af6d662cda1c68c9eb',
        'fields': (
            (0x07, 'REL32', '??0ZunTimer@@QAE@XZ', 0x00403390),
        ),
    },
    {
        'name': 'Th09EclContextView',
        'base': 0x004120D0,
        'size': 47,
        'symbol': '??0Th09EclContextView@@QAE@XZ',
        'sha256': '8057ab5d8e434e5db8598c391f3d527bd7ee8665b15948ca28036748d86520a9',
        'fields': (
            (0x07, 'REL32', '??0ZunTimer@@QAE@XZ', 0x00403390),
            (0x12, 'REL32', '??0ZunTimer@@QAE@XZ', 0x00403390),
            (0x17, 'DIR32', '??0Th09EclInterpolationSlotView@@QAE@XZ', 0x004120C0),
            (0x27, 'REL32', '??_H@YGXPAXIHP6EPAX0@Z@Z', 0x00401470),
        ),
    },
)


def validate_fields(rows: list[dict], expected: tuple) -> None:
    if len(rows) != len(expected):
        raise ValueError(f'constructor fields: expected {len(expected)}, observed {len(rows)}')
    for row, (offset, kind, symbol, _dest) in zip(rows, expected):
        if (row['offset'], row['type'], row['symbol'], row['addend']) != (
            offset, kind, symbol, 0
        ):
            raise ValueError(f'unexpected native constructor operand: {row}')


def audit(obj: Path) -> dict:
    """Check exact constructor code without promoting unknown authoring origin."""
    spec = importlib.util.spec_from_file_location(
        'coff_enemy_constructor_chain', ROOT / 'scripts/compare-coff-function.py'
    )
    if spec is None or spec.loader is None:
        raise ValueError('canonical PE/COFF interpreter unavailable')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    target_file = coff.verified_target()
    reports = []
    for entry in CHAIN:
        base, size = entry['base'], entry['size']
        original = coff.pe_bytes_at(target_file, base, size)
        if hashlib.sha256(original).hexdigest() != entry['sha256']:
            raise ValueError(f'original constructor SHA drift: {entry["name"]}')
        raw, fields = coff.object_function(
            obj, entry['symbol'], include_symbol_locations=True
        )
        if len(raw) != size:
            raise ValueError(f'constructor size drift: {entry["name"]} {len(raw)}/{size}')
        validate_fields(fields, entry['fields'])
        linked = bytearray(raw)
        for row, (offset, kind, _symbol, destination) in zip(
            fields, entry['fields']
        ):
            target_operand, = struct.unpack_from('<I', original, offset)
            encoded = (destination if kind == 'DIR32'
                       else (destination - (base + offset + 4)) & 0xFFFFFFFF)
            if encoded != target_operand:
                raise ValueError(
                    f'original constructor destination disagrees at '
                    f'{entry["name"]}+0x{offset:X}'
                )
            struct.pack_into('<I', linked, offset, encoded)
        reports.append({
            'name': entry['name'],
            'target_address': hex(base),
            'target_bytes': size,
            'candidate_bytes': len(raw),
            'original_fields': len(entry['fields']),
            'bound_fields': len(fields),
            'complete_bytes_equal': linked == original,
            'difference_offsets': [
                i for i, (a, b) in enumerate(zip(original, linked)) if a != b
            ],
            'original_sha256': entry['sha256'],
        })
    return {
        'diagnostic_only': True,
        'exactness_credit': 'none',
        'origin': 'unknown; special-member authorship not inferred',
        'constructors': reports,
    }


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('object', type=Path)
    ap.add_argument('--json', action='store_true')
    args = ap.parse_args()
    try:
        result = audit(args.object)
    except (KeyError, IndexError, OSError, RuntimeError, ValueError, struct.error) as exc:
        print(json.dumps({'error': str(exc)}))
        return 2
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        for entry in result['constructors']:
            print(f'{entry["name"]}: {entry["candidate_bytes"]}/'
                  f'{entry["target_bytes"]} bytes, {entry["bound_fields"]} fields, '
                  f'equal={entry["complete_bytes_equal"]} (unclassified, no credit)')
    return 0 if all(x['complete_bytes_equal'] for x in result['constructors']) else 1


if __name__ == '__main__':
    raise SystemExit(main())
