#!/usr/bin/env python3
"""SHA-pinned, original-operand-bound TH09 ParseNetworkConfigValue audit.

Complete 243-byte callback from Japanese v1.50a, not an exactness award.
The 679-byte PrepareTitleMode4Network exact owner uses the same actual TU.
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
BASE = 0x00432240
SIZE = 243
SYMBOL = '?ParseNetworkConfigValue@@YIPBDPBDPAH0PAD@Z'
TARGET_SHA256 = 'b47499422bdbe8fb915ffa4009afae9f1cf923dc04c3668e100cc2d3afad647e'
TARGET_FIELD = (0x2D, 'REL32', 0x0047CC30)
SOURCE_SYMBOL = '_strncmp'


def encoding(offset: int, destination: int = TARGET_FIELD[2]) -> int:
    if not isinstance(offset, int) or offset < 0 or offset + 4 > SIZE:
        raise ValueError('network parser original/source field lies outside owner')
    return (destination - BASE - offset - 4) & 0xffffffff


def require_target(code: bytes, *, expected_sha256=TARGET_SHA256) -> None:
    if len(code) != SIZE or hashlib.sha256(code).hexdigest() != expected_sha256:
        raise ValueError('TH09 original parser complete SHA-256/extent differs')
    offset, kind, destination = TARGET_FIELD
    if kind != 'REL32' or struct.unpack_from('<I', code, offset)[0] != encoding(offset, destination):
        raise ValueError('TH09 original parser independent call field differs')


def link_source(raw: bytes, rows: list[dict]) -> tuple[bytes, list[tuple]]:
    if not 100 <= len(raw) <= 300 or len(rows) != 1:
        raise ValueError('network parser candidate code or complete field count differs')
    row = rows[0]
    if (row['symbol'] != SOURCE_SYMBOL or row['type'] != 'REL32'
            or row['addend'] != 0):
        raise ValueError('network parser source call symbol/kind/addend changed')
    offset = row['offset']
    if not isinstance(offset, int) or offset < 0 or offset + 4 > len(raw):
        raise ValueError('network parser source call offset is outside owner')
    linked = bytearray(raw)
    struct.pack_into('<I', linked, offset, encoding(offset))
    return bytes(linked), [(offset, 'REL32', TARGET_FIELD[2])]


def decode(code: bytes) -> tuple[list, list, list]:
    import capstone

    cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    cs.detail = True
    instructions = list(cs.disasm(code, BASE))
    if (not instructions or sum(ins.size for ins in instructions) != len(code)
            or instructions[-1].mnemonic != 'ret' or instructions[-1].op_str != '8'):
        raise ValueError('parser owner incomplete or nonoriginal ret 8 ABI')
    fields, calls = [], []
    for ins in instructions:
        if ins.mnemonic.startswith('j'):
            continue
        if ins.mnemonic == 'call':
            if (len(ins.operands) != 1 or ins.operands[0].type != capstone.x86.X86_OP_IMM
                    or ins.imm_size != 4):
                raise ValueError('parser changed to indirect/non-REL32 call')
            address = ins.operands[0].imm
            fields.append((ins.address - BASE + ins.imm_offset, 'REL32', address))
            calls.append(address)
            continue
        for op in ins.operands:
            if (op.type == capstone.x86.X86_OP_MEM and ins.disp_size == 4
                    and 0x00400000 <= op.mem.disp < 0x004E7000):
                fields.append((ins.address - BASE + ins.disp_offset, 'DIR32', op.mem.disp))
            elif (op.type == capstone.x86.X86_OP_IMM and ins.imm_size == 4
                  and 0x00400000 <= op.imm < 0x004E7000):
                fields.append((ins.address - BASE + ins.imm_offset, 'DIR32', op.imm))
    return instructions, fields, calls


def diagnose(object_path: Path) -> dict:
    import capstone
    from diagnostic_branch_alignment import direct_control_flow

    spec = importlib.util.spec_from_file_location('network_parser_coff', ROOT/'scripts/compare-coff-function.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    target = module.pe_bytes_at(module.verified_target(), BASE, SIZE)
    require_target(target)
    ti, tf, tc = decode(target)
    if (len(ti) != 116 or tf != [TARGET_FIELD] or tc != [TARGET_FIELD[2]]):
        raise ValueError('actual original parser call, operand or instruction census drifted')
    raw, rows = module.object_function(object_path, SYMBOL)
    linked, fields = link_source(raw, rows)
    ci, cf, cc = decode(linked)
    if cf != fields or cc != tc:
        raise ValueError('actual network parser machine-code call/field identity differs')
    target_graph = direct_control_flow(ti, capstone.x86.X86_OP_IMM)
    candidate_graph = direct_control_flow(ci, capstone.x86.X86_OP_IMM)
    if len(target_graph) != 33:
        raise ValueError('original network parser control-flow graph census drifted')
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(raw),
        'target_instructions': len(ti), 'candidate_instructions': len(ci),
        'original_fields': len(tf), 'bound_fields': len(cf),
        'ordered_calls': len(tc), 'ordered_calls_agree': tc == cc,
        'target_blocks': len(target_graph), 'candidate_blocks': len(candidate_graph),
        'direct_graphs_agree': target_graph == candidate_graph,
        # Every actual PE-linked differing physical position, not just the
        # total count. Downstream guards protect the 167 already equal bytes.
        'overlap_differences': sum(x != y for x, y in zip(target, linked)),
        'difference_offsets': [i for i, (original, source) in
                               enumerate(zip(target, linked)) if original != source],
        'missing_target_bytes': max(0, SIZE - len(raw)),
        'extra_candidate_bytes': max(0, len(raw) - SIZE),
        'complete_bytes_equal': linked == target,
        'target_sha256': TARGET_SHA256,
        'candidate_raw_sha256': hashlib.sha256(raw).hexdigest(),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        evidence = diagnose(args.object)
        if args.json:
            print(json.dumps(evidence, indent=2))
        else:
            print('NON-CREDITING ParseNetworkConfigValue:',
                  evidence['candidate_bytes'], '/', evidence['target_bytes'],
                  'bytes;', evidence['overlap_differences'], 'differences')
        return 0
    except (KeyError, TypeError, OSError, ValueError, ImportError, struct.error) as error:
        print('network parser original-bound audit rejected:', error, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
