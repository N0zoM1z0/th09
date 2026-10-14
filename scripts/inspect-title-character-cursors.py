#!/usr/bin/env python3
"""TH09 Active/Reverse/Inactive cursor full-owner original-PE/COFF oracle.

These are three independent original target physical extents, not an inferred
identical-source family. Every original absolute/call operand, source COFF
field, actual decoded call and return ABI is checked before reporting byte
similarity. The diagnostic never awards exactness or partial-byte credit.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
SPRITE_SYMBOL = '?SetSprite@TitleAnmView@@QAEHPAUAnmVmView@@H@Z'
SCRIPT_SYMBOL = '?ExecuteScript@TitleAnmManagerView@@QAEHPAUAnmVmView@@@Z'
MANAGER_SYMBOL = '?g_TitleAnmManager@@3PAUTitleAnmManagerView@@A'
SPRITE_ADDRESS = 0x00436AC0
SCRIPT_ADDRESS = 0x00436F30
MANAGER_ADDRESS = 0x004DC550

# Independent original code boundary, owner checksum, physical operand
# offsets, symbol identities and reviewed target addresses (not source-fitted).
OWNERS = {
    'active': {
        'address': 0x00424EDD, 'size': 211,
        'sha256': '787abeae97148590e74189d38d9c320fb5a70ef9310f1fd958f2ea06fd5feb5e',
        'symbol': '?SetCharacterCursorActive@TitleScreenView@@QAEXHHHH@Z',
        'target_instructions': 65,
        'fields': (
            (0x48, 'REL32', SPRITE_SYMBOL, SPRITE_ADDRESS),
            (0x64, 'DIR32', MANAGER_SYMBOL, MANAGER_ADDRESS),
            (0x6C, 'REL32', SCRIPT_SYMBOL, SCRIPT_ADDRESS),
            (0xAC, 'REL32', SPRITE_SYMBOL, SPRITE_ADDRESS),
            (0xC0, 'DIR32', MANAGER_SYMBOL, MANAGER_ADDRESS),
            (0xC8, 'REL32', SCRIPT_SYMBOL, SCRIPT_ADDRESS),
        ),
    },
    'reverse': {
        'address': 0x00424FB0, 'size': 175,
        'sha256': '4039ba94e3b451d55013c80bd3e2099b66a8d13f71969c0d8d2166862dedc68b',
        'symbol': '?SetCharacterCursorReverse@TitleScreenView@@QAEXHHHH@Z',
        'target_instructions': 55,
        'fields': ((0x48, 'REL32', SPRITE_SYMBOL, SPRITE_ADDRESS),
                   (0x98, 'REL32', SPRITE_SYMBOL, SPRITE_ADDRESS)),
    },
    'inactive': {
        'address': 0x0042505F, 'size': 175,
        'sha256': '5bc94b6728de835e7d4e15fa8d28fb3f98089a92499051e1db9233cca4e9a13f',
        'symbol': '?SetCharacterCursorInactive@TitleScreenView@@QAEXHHHH@Z',
        'target_instructions': 55,
        'fields': ((0x48, 'REL32', SPRITE_SYMBOL, SPRITE_ADDRESS),
                   (0x98, 'REL32', SPRITE_SYMBOL, SPRITE_ADDRESS)),
    },
}


def encoded(kind: str, destination: int, base: int, offset: int) -> int:
    if kind not in ('DIR32', 'REL32'):
        raise ValueError('unsupported cursor relocation kind')
    return (destination - (base + offset + 4 if kind == 'REL32' else 0)) & 0xFFFFFFFF


def target_manifest(target: bytes, model: dict) -> None:
    if len(target) != model['size'] or hashlib.sha256(target).hexdigest() != model['sha256']:
        raise ValueError('TH09 original cursor owner identity or extent changed')
    previous = 0
    for offset, kind, _, destination in model['fields']:
        if offset < previous or offset + 4 > len(target):
            raise ValueError('cursor target field manifest overlaps/outside owner')
        previous = offset + 4
        if struct.unpack_from('<I', target, offset)[0] != encoded(kind, destination, model['address'], offset):
            raise ValueError('TH09 original cursor operand identity changed')


def bind_candidate(raw: bytes, rows: list[dict], model: dict) -> bytes:
    if len(rows) != len(model['fields']):
        raise ValueError('cursor candidate lacks full reviewed field inventory')
    if len(raw) < 4 or len(raw) > 500:
        raise ValueError('cursor source candidate extent outside reviewed diagnostic range')
    linked = bytearray(raw)
    previous = 0
    for row, (_, kind, symbol, destination) in zip(rows, model['fields']):
        offset = row['offset']
        if (row['type'], row['symbol'], row['addend']) != (kind, symbol, 0):
            raise ValueError('cursor target/source code field identity, type or order changed')
        if not isinstance(offset, int) or offset < previous or offset + 4 > len(raw):
            raise ValueError('cursor source field order, overlap or extent invalid')
        previous = offset + 4
        struct.pack_into('<I', linked, offset,
                         encoded(kind, destination, model['address'], offset))
    return bytes(linked)


def actual_fields(code: bytes, base: int) -> tuple[list, list, list]:
    import capstone
    cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    cs.detail = True
    instructions = list(cs.disasm(code, base))
    if not instructions or sum(i.size for i in instructions) != len(code):
        raise ValueError('cursor complete function does not decode on instruction boundaries')
    if instructions[-1].mnemonic != 'ret' or instructions[-1].op_str != '0x10':
        raise ValueError('cursor owner does not preserve ret 16 ABI')
    fields = []
    calls = []
    for ins in instructions:
        if ins.mnemonic.startswith('j'):
            continue
        if ins.mnemonic == 'call':
            if len(ins.operands) != 1 or ins.operands[0].type != capstone.x86.X86_OP_IMM or ins.imm_size != 4:
                raise ValueError('unreviewed indirect or short call in cursor')
            address = ins.operands[0].imm
            fields.append((ins.address - base + ins.imm_offset, 'REL32', address))
            calls.append(address)
            continue
        for arg in ins.operands:
            if arg.type == capstone.x86.X86_OP_MEM and ins.disp_size == 4:
                if 0x00400000 <= arg.mem.disp < 0x004E7000:
                    fields.append((ins.address - base + ins.disp_offset,
                                   'DIR32', arg.mem.disp))
            elif arg.type == capstone.x86.X86_OP_IMM and ins.imm_size == 4:
                if 0x00400000 <= arg.imm < 0x004E7000:
                    fields.append((ins.address - base + ins.imm_offset,
                                   'DIR32', arg.imm))
    return instructions, fields, calls


def diagnose(object_file: Path, variant: str) -> dict:
    from diagnostic_branch_alignment import direct_control_flow
    import capstone

    model = OWNERS[variant]
    spec = importlib.util.spec_from_file_location(
        'cursor_th09_coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    target = coff.pe_bytes_at(coff.verified_target(), model['address'], model['size'])
    target_manifest(target, model)
    ti, tf, tcalls = actual_fields(target, model['address'])
    expected_fields = [(off, kind, target) for off, kind, _, target in model['fields']]
    expected_calls = [address for _, kind, _, address in model['fields'] if kind == 'REL32']
    if (len(ti) != model['target_instructions'] or
            tf != expected_fields or tcalls != expected_calls):
        raise ValueError('TH09 original decoded cursor operands/calls changed')

    raw, rows = coff.object_function(object_file, model['symbol'])
    linked = bind_candidate(raw, rows, model)
    ci, cf, ccalls = actual_fields(linked, model['address'])
    actual_source = [(row['offset'], kind, dest)
                     for row, (_, kind, _, dest) in zip(rows, model['fields'])]
    if cf != actual_source or ccalls != expected_calls:
        raise ValueError('cursor candidate COFF fields/calls do not match actual instructions')
    tg = direct_control_flow(ti, capstone.x86.X86_OP_IMM)
    cg = direct_control_flow(ci, capstone.x86.X86_OP_IMM)
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'variant': variant, 'target_address': hex(model['address']),
        'target_bytes': len(target), 'candidate_bytes': len(linked),
        'target_instructions': len(ti), 'candidate_instructions': len(ci),
        'original_fields': len(expected_fields), 'bound_fields': len(rows),
        'ordered_calls': len(tcalls), 'ordered_calls_agree': tcalls == ccalls,
        'original_blocks': len(tg), 'candidate_blocks': len(cg),
        'direct_graphs_agree': tg == cg,
        'overlap_differences': sum(a != b for a, b in zip(target, linked)),
        'missing_target_bytes': max(0, len(target) - len(linked)),
        'extra_candidate_bytes': max(0, len(linked) - len(target)),
        'complete_bytes_equal': linked == target,
        'original_sha256': model['sha256'],
        'candidate_raw_sha256': hashlib.sha256(raw).hexdigest(),
    }


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--variant', required=True, choices=sorted(OWNERS))
    ap.add_argument('object', type=Path)
    args = ap.parse_args()
    try:
        print(json.dumps(diagnose(args.object, args.variant), indent=2))
        return 0
    except (ValueError, OSError, ImportError, KeyError, struct.error) as error:
        print('Title cursor diagnostic rejected input:', error, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
