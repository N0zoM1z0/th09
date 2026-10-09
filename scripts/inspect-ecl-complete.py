#!/usr/bin/env python3
"""Compare every RunEcl code/table byte after independently binding all fields.

Diagnostic only: graph correspondence is used to check identities, never to
move instructions, rebase local labels, mask fields, or grant exactness credit.
"""

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x004086C0
SIZE = 15564
SYMBOL = '?RunEcl@EclManager@@QAEHPAUEnemyView@@@Z'


def load_module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / filename)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def link_owner(raw, rows, external_values, base=BASE):
    """External values include addends; local values come only from COFF.

    In particular, a graph-paired target table entry must not replace the actual
    candidate label position. Doing that would hide shifted switch handlers.
    """
    linked = bytearray(raw)
    occupied = set()
    for row in rows:
        offset = row['offset']
        field = set(range(offset, offset + 4))
        if offset < 0 or offset + 4 > len(raw) or occupied & field:
            raise ValueError('out-of-range or overlapping relocation field')
        occupied.update(field)
        if row['type'] not in ('DIR32', 'REL32'):
            raise ValueError('unsupported relocation type')
        if row['symbol_section'] > 0 and row['symbol_section'] == row['owner_section']:
            relative = row['symbol_value'] - row['owner_value'] + row['addend']
            if not 0 <= relative < len(raw):
                raise ValueError('local relocation destination outside owner')
            value = base + relative
        else:
            value = external_values.get(offset)
            if value is None:
                raise ValueError(f'unbound external field at {offset:#x}')
        if row['type'] == 'REL32':
            value -= base + offset + 4
        struct.pack_into('<I', linked, offset, value & 0xFFFFFFFF)
    return linked, occupied


def report(object_path):
    compare = load_module('ecl_complete_coff', 'compare-coff-function.py')
    audit_module = load_module('ecl_complete_identity', 'audit-ecl-callsite-identities.py')
    identity = audit_module.audit(object_path)
    if not identity['audit_passed']:
        raise ValueError('independent identity audit failed: ' + ', '.join(identity['failures']))
    raw, rows = compare.object_function(object_path, SYMBOL, include_symbol_locations=True)
    if len(raw) != SIZE or len(rows) != identity['accounted_relocations']:
        raise ValueError('incomplete physical extent or relocation coverage')
    external_values = {
        row['candidate_offset'] + 1: row['expected_destination']
        for row in identity['direct_calls']
    }
    external_values.update({
        row['candidate_offset']: row['expected_value']
        for row in identity['body_data_fields']
    })
    linked, fields = link_owner(raw, rows, external_values)
    target = compare.pe_bytes_at(compare.verified_target(), BASE, SIZE)
    differences = [
        {'offset': offset, 'candidate': actual, 'target': expected,
         'relocation_field': offset in fields}
        for offset, (actual, expected) in enumerate(zip(linked, target))
        if actual != expected
    ]
    ordinary = sum(not row['relocation_field'] for row in differences)
    table_start = SIZE - 193 * 4
    return {
        'diagnostic_only': True,
        'exactness_credit': 'none',
        'identity_audit_passed': True,
        'object': str(object_path),
        'object_source_binding': identity['source_observation']['object_source_binding'],
        'target_sha256': identity['target_sha256'],
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
        'linked_sha256': hashlib.sha256(linked).hexdigest(),
        'target_owner_sha256': hashlib.sha256(target).hexdigest(),
        'physical_bytes': len(linked),
        'relocations': len(rows),
        'complete_bytes_equal': linked == target,
        'different_bytes': len(differences),
        'ordinary_different_bytes': ordinary,
        'field_different_bytes': len(differences) - ordinary,
        'table_different_bytes': sum(row['offset'] >= table_start for row in differences),
        'differences': differences,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--output', type=Path, help='Retain the complete diagnostic JSON')
    args = parser.parse_args()
    try:
        result = report(args.object)
        if args.output:
            args.output.write_text(json.dumps(result, indent=2) + '\n')
    except (OSError, ValueError, KeyError, struct.error) as error:
        parser.exit(1, f'RunEcl complete diagnostic failed: {error}\n')
    print(f"RunEcl diagnostic: {result['different_bytes']} differing bytes in "
          f"{result['physical_bytes']} bytes / {result['relocations']} fields "
          f"({result['ordinary_different_bytes']} ordinary, "
          f"{result['field_different_bytes']} field, "
          f"{result['table_different_bytes']} table); no exactness credit")
    return 0 if result['complete_bytes_equal'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
