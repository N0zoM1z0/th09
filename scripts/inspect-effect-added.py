#!/usr/bin/env python3
"""Complete, noncrediting TH09 EffectManager::AddedCallback PE/COFF audit.

Bind only the four independently reviewed target-backed ANM manager, GetAnm,
retail string and PreloadAnm relocations. Decode every instruction/operand,
check the static fastcall ABI and two exact external calls, then compare *all*
51 resolved bytes. Matching fields are not partial or exact codegen credit.
Compile a fresh maintained-source VC7.1 object before invoking this inspector.
"""

from __future__ import annotations

import argparse
from collections import Counter
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x0040D1C0
SIZE = 51
TARGET_SHA256 = 'ddad6b720c7c7d539d71ce27f74660951ebb6abb17fb58a616752a91b5760087'
TARGET_FIELDS = (
    (0x004, 'DIR32', 0x004DC550),
    (0x00F, 'REL32', 0x00401660),
    (0x01B, 'DIR32', 0x0048E488),
    (0x024, 'REL32', 0x0043C7A0),
)
TARGET_BLOCKS = 3
SYMBOL = '?AddedCallback@EffectManager@@SIHPAU1@@Z'
DESTINATIONS = {
    '?g_AnmManager@@3PAUEffectAnmManagerView@@A': 0x004DC550,
    '?GetAnm@EffectAnmManagerView@@QAEPAXH@Z': 0x00401660,
    '??_C@_09NMANGJFI@etama?4anm?$AA@': 0x0048E488,
    '?PreloadAnm@EffectAnmManagerView@@QAEPAXHPBD@Z': 0x0043C7A0,
}
CALL_ORDER = [0x00401660, 0x0043C7A0]
EXPECTED_FIELDS = Counter([
    ('DIR32', 0x004DC550),
    ('REL32', 0x00401660),
    ('DIR32', 0x0048E488),
    ('REL32', 0x0043C7A0),
])


def require_original(target: bytes, *, expected_sha256: str = TARGET_SHA256) -> None:
    if len(target) != SIZE or hashlib.sha256(target).hexdigest() != expected_sha256:
        raise ValueError('TH09 original Effect added callback SHA256/extent differs')
    previous = 0
    for offset, kind, destination in TARGET_FIELDS:
        if offset < previous or offset + 4 > SIZE:
            raise ValueError('original Effect added callback fields are malformed')
        previous = offset + 4
        stored = (destination - (BASE + offset + 4 if kind == 'REL32' else 0)) & 0xffffffff
        if struct.unpack_from('<I', target, offset)[0] != stored:
            raise ValueError('original Effect added callback operand identity differs')


def bind_candidate(code: bytes, rows: list[dict]) -> tuple[bytes, list[tuple]]:
    if len(rows) != 4:
        raise ValueError('Effect AddedCallback requires all four actual relocations')
    linked = bytearray(code)
    used = set()
    bound = []
    previous_end = 0
    for row, (_, expected_kind, expected_destination) in zip(rows, TARGET_FIELDS):
        symbol, kind, addend, offset = (
            row['symbol'], row['type'], int(row['addend']), int(row['offset']))
        if symbol not in DESTINATIONS or kind not in ('DIR32', 'REL32') or addend != 0:
            raise ValueError('unreviewed Effect AddedCallback relocation identity')
        if kind != ('REL32' if symbol in (
                '?GetAnm@EffectAnmManagerView@@QAEPAXH@Z',
                '?PreloadAnm@EffectAnmManagerView@@QAEPAXHPBD@Z') else 'DIR32'):
            raise ValueError('Effect AddedCallback call/data relocation kind changed')
        coverage = set(range(offset, offset+4))
        if offset < 0 or offset+4 > len(linked) or used.intersection(coverage):
            raise ValueError('Effect AddedCallback field is out of owner bounds/overlaps')
        used.update(coverage)
        dest = DESTINATIONS[symbol]
        if (kind, dest) != (expected_kind, expected_destination) or offset < previous_end:
            raise ValueError('Effect AddedCallback candidate field order differs from original')
        previous_end = offset + 4
        encoded = dest - (BASE+offset+4 if kind == 'REL32' else 0)
        struct.pack_into('<I', linked, offset, encoded & 0xffffffff)
        bound.append((offset, kind, dest))
    if Counter((kind, dest) for _, kind, dest in bound) != EXPECTED_FIELDS:
        raise ValueError('Effect AddedCallback missing/duplicate independent destinations')
    return bytes(linked), sorted(bound)


def decode_owner(code: bytes) -> tuple[list, list[tuple], list[int]]:
    import capstone

    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    ins = list(decoder.disasm(code, BASE))
    if not ins or sum(i.size for i in ins) != len(code) or ins[-1].mnemonic != 'ret':
        raise ValueError('incomplete Effect AddedCallback owner / return')
    if ins[-1].op_str:
        raise ValueError('Effect AddedCallback fastcall has unexpected stack cleanup')
    fields, calls = [], []
    for i in ins:
        for operand in i.operands:
            if operand.type == capstone.x86.X86_OP_MEM and 0x400000 <= operand.mem.disp < 0x4E7000:
                if i.disp_size != 4:
                    raise ValueError('unsupported Effect AddedCallback absolute read')
                fields.append((i.address-BASE+i.disp_offset, 'DIR32', operand.mem.disp))
            elif i.mnemonic == 'call' and operand.type == capstone.x86.X86_OP_IMM:
                if i.imm_size != 4:
                    raise ValueError('unsupported Effect AddedCallback direct call encoding')
                fields.append((i.address-BASE+i.imm_offset, 'REL32', operand.imm))
                calls.append(operand.imm)
            elif i.mnemonic == 'push' and operand.type == capstone.x86.X86_OP_IMM and 0x400000 <= operand.imm < 0x4E7000:
                if i.imm_size != 4:
                    raise ValueError('unsupported Effect AddedCallback string literal address')
                fields.append((i.address-BASE+i.imm_offset, 'DIR32', operand.imm))
        if i.mnemonic == 'call' and (len(i.operands) != 1 or i.operands[0].type != capstone.x86.X86_OP_IMM):
            raise ValueError('unreviewed indirect Effect AddedCallback call')
    return ins, sorted(fields), calls


def diagnose(path: Path) -> dict:
    spec = importlib.util.spec_from_file_location(
        'effect_added_coff', ROOT/'scripts/compare-coff-function.py')
    if spec is None or spec.loader is None:
        raise ValueError('cannot import target-bound COFF decoder')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    image = coff.verified_target()
    target = coff.pe_bytes_at(image, BASE, SIZE)
    require_original(target)
    if coff.pe_bytes_at(image, DESTINATIONS['??_C@_09NMANGJFI@etama?4anm?$AA@'], 10) != b'etama.anm\x00':
        raise ValueError('original TH09 ANM filename literal is not independently verified')
    target_instructions, target_fields, target_calls = decode_owner(target)
    if (len(target_instructions) != 19 or tuple(target_fields) != TARGET_FIELDS
            or target_calls != CALL_ORDER):
        raise ValueError('original Effect AddedCallback owner/ABI inventory changed')
    raw, rows = coff.object_function(path, SYMBOL)
    linked, bound = bind_candidate(raw, rows)
    candidate_ins, candidate_fields, candidate_calls = decode_owner(linked)
    if candidate_fields != bound or candidate_calls != CALL_ORDER:
        raise ValueError('candidate Effect AddedCallback operands/call order differ')
    if tuple((kind, dest) for _, kind, dest in bound) != tuple(
            (kind, dest) for _, kind, dest in TARGET_FIELDS):
        raise ValueError('candidate Effect AddedCallback changes original operand order')
    if tuple(target_fields) != TARGET_FIELDS:
        raise ValueError('original Effect AddedCallback field identities differ')
    if len(linked) != SIZE or len(candidate_ins) != len(target_instructions):
        raise ValueError('incomplete Effect AddedCallback physical/instruction extent')
    from diagnostic_branch_alignment import direct_control_flow
    import capstone
    original_graph = direct_control_flow(target_instructions, capstone.x86.X86_OP_IMM)
    candidate_graph = direct_control_flow(candidate_ins, capstone.x86.X86_OP_IMM)
    if len(original_graph) != TARGET_BLOCKS:
        raise ValueError('original Effect callback control-flow block census changed')
    difference_offsets = [n for n, (a, b) in enumerate(zip(target, linked)) if a != b]
    return dict(
        diagnostic_only=True, exactness_credit='none',
        target_bytes=SIZE, candidate_bytes=len(linked),
        target_instructions=len(target_instructions), candidate_instructions=len(candidate_ins),
        target_relocations=len(target_fields), bound_relocations=len(bound),
        ordered_calls_match=target_calls == candidate_calls,
        original_blocks=len(original_graph), candidate_blocks=len(candidate_graph),
        complete_direct_graphs_agree=original_graph == candidate_graph,
        differing_bytes=len(difference_offsets),
        difference_offsets=[hex(n) for n in difference_offsets],
        complete_bytes_equal=linked == target,
        raw_sha256=hashlib.sha256(raw).hexdigest(),
        resolved_sha256=hashlib.sha256(linked).hexdigest(),
        target_sha256=hashlib.sha256(target).hexdigest(),
    )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        report = diagnose(args.object)
    except (OSError, ValueError, KeyError, ImportError, struct.error) as err:
        print(json.dumps({'result':'error', 'message':str(err)}))
        return 2
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print('Effect AddedCallback: %d/%d complete bytes, %d independently bound relocations, %d ordinary differences; exact=%s (NON-CREDITING)' % (
            report['candidate_bytes'], report['target_bytes'], report['bound_relocations'],
            report['differing_bytes'], report['complete_bytes_equal']))
    return 0 if report['complete_bytes_equal'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
