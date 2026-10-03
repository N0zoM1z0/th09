#!/usr/bin/env python3
"""Read-only complete comparison for the opponent-selection owner.

Bindings come from independently reviewed canonical methods/globals and the
Packet437 opponent tables, never from solving candidate fields against target
bytes. A partial match is diagnostic only and does not update any ledger.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import tomllib

ROOT = Path(__file__).resolve().parents[1]
ADDRESS = 0x00415910
SIZE = 799
SYMBOL = '?SelectOpponentConfiguration@GameManagerOpponentSelectionView@@QAEXXZ'
# These private table names have no exact function-owner manifest of their own.
# Their physical identities were reviewed from the original table/caller data.
TABLE_IDENTITIES = {
    '?g_OpponentEntries@@3PAUOpponentSelectionEntry@@A': 0x004A13B8,
    '?g_OpponentLists@@3PAPAUOpponentSelectionEntry@@A': 0x004A14C8,
    '?g_OpponentModeValues@@3PAHA': 0x004A1884,
    '?g_OpponentParameterTable@@3PAHA': 0x004A1500,
    '?g_Rng@@3URngRuntimeView@@A': 0x004ACE0C,
}


def inspect(object_path):
    spec = importlib.util.spec_from_file_location('coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']
    known = {}
    for unit in units.values():
        known.setdefault(unit['symbol'], set()).add(unit['target_address'])
        for field in unit.get('relocations', []):
            known.setdefault(field['symbol'], set()).add(field['target'])

    image = coff.verified_target()
    if coff.pe_bytes_at(image, 0x0048E314, 4) != b'\0' * 4:
        raise ValueError('reviewed float comparison constant is not zero')
    raw, rows = coff.object_function(object_path, SYMBOL)
    relocated = bytearray(raw)
    covered = set()
    bindings = []
    for row in rows:
        symbol = row['symbol']
        destination = TABLE_IDENTITIES.get(symbol)
        if destination is None:
            destinations = known.get(symbol, set())
            if len(destinations) != 1:
                raise ValueError(f'unreviewed or ambiguous identity: {symbol}')
            destination = next(iter(destinations))
        offset = row['offset']
        field_bytes = set(range(offset, offset + 4))
        if row['type'] not in ('DIR32', 'REL32') or offset < 0 or offset + 4 > len(raw):
            raise ValueError(f'unsupported/out-of-range field: {row}')
        if covered & field_bytes:
            raise ValueError(f'overlapping field: {row}')
        covered.update(field_bytes)
        value = destination + row['addend']
        if row['type'] == 'REL32':
            value -= ADDRESS + offset + 4
        struct.pack_into('<I', relocated, offset, value & 0xFFFFFFFF)
        bindings.append(dict(row, target=destination))

    target = coff.pe_bytes_at(image, ADDRESS, SIZE)
    return {
        'status': 'diagnostic only; canonical unit and accepted replay determine credit',
        'size': len(raw),
        'target_size': SIZE,
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
        'complete_equal': relocated == target,
        'differences': [dict(offset=hex(i), candidate=a, target=b)
                        for i, (a, b) in enumerate(zip(relocated, target)) if a != b],
        'bindings': bindings,
    }


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    args = parser.parse_args()
    print(json.dumps(inspect(args.object), indent=2))
