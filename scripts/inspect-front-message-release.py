#!/usr/bin/env python3
"""Strict, non-crediting TH09 FrontMessage::ReleaseForSideCount whole-owner audit.

The original Japanese v1.50a owner has 271 physical bytes, 14 independent
address operands and six ordered calls. Never infer exactness from a function
length, normalized instructions, or a similarity score.
"""

import argparse
from difflib import SequenceMatcher
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x00417520
SIZE = 271
SYMBOL = '?ReleaseForSideCount@FrontMessageRuntimeView@@QAEXH@Z'
TARGET_SHA256 = '1cbe472fe579fb254f790b40e808c074032cff74ac8c75a55bdcc2ec6887aa88'

# Reviewed, independently observed TH09 original PE data and callees.
DESTINATIONS = {
    '?g_GameManager@@3UGameManagerModeView@@A': 0x004A7D90,
    '?IsGameMode2@GameManagerModeView@@QAEHXZ': 0x00415D30,
    '?g_Supervisor@@3VSupervisor@@A': 0x004B3100,
    '?ReleaseMusic@Supervisor@@QAEHH@Z': 0x0042FD90,
    '?g_FrontMessageSource@@3PAUFrontMessageSourceView@@A': 0x004A7E80,
    '?g_ZunMemory@@3VZunMemory@@A': 0x004AD018,
    '?Free@ZunMemory@@QAEXPAX@Z': 0x00401360,
    '?g_FrontMessagePrimaryCache@@3PAUFrontMessageTableView@@A': 0x004A7D88,
    '?g_FrontMessageSecondaryCache@@3PAUFrontMessageTableView@@A': 0x004A7D8C,
    '?Reset@FrontMessageRuntimeResetView@@QAEHXZ': 0x00415C90,
}
TARGET_CALLS = [0x00415D30, 0x0042FD90, 0x0042FD90,
                0x00401360, 0x00401360, 0x00415C90]


def bind_candidate(raw, fields):
    if len(fields) != 14:
        raise ValueError('FrontMessage release expects 14 actual COFF fields')
    result = bytearray(raw)
    previous_end = 0
    bound = []
    for row in fields:
        symbol = row['symbol']
        if symbol not in DESTINATIONS:
            raise ValueError('unreviewed FrontMessage release symbol: ' + symbol)
        off = int(row['offset'])
        kind = 'DIR32' if symbol.startswith('?g_') else 'REL32'
        if row['type'] != kind or int(row['addend']) != 0:
            raise ValueError('FrontMessage release code operand kind/addend changed')
        if off < previous_end or off + 4 > len(result):
            raise ValueError('FrontMessage release code operand overlap, order or extent changed')
        previous_end = off + 4
        address = DESTINATIONS[symbol]
        value = address if kind == 'DIR32' else address - BASE - off - 4
        struct.pack_into('<I', result, off, value & 0xFFFFFFFF)
        bound.append((off, kind, address))
    return bytes(result), bound


def decoded_fields(ins, capstone):
    rows, calls = [], []
    for x in ins:
        off = x.address - BASE
        if x.mnemonic == 'call':
            if (len(x.operands) != 1 or
                x.operands[0].type != capstone.x86.X86_OP_IMM or
                x.imm_size != 4):
                raise ValueError('unreviewed FrontMessage release indirect/short call')
            address = x.operands[0].imm
            rows.append((off + x.imm_offset, 'REL32', address))
            calls.append(address)
        elif not x.mnemonic.startswith('j'):
            for op in x.operands:
                if op.type == capstone.x86.X86_OP_MEM and x.disp_size == 4:
                    m = op.mem
                    if m.base == 0 and m.index == 0 and 0x400000 <= m.disp < 0x4F0000:
                        rows.append((off + x.disp_offset, 'DIR32', m.disp))
                elif op.type == capstone.x86.X86_OP_IMM and x.imm_size == 4:
                    if 0x400000 <= op.imm < 0x4F0000:
                        rows.append((off + x.imm_offset, 'DIR32', op.imm))
    return sorted(rows), calls


def inspect(path, explain=False):
    import capstone
    from diagnostic_branch_alignment import direct_control_flow

    spec = importlib.util.spec_from_file_location(
        'th09_release_coff', ROOT / 'scripts' / 'compare-coff-function.py')
    if spec is None or spec.loader is None:
        raise ValueError('missing bound COFF parser')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    target = coff.pe_bytes_at(coff.verified_target(), BASE, SIZE)
    if hashlib.sha256(target).hexdigest() != TARGET_SHA256:
        raise ValueError('original FrontMessage release owner identity changed')
    raw, fields = coff.object_function(Path(path), SYMBOL)
    candidate, bound = bind_candidate(raw, fields)

    dec = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    dec.detail = True
    def decode(code):
        ii = list(dec.disasm(code, BASE))
        if not ii or sum(x.size for x in ii) != len(code):
            raise ValueError('FrontMessage release owner not completely decoded')
        if ii[-1].mnemonic != 'ret' or ii[-1].op_str != '4' or len([x for x in ii if x.mnemonic == 'ret']) != 1:
            raise ValueError('FrontMessage release ret 4 ABI changed')
        return ii

    t, c = decode(target), decode(candidate)
    target_fields, calls = decoded_fields(t, capstone)
    candidate_fields, ccalls = decoded_fields(c, capstone)
    if len(t) != 87 or len(target_fields) != 14 or calls != TARGET_CALLS:
        raise ValueError('original FrontMessage release field/call/extent drift')
    if candidate_fields != bound or [(k, d) for _, k, d in candidate_fields] != [
           (k, d) for _, k, d in target_fields]:
        raise ValueError('FrontMessage release source code operands not independently covered')
    if ccalls != TARGET_CALLS:
        raise ValueError('FrontMessage release call order/identity differs')
    tgraph = direct_control_flow(t, capstone.x86.X86_OP_IMM)
    cgraph = direct_control_flow(c, capstone.x86.X86_OP_IMM)
    report = {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': len(target), 'candidate_bytes': len(raw),
        'target_instructions': len(t), 'candidate_instructions': len(c),
        'bound_fields': len(bound), 'ordered_calls': len(calls),
        'target_blocks': len(tgraph), 'candidate_blocks': len(cgraph),
        'complete_direct_graphs_agree': tgraph == cgraph,
        'complete_linked_overlap_differences': sum(x != y for x, y in zip(target, candidate)),
        'missing_target_bytes': max(0, SIZE - len(candidate)),
        'extra_candidate_bytes': max(0, len(candidate) - SIZE),
        'complete_bytes_agree': target == candidate,
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
        'target_sha256': hashlib.sha256(target).hexdigest(),
    }
    if explain:
        key = lambda x: (x.mnemonic, 'transfer' if x.mnemonic == 'call' or x.mnemonic.startswith('j') else x.op_str)
        align = SequenceMatcher(None, list(map(key, t)), list(map(key, c)), autojunk=False)
        report['aligned_instruction_count'] = sum(block.size for block in align.get_matching_blocks())
        report['aligned_changes'] = [
            {'kind': tag,
             'target': [(hex(x.address - BASE), x.mnemonic, x.op_str) for x in t[i:j]],
             'candidate': [(hex(x.address - BASE), x.mnemonic, x.op_str) for x in c[k:l]]}
            for tag, i, j, k, l in align.get_opcodes() if tag != 'equal'
        ]
    return report


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('object', type=Path)
    ap.add_argument('--json', action='store_true')
    ap.add_argument('--explain', action='store_true')
    ap.add_argument('--max-spans', type=int, default=8)
    args = ap.parse_args()
    if args.max_spans < 1:
        ap.error('--max-spans must be positive')
    try:
        report = inspect(args.object, explain=args.explain)
    except (ValueError, KeyError, OSError, ImportError, struct.error) as e:
        print(json.dumps({'error': str(e)}))
        return 2
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print('FrontMessage Release %d/%d bytes; fields=%d calls=%d, blocks=%d/%d; linked different=%d; exact=%s (NON-CREDITING)' % (
            report['candidate_bytes'], report['target_bytes'], report['bound_fields'],
            report['ordered_calls'], report['candidate_blocks'], report['target_blocks'],
            report['complete_linked_overlap_differences'], report['complete_bytes_agree']))
        if args.explain:
            print('Aligned changes (uncredited):', len(report['aligned_changes']))
            for row in report['aligned_changes'][:args.max_spans]:
                print('  ', row)
    return 0 if report['complete_bytes_agree'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
