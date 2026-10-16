#!/usr/bin/env python3
"""Non-crediting complete PE/VC7.1 audit of TH09 EtamaController::OnDraw.

The 600-byte target owner and its 27 code operands are decoded independently.
Only reviewed TH09 callees, globals and checked literal payloads may bind the
candidate's COFF fields. The physical graph and every code byte remain visible;
no aligned-instruction or nonsemantic similarity grants exactness credit.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x00413BE0
SIZE = 600
SYMBOL = '?OnDraw@EtamaController@@SIHPAU1@@Z'
TARGET_SHA256 = '6216f7ba73f38523afb6c40385b2b74190a657dc0388b8b8192d65d7f7e8f6b1'

# Independently reviewed original TH09 identities. The two draw callees and
# viewport/Float3 leaves are covered by separate canonical exact match units;
# the literals also have independently checked payloads in the original PE.
DESTINATIONS = {
    '?g_Supervisor@@3VSupervisor@@A': 0x004B3100,
    '?ConfigureGameplayViewport@Supervisor@@QAEXH@Z': 0x00401390,
    '?fsincos@@YIXPAM0M@Z': 0x00412780,
    '__real@3f000000': 0x00490F54,
    '??BFloat3@@QAEPAMXZ': 0x004343D0,
    '?g_GameManager@@3UAsciiGameManagerView@@A': 0x004A7D90,
    '?TransformPopupX@AsciiGameManagerView@@QAEMM@Z': 0x00401680,
    '?TransformPopupY@AsciiGameManagerView@@QAEMM@Z': 0x004016B0,
    '?g_AnmManager@@3PAVAnmManager@@A': 0x004DC550,
    '?Draw2D@AnmManager@@QAEHPAUAnmVm@@@Z': 0x0043AB50,
    '__real@41800000': 0x0048E470,
    '__real@00000000': 0x0048E314,
    '__real@3dcccccd': 0x0048E4B0,
    '__real@3d800000': 0x0048E828,
    '?DrawSingleBullet@@YIHPAUBullet@@@Z': 0x00413AF0,
    '?DrawBulletLayerEffects@BulletDrawEffectManagerView@@QAEHXZ': 0x0040D110,
}


def expected_kind(symbol: str) -> str:
    if symbol not in DESTINATIONS:
        raise ValueError('unknown Etama draw relocation symbol: ' + symbol)
    return 'DIR32' if symbol.startswith(('?g_', '__real@')) else 'REL32'


def bind_candidate(raw: bytes, relocations: list[dict]) -> tuple[bytes, list[tuple]]:
    """Bind actual COFF fields without looking up or fitting any target bytes."""
    if len(relocations) != 27:
        raise ValueError('Etama draw expects exactly 27 reviewed COFF fields')
    linked = bytearray(raw)
    seen = set()
    manifest = []
    for row in relocations:
        symbol = row['symbol']
        kind = expected_kind(symbol)
        offset = int(row['offset'])
        if row['type'] != kind or int(row['addend']) != 0:
            raise ValueError('Etama draw relocation kind/addend differs: ' + symbol)
        section = set(range(offset, offset + 4))
        if offset < 0 or offset + 4 > len(linked) or section & seen:
            raise ValueError('Etama draw relocation outside owner or overlapping')
        seen.update(section)
        destination = DESTINATIONS[symbol]
        value = destination - (BASE + offset + 4 if kind == 'REL32' else 0)
        struct.pack_into('<I', linked, offset, value & 0xffffffff)
        manifest.append((offset, kind, destination))
    if [x[0] for x in manifest] != sorted(x[0] for x in manifest):
        raise ValueError('Etama draw COFF fields are unordered')
    return bytes(linked), manifest


def decoded_fields(instructions, capstone_module):
    """Independently decode direct calls and absolute data operands."""
    fields, calls = [], []
    for ins in instructions:
        if ins.mnemonic == 'call':
            if len(ins.operands) != 1 or ins.operands[0].type != capstone_module.x86.X86_OP_IMM or ins.imm_size != 4:
                raise ValueError('Etama draw has unreviewed indirect call')
            destination = ins.operands[0].imm
            fields.append((ins.address - BASE + ins.imm_offset, 'REL32', destination))
            calls.append(destination)
            continue
        if ins.mnemonic.startswith('j'):
            continue
        for op in ins.operands:
            if op.type == capstone_module.x86.X86_OP_MEM and ins.disp_size == 4:
                mem = op.mem
                if mem.base == 0 and mem.index == 0 and 0x400000 <= mem.disp < 0x4E7000:
                    fields.append((ins.address - BASE + ins.disp_offset, 'DIR32', mem.disp))
            if op.type == capstone_module.x86.X86_OP_IMM and ins.imm_size == 4:
                if 0x400000 <= op.imm < 0x4E7000:
                    fields.append((ins.address - BASE + ins.imm_offset, 'DIR32', op.imm))
    return sorted(fields), calls


def load_local(name: str, filename: str):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'scripts' / filename)
    if spec is None or spec.loader is None:
        raise ValueError('unavailable Etama diagnostic dependency: ' + filename)
    obj = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(obj)
    return obj


def inspect(path: Path, *, explain: bool = False) -> dict:
    import capstone
    from diagnostic_branch_alignment import direct_control_flow
    from ecl_shape_differences import changed_instructions

    coff = load_local('etama_draw_coff', 'compare-coff-function.py')
    pe = coff.verified_target()
    target = coff.pe_bytes_at(pe, BASE, SIZE)
    if hashlib.sha256(target).hexdigest() != TARGET_SHA256:
        raise ValueError('Etama draw original physical owner identity changed')
    for symbol, destination in DESTINATIONS.items():
        if symbol.startswith('__real@'):
            actual = coff.pe_bytes_at(pe, destination, 4)
            if actual != struct.pack('<I', int(symbol[7:], 16)):
                raise ValueError('Etama draw original float constant differs: ' + symbol)

    raw, fields = coff.object_function(path, SYMBOL)
    candidate, bound_fields = bind_candidate(raw, fields)
    cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    cs.detail = True
    ti = list(cs.disasm(target, BASE))
    ci = list(cs.disasm(candidate, BASE))
    for label, data, instructions in (('target', target, ti), ('candidate', candidate, ci)):
        if not instructions or sum(ins.size for ins in instructions) != len(data) or instructions[-1].mnemonic != 'ret':
            raise ValueError('incomplete Etama draw ' + label + ' instruction extent')
        if instructions[-1].op_str:
            raise ValueError('unreviewed Etama draw callback return cleanup')
    target_fields, target_calls = decoded_fields(ti, capstone)
    candidate_fields, candidate_calls = decoded_fields(ci, capstone)
    if len(ti) != 157 or len(target_fields) != 27 or len(target_calls) != 13:
        raise ValueError('Etama draw original field/instruction/call census changed')
    if candidate_fields != bound_fields:
        raise ValueError('Etama draw candidate COFF fields do not cover all code operands')
    if [(kind, dest) for _, kind, dest in candidate_fields] != [(kind, dest) for _, kind, dest in target_fields]:
        raise ValueError('Etama draw field order, multiplicity or destinations changed')
    if candidate_calls != target_calls:
        raise ValueError('Etama draw call destinations/order changed')
    t_graph = direct_control_flow(ti, capstone.x86.X86_OP_IMM)
    c_graph = direct_control_flow(ci, capstone.x86.X86_OP_IMM)
    # Exact physical offsets from independently original-bound code bytes.
    # Keep the entire owner visible: matching relocation fields cannot be
    # masked, nor can instruction alignment move an unequal byte elsewhere.
    difference_offsets = [offset for offset, (original, compiled) in
                          enumerate(zip(target, candidate)) if original != compiled]
    result = {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(candidate),
        'target_instructions': len(ti), 'candidate_instructions': len(ci),
        'bound_fields': len(bound_fields), 'ordered_calls': len(target_calls),
        'target_direct_blocks': len(t_graph), 'candidate_direct_blocks': len(c_graph),
        'complete_direct_graphs_agree': t_graph == c_graph,
        'overlapping_byte_differences': len(difference_offsets),
        'difference_offsets': difference_offsets,
        'missing_target_bytes': max(0, SIZE - len(candidate)),
        'extra_candidate_bytes': max(0, len(candidate) - SIZE),
        'complete_bytes_equal': candidate == target,
        'target_sha256': hashlib.sha256(target).hexdigest(),
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
        'linked_sha256': hashlib.sha256(candidate).hexdigest(),
    }
    if explain:
        shapes = load_local('etama_draw_shapes', 'report-ecl-handler-shapes.py')
        t_stream = shapes.normalized_stream(target, BASE, {off for off, _, _ in target_fields})
        c_stream = shapes.normalized_stream(candidate, BASE, {off for off, _, _ in candidate_fields})
        result['normalized_changes'] = changed_instructions(c_stream, t_stream)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    parser.add_argument('--explain', action='store_true', help='show aligned instruction changes; never exactness credit')
    parser.add_argument('--max-spans', type=int, default=8)
    args = parser.parse_args()
    if args.max_spans < 1:
        parser.error('--max-spans must be positive')
    try:
        report = inspect(args.object, explain=args.explain)
    except (OSError, ValueError, KeyError, ImportError, struct.error) as error:
        print(json.dumps({'error': str(error)}))
        return 2
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print('Etama OnDraw: %d/%d physical bytes, %d bound code operands, %d ordered calls, %d/%d graph blocks, %d overlapping differences (%d missing); exact=%s (NON-CREDITING)' % (
            report['candidate_bytes'], report['target_bytes'], report['bound_fields'],
            report['ordered_calls'], report['candidate_direct_blocks'], report['target_direct_blocks'],
            report['overlapping_byte_differences'], report['missing_target_bytes'], report['complete_bytes_equal']))
        if args.explain:
            changes = report['normalized_changes']
            print('Normalized instruction spans:', len(changes), '(uncredited; scalar immediates are retained)')
            for row in changes[:args.max_spans]:
                print('  ' + row['change'])
                for off, data in row['target']:
                    print('    target    +0x%04X: %s' % (off, data))
                for off, data in row['candidate']:
                    print('    candidate +0x%04X: %s' % (off, data))
            if len(changes) > args.max_spans:
                print('... %d additional change spans omitted' % (len(changes) - args.max_spans))
    return 0 if report['complete_bytes_equal'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
