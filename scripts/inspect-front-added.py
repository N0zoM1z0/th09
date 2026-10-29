#!/usr/bin/env python3
"""Complete original-bound Front initialization diagnostic; no exact credit."""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE, SIZE = 0x00418480, 706
SYMBOL = '?FrontAddedCallback@@YIHPAX@Z'
TARGET_SHA256 = 'a1faabe117043306799d2c256fb5d0d76d0755b3c8e9015019c57dbd6a8377e1'
GAME_MANAGER_SYMBOL = '?g_GameManager@@3UAsciiGameManagerView@@A'
GAME_MANAGER_BASE = 0x004A7D90
GAME_MANAGER_DATA_FIELDS = (
    (0x11A, 0xFC), (0x266, 0x58), (0x277, 0x20),
    (0x299, 0x58), (0x2A4, 0x20),
)
DESTINATIONS = {
    '?g_AnmManager@@3PAVAnmManager@@A': 0x004DC550,
    '?GetAnm@AnmManager@@QAEPAVAnmLoaded@@H@Z': 0x00401660,
    '??_C@_09DDFJDAFG@front?4anm?$AA@': 0x0048EE68,
    '?PreloadAnm@AnmManager@@QAEPAVAnmLoaded@@HPBD@Z': 0x0043C7A0,
    '?ExecuteAnmIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z': 0x00401560,
    '?g_AsciiManager@@3VAsciiManager@@A': 0x004CE458,
    '?g_GameManager@@3UAsciiGameManagerView@@A': 0x004A7D90,
    '?IsGameMode0@AsciiGameManagerView@@QAEHXZ': 0x0040E470,
    '?IsGameMode1@AsciiGameManagerView@@QAEHXZ': 0x00404970,
    '?SetSprite@AnmLoaded@@QAEHPAUAnmVm@@H@Z': 0x00436AC0,
    '?Reset@FrontMessageRuntimeResetView@@QAEHXZ': 0x00415C90,
    '?g_PlayerResourcePaths@@3PAY05PBDA': 0x004A1238,
    '?LoadForSideCount@FrontMessageRuntimeView@@QAEHPAD0H@Z': 0x004182E0,
    '?InitializeMessageRuntime@FrontMessageOwnerView@@QAEXXZ': 0x004181E0,
}


def require_game_manager_fields(rows, target):
    """Reject standalone aliases even when they encode the same addresses."""
    observed = [row for row in rows if row['offset'] in
                {offset for offset, _ in GAME_MANAGER_DATA_FIELDS}]
    if len(target) != SIZE or len(observed) != len(GAME_MANAGER_DATA_FIELDS):
        raise ValueError('Front GameManager data inventory changed')
    for row, (offset, member) in zip(observed, GAME_MANAGER_DATA_FIELDS):
        if (row['offset'] != offset or row['type'] != 'DIR32'
                or row['symbol'] != GAME_MANAGER_SYMBOL or row['addend'] != member
                or row.get('symbol_section') != 0
                or struct.unpack_from('<I', target, offset)[0] != GAME_MANAGER_BASE+member):
            raise ValueError('Front GameManager member root/addend identity changed')
    return dict(symbol=GAME_MANAGER_SYMBOL, root=GAME_MANAGER_BASE,
                fields=len(observed), members=[member for _, member in GAME_MANAGER_DATA_FIELDS])


def module(name, file):
    spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/file)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def address_fields(instructions):
    import capstone
    result = []
    for ins in instructions:
        if ins.mnemonic.startswith('j'):
            continue
        if ins.mnemonic == 'call':
            if (len(ins.operands) != 1 or ins.imm_size != 4 or
                    ins.operands[0].type != capstone.x86.X86_OP_IMM):
                raise ValueError('unreviewed Front indirect/short call')
            result.append((ins.address - BASE + ins.imm_offset, 'REL32', ins.operands[0].imm))
            continue
        for op in ins.operands:
            if (op.type == capstone.x86.X86_OP_MEM and ins.disp_size == 4 and
                    0x00400000 <= op.mem.disp < 0x004E7000):
                result.append((ins.address - BASE + ins.disp_offset, 'DIR32', op.mem.disp))
            elif (op.type == capstone.x86.X86_OP_IMM and ins.imm_size == 4 and
                    0x00400000 <= op.imm < 0x004E7000):
                result.append((ins.address - BASE + ins.imm_offset, 'DIR32', op.imm))
    return result


def bind_candidate(raw, rows, target, original_fields):
    if len(raw) != SIZE or len(target) != SIZE or len(rows) != 37 or len(original_fields) != 37:
        raise ValueError('Front complete 706-byte/37-field inventory changed')
    linked = bytearray(raw)
    previous = -4
    for row, (offset, kind, dest) in zip(rows, original_fields):
        candidate_offset = row['offset']
        if (type(candidate_offset) is not int or candidate_offset < previous + 4 or
                candidate_offset < 0 or candidate_offset + 4 > SIZE or
                type(offset) is not int or offset < 0 or offset + 4 > SIZE or
                kind not in ('DIR32', 'REL32') or
                row['type'] != kind or row['symbol'] not in DESTINATIONS or
                DESTINATIONS[row['symbol']] + row['addend'] != dest):
            raise ValueError('Front original field position/type/symbol/addend identity changed')
        original_value = dest - (BASE + offset + 4 if kind == 'REL32' else 0)
        if struct.unpack_from('<I', target, offset)[0] != original_value & 0xFFFFFFFF:
            raise ValueError('Front original field encoding changed')
        value = dest - (BASE + candidate_offset + 4 if kind == 'REL32' else 0)
        struct.pack_into('<I', linked, candidate_offset, value & 0xFFFFFFFF)
        previous = candidate_offset
    return bytes(linked)


def inspect(path):
    import capstone
    from diagnostic_branch_alignment import direct_control_flow
    coff = module('front_added_coff', 'compare-coff-function.py')
    image = coff.verified_target()
    target = coff.pe_bytes_at(image, BASE, SIZE)
    if hashlib.sha256(target).hexdigest() != TARGET_SHA256:
        raise ValueError('original Front complete-owner digest changed')
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    ti = list(decoder.disasm(target, BASE))
    tf = address_fields(ti)
    if (sum(i.size for i in ti) != SIZE or len(ti) != 192 or
            ti[-1].mnemonic != 'ret' or len(tf) != 37):
        raise ValueError('original Front extent/instruction/field review changed')
    raw, rows = coff.object_function(Path(path), SYMBOL, include_symbol_locations=True)
    native_game_data = require_game_manager_fields(rows, target)
    linked = bind_candidate(raw, rows, target, tf)
    strings = [row for row in rows if row['symbol'].startswith('??_C@')]
    if len(strings) != 1 or coff.pe_bytes_at(image, 0x0048EE68, 10) != b'front.anm\0':
        raise ValueError('Front original resource string changed')
    literals = module('front_added_literal', 'inspect-effect-type34.py')
    if literals.string_at_symbol(path, strings[0]) != b'front.anm\0':
        raise ValueError('Front actual COFF resource string changed')
    ci = list(decoder.disasm(linked, BASE))
    candidate_fields = [(row['offset'], kind, dest) for row, (_, kind, dest) in zip(rows, tf)]
    if (sum(i.size for i in ci) != SIZE or ci[-1].mnemonic != 'ret' or
            address_fields(ci) != candidate_fields):
        raise ValueError('Front actual decoded COFF fields differ from original')
    calls = lambda instructions: [i.operands[0].imm for i in instructions if i.mnemonic == 'call']
    if len(calls(ti)) != 21 or calls(ti) != calls(ci):
        raise ValueError('Front original 21-call order/identity changed')
    tg = direct_control_flow(ti, capstone.x86.X86_OP_IMM)
    cg = direct_control_flow(ci, capstone.x86.X86_OP_IMM)
    offsets = [i for i, (a, b) in enumerate(zip(target, linked)) if a != b]
    return {'diagnostic_only': True, 'exactness_credit': 'none',
            'native_game_manager_data': native_game_data,
            'target_bytes': SIZE, 'candidate_bytes': len(raw), 'target_sha256': TARGET_SHA256,
            'target_instructions': len(ti), 'candidate_instructions': len(ci),
            'original_fields': len(tf), 'bound_fields': len(rows), 'ordered_calls': 21,
            'calls_agree': True, 'target_blocks': len(tg), 'candidate_blocks': len(cg),
            'direct_graphs_agree': tg == cg, 'differences': len(offsets),
            'difference_offsets': offsets, 'complete_bytes_equal': linked == target,
            'field_positions_agree': [row['offset'] for row in rows] == [f[0] for f in tf],
            'candidate_raw_sha256': hashlib.sha256(raw).hexdigest()}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    result = inspect(args.object)
    print(json.dumps(result, indent=2) if args.json else
          f"Front AddedCallback: {result['candidate_bytes']} bytes, {result['differences']} differences; no exact credit")
