#!/usr/bin/env python3
"""Full-field, non-crediting TH09 Type3 update and middle-Z scheduling oracle.

This intentionally distinguishes the 14-byte candidate's premature zero write
from the original's post-middle-Y / pre-tail-X write. Field identities come
from independent TH09 target disassembly, and candidate values only from its
real COFF records. No target bytes are inserted into the source object.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE, SIZE = 0x00442750, 780
TARGET_SHA256 = "19acde6cfdea958c572992f6832e09f51630ce2508f4d983eba4f3cb87e05eb4"
SYMBOL = '?ExAttackUpdateCallbackType3@@YIHPAUExAttackRecord@@@Z'
MIDDLE_Y_CALLEE = 0x00442450
TAIL_X_FIELD = 'eax, dword ptr [edi + 0x130]'
MIDDLE_Z_FIELD = 'dword ptr [ebp - 0x14], 0'


def module(name: str, path: str):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / path)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def decode(data: bytes):
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    instructions = list(md.disasm(data, BASE))
    if not instructions or sum(ins.size for ins in instructions) != len(data):
        raise ValueError('Type3 owner has incomplete or overlapping machine instructions')
    if instructions[-1].mnemonic != 'ret':
        raise ValueError('Type3 owner lacks reviewed terminal return')
    return instructions


def addressed_fields(instructions) -> list[tuple[int, str, int]]:
    """Decode address operands without confusing branches with data/calls."""
    import capstone
    result = []
    for ins in instructions:
        if ins.mnemonic.startswith('j'):
            continue
        if ins.mnemonic == 'call':
            if len(ins.operands) != 1 or ins.operands[0].type != capstone.x86.X86_OP_IMM or ins.imm_size != 4:
                raise ValueError('unexpected Type3 indirect or short call')
            result.append((ins.address - BASE + ins.imm_offset,
                           'REL32', ins.operands[0].imm))
            continue
        for op in ins.operands:
            if op.type == capstone.x86.X86_OP_MEM and ins.disp_size == 4:
                if 0x00400000 <= op.mem.disp < 0x004E7000:
                    result.append((ins.address - BASE + ins.disp_offset,
                                   'DIR32', op.mem.disp))
            elif op.type == capstone.x86.X86_OP_IMM and ins.imm_size == 4:
                if 0x00400000 <= op.imm < 0x004E7000:
                    result.append((ins.address - BASE + ins.imm_offset,
                                   'DIR32', op.imm))
    return result


def bind_candidate(raw: bytes, rows: list[dict],
                   target: bytes, target_fields: list[tuple[int, str, int]],
                   expected_destinations: dict[str, int]) -> bytes:
    if len(raw) != SIZE or len(target) != SIZE:
        raise ValueError('Type3 complete physical owner is not 780 bytes')
    if len(rows) != len(target_fields) or len(rows) != 38:
        raise ValueError('Type3 source/target 38-field inventory changed')
    linked = bytearray(raw)
    prev_end = 0
    for row, original in zip(rows, target_fields):
        orig_offset, orig_kind, orig_dest = original
        symbol = row['symbol']
        if symbol not in expected_destinations:
            raise ValueError('unknown Type3 external owner: ' + symbol)
        off = row['offset']
        kind = row['type']
        dest = expected_destinations[symbol] + row['addend']
        if dest != orig_dest or kind != orig_kind or kind not in ('DIR32', 'REL32'):
            raise ValueError('candidate changed an original Type3 address identity/order')
        if not isinstance(off, int) or off < prev_end or off + 4 > len(raw):
            raise ValueError('Type3 candidate has overlapping, reordered or out-of-range fields')
        prev_end = off + 4
        original_value = dest - (BASE + orig_offset + 4 if kind == 'REL32' else 0)
        if struct.unpack_from('<I', target, orig_offset)[0] != original_value & 0xFFFFFFFF:
            raise ValueError('target Type3 original address identity changed')
        candidate_value = dest - (BASE + off + 4 if kind == 'REL32' else 0)
        struct.pack_into('<I', linked, off, candidate_value & 0xFFFFFFFF)
    return bytes(linked)


def middle_z_order(instructions) -> dict:
    """Classify real Y-call / middle-Z store / tail-X operand order."""
    import capstone
    y_calls = [ins.address - BASE for ins in instructions if ins.mnemonic == 'call'
               and len(ins.operands) == 1
               and ins.operands[0].type == capstone.x86.X86_OP_IMM
               and ins.operands[0].imm == MIDDLE_Y_CALLEE]
    z_writes = [ins.address - BASE for ins in instructions
                if ins.mnemonic == 'mov' and ins.op_str == MIDDLE_Z_FIELD]
    tail_x_loads = [ins.address - BASE for ins in instructions
                    if ins.mnemonic == 'mov' and ins.op_str == TAIL_X_FIELD]
    if len(y_calls) != 3 or len(z_writes) != 1 or len(tail_x_loads) != 1:
        raise ValueError('Type3 middle-Y/Z and tail-X instruction census is not reviewed')
    y, z, tail_x = y_calls[1], z_writes[0], tail_x_loads[0]
    if y < z < tail_x:
        classification = 'matches-original-chronology'
    elif z < y < tail_x:
        classification = 'zero-before-middle-y-call'
    elif y < tail_x < z:
        classification = 'zero-after-tail-x-load'
    else:
        classification = 'unreviewed-order'
    return {'middle_y_call_offset': hex(y), 'middle_z_store_offset': hex(z),
            'tail_x_load_offset': hex(tail_x), 'classification': classification}


def literal_payload(data: bytes, row: dict) -> bytes:
    """Read a defined scalar from its COFF section, independently of its name."""
    if len(data) < 20:
        raise ValueError('truncated Type3 COFF header')
    machine, count = struct.unpack_from('<HH', data)
    optional = struct.unpack_from('<H', data, 16)[0]
    section, value = row.get('symbol_section'), row.get('symbol_value')
    if (machine != 0x14C or optional != 0 or type(section) is not int or
            not 1 <= section <= count or type(value) is not int or value < 0):
        raise ValueError('Type3 literal lacks an actual i386 COFF definition')
    header = 20 + (section - 1) * 40
    if header + 40 > len(data):
        raise ValueError('truncated Type3 COFF section header')
    size, start = struct.unpack_from('<II', data, header + 16)
    if (start < 20 + count * 40 or value + 4 > size or
            start + size > len(data)):
        raise ValueError('Type3 literal definition leaves its actual section')
    return data[start + value:start + value + 4]


def check_literal_definitions(data, rows, image, coff, destinations):
    literals = set()
    for row in rows:
        symbol = row['symbol']
        if not symbol.startswith('__real@'):
            continue
        expected = struct.pack('<I', int(symbol[7:], 16))
        if (symbol not in destinations or
                literal_payload(data, row) != expected or
                coff.pe_bytes_at(image, destinations[symbol], 4) != expected):
            raise ValueError('Type3 source/original literal definition differs: ' + symbol)
        literals.add(symbol)
    return len(literals)


def context_report(object_path: Path) -> dict:
    """Diagnose a complete experimental owner with changed extents or fields.

    This never relaxes bind_candidate or the production frontier. Relocation
    values come from the reviewed referent map, not a candidate-offset match.
    """
    from collections import Counter
    import capstone
    from diagnostic_branch_alignment import direct_control_flow
    coff = module('type3_context_coff', 'compare-coff-function.py')
    own = module('type3_context_dest', 'inspect-exattack-type3.py')
    destinations = own.diagnostic.DESTINATIONS
    image = coff.verified_target()
    target = coff.pe_bytes_at(image, BASE, SIZE)
    if hashlib.sha256(target).hexdigest() != TARGET_SHA256:
        raise ValueError('original TH09 Type3 update full-owner SHA changed')
    ti = decode(target)
    target_fields = addressed_fields(ti)
    if len(ti) != 240 or len(target_fields) != 38:
        raise ValueError('original Type3 instruction/field inventory changed')
    original_referents = {(kind, dest) for _, kind, dest in target_fields}
    raw, rows = coff.object_function(object_path, SYMBOL, include_symbol_locations=True)
    linked = bytearray(raw)
    previous_end = 0
    for row in rows:
        off, kind = row['offset'], row['type']
        if (row['symbol'] not in destinations or kind not in ('DIR32', 'REL32') or
                type(off) is not int or off < previous_end or off + 4 > len(raw)):
            raise ValueError('unreviewed or overlapping Type3 context field')
        destination = destinations[row['symbol']] + row['addend']
        if (kind, destination) not in original_referents:
            raise ValueError('Type3 context introduced a new original referent')
        value = destination - (BASE + off + 4 if kind == 'REL32' else 0)
        struct.pack_into('<I', linked, off, value & 0xFFFFFFFF)
        previous_end = off + 4
    ci = decode(linked)
    candidate_fields = addressed_fields(ci)
    expected_fields = [(r['offset'], r['type'], destinations[r['symbol']] + r['addend'])
                       for r in rows]
    if candidate_fields != expected_fields:
        raise ValueError('Type3 context actual instruction/COFF coverage differs')
    literal_count = check_literal_definitions(
        object_path.read_bytes(), rows, image, coff, destinations)
    original_counts = Counter((kind, dest) for _, kind, dest in target_fields)
    candidate_counts = Counter((kind, dest) for _, kind, dest in candidate_fields)
    calls = lambda ins: [i.operands[0].imm for i in ins if i.mnemonic == 'call']
    differences = [hex(i) for i, (a, b) in enumerate(zip(target, linked)) if a != b]
    def census(counts):
        return [{'type': k, 'destination': hex(d), 'count': count}
                for (k, d), count in sorted(counts.items())]
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_sha256': hashlib.sha256(target).hexdigest(),
        'candidate_raw_sha256': hashlib.sha256(raw).hexdigest(),
        'target_bytes': len(target), 'candidate_bytes': len(raw),
        'target_instructions': len(ti), 'candidate_instructions': len(ci),
        'original_fields': len(target_fields), 'bound_fields': len(rows),
        'literal_definitions_checked': literal_count,
        'original_only_referents': census(original_counts - candidate_counts),
        'candidate_only_referents': census(candidate_counts - original_counts),
        'ordered_calls_agree': calls(ti) == calls(ci),
        'direct_graphs_agree': direct_control_flow(ti, capstone.x86.X86_OP_IMM) ==
                              direct_control_flow(ci, capstone.x86.X86_OP_IMM),
        'overlap_differences': len(differences), 'difference_offsets': differences,
        'missing_target_bytes': max(0, len(target) - len(raw)),
        'extra_candidate_bytes': max(0, len(raw) - len(target)),
        'complete_bytes_equal': target == linked,
    }


def analyze(object_path: Path) -> dict:
    from diagnostic_branch_alignment import direct_control_flow
    import capstone
    from collections import Counter
    coff = module('th09_type3_schedule_coff', 'compare-coff-function.py')
    own = module('th09_type3_schedule_dest', 'inspect-exattack-type3.py')
    pe = coff.verified_target()
    target = coff.pe_bytes_at(pe, BASE, SIZE)
    if hashlib.sha256(target).hexdigest() != TARGET_SHA256:
        raise ValueError("original TH09 Type3 update full-owner SHA changed")
    raw, rows = coff.object_function(object_path, SYMBOL, include_symbol_locations=True)
    literal_count = check_literal_definitions(
        object_path.read_bytes(), rows, pe, coff, own.diagnostic.DESTINATIONS)
    ti = decode(target)
    target_fields = addressed_fields(ti)
    if len(ti) != 240 or len(target_fields) != 38:
        raise ValueError('original Type3 240-instruction /38-field inventory changed')
    linked = bind_candidate(raw, rows, target, target_fields, own.diagnostic.DESTINATIONS)
    ci = decode(linked)
    if len(ci) != 240 or addressed_fields(ci) != [
            (row['offset'], kind, dest) for row, (_, kind, dest) in zip(rows, target_fields)]:
        raise ValueError('decoded Type3 operand fields diverge from actual COFF fields')
    target_order = middle_z_order(ti)
    candidate_order = middle_z_order(ci)
    if target_order['classification'] != 'matches-original-chronology':
        raise ValueError('original Type3 middle-Z semantics/order changed')
    def calls(ins):
        return [i.operands[0].imm for i in ins if i.mnemonic == 'call']
    if len(calls(ti)) != 24 or calls(ti) != calls(ci):
        raise ValueError('Type3 24 independently addressed call identities differ')
    tg = direct_control_flow(ti, capstone.x86.X86_OP_IMM)
    cg = direct_control_flow(ci, capstone.x86.X86_OP_IMM)
    different_offsets = [off for off, (a, b) in enumerate(zip(target, linked)) if a != b]
    return {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(raw),
        'target_instructions': len(ti), 'candidate_instructions': len(ci),
        'target_fields': len(target_fields), 'bound_fields': len(rows),
        'literal_definitions_checked': literal_count,
        'ordered_calls': len(calls(ti)), 'calls_agree': True,
        'target_blocks': len(tg), 'candidate_blocks': len(cg),
        'direct_graphs_agree': tg == cg,
        'original_z_order': target_order,
        'candidate_z_order': candidate_order,
        'ordinary_differences': len(different_offsets),
        'difference_offsets': [hex(off) for off in different_offsets],
        'complete_bytes_equal': target == linked,
        'candidate_raw_sha256': hashlib.sha256(raw).hexdigest(),
        'target_sha256': hashlib.sha256(target).hexdigest(),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--context', action='store_true',
                        help='diagnose a changed owner; never production acceptance')
    args = parser.parse_args()
    try:
        result = context_report(args.object) if args.context else analyze(args.object)
        print(json.dumps(result, indent=2))
        return 0
    except (OSError, ValueError, KeyError, ImportError, struct.error) as err:
        print('Type3 schedule input rejected:', err, file=__import__('sys').stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
