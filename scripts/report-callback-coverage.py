#!/usr/bin/env python3
"""Audit two original callback-table prefixes against the live function catalog.

This is a coverage report. A missing entry needs boundary/origin/source review;
table membership grants no authored-byte or exactness credit. Prefix extents
are explicit observations, not proof of either complete table or game inventory.
"""
import argparse
import csv
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import tomllib

ROOT = Path(__file__).resolve().parents[1]
TABLES = {
    'effect': (0x004A0B48, 38, 24, 12,
        ('initialize', 'update', 'draw'),
        '318a2c5a7e469179735911b94d8837b8b96651b2c89153ea2a15023ec20942b8',
        'effect-manager-initialize-effect', 0x0040CA60),
    'exattack': (0x004A0F78, 27, 16, 0,
        ('initialize', 'update', 'draw', 'release'),
        '5624ff09e220cca729a896c0dd7cc64f2b1a24a0fdafd05402ed67480cc6f48e',
        'exattack-controller-spawn', 0x004151F0),
}


def executable_intervals(image):
    pe = struct.unpack_from('<I', image, 0x3C)[0]
    if image[pe:pe+4] != b'PE\0\0':
        raise ValueError('missing PE signature')
    machine, count, _, _, _, optional, _ = struct.unpack_from('<HHIIIHH', image, pe+4)
    if machine != 0x14C or struct.unpack_from('<H', image, pe+24)[0] != 0x10B:
        raise ValueError('expected original i386 PE32')
    base = struct.unpack_from('<I', image, pe+24+28)[0]
    result = []
    for n in range(count):
        off = pe+24+optional+40*n
        _, virtual, rva, raw, _, _, _, _, _, flags = struct.unpack_from('<8sIIIIIIHHI', image, off)
        if flags & 0x20000000:
            result.append((base+rva, base+rva+max(virtual, raw)))
    if not result:
        raise ValueError('no executable section')
    return result


def decode_prefix(data, base, count, stride, callback_offset, roles, digest, executable):
    if (len(data) != count*stride or hashlib.sha256(data).hexdigest() != digest or
            callback_offset+4*len(roles) != stride):
        raise ValueError('callback prefix identity/extent changed')
    rows = []
    for n in range(count):
        for k, role in enumerate(roles):
            off = n*stride+callback_offset+4*k
            destination = struct.unpack_from('<I', data, off)[0]
            if destination and not any(start <= destination < end for start, end in executable):
                raise ValueError('non-executable callback pointer in reviewed prefix')
            if destination:
                rows.append({'row': n, 'role': role, 'pointer_address': base+off,
                             'destination': destination})
    return rows


def classify_entries(slots, functions):
    catalog = {int(row['address'], 0): row for row in functions}
    if len(catalog) != len(functions):
        raise ValueError('duplicate function catalog address')
    entries = {}
    for slot in slots:
        address = slot['destination']
        if address not in entries:
            known = catalog.get(address)
            containing = [row['address'] for row in functions
                          if int(row['address'], 0) < address <= int(row['span_end'], 0)]
            entries[address] = {'address': address,
                'catalog_state': 'listed-entry' if known else
                    'interior-entry-needs-review' if containing else 'missing-entry',
                'name': known['proposed_name'] if known else None,
                'status': known['status'] if known else None,
                'containing_catalog_entries': containing, 'slots': []}
        entries[address]['slots'].append(slot)
    return [entries[address] for address in sorted(entries)]


def report():
    spec = importlib.util.spec_from_file_location('callback_target', ROOT/'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    image = coff.verified_target()
    executable = executable_intervals(image)
    with (ROOT/'config/functions.csv').open() as stream:
        functions = list(csv.DictReader(stream))
    with (ROOT/'config/match-units.toml').open('rb') as stream:
        units = tomllib.load(stream)['units']
    result = {'diagnostic_only': True, 'exactness_credit': 'none',
              'complete_inventory_proven': False, 'tables': {}}
    for name, (base, count, stride, first, roles, digest, factory, address) in TABLES.items():
        unit = units[factory]
        if (unit['target_address'] != address or
                not any(row['target'] == base for row in unit['relocations'])):
            raise ValueError('canonical factory/table identity changed: '+name)
        slots = decode_prefix(coff.pe_bytes_at(image, base, count*stride),
                              base, count, stride, first, roles, digest, executable)
        entries = classify_entries(slots, functions)
        result['tables'][name] = {'base': base, 'reviewed_prefix_rows': count,
            'prefix_sha256': digest, 'nonzero_callback_slots': len(slots),
            'unique_callbacks': len(entries),
            'missing_entries': sum(row['catalog_state'] == 'missing-entry' for row in entries),
            'interior_entries': sum(row['catalog_state'] == 'interior-entry-needs-review' for row in entries),
            'entries': entries}
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        result = report()
    except (OSError, ValueError, KeyError, struct.error) as exc:
        print(json.dumps({'result': 'error', 'message': str(exc)}))
        return 2
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        for name, table in result['tables'].items():
            print(f"{name}: {table['reviewed_prefix_rows']} reviewed rows, "
                  f"{table['nonzero_callback_slots']} nonzero slots, "
                  f"{table['missing_entries']} missing entries, "
                  f"{table['interior_entries']} interior entries needing review")
        print('Complete game/table inventory is unproven; no exactness credit.')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
