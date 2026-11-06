#!/usr/bin/env python3
"""TH09 1.50a Title OnUpdateResult full-target/candidate COFF audit.

Validates the original 485-byte owner SHA, all 34 independently reviewed
original external address operands and nine calls, the candidate COFF field
order, full decoded instruction streams, and graph/return semantics.  Reports
but never credits a non-exact maintained source or any isolated trial.
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
BASE, SIZE = 0x004266B5, 485
SYMBOL = '?OnUpdateResult@TitleScreenView@@QAEHXZ'
TARGET_SHA256 = '526beca1d1f7fe3e410002739257bcf486d0c93e6246fae21f6f7902945e1d48'
FIELDS = (
    (0x01B, 'DIR32', '?g_TitleResultUnlockStep@@3HA', 0x4AC8D4),
    (0x023, 'DIR32', '?g_TitleInputFlags@@3IA', 0x4ACF3A),
    (0x02E, 'DIR32', '?g_TitleResultUnlockStep@@3HA', 0x4AC8D4),
    (0x036, 'DIR32', '?g_TitleInputFlags@@3IA', 0x4ACF3A),
    (0x041, 'DIR32', '?g_TitleResultUnlockStep@@3HA', 0x4AC8D4),
    (0x049, 'DIR32', '?g_TitleInputFlags@@3IA', 0x4ACF3A),
    (0x054, 'DIR32', '?g_TitleResultUnlockStep@@3HA', 0x4AC8D4),
    (0x05C, 'DIR32', '?g_TitleInputFlags@@3IA', 0x4ACF3A),
    (0x067, 'DIR32', '?g_TitleResultUnlockStep@@3HA', 0x4AC8D4),
    (0x074, 'DIR32', '?g_TitleResultUnlockStep@@3HA', 0x4AC8D4),
    (0x07A, 'DIR32', '?g_TitleResultUnlockStep@@3HA', 0x4AC8D4),
    (0x08C, 'DIR32', '?g_TitleCharacterUnlocked@@3PAEA', 0x4A81CC),
    (0x09E, 'DIR32', '?g_TitleCharacterUnlockedNormal@@3PAEA', 0x4A81DC),
    (0x0A5, 'DIR32', '?g_TitleCharacterUnlockedMode4@@3PAEA', 0x4A81EC),
    (0x0B0, 'DIR32', '?g_SoundPlayer@@3VSoundPlayer@@A', 0x4DC698),
    (0x0B5, 'REL32', '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z', 0x43E2F0),
    (0x0BB, 'DIR32', '?g_TitleResultUnlockStep@@3HA', 0x4AC8D4),
    (0x0C4, 'REL32', '?MoveCursorHorizontal@TitleScreenView@@QAEHH@Z', 0x424601),
    (0x0D2, 'DIR32', '?g_TitleCharacterUnlockedMode4@@3PAEA', 0x4A81EC),
    (0x0F6, 'DIR32', '?g_TitleCharacterUnlockedNormal@@3PAEA', 0x4A81DC),
    (0x101, 'DIR32', '?g_TitleResultCharacterOrder@@3PADA', 0x4A1DAC),
    (0x112, 'REL32', '?SetCharacterCursorInactive@TitleScreenView@@QAEXHHHH@Z', 0x42505F),
    (0x11B, 'DIR32', '?g_TitleInputFlags@@3IA', 0x4ACF3A),
    (0x12C, 'REL32', '?PlayMenuSound@TitleScreenView@@QAEHHH@Z', 0x422F17),
    (0x138, 'REL32', '?ChangeCurrentScreen@TitleScreenView@@QAEHH@Z', 0x422F39),
    (0x151, 'DIR32', '?g_TitleAnmManager@@3PAUTitleAnmManagerView@@A', 0x4DC550),
    (0x156, 'DIR32', '??_C@_0BD@JGJJAHCD@title?1result00?4png?$AA@', 0x48F640),
    (0x15C, 'REL32', '?LoadSurface@TitleAnmManagerView@@QAEHHPBD@Z', 0x43CAE0),
    (0x16A, 'DIR32', '?g_TitleAnmManager@@3PAUTitleAnmManagerView@@A', 0x4DC550),
    (0x181, 'REL32', '?SetInterruptArray@TitleAnmManagerView@@QAEXPAUAnmVmView@@HH@Z', 0x436DE0),
    (0x189, 'DIR32', '?g_TitleAnmManager@@3PAUTitleAnmManagerView@@A', 0x4DC550),
    (0x190, 'REL32', '?ExecuteScriptArray@TitleAnmManagerView@@QAEXPAUAnmVmView@@H@Z', 0x439560),
    (0x1A0, 'DIR32', '?g_TitleResultCharacterOrder@@3PADA', 0x4A1DAC),
    (0x1AC, 'REL32', '?SetCharacterCursorInactive@TitleScreenView@@QAEXHHHH@Z', 0x42505F),
)
CALLS = tuple(dest for _, kind, _, dest in FIELDS if kind == 'REL32')

# Native roots are independently established by exact PlayStats and input owners.
# Keep old aliases readable for historical source controls, without fitting a
# new root or addend to any candidate instruction bytes.
SOURCE_ROOTS = {symbol: address for _, _, symbol, address in FIELDS}
SOURCE_ROOTS.update({
    '?g_ReplayInputStates@@3PAUReplayInputState@@A': 0x004ACE18,
    '?g_PlayStatsRecord@@3UPlayStatsRecordView@@A': 0x004A8180,
    '?g_AnmManager@@3PAVAnmManager@@A': 0x004DC550,
    '?LoadSurface@AnmManager@@QAEHHPBD@Z': 0x0043CAE0,
    '?SetInterruptArray@AnmManager@@QAEXPAUAnmVm@@HF@Z': 0x00436DE0,
    '?ExecuteScriptArray@AnmManager@@QAEXPAUAnmVm@@H@Z': 0x00439560,
})
NATIVE_ADDENDS = {
    '?g_ReplayInputStates@@3PAUReplayInputState@@A': (0x122,),
    '?g_PlayStatsRecord@@3UPlayStatsRecordView@@A': (0x4C, 0x5C, 0x6C),
}
NATIVE_ANM_SYMBOLS = (
    '?g_AnmManager@@3PAVAnmManager@@A',
    '?LoadSurface@AnmManager@@QAEHHPBD@Z',
    '?SetInterruptArray@AnmManager@@QAEXPAUAnmVm@@HF@Z',
    '?ExecuteScriptArray@AnmManager@@QAEXPAUAnmVm@@H@Z',
)


def encode(kind: str, address: int, offset: int) -> int:
    if kind not in ('DIR32', 'REL32'):
        raise ValueError('unreviewed Title result address operand type')
    return (address - (BASE + offset + 4 if kind == 'REL32' else 0)) & 0xFFFFFFFF


def validate_original(code: bytes, *, target_hash: str = TARGET_SHA256) -> None:
    if len(code) != SIZE or hashlib.sha256(code).hexdigest() != target_hash:
        raise ValueError('TH09 original result updater extent or SHA256 identity changed')
    if len(FIELDS) != 34 or len(CALLS) != 9:
        raise ValueError('TH09 result updater field/call count changed')
    previous_end = 0
    for offset, kind, symbol, dest in FIELDS:
        if offset < previous_end or offset + 4 > SIZE or not symbol:
            raise ValueError('TH09 original result updater field coverage invalid')
        previous_end = offset + 4
        if struct.unpack_from('<I', code, offset)[0] != encode(kind, dest, offset):
            raise ValueError('TH09 original result updater operand identity changed')


def bind_candidate(raw: bytes, rows: list[dict]) -> bytes:
    if not 450 <= len(raw) <= 520:
        raise ValueError('Title result updater source has unreviewed physical extent')
    if len(rows) != len(FIELDS):
        raise ValueError('Title result updater has missing or extra source fields')
    linked = bytearray(raw)
    previous_end = 0
    for row, (_, kind, name, dest) in zip(rows, FIELDS):
        offset = row['offset']
        symbol, addend = row['symbol'], row['addend']
        if (symbol not in SOURCE_ROOTS or row['type'] != kind or
                type(addend) is not int or
                addend not in NATIVE_ADDENDS.get(symbol, (0,)) or
                SOURCE_ROOTS[symbol] + addend != dest):
            raise ValueError('Title result updater source field identity/addend/order changed')
        if not isinstance(offset, int) or offset < previous_end or offset + 4 > len(raw):
            raise ValueError('Title result updater source field overlap/extent changed')
        previous_end = offset + 4
        struct.pack_into('<I', linked, offset, encode(kind, dest, offset))
    return bytes(linked)


def decoded_fields(code: bytes) -> tuple[list, list, list]:
    import capstone
    cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    cs.detail = True
    ins = list(cs.disasm(code, BASE))
    if not ins or sum(i.size for i in ins) != len(code):
        raise ValueError('Title result updater does not decode over entire physical extent')
    if ins[-1].mnemonic != 'ret' or ins[-1].op_str:
        raise ValueError('Title result updater does not end in original ret ABI')
    fields, calls = [], []
    for i in ins:
        if i.mnemonic.startswith('j'):
            continue
        if i.mnemonic == 'call':
            if (len(i.operands) != 1 or i.operands[0].type != capstone.x86.X86_OP_IMM
                    or i.imm_size != 4):
                raise ValueError('Title result updater has an unknown indirect/short call')
            address = i.operands[0].imm
            fields.append((i.address - BASE + i.imm_offset, 'REL32', address))
            calls.append(address)
            continue
        for arg in i.operands:
            if arg.type == capstone.x86.X86_OP_MEM and i.disp_size == 4:
                if 0x400000 <= arg.mem.disp < 0x4E7000:
                    fields.append((i.address - BASE + i.disp_offset, 'DIR32', arg.mem.disp))
            elif arg.type == capstone.x86.X86_OP_IMM and i.imm_size == 4:
                if 0x400000 <= arg.imm < 0x4E7000:
                    fields.append((i.address - BASE + i.imm_offset, 'DIR32', arg.imm))
    return ins, fields, calls


def require_literal_definitions(data, rows, image, parser):
    spec = importlib.util.spec_from_file_location(
        'title_result_literal_payload', ROOT / 'scripts/coff_literal_definitions.py')
    literals = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(literals)
    definitions = literals.check_literal_definitions(data, rows, image, parser, SOURCE_ROOTS)
    if (len(definitions) != 1 or definitions[0]['symbol'] !=
            '??_C@_0BD@JGJJAHCD@title?1result00?4png?$AA@' or
            definitions[0]['size'] != 19):
        raise ValueError('Title result updater literal definition inventory changed')
    return definitions


def diagnose(obj: Path) -> dict:
    from diagnostic_branch_alignment import direct_control_flow
    import capstone
    spec = importlib.util.spec_from_file_location(
        'title_update_result_coff', ROOT / 'scripts/compare-coff-function.py')
    parser = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(parser)
    original = parser.pe_bytes_at(parser.verified_target(), BASE, SIZE)
    validate_original(original)
    ti, tf, tc = decoded_fields(original)
    if (len(ti) != 147 or tf != [(off, kind, dest) for off, kind, _, dest in FIELDS]
            or tuple(tc) != CALLS):
        raise ValueError('Title result updater original decoded field/call/instruction inventory changed')
    raw, rows = parser.object_function(obj, SYMBOL, include_symbol_locations=True)
    linked = bind_candidate(raw, rows)
    literal_definitions = require_literal_definitions(
        obj.read_bytes(), rows, parser.verified_target(), parser)
    ci, cf, cc = decoded_fields(linked)
    if (cf != [(row['offset'], kind, dest) for row, (_, kind, _, dest) in zip(rows, FIELDS)]
            or tuple(cc) != CALLS):
        raise ValueError('Title result updater candidate decoded fields/calls disagree with original')
    target_graph = direct_control_flow(ti, capstone.x86.X86_OP_IMM)
    candidate_graph = direct_control_flow(ci, capstone.x86.X86_OP_IMM)
    if len(target_graph) != 40:
        raise ValueError('Title result updater original 40-block graph changed')
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(raw),
        'target_instructions': len(ti), 'candidate_instructions': len(ci),
        'original_fields': len(tf), 'bound_fields': len(rows),
        'ordered_calls': len(tc), 'calls_agree': tc == cc,
        'original_blocks': len(target_graph), 'candidate_blocks': len(candidate_graph),
        'direct_graphs_agree': target_graph == candidate_graph,
        'overlap_differences': sum(a != b for a, b in zip(original, linked)),
        'difference_offsets': [i for i, (a, b) in enumerate(zip(original, linked)) if a != b],
        'literal_definitions': literal_definitions,
        'native_fields': [dict(offset=r['offset'], symbol=r['symbol'],
                               root=SOURCE_ROOTS[r['symbol']], addend=r['addend'],
                               effective=SOURCE_ROOTS[r['symbol']] + r['addend'])
                          for r in rows if r['symbol'] in NATIVE_ADDENDS],
        'missing_target_bytes': max(0, SIZE-len(linked)),
        'native_anm_fields': [dict(offset=r['offset'], type=r['type'],
                                  symbol=r['symbol'], addend=r['addend'],
                                  root=SOURCE_ROOTS[r['symbol']],
                                  effective=SOURCE_ROOTS[r['symbol']] + r['addend'])
                             for r in rows if r['symbol'] in NATIVE_ANM_SYMBOLS],
        'extra_candidate_bytes': max(0, len(linked)-SIZE),
        'complete_bytes_equal': linked == original,
        'candidate_raw_sha256': hashlib.sha256(raw).hexdigest(),
        'target_sha256': TARGET_SHA256,
    }


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('object', type=Path)
    args = ap.parse_args()
    try:
        print(json.dumps(diagnose(args.object), indent=2))
        return 0
    except (ValueError, OSError, ImportError, KeyError, struct.error) as err:
        print('Title result updater audit rejected:', err, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
