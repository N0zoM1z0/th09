#!/usr/bin/env python3
"""Complete non-crediting original JP TH09 Player initialization comparison.

The 552-byte code owner also owns its 20-byte switch table. Independent
original instruction/callee observations fix the fields below; private COFF
labels resolve through their actual definitions. No source or exactness claim
follows merely from supplying an object to this inspector.
"""

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct


ROOT = Path(__file__).resolve().parents[1]
BASE = 0x0041EBC0
CODE_SIZE = 552
OWNER_SIZE = 572
SYMBOL = '?InitializeAddedState@PlayerLifecycleView@@QAEXXZ'
TARGET_SHA256 = 'b9b3e4115f8fb3857c373215f960b8eb0a73967cb33d581bb19d0cc8e868fb98'
TARGET_TABLE = (0x0041ED71, 0x0041ED71, 0x0041ED74, 0x0041ED79, 0x0041ED7E)

# These are observed callee/data boundaries, not claims of original C++ names.
EXTERNALS = {
    '?SetAndExecuteScriptIdx@PlayerAddedAnmLoadedView@@QAEXPAUPlayerAnmVmView@@H@Z': 0x00403E00,
    '??BPlayerPositionView@@QAEPAMXZ': 0x004343D0,
    '?g_PlayerAddedStartY@@3MA': 0x004A80EC,
    '__real@42800000': 0x0048E4A8,
    '?Reset@PlayerAddedCollisionRegionView@@QAEHXZ': 0x0041BDA0,
    '?SetState@PlayerAddedStateSetterView@@QAEHH@Z': 0x00403C20,
    '??4PlayerAddedTimerView@@QAEXH@Z': 0x00401500,
    '?g_PlayerAddedRng@@3UPlayerAddedRngView@@A': 0x004ACE0C,
    '?SampleRange@PlayerAddedRngView@@QAEMM@Z': 0x00404910,
    '__real@42c80000': 0x0048E3C8,
    '__real@43c80000': 0x0048E3C4,
    '?Reset@PlayerAddedOwnerStateView@@QAEXXZ': 0x0041D7E0,
    '?g_PlayerAddedSideTable@@3PAHA': 0x004A8108,
    '?g_PlayerAddedClampValue@@3HA': 0x004A7E90,
    '?g_PlayerAddedModeSelector@@3HA': 0x004A7EAC,
    '?Reset@PlayerAddedHeaderResetView@@QAEXXZ': 0x0041BDC0,
}
TARGET_FIELDS = {
    0x019: ('REL32', 0x00403E00), 0x024: ('REL32', 0x004343D0),
    0x030: ('DIR32', 0x004A80EC), 0x036: ('DIR32', 0x0048E4A8),
    0x05A: ('REL32', 0x0041BDA0), 0x09C: ('REL32', 0x00403C20),
    0x0A9: ('REL32', 0x00401500), 0x136: ('REL32', 0x00401500),
    0x141: ('DIR32', 0x004ACE0C), 0x146: ('REL32', 0x00404910),
    0x14C: ('DIR32', 0x0048E3C8), 0x155: ('DIR32', 0x0048E3C4),
    0x16A: ('REL32', 0x0041D7E0), 0x176: ('REL32', 0x00401500),
    0x186: ('DIR32', 0x004A8108), 0x191: ('DIR32', 0x004A7E90),
    0x1A1: ('DIR32', 0x004A7EAC), 0x1AD: ('DIR32', 0x0041EDE8),
    0x1F6: ('REL32', 0x00401500), 0x20E: ('REL32', 0x0041BDC0),
    **{CODE_SIZE + 4 * i: ('DIR32', va) for i, va in enumerate(TARGET_TABLE)},
}


def require_original(target):
    if (len(target) != OWNER_SIZE or
            hashlib.sha256(target).hexdigest() != TARGET_SHA256):
        raise ValueError('Player added-state original complete owner identity changed')


def field_destination(row):
    off = row['offset']
    if (type(off) is not int or off not in TARGET_FIELDS or
            row['type'] != TARGET_FIELDS[off][0] or row['addend'] != 0):
        raise ValueError('Player added-state field position, kind or addend changed')
    symbol = row['symbol']
    if symbol in EXTERNALS:
        destination = EXTERNALS[symbol]
    elif (symbol.startswith('$L') and row['symbol_section'] > 0 and
          row['symbol_section'] == row['owner_section']):
        relative = row['symbol_value'] - row['owner_value']
        if not 0 <= relative < OWNER_SIZE:
            raise ValueError('Player added-state local label leaves owner')
        destination = BASE + relative
    else:
        raise ValueError('Player added-state unreviewed symbol: ' + symbol)
    if destination != TARGET_FIELDS[off][1]:
        raise ValueError('Player added-state independent field destination changed')
    return destination


def link_candidate(raw, rows):
    if len(raw) != OWNER_SIZE or len(rows) != len(TARGET_FIELDS):
        raise ValueError('Player added-state requires all 572 bytes and 25 fields')
    linked = bytearray(raw)
    seen = set()
    bindings = []
    for row in rows:
        off = row['offset']
        destination = field_destination(row)
        if off in seen:
            raise ValueError('Player added-state duplicate field')
        if struct.unpack_from('<I', raw, off)[0] != row['addend']:
            raise ValueError('Player added-state raw field/addend disagreement')
        seen.add(off)
        value = destination - (BASE + off + 4 if row['type'] == 'REL32' else 0)
        struct.pack_into('<I', linked, off, value & 0xFFFFFFFF)
        bindings.append({**row, 'destination': destination})
    if seen != set(TARGET_FIELDS):
        raise ValueError('Player added-state incomplete field census')
    return bytes(linked), bindings


def diagnose(path):
    import capstone
    from diagnostic_switch_graph import switch_graph

    spec = importlib.util.spec_from_file_location(
        'player_added_coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    target_file = coff.verified_target()
    target = coff.pe_bytes_at(target_file, BASE, OWNER_SIZE)
    require_original(target)
    if coff.pe_bytes_at(target_file, BASE + OWNER_SIZE, 4) != b'\xCC' * 4:
        raise ValueError('Player added-state trailing boundary changed')
    raw, rows = coff.object_function(Path(path), SYMBOL, include_symbol_locations=True)
    linked, bindings = link_candidate(raw, rows)
    for off, (kind, destination) in TARGET_FIELDS.items():
        encoded = destination - (BASE + off + 4 if kind == 'REL32' else 0)
        if struct.unpack_from('<I', target, off)[0] != (encoded & 0xFFFFFFFF):
            raise ValueError('Player added-state original field observation changed')
    for symbol, destination in EXTERNALS.items():
        if symbol.startswith('__real@'):
            if coff.pe_bytes_at(target_file, destination, 4) != struct.pack('<I', int(symbol[7:], 16)):
                raise ValueError('Player added-state float literal changed')
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    original_ins = list(decoder.disasm(target[:CODE_SIZE], BASE))
    candidate_ins = list(decoder.disasm(linked[:CODE_SIZE], BASE))
    for instructions in (original_ins, candidate_ins):
        if (len(instructions) != 136 or sum(i.size for i in instructions) != CODE_SIZE or
                instructions[-1].mnemonic != 'ret' or instructions[-1].op_str):
            raise ValueError('Player added-state full code extent or return ABI changed')
    calls = lambda instructions: [i.operands[0].imm for i in instructions if i.mnemonic == 'call']
    def graph(instructions, data):
        table = struct.unpack_from('<5I', data, CODE_SIZE)
        if table != TARGET_TABLE:
            raise ValueError('Player added-state complete five-entry switch changed')
        return switch_graph(instructions, capstone.x86.X86_OP_IMM,
                            0x0041ED6A, 'dword ptr [ecx*4 + 0x41ede8]', table)
    original_graph = graph(original_ins, target)
    candidate_graph = graph(candidate_ins, linked)
    offsets = [i for i, (a, b) in enumerate(zip(target, linked)) if a != b]
    return dict(
        diagnostic_only=True, exactness_credit='none', object=str(path),
        target_sha256=TARGET_SHA256, target_bytes=OWNER_SIZE,
        candidate_bytes=len(raw), code_bytes=CODE_SIZE,
        bound_fields=len(rows), fields_agree=sum(
            linked[r['offset']:r['offset'] + 4] == target[r['offset']:r['offset'] + 4]
            for r in rows), bindings=bindings,
        target_instructions=len(original_ins), candidate_instructions=len(candidate_ins),
        target_calls=len(calls(original_ins)), candidate_calls=len(calls(candidate_ins)),
        ordered_calls_agree=calls(original_ins) == calls(candidate_ins),
        target_blocks=len(original_graph), candidate_blocks=len(candidate_graph),
        direct_graph_agrees=original_graph == candidate_graph, switch_table_agrees=True,
        complete_bytes_equal=linked == target, complete_differences=len(offsets),
        difference_offsets=[f'0x{i:03X}' for i in offsets],
        raw_sha256=hashlib.sha256(raw).hexdigest(),
        linked_sha256=hashlib.sha256(linked).hexdigest())


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        report = diagnose(args.object)
    except (OSError, ValueError) as error:
        print(json.dumps({'error': str(error)}))
        return 2
    print(json.dumps(report, indent=2))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
