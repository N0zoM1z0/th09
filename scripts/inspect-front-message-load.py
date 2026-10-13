#!/usr/bin/env python3
"""Non-crediting complete TH09 FrontMessageRuntime::LoadForSideCount audit.

The original 405-byte body, physical instructions, 18 address operands, eight
ordered calls and three ret 0xC exits are independently verified. Bind only
reviewed original TH09 addresses from actual candidate COFF fields. This is
strict *diagnostic* evidence, never an exactness claim or runtime validation.
"""

from __future__ import annotations

import argparse
from difflib import SequenceMatcher
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x004182E0
SIZE = 405
SYMBOL = '?LoadForSideCount@FrontMessageRuntimeView@@QAEHPAD0H@Z'
TARGET_SHA256 = 'c298c2b6287729365cf88504307a0db350e0b1fd27bdbb680823d7e0463dafba'

# These exact destinations are observed in the verified TH09 PE, not fitted
# by reading each source function's unresolved relocation field from the PE.
DESTINATIONS = {
    '?ReleaseForSideCount@FrontMessageRuntimeView@@QAEXH@Z': 0x00417520,
    '?Reset@FrontMessageRuntimeResetView@@QAEHXZ': 0x00415C90,
    '?g_FrontMessagePrimaryCache@@3PAUFrontMessageTableView@@A': 0x004A7D88,
    '?OpenFile@FileSystem@@YIPAEPBDPAHH@Z': 0x0042C970,
    '?g_GameManager@@3UGameManagerModeView@@A': 0x004A7D90,
    '?IsGameMode2@GameManagerModeView@@QAEHXZ': 0x00415D30,
    '?g_Supervisor@@3VSupervisor@@A': 0x004B3100,
    '?LoadMusic@Supervisor@@QAEHH@Z': 0x0042FC20,
    '?g_FrontMessageSecondaryCache@@3PAUFrontMessageTableView@@A': 0x004A7D8C,
    '?g_FrontMessageLoadError@@3QBDB': 0x0048EEB4,
    '?g_GameErrorContext@@3VGameErrorContext@@A': 0x004B1020,
    '?Log@GameErrorContext@@QAAPBDPBDZZ': 0x0042C5C0,
    '?g_FrontMessageSource@@3PAUFrontMessageSourceView@@A': 0x004A7E80,
}
CALLEES = [0x00417520, 0x00415C90, 0x0042C970, 0x00415D30,
           0x0042FC20, 0x0042C970, 0x0042C5C0, 0x0042FC20]


def bind_candidate(raw: bytes, rows: list[dict]) -> tuple[bytes, list[tuple]]:
    """Solve from reviewed identities and COFF; never guess addresses/bytes."""
    if len(rows) != 18:
        raise ValueError('FrontMessage Load expects 18 bound code relocation fields')
    resolved = bytearray(raw)
    fields = []
    prev_end = 0
    for r in rows:
        symbol = r['symbol']
        if symbol not in DESTINATIONS:
            raise ValueError('unreviewed FrontMessage relocation: ' + symbol)
        off = int(r['offset'])
        kind = 'DIR32' if symbol.startswith('?g_') else 'REL32'
        if r['type'] != kind or int(r['addend']) != 0:
            raise ValueError('FrontMessage Load relocation type/addend mismatch')
        if off < prev_end or off + 4 > len(resolved):
            raise ValueError('FrontMessage Load relocation overlap/extent/order mismatch')
        prev_end = off + 4
        address = DESTINATIONS[symbol]
        data = address if kind == 'DIR32' else address - (BASE + off + 4)
        struct.pack_into('<I', resolved, off, data & 0xFFFFFFFF)
        fields.append((off, kind, address))
    return bytes(resolved), fields


def decoded_fields(instructions, capstone_module):
    """Independently find all direct call and absolute-address operands."""
    address_types = capstone_module.x86
    fields, calls = [], []
    for ins in instructions:
        off = ins.address - BASE
        if ins.mnemonic == 'call':
            if len(ins.operands) != 1 or ins.operands[0].type != address_types.X86_OP_IMM or ins.imm_size != 4:
                raise ValueError('FrontMessage Load contains unreviewed call encoding')
            address = ins.operands[0].imm
            fields.append((off + ins.imm_offset, 'REL32', address))
            calls.append(address)
            continue
        if ins.mnemonic.startswith('j'):
            continue
        for op in ins.operands:
            if op.type == address_types.X86_OP_MEM and ins.disp_size == 4:
                mem = op.mem
                if not mem.base and not mem.index and 0x400000 <= mem.disp < 0x4F0000:
                    fields.append((off + ins.disp_offset, 'DIR32', mem.disp))
            elif op.type == address_types.X86_OP_IMM and ins.imm_size == 4:
                if 0x400000 <= op.imm < 0x4F0000:
                    fields.append((off + ins.imm_offset, 'DIR32', op.imm))
    return sorted(fields), calls


def load_coff():
    spec = importlib.util.spec_from_file_location(
        'front_load_coff', ROOT / 'scripts' / 'compare-coff-function.py')
    if spec is None or spec.loader is None:
        raise ValueError('missing target-bound COFF reader')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    return coff


def inspect(object_path: Path, *, explain: bool = False) -> dict:
    import capstone
    from diagnostic_branch_alignment import direct_control_flow

    coff = load_coff()
    target = coff.pe_bytes_at(coff.verified_target(), BASE, SIZE)
    if hashlib.sha256(target).hexdigest() != TARGET_SHA256:
        raise ValueError('TH09 FrontMessage Load original owner hash drift')
    original, fields = coff.object_function(object_path, SYMBOL)
    linked, bound_fields = bind_candidate(original, fields)
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True

    def decode(blob: bytes):
        ins = list(decoder.disasm(blob, BASE))
        if not ins or sum(i.size for i in ins) != len(blob):
            raise ValueError('FrontMessage Load owner is not completely decoded')
        if len([i for i in ins if i.mnemonic == 'ret']) != 3:
            raise ValueError('FrontMessage Load three-return contract changed')
        if any(i.op_str != '0xc' for i in ins if i.mnemonic == 'ret') or ins[-1].mnemonic != 'ret':
            raise ValueError('FrontMessage Load ret 12 caller cleanup changed')
        return ins

    target_ins = decode(target)
    candidate_ins = decode(linked)
    actual_target, target_calls = decoded_fields(target_ins, capstone)
    actual_candidate, candidate_calls = decoded_fields(candidate_ins, capstone)
    if len(target_ins) != 146 or len(actual_target) != 18 or target_calls != CALLEES:
        raise ValueError('verified TH09 FrontMessage owner field/call fingerprint differs')
    if actual_candidate != bound_fields:
        raise ValueError('FrontMessage Load COFF fields do not cover actual instructions')
    if [(kind, addr) for _, kind, addr in actual_candidate] != [
            (kind, addr) for _, kind, addr in actual_target]:
        raise ValueError('FrontMessage Load actual data/call field identities/order changed')
    if candidate_calls != CALLEES:
        raise ValueError('FrontMessage Load ordered callee identities changed')
    t_cfg = direct_control_flow(target_ins, capstone.x86.X86_OP_IMM)
    c_cfg = direct_control_flow(candidate_ins, capstone.x86.X86_OP_IMM)
    result = {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(original),
        'target_instructions': len(target_ins), 'candidate_instructions': len(candidate_ins),
        'relocations': len(fields), 'ordered_calls': len(target_calls),
        'direct_blocks': [len(t_cfg), len(c_cfg)],
        'complete_direct_graphs_agree': t_cfg == c_cfg,
        'complete_linked_overlap_differences': sum(x != y for x, y in zip(target, linked)),
        'missing_target_bytes': max(0, SIZE - len(linked)),
        'extra_candidate_bytes': max(0, len(linked) - SIZE),
        'complete_bytes_agree': linked == target,
        'raw_sha256': hashlib.sha256(original).hexdigest(),
        'target_sha256': hashlib.sha256(target).hexdigest(),
    }
    if explain:
        def token(ins):
            if ins.mnemonic == 'call' or ins.mnemonic.startswith('j'):
                return ins.mnemonic, 'control-transfer'
            return ins.mnemonic, ins.op_str
        matching = SequenceMatcher(None, list(map(token, target_ins)),
                                   list(map(token, candidate_ins)), autojunk=False)
        result['aligned_instruction_count'] = sum(m.size for m in matching.get_matching_blocks())
        result['instruction_changes'] = [
            {'change': tag,
             'target': [(hex(i.address - BASE), i.mnemonic, i.op_str) for i in target_ins[ti:tj]],
             'candidate': [(hex(i.address - BASE), i.mnemonic, i.op_str) for i in candidate_ins[ci:cj]]}
            for tag, ti, tj, ci, cj in matching.get_opcodes() if tag != 'equal'
        ]
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    parser.add_argument('--explain', action='store_true')
    parser.add_argument('--max-spans', type=int, default=8)
    args = parser.parse_args()
    if args.max_spans < 1:
        parser.error('--max-spans must be positive')
    try:
        data = inspect(args.object, explain=args.explain)
    except (OSError, KeyError, ValueError, ImportError, struct.error) as err:
        print(json.dumps({'error': str(err)}))
        return 2
    if args.json:
        print(json.dumps(data, indent=2))
    else:
        print('FrontMessage Load %d/%d bytes; %d bound fields, %d ordered calls; %d/%d control blocks; %d overlapping differences, %d missing; exact=%s (NON-CREDITING)' % (
            data['candidate_bytes'], data['target_bytes'], data['relocations'],
            data['ordered_calls'], *reversed(data['direct_blocks']),
            data['complete_linked_overlap_differences'], data['missing_target_bytes'],
            data['complete_bytes_agree']))
        if args.explain:
            print('Aligned instruction spans: %d (diagnostic only)' % len(data['instruction_changes']))
            for item in data['instruction_changes'][:args.max_spans]:
                print('  ', item)
    return 0 if data['complete_bytes_agree'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
