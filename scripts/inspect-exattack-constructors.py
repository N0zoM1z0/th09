#!/usr/bin/env python3
"""SHA-bound TH09 ExAttack special-member codegen diagnostics, NOT authored exact.

Two complete Japanese TH09 v1.50a functions are reproduced by natural VC7.1
C++ source under *different* pinned optimization profiles. Both the explicit
empty-definition and implicit compiler-generated constructor hypotheses are
observationally identical. Such a codegen match cannot settle source origin.
No match ledger, Factory acceptance or compiler-origin inference is made here.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]

# Independently reviewed original PE extents and x86 address operands.
OWNERS = {
    'exattack_record_constructor': {
        'address': 0x004157A0, 'size': 15,
        'sha256': 'e50af3e2df667c5e2d7a761e3fecfe20b3cd7805fedbb0aeef8f2e68375ea9c0',
        'symbol': '??0ExAttackRecord@@QAE@XZ',
        'fields': ((0x07, 'REL32', '??0ZunTimer@@QAE@XZ', 0x00403390),),
        'profile': 'normal',
    },
    'exattack_controller_constructor': {
        'address': 0x004157B0, 'size': 28,
        'sha256': '39e362e7d5d17bbcc30c972966ca0114607fca2df3c3be4e4aa7d4e18685d81d',
        'symbol': '??0ExAttackController@@QAE@XZ',
        'fields': (
            (0x02, 'DIR32', '??0ExAttackRecord@@QAE@XZ', 0x004157A0),
            (0x14, 'REL32', '??_H@YGXPAXIHP6EPAX0@Z@Z', 0x00401470),
        ),
        'profile': 'size',
    },
}


def validate_source_fields(rows: list[dict], owner: dict) -> None:
    """No field-count-only credit; prove each true COFF kind/symbol/addend."""
    expected = owner['fields']
    if not isinstance(rows, list) or len(rows) != len(expected):
        raise ValueError('constructor actual COFF field count differs from original')
    for row, (offset, kind, symbol, target) in zip(rows, expected):
        if (not isinstance(row, dict) or type(row.get('offset')) is not int or
                row['offset'] != offset or row.get('type') != kind or
                row.get('symbol') != symbol or row.get('addend') != 0):
            raise ValueError('constructor altered original-linked call/data operand')
        if target < 0x00400000 or target >= 0x004E7000:
            raise ValueError('constructor target address outside original PE')


def bind_and_compare(original: bytes, compiled: bytes, rows: list[dict],
                     owner: dict) -> dict:
    """Compare every physical byte after independent, exact target bindings."""
    if (len(original) != owner['size'] or
            hashlib.sha256(original).hexdigest() != owner['sha256']):
        raise ValueError('constructor original PE extent/SHA changed')
    if len(compiled) != owner['size']:
        raise ValueError('constructor source-C++ physical size differs')
    validate_source_fields(rows, owner)
    linked = bytearray(compiled)
    for offset, kind, _, destination in owner['fields']:
        # Verify original PE encoded displacement/address; source can never
        # provide the expected target by choosing its own relocation values.
        value = destination if kind == 'DIR32' else destination - (owner['address'] + offset + 4)
        expected = value & 0xFFFFFFFF
        if struct.unpack_from('<I', original, offset)[0] != expected:
            raise ValueError('constructor PE real operand destination drifted')
        struct.pack_into('<I', linked, offset, expected)
    differences = [offset for offset, (retail, emitted) in
                   enumerate(zip(original, linked)) if retail != emitted]
    if differences:
        raise ValueError(f'constructor not byte-exact after original-bound relocations: '
                         f'{[hex(x) for x in differences]}')
    return {
        'diagnostic_only': True,
        'exactness_credit': 'none',
        'origin_classification': 'unknown/review',
        'original_full_extent_bytes': owner['size'],
        'candidate_full_extent_bytes': len(compiled),
        'original_sha256': owner['sha256'],
        'bound_original_fields': len(rows),
        'original_physical_bytes_equal': True,
        'relocation_target_addresses': [item[3] for item in owner['fields']],
    }


def validate_origin_and_exact_callees(units: dict) -> None:
    import csv

    timer = units.get('zun-timer-constructor')
    if (not isinstance(timer, dict) or timer.get('target_address') != 0x00403390 or
            timer.get('symbol') != '??0ZunTimer@@QAE@XZ'):
        raise ValueError('ExAttackRecord timer constructor is no longer canonical exact')
    with (ROOT / 'config/functions.csv').open(newline='') as stream:
        rows = {row['address']: row for row in csv.DictReader(stream)}
    with (ROOT / 'config/matches.csv').open(newline='') as stream:
        matches = {row['address'] for row in csv.DictReader(stream)}
    for label in ('0x004157A0', '0x004157B0'):
        if (rows[label]['status'] != 'unclassified' or rows[label]['owner'] != 'unknown'
                or label in matches):
            raise ValueError('unknown-origin ExAttack constructors gained false exact credit')


def diagnose(normal: Path, optimized_size: Path) -> dict:
    import capstone

    spec = importlib.util.spec_from_file_location(
        'constructor_coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        units = tomllib.load(stream)['units']
    validate_origin_and_exact_callees(units)
    compiled = {'normal': normal, 'size': optimized_size}
    results = {}
    for name, owner in OWNERS.items():
        original = coff.pe_bytes_at(coff.verified_target(), owner['address'], owner['size'])
        candidate, rows = coff.object_function(compiled[owner['profile']], owner['symbol'])
        # Independent x86 instruction census through the exact original RET.
        decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        ins = list(decoder.disasm(original, owner['address']))
        if (not ins or sum(i.size for i in ins) != owner['size'] or
                ins[-1].mnemonic != 'ret' or ins[-1].op_str):
            raise ValueError('original ExAttack special-member code not fully decoded')
        result = bind_and_compare(original, bytes(candidate), rows, owner)
        result['original_instructions'] = len(ins)
        results[name] = result
    return results


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--normal', type=Path, required=True)
    parser.add_argument('--size-optimized', type=Path, required=True)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        results = diagnose(args.normal, args.size_optimized)
        if args.json:
            print(json.dumps(results, indent=2))
        else:
            for name, entry in results.items():
                print(name, entry['original_full_extent_bytes'], 'physical bytes: '
                      'compiler matches original, origin unknown, NO exact credit')
        return 0
    except (ValueError, KeyError, OSError, ImportError) as exc:
        print('original-bound ExAttack constructor probe rejected:', exc, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
