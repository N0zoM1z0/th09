#!/usr/bin/env python3
"""Complete, noncrediting TH09 Enemy collision-box wrapper byte audit.

The two callee identities are pinned independently to canonical TH09 targets:
Float3::operator/(float) and PlayerCollisionQueryStateView::AppendBoxRecord.
The original PE is read-only and SHA-attested. This diagnostic cannot grant
exact codegen credit, prove object ownership or validate runtime behavior.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x00410250
SIZE = 82
TARGET_SHA256 = "e2be799860dde7b752f525cb13f4135c4666917745006a511e734e10cd2ff9c0"
SYMBOL = '?AppendPlayerCollisionBox@EnemyAppendCollisionView@@QAEXPBUFloat3@@0@Z'
CALL_TARGETS = {
    '??KFloat3@@QBE?AU0@M@Z': 0x0040F5A0,
    '?AppendBoxRecord@PlayerCollisionQueryStateView@@QAEXPBUPlayerPositionView@@0H@Z': 0x0040F8E0,
}
ORDERED_CALLS = list(CALL_TARGETS.values())


def bind_candidate(raw: bytes, fields: list[dict]) -> bytes:
    """Bind only the two reviewed, complete 32-bit relative call fields."""
    if len(fields) != 2:
        raise ValueError('collision wrapper requires precisely two COFF call fields')
    out = bytearray(raw)
    used = set()
    for row, (symbol, destination) in zip(fields, CALL_TARGETS.items()):
        off = int(row['offset'])
        if (row['symbol'], row['type'], int(row['addend'])) != (symbol, 'REL32', 0):
            raise ValueError('collision wrapper call symbol/ABI/field type mismatch')
        span = set(range(off, off + 4))
        if off < 0 or off + 4 > len(out) or span & used:
            raise ValueError('collision wrapper call field outside extent/overlapping')
        used.update(span)
        struct.pack_into('<I', out, off,
                         (destination - (BASE + off + 4)) & 0xffffffff)
    return bytes(out)


def decode_owner(raw: bytes) -> tuple[list, list[int]]:
    import capstone

    cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    cs.detail = True
    ins = list(cs.disasm(raw, BASE))
    if not ins or sum(i.size for i in ins) != len(raw):
        raise ValueError('collision wrapper disassembly does not cover all bytes')
    if ins[-1].mnemonic != 'ret' or ins[-1].op_str != '8':
        raise ValueError('collision wrapper ABI requires terminal ret 8')
    calls = []
    for i in ins:
        if i.mnemonic == 'call':
            if (len(i.operands) != 1 or
                    i.operands[0].type != capstone.x86.X86_OP_IMM or
                    i.imm_size != 4):
                raise ValueError('collision wrapper contains unreviewed indirect call')
            calls.append(i.operands[0].imm)
    return ins, calls


def diagnose(obj: Path) -> dict:
    spec = importlib.util.spec_from_file_location(
        'enemy_collision_coff', ROOT / 'scripts/compare-coff-function.py')
    if spec is None or spec.loader is None:
        raise ValueError('cannot load target-bound COFF parser')
    compare = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(compare)
    target = compare.pe_bytes_at(compare.verified_target(), BASE, SIZE)
    if hashlib.sha256(target).hexdigest() != TARGET_SHA256:
        raise ValueError("original Enemy collision-box full-owner SHA changed")
    ti, tc = decode_owner(target)
    if tc != ORDERED_CALLS or len(ti) != 30:
        raise ValueError('verified TH09 target call/physical extent drifted')
    raw, relocations = compare.object_function(obj, SYMBOL)
    linked = bind_candidate(raw, relocations)
    ci, cc = decode_owner(linked)
    if cc != ORDERED_CALLS:
        raise ValueError('collision wrapper candidate calls differ from original')
    if len(linked) != SIZE:
        raise ValueError('collision wrapper candidate owner extent differs from original')
    for row, destination in zip(relocations, ORDERED_CALLS):
        off = int(row['offset'])
        if not any((i.address - BASE + i.imm_offset) == off
                   for i in ci if i.mnemonic == 'call' and i.imm_size == 4):
            raise ValueError('COFF call field does not coincide with decoded call')
        if destination not in cc:
            raise ValueError('reviewed collision destination not present')
    diffs = [off for off, (a, b) in enumerate(zip(target, linked)) if a != b]
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_sha256': hashlib.sha256(target).hexdigest(),
        'target_bytes': SIZE, 'candidate_bytes': len(linked),
        'target_instructions': len(ti), 'candidate_instructions': len(ci),
        'bound_call_fields': len(relocations), 'ordered_call_addresses':
            [hex(x) for x in cc],
        'ordinary_differing_bytes': len(diffs),
        'difference_offsets': [hex(x) for x in diffs],
        'complete_bytes_agree': linked == target,
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
        'linked_sha256': hashlib.sha256(linked).hexdigest(),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        report = diagnose(args.object)
    except (OSError, KeyError, ValueError, ImportError, struct.error) as exc:
        print(json.dumps({'error': str(exc)}))
        return 2
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print('Enemy collision wrapper %d/%d bytes, %d true call fields, %d differing bytes; exact=%s (NON-CREDITING)' %
              (report['candidate_bytes'], report['target_bytes'],
               report['bound_call_fields'], report['ordinary_differing_bytes'],
               report['complete_bytes_agree']))
    return 0 if report['complete_bytes_agree'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
