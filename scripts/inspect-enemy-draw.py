#!/usr/bin/env python3
"""Read-only complete Enemy draw diagnostic; never grants exactness credit.

Call and global identities are independently reviewed TH09 entries. Literals
must agree with existing canonical bindings and verified PE contents. No target
relocation field is solved backwards to produce a symbol destination.
Requires optional Capstone for complete instruction/operand decoding.
"""
import argparse
from collections import Counter
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


def compare_extents(target, candidate):
    return {
        'complete_bytes_equal': candidate == target,
        'missing_target_bytes': max(0, len(target) - len(candidate)),
        'extra_candidate_bytes': max(0, len(candidate) - len(target)),
        'resolved_sha256': hashlib.sha256(candidate).hexdigest(),
        'target_extent_sha256': hashlib.sha256(target).hexdigest(),
        'overlapping_differences': [
            dict(offset=i, target=expected, candidate=actual)
            for i, (expected, actual) in enumerate(zip(target, candidate))
            if expected != actual
        ],
    }


def decode_fields(code):
    """Decode this table-free owner independently of its COFF records.

    TH09 draw loads its object address through MOV immediates. TEST's 0x400000
    flag mask is a scalar, even though it is inside the image address range.
    This is a bounded draw classifier, not a general pointer inference rule.
    """
    import capstone

    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(code, BASE))
    if sum(i.size for i in instructions) != len(code):
        raise ValueError('incomplete instruction decoding')
    fields = []
    for i in instructions:
        offset = i.address - BASE
        if (i.id == capstone.x86.X86_INS_CALL and i.operands
                and i.operands[0].type == capstone.x86.X86_OP_IMM):
            if i.imm_size != 4:
                raise ValueError('expected four-byte direct call operand')
            fields.append(dict(offset=offset+i.imm_offset, type='REL32',
                               effective_destination=i.operands[0].imm))
        for o in i.operands:
            if (o.type == capstone.x86.X86_OP_MEM
                    and 0x400000 <= o.mem.disp < 0x4E7000):
                if i.disp_size != 4:
                    raise ValueError('expected four-byte data operand')
                fields.append(dict(offset=offset+i.disp_offset, type='DIR32',
                                   effective_destination=o.mem.disp))
            elif (i.id == capstone.x86.X86_INS_MOV
                    and o.type == capstone.x86.X86_OP_IMM
                    and 0x400000 <= o.imm < 0x4E7000):
                if i.imm_size != 4:
                    raise ValueError('expected four-byte object address')
                fields.append(dict(offset=offset+i.imm_offset, type='DIR32',
                                   effective_destination=o.imm))
    return sorted(fields, key=lambda r: r['offset']), len(instructions)


def require_field_coverage(decoded, bound):
    key = lambda r: (r['offset'], r['type'], r['effective_destination'])
    if Counter(map(key, decoded)) != Counter(map(key, bound)):
        raise ValueError('decoded operands do not cover actual COFF fields')


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
    bound = []
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
        bound.append(dict(offset=offset, type=row['type'],
                          effective_destination=destination+row['addend']))
    target_fields, target_instructions = decode_fields(target)
    candidate_fields, candidate_instructions = decode_fields(code)
    if (len(target_fields) != 53 or target_instructions != 495
            or sum(r['type'] == 'REL32' for r in target_fields) != 28):
        raise ValueError('unreviewed target code or field coverage')
    require_field_coverage(candidate_fields, bound)
    calls = lambda fs: [r['effective_destination'] for r in fs if r['type'] == 'REL32']
    print(json.dumps(dict(
        status='diagnostic-only',
        object_source_binding='supplied object inspected; no compile or source proof',
        raw_sha256=hashlib.sha256(raw).hexdigest(),
        bytes=len(raw), expected_bytes=SIZE, fields=len(rows),
        **compare_extents(target, code),
        target_instructions=target_instructions,
        candidate_instructions=candidate_instructions,
        target_decoded_fields=target_fields,
        candidate_decoded_fields=candidate_fields,
        decoded_coff_field_coverage=True,
        ordered_direct_calls_agree=calls(target_fields) == calls(candidate_fields),
        relocations=resolved), indent=2))


if __name__ == '__main__':
    main()
