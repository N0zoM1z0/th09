#!/usr/bin/env python3
"""Original-PE/real-COFF audit of TH09 FrontMessageRandom::SetupRandomForSide.

Non-crediting: all original bytes, independent physical address operands and
source symbols, actual direct calls and complete x86 CFG must be explained.
Neither a score nor a close body can create an accepted exact-match receipt.
"""
from __future__ import annotations

import argparse
from difflib import SequenceMatcher
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x004162D0
SIZE = 693
SYMBOL = '?SetupRandomForSide@FrontMessageRandomView@@QAEXH@Z'
TARGET_SHA256 = '915acb9e918496d6397b6fe577ae5250628892741d94a8961c3555b7ab94ddda'

# Independently decoded ORIGINAL physical operands, in original byte order.
# These are not obtained from candidate offsets or fitted by symbolic search.
ORIGINAL_FIELDS = (
    (0x011, 'DIR32', 0x004A7DE8),
    (0x04E, 'DIR32', 0x004ACE0C),
    (0x056, 'REL32', 0x004048B0),
    (0x095, 'DIR32', 0x004ACE0C),
    (0x09A, 'REL32', 0x004048B0),
    (0x217, 'DIR32', 0x004B36CC),
    (0x223, 'REL32', 0x00403E00),
    (0x229, 'DIR32', 0x004B36CC),
    (0x237, 'REL32', 0x00403E00),
    (0x26F, 'DIR32', 0x004DC550),
    (0x274, 'DIR32', 0x0048EE7C),
    (0x27D, 'REL32', 0x0043BDC0),
    (0x297, 'DIR32', 0x004DC550),
    (0x29C, 'DIR32', 0x0048EE7C),
    (0x2A5, 'REL32', 0x0043BDC0),
)
CALL_ORDER = (0x004048B0, 0x004048B0, 0x00403E00, 0x00403E00,
              0x0043BDC0, 0x0043BDC0)

# Actual VC7.1 COFF symbols and original referents; the GameManager offset
# is a verified +0x58 member of the already-mapped global, not a new global.
REFERENTS = {
    '?g_GameManager@@3UGameManagerModeView@@A': ('DIR32', 0x004A7D90, (0x58,)),
    '?g_Rng@@3URngRuntimeView@@A': ('DIR32', 0x004ACE0C, (0,)),
    '?GetRandomU16InRange@RngRuntimeView@@QAEGG@Z': ('REL32', 0x004048B0, (0,)),
    '?g_FrontMessageAnm@@3PAVAnmLoaded@@A': ('DIR32', 0x004B36CC, (0,)),
    '?SetAndExecuteScriptIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z': ('REL32', 0x00403E00, (0,)),
    '?g_AnmManager@@3PAVAnmManager@@A': ('DIR32', 0x004DC550, (0,)),
    '??_C@_01CLKCMJKC@?5?$AA@': ('DIR32', 0x0048EE7C, (0,)),
    '?DrawTextLeft@AnmManager@@QAAXPAUAnmVm@@KKPBDZZ': ('REL32', 0x0043BDC0, (0,)),
}


def encoded(kind: str, dest: int, offset: int, *, base: int = BASE) -> int:
    if kind not in ('REL32', 'DIR32') or not isinstance(offset, int) or offset < 0:
        raise ValueError('unsupported Front code operand encoding')
    return (dest - (base + offset + 4 if kind == 'REL32' else 0)) & 0xffffffff


def original_manifest(code: bytes, *, expected_sha: str = TARGET_SHA256) -> None:
    if len(code) != SIZE or hashlib.sha256(code).hexdigest() != expected_sha:
        raise ValueError('original Front random full SHA-256 / extent changed')
    previous_end = 0
    for off, kind, dest in ORIGINAL_FIELDS:
        if off < previous_end or off + 4 > SIZE:
            raise ValueError('original Front random operand collision/extent')
        previous_end = off + 4
        if struct.unpack_from('<I', code, off)[0] != encoded(kind, dest, off):
            raise ValueError('original Front random independently decoded address field differs')


def bind_candidate(raw: bytes, rows: list[dict]) -> tuple[bytes, list[tuple]]:
    if not 600 <= len(raw) <= 760 or len(rows) != len(ORIGINAL_FIELDS):
        raise ValueError('Front random candidate lost whole body or address fields')
    bound = []
    linked = bytearray(raw)
    previous_end = 0
    for row in rows:
        symbol = row['symbol']
        if symbol not in REFERENTS:
            raise ValueError('unreviewed Front random COFF symbol: ' + symbol)
        kind, base, accepted_addends = REFERENTS[symbol]
        addend = row['addend']
        off = row['offset']
        if (row['type'] != kind or addend not in accepted_addends or
                not isinstance(off, int) or off < previous_end or off + 4 > len(raw)):
            raise ValueError('Front random candidate address kind/addend/order/bounds drifted')
        previous_end = off + 4
        dest = base + addend
        struct.pack_into('<I', linked, off, encoded(kind, dest, off))
        bound.append((off, kind, dest))
    if [(k, d) for _, k, d in bound] != [(k, d) for _, k, d in ORIGINAL_FIELDS]:
        raise ValueError('Front random source fields differ from independent target identities/order')
    return bytes(linked), bound


def decode(code: bytes):
    import capstone

    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    instructions = list(md.disasm(code, BASE))
    if (not instructions or sum(i.size for i in instructions) != len(code) or
            instructions[-1].mnemonic != 'ret' or instructions[-1].op_str not in ('4', '0x4')):
        raise ValueError('Front random incomplete physical x86 decode or wrong ret 4 ABI')
    fields, calls = [], []
    for ins in instructions:
        if ins.mnemonic.startswith('j'):
            continue
        if ins.mnemonic == 'call':
            if (len(ins.operands) != 1 or ins.operands[0].type != capstone.x86.X86_OP_IMM
                    or ins.imm_size != 4):
                raise ValueError('Front random unreviewed indirect or non-REL32 call')
            dest = ins.operands[0].imm
            fields.append((ins.address - BASE + ins.imm_offset, 'REL32', dest))
            calls.append(dest)
            continue
        for operand in ins.operands:
            if (operand.type == capstone.x86.X86_OP_MEM and ins.disp_size == 4 and
                    0x400000 <= operand.mem.disp < 0x4F0000):
                fields.append((ins.address - BASE + ins.disp_offset,
                               'DIR32', operand.mem.disp))
            elif (operand.type == capstone.x86.X86_OP_IMM and ins.imm_size == 4 and
                  0x400000 <= operand.imm < 0x4F0000):
                fields.append((ins.address - BASE + ins.imm_offset,
                               'DIR32', operand.imm))
    return instructions, fields, calls


def normalized_instructions(instructions, fields):
    """Diagnostic-only LCS: ignore reviewed operands and direct branch deltas."""
    normalized = []
    for ins in instructions:
        blob = bytearray(ins.bytes)
        if ins.mnemonic.startswith('j') and ins.imm_size:
            blob[ins.imm_offset:ins.imm_offset + ins.imm_size] = bytes(ins.imm_size)
        for off, _, _ in fields:
            local_off = off - (ins.address - BASE)
            if 0 <= local_off <= len(blob) - 4:
                blob[local_off:local_off + 4] = bytes(4)
        normalized.append(bytes(blob))
    return normalized


def diagnose(path: Path) -> dict:
    import capstone
    from diagnostic_branch_alignment import direct_control_flow

    spec = importlib.util.spec_from_file_location('front_random_coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    pe = coff.verified_target()
    original = coff.pe_bytes_at(pe, BASE, SIZE)
    original_manifest(original)
    if coff.pe_bytes_at(pe, 0x0048EE7C, 2) != b' \x00':
        raise ValueError('Front random original text literal identity changed')
    original_ins, original_fields, original_calls = decode(original)
    if (len(original_ins) != 206 or original_fields != list(ORIGINAL_FIELDS) or
            tuple(original_calls) != CALL_ORDER):
        raise ValueError('Front random original instruction/field/call census changed')
    raw, rows = coff.object_function(path, SYMBOL)
    linked, bound = bind_candidate(raw, rows)
    candidate_ins, candidate_fields, candidate_calls = decode(linked)
    if candidate_fields != bound or candidate_calls != original_calls:
        raise ValueError('Front random actual instructions disagree with COFF field or call identity')
    original_graph = direct_control_flow(original_ins, capstone.x86.X86_OP_IMM)
    candidate_graph = direct_control_flow(candidate_ins, capstone.x86.X86_OP_IMM)
    if len(original_graph) != 40:
        raise ValueError('Front random original control-flow graph census changed')
    aligned = SequenceMatcher(None,
        normalized_instructions(original_ins, original_fields),
        normalized_instructions(candidate_ins, candidate_fields),
        autojunk=False).get_matching_blocks()
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(raw),
        'target_instructions': len(original_ins), 'candidate_instructions': len(candidate_ins),
        'original_fields': len(original_fields), 'bound_fields': len(bound),
        'ordered_calls': len(original_calls), 'ordered_calls_agree': candidate_calls == original_calls,
        'target_blocks': len(original_graph), 'candidate_blocks': len(candidate_graph),
        'direct_graphs_agree': original_graph == candidate_graph,
        'normalized_aligned_instructions': sum(block.size for block in aligned),
        'aligned_instructions': sum(block.size for block in aligned),
        'overlap_differences': sum(a != b for a, b in zip(original, linked)),
        'missing_target_bytes': max(0, SIZE - len(linked)),
        'extra_candidate_bytes': max(0, len(linked) - SIZE),
        'complete_bytes_equal': linked == original,
        'target_sha256': TARGET_SHA256,
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        diagnostic = diagnose(args.object)
    except (ValueError, TypeError, KeyError, OSError, ImportError, struct.error) as exc:
        print('Front random original-bound audit rejected:', exc, file=sys.stderr)
        return 2
    if args.json:
        print(json.dumps(diagnostic, indent=2))
    else:
        print('NON-CREDITING Front SetupRandomForSide:', diagnostic['candidate_bytes'], '/',
              SIZE, 'bytes,', diagnostic['bound_fields'], 'fields,',
              diagnostic['overlap_differences'], 'overlapping differences')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
