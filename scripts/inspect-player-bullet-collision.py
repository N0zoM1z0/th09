#!/usr/bin/env python3
"""Complete original-PE/COFF audit of TH09 Player::CheckBulletCollision.

Non-crediting: all 862 original physical bytes and 26 reviewed external fields
are independently decoded, not fitted from a candidate's relocation offsets.
The original 33-block graph differs from the current 32-block source; a good
candidate can improve that without silently changing calls or return ABI.
"""
from __future__ import annotations

import argparse
from collections import Counter
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x0041DFF0
SIZE = 862
SYMBOL = '?CheckBulletCollision@PlayerLifecycleView@@QAEHPAUPlayerPositionView@@0PAUBullet@@@Z'
TARGET_SHA256 = '9dcfe1aca4ac06526072014ee6f956120d9c1d84cc1bba044638556b9b82445c'

# Original Japanese TH09 v1.50a, physical per-instruction address/call operands.
# Target-first, NOT copied from the candidate's COFF relocation offsets.
TARGET_FIELDS = (
    (0x050, 'DIR32', 0x0048E818), (0x0BD, 'DIR32', 0x0048E314),
    (0x0F0, 'REL32', 0x0042AF40),
    (0x1BD, 'DIR32', 0x004A7D90), (0x1C5, 'REL32', 0x00401680),
    (0x1D4, 'DIR32', 0x004A7D90), (0x1D9, 'REL32', 0x004016B0),
    (0x1EC, 'DIR32', 0x004B3100), (0x1F8, 'REL32', 0x00401440),
    (0x1FE, 'DIR32', 0x004A80E8), (0x204, 'DIR32', 0x00490F54),
    (0x20A, 'DIR32', 0x004ACE0C), (0x210, 'DIR32', 0x0048E5D8),
    (0x218, 'REL32', 0x00406200), (0x225, 'DIR32', 0x004ACE0C),
    (0x22A, 'REL32', 0x00404910), (0x244, 'DIR32', 0x004A7E0C),
    (0x250, 'REL32', 0x0040CCC0), (0x259, 'DIR32', 0x004B3100),
    (0x260, 'REL32', 0x00401440), (0x2B3, 'REL32', 0x0041BC90),
    (0x2CC, 'DIR32', 0x004DC698), (0x2D1, 'REL32', 0x0043E2F0),
    (0x2D7, 'DIR32', 0x004A7EAC), (0x30E, 'REL32', 0x0041D150),
    (0x318, 'REL32', 0x0040F860),
)
CALLS = (
    0x0042AF40, 0x00401680, 0x004016B0, 0x00401440,
    0x00406200, 0x00404910, 0x0040CCC0, 0x00401440,
    0x0041BC90, 0x0043E2F0, 0x0041D150, 0x0040F860,
)

# Source symbol identities are bound to independently reviewed TH09 targets
# and preserved original-data operands, never to guessed candidate addresses.
SOURCE_TARGETS = {
    '__real@0000000000000000': 0x0048E818,
    '__real@00000000': 0x0048E314,
    '__real@3f000000': 0x00490F54,
    '__real@41000000': 0x0048E5D8,
    '?RotatePlayerCollisionVector@@YIXPAUPlayerPositionView@@0M@Z': 0x0042AF40,
    '?g_GameManager@@3UAsciiGameManagerView@@A': 0x004A7D90,
    '?TransformPopupX@AsciiGameManagerView@@QAEMM@Z': 0x00401680,
    '?TransformPopupY@AsciiGameManagerView@@QAEMM@Z': 0x004016B0,
    '?g_Supervisor@@3VSupervisor@@A': 0x004B3100,
    '?SelectSide@Supervisor@@QAEXH@Z': 0x00401440,
    '?g_ReplayRng@@3URngRuntimeView@@A': 0x004ACE0C,
    '?GetRandomF32SignedInRange@RngRuntimeView@@QAEMM@Z': 0x00406200,
    '?GetRandomF32InRange@RngRuntimeView@@QAEMM@Z': 0x00404910,
    '?g_PlayerRewardEffectManager@@3PAUEffectManager@@A': 0x004A7E0C,
    '?SpawnEffectWithVelocity@EffectManager@@QAEPAUEffect@@HPBUEffectFloat3@@0HI@Z': 0x0040CCC0,
    '?AddRespawnResource@PlayerCancelState4OpsView@@QAEXM@Z': 0x0041BC90,
    '?g_SoundPlayer@@3VSoundPlayer@@A': 0x004DC698,
    '?PlaySoundByIdx@SoundPlayer@@QAEXHH@Z': 0x0043E2F0,
    '?ApplyReward@PlayerOwnerStateView@@QAEXPAUPlayerPositionView@@HHHH@Z': 0x0041D150,
    '?AddScore@PlayerCancelSideView@@QAEXH@Z': 0x0040F860,
}


def encode(kind: str, dest: int, offset: int) -> int:
    if kind not in ('REL32', 'DIR32') or not isinstance(offset, int):
        raise ValueError('unsupported Player collision field encoding')
    return (dest - (BASE + offset + 4 if kind == 'REL32' else 0)) & 0xFFFFFFFF


def original_manifest(code: bytes, *, expected_sha=TARGET_SHA256) -> None:
    if len(code) != SIZE or hashlib.sha256(code).hexdigest() != expected_sha:
        raise ValueError('original Player collision full SHA-256 / extent changed')
    previous_end = 0
    for off, kind, dest in TARGET_FIELDS:
        if off < previous_end or off + 4 > SIZE:
            raise ValueError('original Player collision overlapping or out-of-range field')
        previous_end = off + 4
        if struct.unpack_from('<I', code, off)[0] != encode(kind, dest, off):
            raise ValueError('original Player collision field operand differs')


def bind_candidate(raw: bytes, rows: list[dict]) -> tuple[bytes, list[tuple]]:
    if not 780 <= len(raw) <= 900 or len(rows) != len(TARGET_FIELDS):
        raise ValueError('Player collision candidate physical extent/26-field inventory changed')
    linked = bytearray(raw)
    bound = []
    previous_end = 0
    for r in rows:
        sym, kind, off, add = r['symbol'], r['type'], r['offset'], r['addend']
        if sym not in SOURCE_TARGETS or kind not in ('DIR32', 'REL32') or not isinstance(off, int):
            raise ValueError('unreviewed Player collision source relocation symbol or type')
        expect_kind = 'DIR32' if sym.startswith(('?g_', '__real@')) else 'REL32'
        if kind != expect_kind or off < previous_end or off + 4 > len(raw):
            raise ValueError('Player collision source relocation order/ABI/bounds changed')
        if sym.startswith('__real@') and add != 0:
            raise ValueError('Player collision float literal addend changed')
        if sym.startswith('?g_GameManager@@') and add not in (0, 0x11C, 0x358):
            raise ValueError('Player collision GameManager member offset changed')
        if not sym.startswith('?g_') and not sym.startswith('__real@') and add != 0:
            raise ValueError('Player collision callee ABI/addend changed')
        previous_end = off + 4
        dest = SOURCE_TARGETS[sym] + add
        struct.pack_into('<I', linked, off, encode(kind, dest, off))
        bound.append((off, kind, dest))
    if Counter((kind, dest) for _, kind, dest in bound) != Counter(
            (kind, dest) for _, kind, dest in TARGET_FIELDS):
        raise ValueError('candidate Player collision field multiset disagrees with original')
    return bytes(linked), bound


def disassemble(code: bytes):
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    ins = list(md.disasm(code, BASE))
    if not ins or sum(i.size for i in ins) != len(code):
        raise ValueError('Player collision complete x86 physical instruction decode failed')
    returns = [(i.address-BASE, i.op_str) for i in ins if i.mnemonic == 'ret']
    if len(returns) != 3 or any(op != '0xc' for _, op in returns) or ins[-1].mnemonic != 'ret':
        raise ValueError('Player collision three ret 12 ABI exit contracts changed')
    fields, calls = [], []
    for i in ins:
        if i.mnemonic.startswith('j'):
            continue
        if i.mnemonic == 'call':
            if (len(i.operands) != 1 or i.operands[0].type != capstone.x86.X86_OP_IMM
                    or i.imm_size != 4):
                raise ValueError('unreviewed Player collision indirect/short call')
            dest = i.operands[0].imm
            calls.append(dest)
            fields.append((i.address-BASE+i.imm_offset, 'REL32', dest))
            continue
        for op in i.operands:
            if op.type == capstone.x86.X86_OP_MEM and i.disp_size == 4:
                if 0x00400000 <= op.mem.disp < 0x004F0000:
                    fields.append((i.address-BASE+i.disp_offset, 'DIR32', op.mem.disp))
            elif op.type == capstone.x86.X86_OP_IMM and i.imm_size == 4:
                if 0x00400000 <= op.imm < 0x004F0000:
                    fields.append((i.address-BASE+i.imm_offset, 'DIR32', op.imm))
    return ins, sorted(fields), calls


def diagnose(path: Path) -> dict:
    import capstone
    from diagnostic_branch_alignment import direct_control_flow
    spec = importlib.util.spec_from_file_location('player_bullet_coff', ROOT/'scripts/compare-coff-function.py')
    cof = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(cof)
    pe = cof.verified_target()
    target = cof.pe_bytes_at(pe, BASE, SIZE)
    original_manifest(target)
    for sym, address in SOURCE_TARGETS.items():
        if sym.startswith('__real@'):
            bits = int(sym[7:], 16)
            data = struct.pack('<Q' if len(sym[7:]) == 16 else '<I', bits)
            if cof.pe_bytes_at(pe, address, len(data)) != data:
                raise ValueError('Player collision original referenced constant does not match')
    ti, tf, tc = disassemble(target)
    if len(ti) != 277 or tf != list(TARGET_FIELDS) or tuple(tc) != CALLS:
        raise ValueError('independently decoded original Player collision fields/calls differ')
    raw, rows = cof.object_function(path, SYMBOL)
    linked, bound = bind_candidate(raw, rows)
    ci, cf, cc = disassemble(linked)
    if cf != bound or cc != tc:
        raise ValueError('Player collision actual source instructions do not bind to reviewed COFF calls/fields')
    tg = direct_control_flow(ti, capstone.x86.X86_OP_IMM)
    cg = direct_control_flow(ci, capstone.x86.X86_OP_IMM)
    if len(tg) != 33:
        raise ValueError('original Player collision 33-block graph drifted')
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(raw),
        'target_instructions': len(ti), 'candidate_instructions': len(ci),
        'original_fields': len(tf), 'bound_fields': len(cf),
        'ordered_calls': len(tc), 'ordered_calls_agree': cc == tc,
        'ordered_fields_agree': [(k,v) for _,k,v in cf] == [(k,v) for _,k,v in tf],
        'original_blocks': len(tg), 'candidate_blocks': len(cg),
        'direct_graphs_agree': cg == tg,
        'overlap_differences': sum(a != b for a, b in zip(target, linked)),
        'missing_target_bytes': max(0, SIZE-len(raw)),
        'extra_candidate_bytes': max(0, len(raw)-SIZE),
        'complete_bytes_equal': target == linked,
        'target_sha256': TARGET_SHA256, 'raw_sha256': hashlib.sha256(raw).hexdigest(),
    }


def main() -> int:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('object', type=Path)
    p.add_argument('--json', action='store_true')
    args = p.parse_args()
    try:
        report = diagnose(args.object)
    except (OSError, ValueError, KeyError, ImportError, struct.error) as exc:
        print('Player bullet collision original-bound audit rejected:', exc, file=sys.stderr)
        return 2
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print('NON-CREDITING Player CheckBulletCollision:', report['candidate_bytes'], '/', SIZE,
              'physical bytes,', report['bound_fields'], 'original fields;',
              report['overlap_differences'], 'different linked bytes')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
