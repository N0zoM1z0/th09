#!/usr/bin/env python3
"""Complete, noncrediting TH09 ExAttack14/22 Draw PE/COFF codegen audit.

The 959-byte original function is hash-attested independently of the current
candidate. All 53 original address operands and 20 ordered direct/12 indirect
calls are decoded rather than borrowed from guessed candidate identities.
The twelve COM calls require original D3D8 slots, immediate arguments and a
locally proved native device/vtable; the dynamic row pointer remains unknown.
Only reviewed TH09 function/global/literal targets may bind COFF relocation
symbols. The diagnostic never suppresses differing operands or grants credit.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x00444130
SIZE = 959
TARGET_OWNER_SHA256 = 'af3c06ea2c9f68861067f5feb784206035dd67ea3eb07ef3994ed4ec17df74f4'
SYMBOL = '?ExAttackDrawCallbackType14_22@@YIHPAUExAttackRecord@@@Z'

# Address identities come from the verified TH09 target instructions and the
# already-reviewed callee/data owners. This is a name-to-identity allowlist,
# not a mechanism for solving addresses from the candidate's byte stream.
DESTINATIONS = {
    '?AddNormalizeAngle@@YIMMM@Z': 0x0042AED0,
    '??BZunTimer@@QAEHXZ': 0x00435F00,
    '?g_GameManager@@3UAsciiGameManagerView@@A': 0x004A7D90,
    '?TransformPopupX@AsciiGameManagerView@@QAEMM@Z': 0x00401680,
    '?TransformPopupY@AsciiGameManagerView@@QAEMM@Z': 0x004016B0,
    '__real@40490fdb': 0x0048E2C4,
    '__real@40c90fdb': 0x0048E4C4,
    '?FromAngleMagnitude@Float3@@QAEXMM@Z': 0x00441890,
    '??YFloat3@@QAEAAU0@ABU0@@Z': 0x00405730,
    '__real@3e4f8c3d': 0x00491608,
    '?g_AnmManager@@3PAVAnmManager@@A': 0x004DC550,
    '?DrawTriangleFan@AnmManager@@QAEHPAUAnmVm@@PAUVertexDiffuseXyzrhw@@H@Z': 0x0043B7B0,
    '?FlushVertexBuffer@AnmManager@@QAEXXZ': 0x004396A0,
    '?g_Supervisor@@3VSupervisor@@A': 0x004B3100,
    '__real@3d4f8c3d': 0x00491604,
    '?ClearVertexShader@AnmManager@@QAEXXZ': 0x0040E380,
    '?ClearSprite@AnmManager@@QAEXXZ': 0x0040E390,
    '?ClearTexture@AnmManager@@QAEXXZ': 0x004015F0,
    '?ClearColorOp@AnmManager@@QAEXXZ': 0x00415D00,
    '?ClearBlendMode@AnmManager@@QAEXXZ': 0x0040E370,
    '?ClearZWrite@AnmManager@@QAEXXZ': 0x00415D10,
}


# Independently decoded original COM calls, checked against the pinned D3D8
# interface: each tuple lists the vtable byte slot and reverse stack arguments.
# The row pointer is dynamic; this audit does not claim its full runtime value.
DEVICE_POINTER_ADDRESS = 0x004B3108
COM_CALLS = (
    (0xFC, (2, 4, 0)), (0xFC, (2, 1, 0)),
    (0xFC, (0, 5, 0)), (0xFC, (0, 2, 0)),
    (0xC8, (0, 14)), (0xC8, (6, 20)),
    (0x130, (0x44,)), (0x120, (20, 'dynamic', 31, 3)),
    (0xFC, (4, 4, 0)), (0xFC, (4, 1, 0)),
    (0xFC, (2, 5, 0)), (0xFC, (2, 2, 0)),
)


def reference_com_calls() -> list[dict]:
    return [dict(slot=slot, reverse_arguments=list(args),
                 receiver_address=DEVICE_POINTER_ADDRESS)
            for slot, args in COM_CALLS]


def require_com_calls(rows: list[dict]) -> None:
    if not isinstance(rows, list) or len(rows) != len(COM_CALLS):
        raise ValueError('Type14/22 requires all twelve original COM calls')
    for row in rows:
        if (not isinstance(row, dict) or set(row) != {
                'slot', 'reverse_arguments', 'receiver_address'}
                or type(row['slot']) is not int
                or type(row['receiver_address']) is not int
                or not isinstance(row['reverse_arguments'], list)
                or any(type(value) is not int and
                       not (type(value) is str and value == 'dynamic')
                       for value in row['reverse_arguments'])):
            raise ValueError('malformed Type14/22 COM call trace')
    if rows != reference_com_calls():
        raise ValueError('Type14/22 COM slot, arguments or native device changed')


def trace_com_events(events: list[dict]) -> list[dict]:
    """Small fail-closed dataflow check over decoded straight-line call setup.

    Track only the native device load and its vtable. General registers and
    the vertex-row pointer remain unknown. Drop facts at control-flow entries
    and branches; every reviewed COM call reloads its own device locally.
    """
    registers, pushes, calls = {}, [], []
    device = ('device', DEVICE_POINTER_ADDRESS)
    aliases = {alias: full for full, names in (
        ('eax', ('eax', 'ax', 'al', 'ah')),
        ('ebx', ('ebx', 'bx', 'bl', 'bh')),
        ('ecx', ('ecx', 'cx', 'cl', 'ch')),
        ('edx', ('edx', 'dx', 'dl', 'dh')),
        ('esi', ('esi', 'si')), ('edi', ('edi', 'di')),
        ('ebp', ('ebp', 'bp')), ('esp', ('esp', 'sp')))
        for alias in names}
    for event in events:
        if event['entry']:
            registers.clear()
            pushes.clear()
        old = dict(registers)
        for name in event['writes']:
            registers.pop(aliases.get(name, name), None)
        name, operands = event['mnemonic'], event['operands']
        if name == 'mov' and len(operands) == 2:
            dst, src = operands
            if (dst['kind'] == 'reg' and dst['size'] == 4
                    and src['kind'] == 'mem' and src['size'] == 4
                    and src['index'] is None):
                if src['base'] is None and src['disp'] == DEVICE_POINTER_ADDRESS:
                    registers[dst['reg']] = device
                elif src['disp'] == 0 and old.get(src['base']) == device:
                    registers[dst['reg']] = ('vtable', DEVICE_POINTER_ADDRESS)
        elif name == 'push' and len(operands) == 1:
            operand = operands[0]
            if operand['kind'] == 'imm':
                pushes.append(operand['imm'])
            elif operand['kind'] == 'reg':
                pushes.append(old.get(operand['reg'], 'dynamic'))
            elif operand['kind'] == 'mem':
                pushes.append('dynamic')
            else:
                raise ValueError('unreviewed Type14/22 stack operand')
        elif name == 'call':
            if len(operands) != 1:
                raise ValueError('malformed Type14/22 call')
            operand = operands[0]
            if operand['kind'] == 'mem':
                slot = operand['disp']
                arity = {0xFC: 4, 0xC8: 3, 0x130: 2, 0x120: 5}.get(slot)
                if (arity is None or operand['size'] != 4
                        or operand['index'] is not None
                        or old.get(operand['base']) != ('vtable', DEVICE_POINTER_ADDRESS)
                        or len(pushes) != arity or pushes[-1] != device):
                    raise ValueError('unproved Type14/22 COM vtable/receiver/stack')
                calls.append(dict(slot=slot, reverse_arguments=pushes[:-1],
                                  receiver_address=DEVICE_POINTER_ADDRESS))
            elif operand['kind'] != 'imm':
                raise ValueError('unreviewed Type14/22 indirect call')
            pushes.clear()
            for reg in ('eax', 'ecx', 'edx'):
                registers.pop(reg, None)
        elif name.startswith(('j', 'ret')) or name in ('pop', 'leave'):
            registers.clear()
            pushes.clear()
        elif any(aliases.get(reg, reg) == 'esp' for reg in event['writes']):
            pushes.clear()
    require_com_calls(calls)
    return calls


def com_call_events(instructions: list) -> list[dict]:
    """Normalize actual Capstone operands; never parse guessed call names."""
    import capstone

    entries = {ins.operands[0].imm for ins in instructions
               if ins.mnemonic.startswith('j') and len(ins.operands) == 1
               and ins.operands[0].type == capstone.x86.X86_OP_IMM}
    events = []
    for ins in instructions:
        operands = []
        for operand in ins.operands:
            item = dict(size=operand.size)
            if operand.type == capstone.x86.X86_OP_REG:
                item.update(kind='reg', reg=ins.reg_name(operand.reg))
            elif operand.type == capstone.x86.X86_OP_IMM:
                item.update(kind='imm', imm=operand.imm)
            elif operand.type == capstone.x86.X86_OP_MEM:
                item.update(kind='mem', base=ins.reg_name(operand.mem.base) or None,
                            index=ins.reg_name(operand.mem.index) or None,
                            disp=operand.mem.disp)
            else:
                item.update(kind='unsupported')
            operands.append(item)
        events.append(dict(mnemonic=ins.mnemonic, operands=operands,
                           writes=[ins.reg_name(reg) for reg in ins.regs_access()[1]],
                           entry=ins.address in entries))
    return events

def bind_candidate(raw: bytes, rows: list[dict]) -> tuple[bytes, list[tuple]]:
    """Bind only independently identified real TH09 relocations."""
    if len(rows) != 53:
        raise ValueError('Type14/22 draw requires all 53 actual COFF fields')
    linked = bytearray(raw)
    used = set()
    bound = []
    for row in rows:
        symbol, kind = row['symbol'], row['type']
        offset, addend = int(row['offset']), int(row['addend'])
        if symbol not in DESTINATIONS or kind not in ('REL32', 'DIR32'):
            raise ValueError('unreviewed ExAttack draw relocation symbol/kind')
        is_code_symbol = symbol.startswith('?') and not symbol.startswith('?g_') and not symbol.startswith('??')
        is_code_symbol |= symbol.startswith('??Y') or symbol.startswith('??B')
        if kind != ('REL32' if is_code_symbol else 'DIR32'):
            raise ValueError('ExAttack draw call/data relocation identity altered')
        if addend != (8 if symbol == '?g_Supervisor@@3VSupervisor@@A' else 0):
            raise ValueError('unreviewed ExAttack draw field addend')
        covered = set(range(offset, offset + 4))
        if offset < 0 or offset + 4 > len(linked) or used.intersection(covered):
            raise ValueError('ExAttack draw COFF field outside owner extent or overlapping')
        used.update(covered)
        dest = DESTINATIONS[symbol] + addend
        value = dest - (BASE + offset + 4 if kind == 'REL32' else 0)
        struct.pack_into('<I', linked, offset, value & 0xffffffff)
        bound.append((offset, kind, dest))
    return bytes(linked), sorted(bound)


def decode_owner(data: bytes):
    import capstone

    cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    cs.detail = True
    inst = list(cs.disasm(data, BASE))
    if not inst or sum(x.size for x in inst) != len(data) or inst[-1].mnemonic != 'ret':
        raise ValueError('ExAttack draw instruction decoding/return is incomplete')
    if inst[-1].op_str:
        raise ValueError('ExAttack draw fastcall callee cleanup unexpected')
    fields, calls, indirect = [], [], 0
    for x in inst:
        for op in x.operands:
            if op.type == capstone.x86.X86_OP_MEM and 0x400000 <= op.mem.disp < 0x4E8000:
                if x.disp_size != 4:
                    raise ValueError('unsupported ExAttack draw absolute memory operand')
                fields.append((x.address - BASE + x.disp_offset, 'DIR32', op.mem.disp))
            elif x.mnemonic == 'call' and op.type == capstone.x86.X86_OP_IMM:
                if x.imm_size != 4:
                    raise ValueError('unsupported ExAttack draw direct call operand')
                fields.append((x.address - BASE + x.imm_offset, 'REL32', op.imm))
                calls.append(op.imm)
            elif x.mnemonic != 'call' and not x.mnemonic.startswith('j') and op.type == capstone.x86.X86_OP_IMM and 0x400000 <= op.imm < 0x4E8000:
                if x.imm_size != 4:
                    raise ValueError('unsupported ExAttack draw global/literal immediate')
                fields.append((x.address - BASE + x.imm_offset, 'DIR32', op.imm))
        if x.mnemonic == 'call' and x.operands[0].type != capstone.x86.X86_OP_IMM:
            indirect += 1
    return inst, sorted(fields), calls, indirect


def normalized_owner_shape(instructions, fields, *, base: int = BASE):
    """Keep actual offsets, mask only audited operands and internal jump displacements.

    The caller must first independently bind/verify every COFF field and decode
    the complete owner. A normalized shape is a *diagnostic*, not exactness.
    """
    starts = {ins.address for ins in instructions}
    if len(starts) != len(instructions):
        raise ValueError('duplicate Type14/22 instruction boundary')
    field_list = sorted((int(offset), kind, dest) for offset, kind, dest in fields)
    if any(left.address + len(left.bytes) != right.address for left, right in zip(instructions, instructions[1:])):
        raise ValueError('noncontiguous Type14/22 instruction stream')
    field_index = 0
    previous_field_end = -1
    jumps = []
    normalized = []
    for ins in instructions:
        start = ins.address - base
        end = start + len(ins.bytes)
        data = bytearray(ins.bytes)
        while field_index < len(field_list) and field_list[field_index][0] < end:
            offset, kind, _ = field_list[field_index]
            if offset < previous_field_end:
                raise ValueError('Type14/22 overlapping reviewed fields')
            if offset < start or offset + 4 > end:
                raise ValueError('Type14/22 reviewed field crosses an instruction')
            if kind not in ('REL32', 'DIR32'):
                raise ValueError('Type14/22 unknown reviewed field kind')
            data[offset-start:offset-start+4] = bytes(4)
            previous_field_end = offset + 4
            field_index += 1
        if ins.mnemonic.startswith('j'):
            if not ins.op_str.startswith('0x'):
                raise ValueError('Type14/22 indirect or unknown jump destination')
            try:
                destination = int(ins.op_str, 16)
            except ValueError as error:
                raise ValueError('Type14/22 unparseable jump destination') from error
            if destination not in starts:
                raise ValueError('Type14/22 jump does not reach an instruction boundary')
            if ins.imm_size not in (1, 4) or not 1 <= ins.imm_offset <= len(data)-ins.imm_size:
                raise ValueError('Type14/22 unsupported jump displacement')
            actual_displacement = int.from_bytes(ins.bytes[ins.imm_offset:ins.imm_offset+ins.imm_size], 'little', signed=True)
            if ins.address + len(ins.bytes) + actual_displacement != destination:
                raise ValueError('Type14/22 decoded jump and encoded displacement disagree')
            data[ins.imm_offset:ins.imm_offset+ins.imm_size] = bytes(ins.imm_size)
            jumps.append((start, destination - base, ins.mnemonic))
        normalized.append((start, bytes(data)))
    if field_index != len(field_list):
        raise ValueError('Type14/22 reviewed field outside decoded instruction stream')
    return normalized, jumps


def aligned_shape_changes(candidate_stream, target_stream):
    """Pair only exact normalized opcode encodings, retaining unmatched spans."""
    spec = importlib.util.spec_from_file_location(
        'exattack1422_aligned_spans', ROOT / 'scripts/ecl_shape_differences.py')
    if spec is None or spec.loader is None:
        raise ValueError('aligned instruction diff is unavailable')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module.changed_instructions(candidate_stream, target_stream)


def owner_graph(instructions):
    """Keep block edges and per-block calls independent of physical scheduling."""
    import capstone
    if not instructions:
        raise ValueError('empty ExAttack draw control-flow owner')
    base = instructions[0].address
    addresses = {i.address for i in instructions}
    starts = {base}
    for ins in instructions:
        if ins.mnemonic.startswith(('j', 'ret')):
            if ins.address + ins.size in addresses:
                starts.add(ins.address + ins.size)
            if ins.mnemonic.startswith('j'):
                if ins.operands[0].type != capstone.x86.X86_OP_IMM:
                    raise ValueError('unreviewed indirect ExAttack draw branch')
                starts.add(ins.operands[0].imm)
    ordered = sorted(starts)
    if not starts <= addresses:
        raise ValueError('ExAttack draw branch leaves decoded owner')
    indices = {address: n for n, address in enumerate(ordered)}
    graph = []
    for n, start in enumerate(ordered):
        end = ordered[n+1] if n+1 < len(ordered) else instructions[-1].address + instructions[-1].size
        body = [i for i in instructions if start <= i.address < end]
        tail = body[-1]
        edges = []
        if tail.mnemonic.startswith('j'):
            edges.append(indices[tail.operands[0].imm])
        if tail.mnemonic != 'jmp' and not tail.mnemonic.startswith('ret') and end in indices:
            edges.append(indices[end])
        calls = []
        for ins in body:
            if ins.mnemonic != 'call':
                continue
            op = ins.operands[0]
            if op.type == capstone.x86.X86_OP_IMM:
                calls.append(('direct', op.imm))
            elif op.type == capstone.x86.X86_OP_MEM:
                calls.append(('indirect-slot', op.mem.disp))
            else:
                raise ValueError('unreviewed ExAttack draw call operand')
        kind = tail.mnemonic if tail.mnemonic.startswith(('j', 'ret')) else 'fallthrough'
        graph.append((kind, edges, calls))
    return [x-base for x in ordered], graph


def normalize_next_edges(graph):
    """An unconditional edge to the next block has the same successor as falling through."""
    return [('next' if kind in ('jmp', 'fallthrough') and edges == [n+1] else kind,
             edges, calls) for n, (kind, edges, calls) in enumerate(graph)]


def reachable_owner_graph(instructions):
    import capstone
    by_address = {i.address: i for i in instructions}
    pending = [instructions[0].address]
    seen = set()
    while pending:
        address = pending.pop()
        if address in seen:
            continue
        if address not in by_address:
            raise ValueError('ExAttack draw reachable edge leaves owner')
        seen.add(address)
        ins = by_address[address]
        if ins.mnemonic.startswith('ret'):
            continue
        if ins.mnemonic.startswith('j'):
            if ins.operands[0].type != capstone.x86.X86_OP_IMM:
                raise ValueError('unreviewed ExAttack draw reachable branch')
            pending.append(ins.operands[0].imm)
        if ins.mnemonic != 'jmp':
            pending.append(ins.address + ins.size)
    unreachable = [i for i in instructions if i.address not in seen]
    # These observed compiler alignment bytes stay in the complete comparison.
    # Only their graph nodes are unreachable; arbitrary dead code is rejected.
    for ins in unreachable:
        if bytes(ins.bytes) not in (b'\x90', b'\x8d\xa4\x24\x00\x00\x00\x00'):
            raise ValueError('unreviewed unreachable ExAttack draw instruction')
    reachable = [i for i in instructions if i.address in seen]
    starts, graph = owner_graph(reachable)
    layout_jumps = 0
    for ins in reachable:
        if ins.mnemonic != 'jmp':
            continue
        end = ins.address + ins.size
        destination = ins.operands[0].imm
        gap = [i for i in unreachable if end <= i.address < destination]
        if gap and sum(i.size for i in gap) == destination-end:
            layout_jumps += 1
    return (starts, normalize_next_edges(graph), len(reachable),
            sum(i.size for i in unreachable), layout_jumps)


def inspect(path: Path, *, explain: bool = False) -> dict:
    spec = importlib.util.spec_from_file_location('exattack14_coff', ROOT/'scripts/compare-coff-function.py')
    if spec is None or spec.loader is None:
        raise ValueError('target-bound COFF parser unavailable')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    image = coff.verified_target()
    target = coff.pe_bytes_at(image, BASE, SIZE)
    if hashlib.sha256(target).hexdigest() != TARGET_OWNER_SHA256:
        raise ValueError('verified original Type14/22 draw owner identity drifted')
    t_ins, t_fields, t_calls, t_ind = decode_owner(target)
    if len(t_ins) != 290 or len(t_fields) != 53 or len(t_calls) != 20 or t_ind != 12:
        raise ValueError('original TH09 Type14/22 physical/call/field inventory drifted')
    for symbol, dest in DESTINATIONS.items():
        if symbol.startswith('__real@'):
            expected = struct.pack('<I', int(symbol[7:], 16))
            if coff.pe_bytes_at(coff.verified_target(), dest, 4) != expected:
                raise ValueError('ExAttack draw literal does not match actual target float')
    raw, rows = coff.object_function(path, SYMBOL, include_symbol_locations=True)
    literal_spec = importlib.util.spec_from_file_location(
        'exattack1422_literal_definitions', ROOT/'scripts/coff_literal_definitions.py')
    literals = importlib.util.module_from_spec(literal_spec)
    literal_spec.loader.exec_module(literals)
    definitions = literals.check_literal_definitions(path.read_bytes(), rows, image, coff, DESTINATIONS)
    polar_symbol = '?FromAngleMagnitude@Float3@@QAEXMM@Z'
    polar, polar_fields = coff.object_function(path, polar_symbol, include_symbol_locations=True)
    if polar != coff.pe_bytes_at(image, 0x00441890, 32) or polar_fields:
        raise ValueError('ExAttack draw actual native polar definition differs')
    polar_calls = [row for row in rows if row['symbol'] == polar_symbol]
    if len(polar_calls) != 2 or any(row['symbol_section'] <= 0 for row in polar_calls):
        raise ValueError('ExAttack draw requires both actual same-object polar calls')
    candidate, candidate_fields = bind_candidate(raw, rows)
    c_ins, c_fields, c_calls, c_ind = decode_owner(candidate)
    if c_fields != candidate_fields:
        raise ValueError('ExAttack draw COFF fields do not cover decoded code operands')
    # Preserve order, multiplicity and distinct call ownership, not just set equality.
    if [(kind, dest) for _, kind, dest in t_fields] != [(kind, dest) for _, kind, dest in c_fields]:
        raise ValueError('ExAttack draw candidate call/data field order or destinations differ')
    if c_calls != t_calls or c_ind != t_ind:
        raise ValueError('ExAttack draw candidate direct/indirect call sequence changed')
    target_stream, target_jumps = normalized_owner_shape(t_ins, t_fields)
    candidate_stream, candidate_jumps = normalized_owner_shape(c_ins, c_fields)
    target_com = trace_com_events(com_call_events(t_ins))
    candidate_com = trace_com_events(com_call_events(c_ins))
    target_starts, target_graph = owner_graph(t_ins)
    candidate_starts, candidate_graph = owner_graph(c_ins)
    _, target_reachable, t_reachable_count, t_padding, t_layout = reachable_owner_graph(t_ins)
    _, candidate_reachable, c_reachable_count, c_padding, c_layout = reachable_owner_graph(c_ins)
    differences = [i for i, (x, y) in enumerate(zip(target, candidate)) if x != y]
    result = {
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(candidate),
        'target_instructions': len(t_ins), 'candidate_instructions': len(c_ins),
        'target_fields': len(t_fields), 'candidate_fields': len(c_fields),
        'direct_calls': len(c_calls), 'indirect_calls': c_ind,
        'target_direct_jumps': len(target_jumps),
        'candidate_direct_jumps': len(candidate_jumps),
        'field_order_and_destinations_equal': True,
        'ordered_direct_calls_equal': True,
        'field_positions_equal': t_fields == c_fields,
        'literal_definitions_checked': len(definitions),
        'literal_definitions_original_equal': True,
        'literal_definitions': definitions,
        'native_polar_bytes': len(polar),
        'native_polar_original_equal': True,
        'native_polar_same_object_calls': len(polar_calls),
        'native_polar_raw_sha256': hashlib.sha256(polar).hexdigest(),
        'target_blocks': len(target_graph), 'candidate_blocks': len(candidate_graph),
        'ordinal_graphs_agree': target_graph == candidate_graph,
        'address_graphs_agree': (target_starts, target_graph) == (candidate_starts, candidate_graph),
        'reachable_successors_and_calls_equal': target_reachable == candidate_reachable,
        'target_reachable_blocks': len(target_reachable),
        'candidate_reachable_blocks': len(candidate_reachable),
        'target_reachable_instructions': t_reachable_count,
        'candidate_reachable_instructions': c_reachable_count,
        'target_unreachable_padding_bytes': t_padding,
        'candidate_unreachable_padding_bytes': c_padding,
        'target_layout_jumps': t_layout, 'candidate_layout_jumps': c_layout,
        'target_COM_calls': target_com, 'candidate_COM_calls': candidate_com,
        'overlapping_differences': len(differences),
        'absent_target_bytes': max(0, SIZE-len(candidate)),
        'extra_candidate_bytes': max(0, len(candidate)-SIZE),
        'first_differences': [hex(x) for x in differences[:24]],
        # Never hide the other original-bound differing code bytes from the
        # boss gate. Exact prior byte positions need preservation even when
        # the candidate is longer than the original owner.
        'difference_offsets': differences,
        'complete_bytes_equal': candidate == target,
        'target_sha256': hashlib.sha256(target).hexdigest(),
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
        'resolved_sha256': hashlib.sha256(candidate).hexdigest(),
    }
    if explain:
        changes = aligned_shape_changes(candidate_stream, target_stream)
        result['normalized_aligned_changes'] = changes
        result['normalized_aligned_change_spans'] = len(changes)
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--json', action='store_true')
    parser.add_argument('--explain', action='store_true',
                        help='show independently bound, aligned instruction differences')
    parser.add_argument('--max-spans', type=int, default=12,
                        help='maximum aligned difference spans in text output (default: 12)')
    args = parser.parse_args()
    if args.max_spans < 1:
        parser.error('--max-spans must be positive')
    try:
        result = inspect(args.object, explain=args.explain)
    except (OSError, ValueError, ImportError, KeyError, struct.error) as error:
        print(json.dumps({'error':str(error)}))
        return 2
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        print('Type14/22 draw: %d/%d physical bytes, %d independently bound fields, %d direct + %d indirect calls, %d differences + %d extra bytes, exact=%s (NON-CREDITING)' % (
            result['candidate_bytes'], result['target_bytes'],result['candidate_fields'],
            result['direct_calls'],result['indirect_calls'],result['overlapping_differences'],
            result['extra_candidate_bytes'],result['complete_bytes_equal']))
        if args.explain:
            changes = result['normalized_aligned_changes']
            print('Direct jumps target/candidate: %d/%d; aligned spans: %d (diagnostic only)' % (
                result['target_direct_jumps'], result['candidate_direct_jumps'], len(changes)))
            print('Only independently bound four-byte fields and verified internal jump displacements were zeroed.')
            for row in changes[:args.max_spans]:
                print(' ', row['change'])
                for offset, data in row['target']:
                    print('    target    +0x%04X: %s' % (offset, data))
                for offset, data in row['candidate']:
                    print('    candidate +0x%04X: %s' % (offset, data))
            if len(changes) > args.max_spans:
                print('... %d further mismatched spans not displayed' % (len(changes)-args.max_spans))
    return 0 if result['complete_bytes_equal'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
