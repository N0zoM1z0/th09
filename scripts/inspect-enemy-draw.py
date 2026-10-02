#!/usr/bin/env python3
"""Read-only complete Enemy draw diagnostic; never grants exactness credit.

Call and global identities are independently reviewed TH09 entries. Literals
must agree with existing canonical bindings and verified PE contents. No target
relocation field is solved backwards to produce a symbol destination.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import tomllib

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x411670
SIZE = 1758
SYMBOL = '?EnemyManagerDrawImpl@@YIHPAUEnemyManagerView@@HH@Z'
DESTINATIONS = {'??HFloat3@@QBE?AU0@ABU0@@Z': 4198656,
 '?Draw2D@EnemyDrawAnmManagerView@@QAEHPAUEnemyDrawVmView@@@Z': 4434768,
 '?DrawVertices@EnemyDrawAnmManagerView@@QAEHPAUEnemyDrawVmView@@PAUEnemyDrawVertex@@H@Z': 4437200,
 '?EnemyDrawAbs@@YGOM@Z': 4256192,
 '?EnemyDrawCos@@YGMM@Z': 4198496,
 '?EnemyDrawInterpolateWrappedAngle@@YGMMMM@Z': 4257248,
 '?EnemyDrawSin@@YGMM@Z': 4198512,
 '?SetZRotation@EnemyDrawVmView@@QAEXM@Z': 4257856,
 '?TransformPopupX@EnemyDrawGameManagerView@@QAEMM@Z': 4200064,
 '?TransformPopupY@EnemyDrawGameManagerView@@QAEMM@Z': 4200112,
 '?g_AnmManager@@3PAUEnemyDrawAnmManagerView@@A': 5096784,
 '?g_GameManager@@3UEnemyDrawGameManagerView@@A': 4881808}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    args = parser.parse_args()
    spec = importlib.util.spec_from_file_location(
        'coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    image = coff.verified_target()
    target = coff.pe_bytes_at(image, BASE, SIZE)
    raw, rows = coff.object_function(
        args.object, SYMBOL, include_symbol_locations=True)
    code = bytearray(raw)
    units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']
    known = {}
    for unit in units.values():
        for row in unit.get('relocations', []):
            known.setdefault(row['symbol'], set()).add(row['target'])
    resolved = []
    occupied = set()
    for row in rows:
        symbol = row['symbol']
        if symbol.startswith('__real@'):
            choices = known.get(symbol, set())
            if len(choices) != 1:
                raise ValueError(f'ambiguous or missing literal: {symbol}')
            destination = next(iter(choices))
            bits = int(symbol[7:], 16)
            if coff.pe_bytes_at(image, destination, 4) != struct.pack('<I', bits):
                raise ValueError(f'literal contents disagree: {symbol}')
        else:
            destination = DESTINATIONS[symbol]
        offset = row['offset']
        field = set(range(offset, offset + 4))
        if offset < 0 or offset + 4 > len(code) or occupied.intersection(field):
            raise ValueError('out-of-range or overlapping relocation field')
        occupied.update(field)
        value = destination + row['addend']
        if row['type'] == 'REL32':
            value -= BASE + offset + 4
        elif row['type'] != 'DIR32':
            raise ValueError(f'unsupported relocation type: {row["type"]}')
        struct.pack_into('<I', code, offset, value & 0xFFFFFFFF)
        resolved.append(dict(row, target=destination))
    differences = [
        dict(offset=index, target=expected, candidate=actual)
        for index, (expected, actual) in enumerate(zip(target, code))
        if expected != actual
    ]
    print(json.dumps(dict(
        status='diagnostic-only',
        raw_sha256=hashlib.sha256(raw).hexdigest(),
        bytes=len(raw), expected_bytes=SIZE, fields=len(rows),
        complete_bytes_equal=code == target,
        overlapping_differences=differences,
        relocations=resolved), indent=2))


if __name__ == '__main__':
    main()
