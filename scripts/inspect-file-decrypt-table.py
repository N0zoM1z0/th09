#!/usr/bin/env python3
"""Complete non-crediting TH09 FileSystem::TryDecryptFromTable PE/COFF audit.

Every address operand is independently bound to the original Japanese v1.50a
PE and checked against actual decoded target and source instructions. A close
candidate never receives exactness credit from this diagnostic.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x0042C290
SIZE = 220
SYMBOL = '?TryDecryptFromTable@FileSystem@@YIPAEPAEPAHH@Z'
TARGET_SHA256 = 'c9195eb6365a7e3d6905b3e2cead12ed480c4fd9ad968992db2b397e9b6fd8a7'
PARAMS_SHA256 = 'eaf72ba8a6e9e35d75f3c13e202c052b0ac0fbc72828022ef96b640a2d3c0af2'
DESTINATIONS = {
    '?g_CryptSignature@FileSystem@@3PAEA': 0x004A1EC0,
    '?g_DecryptParams@FileSystem@@3PAUDecryptParams@1@A': 0x004A1E60,
    '?Decrypt@FileSystem@@YIPAEPAEHEEHH@Z': 0x0042C180,
    '?g_ZunMemory@@3VZunMemory@@A': 0x004AD018,
    '?Free@ZunMemory@@QAEXPAX@Z': 0x00401360,
}
# Field identities and their *order* are observed in the independent target
# disassembly, not inferred by matching the candidate's relocation values.
FIELD_IDENTITIES = [
    ('DIR32', 0x004A1EC0), ('DIR32', 0x004A1EC1),
    ('DIR32', 0x004A1EC2), ('DIR32', 0x004A1E60),
    ('DIR32', 0x004A1E6C), ('DIR32', 0x004A1E68),
    ('DIR32', 0x004A1E64), ('DIR32', 0x004A1E62),
    ('DIR32', 0x004A1E61), ('REL32', 0x0042C180),
    ('DIR32', 0x004AD018), ('REL32', 0x00401360),
]
CALLEES = [0x0042C180, 0x00401360]


def bind_candidate(raw, fields):
    """Apply only reviewed symbol+addend COFF bindings to actual source bytes."""
    if len(fields) != 12:
        raise ValueError('TryDecryptFromTable requires twelve code relocation fields')
    linked = bytearray(raw)
    visited = 0
    bound = []
    for row, identity in zip(fields, FIELD_IDENTITIES):
        symbol = row['symbol']
        if symbol not in DESTINATIONS:
            raise ValueError('unreviewed FileSystem decryption code operand: ' + symbol)
        kind = 'REL32' if symbol.startswith(('?Decrypt@', '?Free@')) else 'DIR32'
        offset = int(row['offset'])
        expected = DESTINATIONS[symbol] + int(row['addend'])
        if (kind, expected) != identity or row['type'] != kind:
            raise ValueError('FileSystem decryption field kind, destination or order changed')
        if offset < visited or offset + 4 > len(linked):
            raise ValueError('FileSystem decryption code operand overlap/extent/order changed')
        visited = offset + 4
        stored = expected - BASE - offset - 4 if kind == 'REL32' else expected
        struct.pack_into('<I', linked, offset, stored & 0xFFFFFFFF)
        bound.append((offset, kind, expected))
    return bytes(linked), bound


def decoded_fields(instructions, capstone):
    """Discover absolute, possibly indexed, table operands and real calls."""
    fields, calls = [], []
    for ins in instructions:
        relative = ins.address - BASE
        if ins.mnemonic == 'call':
            if (len(ins.operands) != 1 or
                    ins.operands[0].type != capstone.x86.X86_OP_IMM or
                    ins.imm_size != 4):
                raise ValueError('unreviewed indirect or short decrypt call')
            dest = ins.operands[0].imm
            calls.append(dest)
            fields.append((relative + ins.imm_offset, 'REL32', dest))
            continue
        if ins.mnemonic.startswith('j'):
            continue
        for op in ins.operands:
            if op.type == capstone.x86.X86_OP_MEM and ins.disp_size == 4:
                disp = op.mem.disp
                # Absolute physical section addresses are valid even when
                # indexed by EAX; ordinary stack/member offsets are not.
                if 0x00400000 <= disp < 0x004F0000:
                    fields.append((relative + ins.disp_offset, 'DIR32', disp))
            elif op.type == capstone.x86.X86_OP_IMM and ins.imm_size == 4:
                if 0x00400000 <= op.imm < 0x004F0000:
                    fields.append((relative + ins.imm_offset, 'DIR32', op.imm))
    return sorted(fields), calls


def inspect(path):
    import capstone
    from diagnostic_branch_alignment import direct_control_flow

    sp = importlib.util.spec_from_file_location(
        'decrypt_original_coff', ROOT / 'scripts' / 'compare-coff-function.py')
    if sp is None or sp.loader is None:
        raise ValueError('missing source-bound COFF parser')
    coff = importlib.util.module_from_spec(sp)
    sp.loader.exec_module(coff)
    pe = coff.verified_target()
    target = coff.pe_bytes_at(pe, BASE, SIZE)
    if hashlib.sha256(target).hexdigest() != TARGET_SHA256:
        raise ValueError('original Japanese decryption owner changed')
    if coff.pe_bytes_at(pe, 0x004A1EC0, 3) != bytes.fromhex('85a4da'):
        raise ValueError('original Japanese encryption signature changed')
    if hashlib.sha256(coff.pe_bytes_at(pe, 0x004A1E60, 96)).hexdigest() != PARAMS_SHA256:
        raise ValueError('original Japanese eight decryption rows changed')

    raw, rows = coff.object_function(path, SYMBOL)
    code, fields = bind_candidate(raw, rows)
    dec = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    dec.detail = True
    def decode(blob):
        instructions = list(dec.disasm(blob, BASE))
        if (not instructions or sum(x.size for x in instructions) != len(blob) or
                instructions[-1].mnemonic != 'ret'):
            raise ValueError('incomplete decryption function body')
        rets = [x for x in instructions if x.mnemonic == 'ret']
        if len(rets) != 3 or any(x.op_str != '4' for x in rets):
            raise ValueError('decryption ret 4 ABI/exit census changed')
        return instructions
    ti, ci = decode(target), decode(code)
    target_fields, tcalls = decoded_fields(ti, capstone)
    candidate_fields, ccalls = decoded_fields(ci, capstone)
    if (len(ti) != 77 or len(target_fields) != 12 or
            [(kind, dest) for _, kind, dest in target_fields] != FIELD_IDENTITIES or
            tcalls != CALLEES):
        raise ValueError('original decryption field or call census drifted')
    if candidate_fields != fields or ccalls != CALLEES:
        raise ValueError('candidate fields or ordered callees not fully decoded/bound')
    tg = direct_control_flow(ti, capstone.x86.X86_OP_IMM)
    cg = direct_control_flow(ci, capstone.x86.X86_OP_IMM)
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(code),
        'target_instructions': len(ti), 'candidate_instructions': len(ci),
        'bound_fields': len(fields), 'ordered_calls': len(tcalls),
        'target_blocks': len(tg), 'candidate_blocks': len(cg),
        'complete_direct_graphs_agree': tg == cg,
        'complete_overlap_differences': sum(a != b for a, b in zip(target, code)),
        'missing_target_bytes': max(0, SIZE - len(code)),
        'extra_candidate_bytes': max(0, len(code) - SIZE),
        'complete_bytes_equal': target == code,
        'target_sha256': hashlib.sha256(target).hexdigest(),
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        result = inspect(args.object)
    except (ValueError, KeyError, OSError, ImportError, struct.error) as e:
        print(json.dumps({'error': str(e)}))
        return 2
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        print('TryDecryptFromTable complete owner: %d/%d bytes, %d fields, %d ordered calls, %d/%d blocks, %d differences; exact=%s (NON-CREDITING)' % (
            result['candidate_bytes'], result['target_bytes'], result['bound_fields'],
            result['ordered_calls'], result['candidate_blocks'], result['target_blocks'],
            result['complete_overlap_differences'], result['complete_bytes_equal']))
    return 0 if result['complete_bytes_equal'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
