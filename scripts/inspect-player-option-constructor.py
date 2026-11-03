#!/usr/bin/env python3
"""Original-bound, *non-crediting* PE/COFF proof of Player option special member.

The terminal-return function at 0x0041F010 is still ledger-unclassified:
explicit source authorship vs VC7.1 implicit generation is unresolved.
Proving an equivalent source constructor does not change its origin/status.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x0041F010
SIZE = 23
ORIGINAL_SHA256 = 'eb751682fb036efdc7f0c63d063a5cb4f64ceb3208c1d792c662951944edc92d'
SYMBOL = '??0PlayerOptionStateCtorView@@QAE@XZ'
FIELDS = (
    (4, 'REL32', '??0AnmVm@@QAE@XZ', 0x00403A10),
    (15, 'REL32', '??0ZunTimer@@QAE@XZ', 0x00403390),
)


def inspect(obj: Path) -> dict:
    spec = importlib.util.spec_from_file_location('coff', ROOT/'scripts/compare-coff-function.py')
    if not spec or not spec.loader:
        raise ValueError('canonical COFF/PE interpreter is not available')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    original = coff.pe_bytes_at(coff.verified_target(), BASE, SIZE)
    if hashlib.sha256(original).hexdigest() != ORIGINAL_SHA256:
        raise ValueError('original Japanese 1.50a option special-member extent is not verified')
    raw, rows = coff.object_function(obj, SYMBOL, include_symbol_locations=True)
    if len(raw) != SIZE or len(rows) != len(FIELDS):
        raise ValueError('candidate does not have the original physical bytes/two call fields')
    bound = bytearray(raw)
    for row, (offset, kind, symbol, destination) in zip(rows, FIELDS):
        if (row['offset'], row['type'], row['symbol'], row['addend']) != (offset, kind, symbol, 0):
            raise ValueError(f'non-original option constructor relocation: {row}')
        original_word, = struct.unpack_from('<I', original, offset)
        encoded = (destination - (BASE + offset + 4)) & 0xFFFFFFFF
        if original_word != encoded:
            raise ValueError('original PE operand is not the independently reviewed native ctor')
        struct.pack_into('<I', bound, offset, encoded)
    return {
        'owner_address': hex(BASE),
        'original_physical_bytes': SIZE,
        'candidate_physical_bytes': len(raw),
        'relocations': len(rows),
        'original_sha256': ORIGINAL_SHA256,
        'candidate_linked_sha256': hashlib.sha256(bound).hexdigest(),
        'complete_original_bytes_equal': bound == original,
        'status': 'unclassified special member; non-crediting source equivalence',
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        result = inspect(args.object)
    except (OSError, KeyError, ValueError, IndexError, struct.error) as exc:
        print(json.dumps({'error': str(exc)}))
        return 2
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        print(f"Option constructor 0x{BASE:08X}: "
              f"{SIZE} bytes / {result['relocations']} native calls; "
              f"exact={result['complete_original_bytes_equal']} (unclassified, no exact credit)")
    return 0 if result['complete_original_bytes_equal'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
