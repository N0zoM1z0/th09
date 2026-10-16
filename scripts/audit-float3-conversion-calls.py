#!/usr/bin/env python3
"""Original-bound cross-owner CALL census for the folded 0x4343D0 helper.

The 3-byte target is shared by Float3::operator float* and constructor-shaped
uses. CALL references alone cannot assign physical authorship or exact credit.
A byte-pattern census is a routing aid, not an instruction-boundary Oracle:
check the interesting sites in IDA and use owner-specific cold comparisons.
"""

import argparse
import bisect
import csv
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
TEXT_START, TEXT_END = 0x00401000, 0x0048DAD9
FOLDED_VA = 0x004343D0
FOLDED_BODY_SHA = '58367ffa2a0179375018fa0f5c26da24391e42ebe0ed8fdda35a21fc7bdc396f'
BIASED_Y_LEA_VA = 0x00407F64
BIASED_Y_LEA_SHA = '0141ec913f701eb29274212b0efb8f55b56bceec2694a516d5411995b49788f9'
# Every CODE xref returned independently by the target-attested th09-ida
# get_xrefs_to(0x004343D0). Data xrefs are deliberately not counted as calls.
# The 27 physical E8 encodings below were separately read from the retail PE.
REVIEWED_CODE_CALLS = (
    0x00402FE0, 0x00407C92, 0x00407F53, 0x00408452,
    0x004101E2, 0x00410D29, 0x00412D93, 0x004138D4,
    0x00413B35, 0x00413B43, 0x00413C61, 0x00413C6A,
    0x00413D0C, 0x004149AC, 0x0041BB0A, 0x0041BB7A,
    0x0041C678, 0x0041E786, 0x0041EBE3, 0x0041F39A,
    0x0041F5E3, 0x0041FE3A, 0x0042168F, 0x00439860,
    0x0044163D, 0x0044286F, 0x00442D04,
)
# Distinct exercised subsystems, not extra function boundaries or new credit.
KNOWN_SITES = (0x00407C92, 0x00407F53, 0x004149AC, 0x00439860)


def require_reviewed_code_calls(calls: list[int]) -> None:
    """A raw-byte E8 search may only describe the independently reviewed set."""
    if (not REVIEWED_CODE_CALLS or
            tuple(sorted(set(REVIEWED_CODE_CALLS))) != REVIEWED_CODE_CALLS or
            calls != list(REVIEWED_CODE_CALLS)):
        raise ValueError('original folded helper E8 candidates differ from IDA code xrefs')


def rel32_call_candidates(code: bytes, base: int, callee: int) -> list[int]:
    """Find E8 rel32 encodings by their *actual* displacement and VA.

    Do not confuse this search with CFG-decoded calls: the original .text
    region also contains data and compiler-owned jump tables.
    """
    result = []
    for offset in range(max(0, len(code) - 4)):
        if code[offset] != 0xE8:
            continue
        displacement = struct.unpack_from('<i', code, offset + 1)[0]
        if base + offset + 5 + displacement == callee:
            result.append(base + offset)
    return result


def owner_at(address: int, owners: list[dict]) -> dict | None:
    """Resolve the *reviewed* source owner, without assigning a folded callee."""
    positions = [row['begin'] for row in owners]
    index = bisect.bisect_right(positions, address) - 1
    if index < 0 or address > owners[index]['end']:
        return None
    return owners[index]


def read_owners() -> list[dict]:
    with (ROOT / 'config/functions.csv').open(newline='') as stream:
        reader = csv.DictReader(stream)
        owners = [{
            'begin': int(row['address'], 16),
            'end': int(row['span_end'], 16),
            'name': row['proposed_name'] or row['current_name'],
            'source': row['source_file'],
            'status': row['status'],
        } for row in reader]
    return sorted(owners, key=lambda row: row['begin'])


def audit_target() -> dict:
    spec = importlib.util.spec_from_file_location(
        'float3_call_audit_coff', ROOT / 'scripts/compare-coff-function.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    image = module.verified_target()
    folded = module.pe_bytes_at(image, FOLDED_VA, 3)
    y_lea = module.pe_bytes_at(image, BIASED_Y_LEA_VA, 3)
    if hashlib.sha256(folded).hexdigest() != FOLDED_BODY_SHA:
        raise ValueError('folded target helper identity/boundary changed')
    if hashlib.sha256(y_lea).hexdigest() != BIASED_Y_LEA_SHA:
        raise ValueError('original RunEcl MoveRandomBiased Y cursor changed')

    text = module.pe_bytes_at(image, TEXT_START, TEXT_END - TEXT_START)
    calls = rel32_call_candidates(text, TEXT_START, FOLDED_VA)
    require_reviewed_code_calls(calls)
    if not set(KNOWN_SITES).issubset(calls):
        raise ValueError('original independently reviewed folded-helper caller missing')
    owners = read_owners()
    if any(owner_at(va, owners) is None for va in calls):
        raise ValueError('folded-helper CODE call lacks a reviewed physical owner')
    return {
        'target': 'th09-v1.50a-original-japanese',
        'physical_callee': hex(FOLDED_VA),
        'callee_authorship': 'unresolved-folded-special-member',
        'exactness_credit': 'none',
        'call_encoding_candidates': len(calls),
        'independent_ida_code_xrefs': len(REVIEWED_CODE_CALLS),
        'sites': [
            {
                'address': hex(va),
                'owner_address': hex(owner['begin']) if owner else None,
                'owner': owner['name'] if owner else None,
                'owner_status': owner['status'] if owner else None,
                'source': owner['source'] if owner else None,
            } for va in calls for owner in [owner_at(va, owners)]
        ],
        'biased_movement_y_address': hex(BIASED_Y_LEA_VA),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    report = audit_target()
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print('folded Float3/constructor helper:', report['physical_callee'])
        print('verified E8 rel32 encoding candidates:', report['call_encoding_candidates'])
        print('RunEcl-biased Y LEA:', report['biased_movement_y_address'])
        print('physical ownership: unresolved; exactness credit: none')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
