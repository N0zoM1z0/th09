#!/usr/bin/env python3
"""Complete non-crediting TH09 network-message physical-owner audit.

Includes all 768 code bytes, six switch destinations and the 16-byte map.
Original operands are fixed independently of actual candidate COFF offsets.
--build serially produces just this source using the established diagnostic
profile; no canonical exactness or runtime claim follows from a passing audit.
"""
from __future__ import annotations
import argparse
from collections import Counter
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
BASE, CODE_SIZE, SIZE = 0x00432B70, 768, 808
SYMBOL = '?HandleMessage@SupervisorNetworkMessageView@@QAEHPAXK0@Z'
TARGET_SHA256 = 'c77e9349bb3a63990969608fc2570bfb15d17575f8b0b2b86dc32f1ce16c5cd3'
BYTE_MAP = bytes((0, 5, 1, 5, 2, 5, 5, 5, 5, 5, 5, 5, 3, 5, 5, 4))
PROFILE = ['/MT', '/EHsc', '/Gs', '/DNDEBUG', '/Zi', '/Gy', '/GF',
           '/Oi', '/Gr', '/O2', '/Ob1', '/Oy-', '/I', 'src']
SOURCE = 'src/SupervisorNetworkMessageHandler.cpp'
OBJECT = 'build/diagnostics/SupervisorNetworkMessageHandler.obj'

TARGET_FIELDS = (
    (0x01C, 'DIR32', 0x00432E88),
    (0x023, 'DIR32', 0x00432E70),
    (0x02A, 'DIR32', 0x004B3100),
    (0x02F, 'REL32', 0x0042B130),
    (0x044, 'DIR32', 0x004B42D0),
    (0x084, 'DIR32', 0x004B3100),
    (0x089, 'REL32', 0x0042E9E0),
    (0x08E, 'DIR32', 0x004B356C),
    (0x093, 'DIR32', 0x004B3564),
    (0x0AA, 'DIR32', 0x004B3558),
    (0x0C7, 'DIR32', 0x004B356C),
    (0x0CC, 'DIR32', 0x004B3564),
    (0x106, 'DIR32', 0x004ACE0C),
    (0x10C, 'REL32', 0x00420160),
    (0x141, 'DIR32', 0x004ACE0C),
    (0x146, 'REL32', 0x0041A89E),
    (0x14B, 'DIR32', 0x004B3100),
    (0x156, 'REL32', 0x0042E9E0),
    (0x15B, 'DIR32', 0x004ACE0C),
    (0x160, 'REL32', 0x0041A89E),
    (0x165, 'DIR32', 0x004B3100),
    (0x16F, 'REL32', 0x0042E9E0),
    (0x18F, 'DIR32', 0x004B356C),
    (0x194, 'DIR32', 0x004B3564),
    (0x1A0, 'DIR32', 0x004B3100),
    (0x1A5, 'REL32', 0x0042B130),
    (0x1CC, 'REL32', 0x0047B24E),
    (0x20E, 'REL32', 0x0047B249),
    (0x218, 'DIR32', 0x004B3100),
    (0x21D, 'REL32', 0x0042B160),
    (0x242, 'REL32', 0x0047B249),
    (0x266, 'REL32', 0x0047B249),
    (0x272, 'DIR32', 0x004B3100),
    (0x277, 'REL32', 0x0042B130),
    (0x2A1, 'DIR32', 0x004B3100),
    (0x2A6, 'REL32', 0x0042B130),
    (0x2B9, 'DIR32', 0x004B3100),
    (0x2BE, 'REL32', 0x0042B130),
    (0x2EC, 'DIR32', 0x004B3100),
    (0x2F3, 'REL32', 0x0042B160),
    (0x300, 'DIR32', 0x00432E26),
    (0x304, 'DIR32', 0x00432D0D),
    (0x308, 'DIR32', 0x00432DDF),
    (0x30C, 'DIR32', 0x00432B97),
    (0x310, 'DIR32', 0x00432E0E),
    (0x314, 'DIR32', 0x00432E67),
)

REFERENTS = {
    '?g_Supervisor@@3VSupervisor@@A': 0x004B3100,
    '?EnterCriticalSectionWrapper@Supervisor@@QAEXH@Z': 0x0042B130,
    '?g_SupervisorNetworkState@@3PAUSupervisorNetworkState@@A': 0x004B42D0,
    '?InsertReceivedFrame@SupervisorFrameQueueView@@QAEXHHHF@Z': 0x0042E9E0,
    '?g_NetworkCurrentTime@@3IA': 0x004B356C,
    '?g_NetworkLastReceiveTime@@3IA': 0x004B3564,
    '?g_NetworkHandshakeToken@@3HA': 0x004B3558,
    '?g_ReplayRng@@3UReplayRngView@@A': 0x004ACE0C,
    '?SetSeed@ReplayRngView@@QAEXG@Z': 0x00420160,
    '?GetSeed@ReplayRngView@@QAEGXZ': 0x0041A89E,
    '??2@YAPAXI@Z': 0x0047B24E,
    '??3@YAXPAX@Z': 0x0047B249,
    '?LeaveCriticalSectionWrapper@Supervisor@@QAEXH@Z': 0x0042B160,
}

# Complete original-linked residuals, protecting all other 697 physical bytes.
RESIDUAL_OFFSETS = frozenset((
    0x03E, 0x06C, 0x06D, 0x06E, 0x06F, 0x070, 0x071, 0x072, 0x073,
    0x074, 0x075, 0x076, 0x077, 0x078, 0x079, 0x07A, 0x07B, 0x07C,
    0x07D, 0x07E, 0x07F, 0x080, 0x081, 0x082, 0x083, 0x084, 0x085,
    0x086, 0x087, 0x088, 0x089, 0x08A, 0x08B, 0x08C, 0x08D, 0x08E,
    0x08F, 0x090, 0x091, 0x092, 0x093, 0x094, 0x095, 0x096, 0x097,
    0x098, 0x099, 0x09A, 0x09B, 0x09C, 0x09D, 0x09F, 0x0A0, 0x0A1,
    0x0A2, 0x0A3, 0x0A4, 0x0A5, 0x0A6, 0x0A7, 0x0A8, 0x0A9, 0x0AA,
    0x0AB, 0x0AC, 0x0AD, 0x0AE, 0x0AF, 0x0B0, 0x0B2, 0x0B3, 0x0B4,
    0x0B5, 0x0B6, 0x0B7, 0x0B8, 0x0B9, 0x0BA, 0x0BB, 0x0BC, 0x0BE,
    0x0BF, 0x0C0, 0x0C1, 0x0C2, 0x0C4, 0x0C5, 0x0C6, 0x0C7, 0x0C8,
    0x0C9, 0x0CA, 0x0CB, 0x0EA, 0x0EC, 0x0EE, 0x100, 0x103, 0x10A,
    0x111, 0x114, 0x11A, 0x11D, 0x122, 0x123, 0x124, 0x125, 0x126,
    0x127, 0x128, 0x129,
))

def module(name, filename):
    spec = importlib.util.spec_from_file_location(name, ROOT/'scripts'/filename)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def require_original(data):
    if len(data) != SIZE or hashlib.sha256(data).hexdigest() != TARGET_SHA256:
        raise ValueError('network message original complete SHA/extent differs')


def switch_entries(data, code_size):
    if len(data) != code_size+40 or data[code_size+24:] != BYTE_MAP:
        raise ValueError('network message six-way switch/16-byte map differs')
    entries = struct.unpack_from('<6I', data, code_size)
    if len(set(entries)) != 6:
        raise ValueError('network message missing distinct switch destinations')
    return entries


def bind_candidate(raw, rows):
    if not 700 <= len(raw) <= 880 or len(rows) != 46:
        raise ValueError('network message complete extent/46-field inventory differs')
    linked = bytearray(raw)
    fields, local_tables = [], []
    previous_end = 0
    for row in rows:
        off, kind, symbol, addend = (row['offset'], row['type'], row['symbol'], row['addend'])
        if (type(off) is not int or off < previous_end or off+4 > len(raw)
                or kind not in ('DIR32', 'REL32') or type(addend) is not int):
            raise ValueError('network message overlapping/reordered/unsupported field')
        previous_end = off+4
        owner_section, section = row.get('owner_section'), row.get('symbol_section')
        if (type(owner_section) is not int or owner_section <= 0 or type(section) is not int):
            raise ValueError('network message field lacks actual COFF section ownership')
        local = section == owner_section
        if local:
            if kind != 'DIR32':
                raise ValueError('network message unreviewed local relative call')
            delta = row['symbol_value']-row['owner_value']+addend
            if not 0 <= delta < len(raw):
                raise ValueError('network message local referent leaves its real owner')
            destination = BASE+delta
            if off < 40:
                local_tables.append(delta)
        else:
            if symbol not in REFERENTS or addend != 0:
                raise ValueError('network message unknown external symbol/root/addend')
            expected_kind = 'DIR32' if symbol.startswith('?g_') else 'REL32'
            if kind != expected_kind:
                raise ValueError('network message external operand kind differs')
            destination = REFERENTS[symbol]
        value = destination-(BASE+off+4 if kind == 'REL32' else 0)
        struct.pack_into('<I', linked, off, value & 0xFFFFFFFF)
        fields.append((off, kind, destination))
    if (len(local_tables) != 2 or sorted(local_tables) !=
            [len(raw)-40, len(raw)-16]):
        raise ValueError('network message actual COFF switch/map owners differ')
    return bytes(linked), fields


def decode(data, code_size):
    import capstone
    cs = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    cs.detail = True
    instructions = list(cs.disasm(data[:code_size], BASE))
    if not instructions or sum(i.size for i in instructions) != code_size:
        raise ValueError('network message incomplete instruction decode')
    returns = [i for i in instructions if i.mnemonic == 'ret']
    if (not returns or instructions[-1].mnemonic != 'ret'
            or any(i.op_str != '0xc' for i in returns)):
        raise ValueError('network message ret 12 transport differs')
    fields = []
    for ins in instructions:
        control = ins.group(capstone.CS_GRP_JUMP) or ins.group(capstone.CS_GRP_CALL)
        if ins.mnemonic == 'call' and ins.operands[0].type == capstone.x86.X86_OP_IMM:
            if ins.imm_size != 4:
                raise ValueError('network message non-REL32 call')
            fields.append((ins.address-BASE+ins.imm_offset, 'REL32', ins.operands[0].imm))
        for op in ins.operands:
            if (op.type == capstone.x86.X86_OP_MEM and 0x400000 <= op.mem.disp < 0x4E7000):
                if ins.disp_size != 4:
                    raise ValueError('network message unsupported absolute displacement')
                fields.append((ins.address-BASE+ins.disp_offset, 'DIR32', op.mem.disp))
            elif (not control and op.type == capstone.x86.X86_OP_IMM
                    and 0x400000 <= op.imm < 0x4E7000):
                if ins.imm_size != 4:
                    raise ValueError('network message unsupported absolute immediate')
                fields.append((ins.address-BASE+ins.imm_offset, 'DIR32', op.imm))
    entries = switch_entries(data, code_size)
    fields.extend((code_size+4*k, 'DIR32', va) for k, va in enumerate(entries))
    return instructions, sorted(fields)


def call_identity(ins):
    import capstone
    op = ins.operands[0]
    if op.type == capstone.x86.X86_OP_IMM:
        return ('direct', op.imm)
    if (op.type == capstone.x86.X86_OP_MEM and op.mem.base != 0
            and op.mem.index == 0 and op.mem.disp == 0x54):
        return ('vtable-slot', 0x54)
    raise ValueError('network message unreviewed indirect call')


def graph(instructions, data, code_size):
    import capstone
    entries = switch_entries(data, code_size)
    positions = {ins.address: n for n, ins in enumerate(instructions)}
    if any(va not in positions for va in entries):
        raise ValueError('network message switch destination is not an instruction')
    leaders, destinations, calls = {0}, {}, {}
    for n, ins in enumerate(instructions):
        if ins.mnemonic in ('loop', 'loope', 'loopne', 'retf', 'iret', 'iretd',
                            'int', 'int3', 'sysenter', 'sysexit', 'ud2'):
            raise ValueError('network message unsupported control transfer')
        if ins.mnemonic == 'call':
            calls[n] = call_identity(ins)
        if ins.mnemonic.startswith('j'):
            op = ins.operands[0]
            if op.type == capstone.x86.X86_OP_IMM:
                if op.imm not in positions:
                    raise ValueError('network message direct edge leaves instructions')
                targets = [positions[op.imm]]
            elif (ins.mnemonic == 'jmp' and op.type == capstone.x86.X86_OP_MEM
                    and op.mem.base == 0 and op.mem.index != 0 and op.mem.scale == 4
                    and op.mem.disp == BASE+code_size):
                targets = [positions[entries[k]] for k in BYTE_MAP]
            else:
                raise ValueError('network message unreviewed indirect branch')
            destinations[n] = targets
            leaders.update(targets)
        if ins.mnemonic.startswith('j') or ins.mnemonic == 'ret':
            if n+1 < len(instructions):
                leaders.add(n+1)
    starts = sorted(leaders)
    spans = list(zip(starts, starts[1:]+[len(instructions)]))
    owners = {n: b for b, (a, z) in enumerate(spans) for n in range(a, z)}
    result = []
    for a, z in spans:
        last = instructions[z-1]
        kind, successors, cleanup = last.mnemonic, [], ()
        if kind.startswith('j'):
            successors = [owners[n] for n in destinations[z-1]]
            if kind != 'jmp':
                if z not in owners:
                    raise ValueError('network message missing conditional fallthrough')
                successors.append(owners[z])
        elif kind == 'ret':
            cleanup = tuple(op.imm for op in last.operands)
        else:
            if z not in owners:
                raise ValueError('network message missing terminal transfer')
            kind, successors = 'fallthrough', [owners[z]]
        result.append((kind, tuple(successors), tuple(calls[n] for n in range(a, z) if n in calls), cleanup))
    return result


def diagnose(path):
    coff = module('network_message_coff', 'compare-coff-function.py')
    target = coff.pe_bytes_at(coff.verified_target(), BASE, SIZE)
    require_original(target)
    ti, tf = decode(target, CODE_SIZE)
    if len(ti) != 235 or tuple(tf) != TARGET_FIELDS:
        raise ValueError('network message independently decoded original census differs')
    raw, rows = coff.object_function(path, SYMBOL, include_symbol_locations=True)
    linked, bound = bind_candidate(raw, rows)
    code_size = len(raw)-40
    ci, cf = decode(linked, code_size)
    if cf != bound:
        raise ValueError('network message COFF fields do not equal actual machine operands')
    normalize = lambda items: Counter((kind, 'local' if BASE <= va < BASE+SIZE else va)
                                      for _, kind, va in items)
    if normalize(tf) != normalize(cf):
        raise ValueError('network message original/source field identities or multiplicity differ')
    tc = [call_identity(i) for i in ti if i.mnemonic == 'call']
    cc = [call_identity(i) for i in ci if i.mnemonic == 'call']
    if (len(tc) != 19 or sum(x[0] == 'direct' for x in tc) != 17
            or tc != cc):
        raise ValueError('network message 17 direct/two virtual call operands differ')
    tg, cg = graph(ti, target, CODE_SIZE), graph(ci, linked, code_size)
    if len(tg) != 34:
        raise ValueError('network message original graph census differs')
    offsets = [i for i, (a, b) in enumerate(zip(target, linked)) if a != b]
    return dict(diagnostic_only=True, exactness_credit='none', target_bytes=SIZE,
                target_code_bytes=CODE_SIZE, candidate_bytes=len(raw),
                candidate_code_bytes=code_size, target_sha256=TARGET_SHA256,
                raw_sha256=hashlib.sha256(raw).hexdigest(),
                object_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),
                target_instructions=len(ti), candidate_instructions=len(ci),
                bound_fields=len(cf), target_fields=len(tf),
                ordered_call_operands_agree=tc == cc, direct_calls=17, virtual_calls=2,
                target_blocks=len(tg), candidate_blocks=len(cg), direct_graphs_agree=tg == cg,
                switch_destinations=6, switch_map_bytes=16,
                overlap_differences=len(offsets), difference_offsets=offsets,
                missing_target_bytes=max(0, SIZE-len(raw)),
                extra_candidate_bytes=max(0, len(raw)-SIZE), complete_bytes_equal=target == linked)


def guard(result):
    expected = dict(diagnostic_only=True, exactness_credit='none', target_bytes=808,
                    target_code_bytes=768, candidate_bytes=808, candidate_code_bytes=768,
                    target_sha256=TARGET_SHA256, target_instructions=235,
                    bound_fields=46, target_fields=46,
                    ordered_call_operands_agree=True, direct_calls=17, virtual_calls=2,
                    target_blocks=34, candidate_blocks=34, direct_graphs_agree=True,
                    switch_destinations=6, switch_map_bytes=16,
                    missing_target_bytes=0, extra_candidate_bytes=0)
    if (any(type(result.get(k)) is not type(v) or result[k] != v for k, v in expected.items())
            or type(result.get('candidate_instructions')) is not int
            or not 234 <= result['candidate_instructions'] <= 235):
        raise ValueError('network message whole physical frontier regressed')
    offsets, count = result.get('difference_offsets'), result.get('overlap_differences')
    if (not isinstance(offsets, list) or type(count) is not int or count != len(offsets)
            or offsets != sorted(set(offsets))
            or any(type(x) is not int or x not in RESIDUAL_OFFSETS for x in offsets)
            or result.get('complete_bytes_equal') is not (count == 0)):
        raise ValueError('network message damaged a previously equal physical byte')


def project_inputs(path):
    result = {}
    def visit(p):
        if str(p) in result:
            return
        data = (ROOT/p).read_bytes()
        result[str(p)] = hashlib.sha256(data).hexdigest()
        for name in re.findall(r'^\s*#include\s*"([^"]+)"', data.decode(), re.M):
            q = p.parent/name
            if (ROOT/q).is_file():
                visit(q)
    visit(Path(path))
    for p in ('scripts/compile-probe.sh', 'config/tools.lock.toml'):
        result[p] = hashlib.sha256((ROOT/p).read_bytes()).hexdigest()
    return result


def build():
    # Target is verified before invoking the pinned compiler provider.
    module('network_message_build_coff', 'compare-coff-function.py').verified_target()
    inputs = project_inputs(SOURCE)
    completed = subprocess.run(['nice', '-n', '10', str(ROOT/'scripts/compile-probe.sh'),
                                SOURCE, OBJECT, *PROFILE], cwd=ROOT,
                               capture_output=True, text=True)
    if completed.returncode:
        raise ValueError('network message pinned compile failed: '+completed.stdout+completed.stderr)
    if project_inputs(SOURCE) != inputs:
        raise ValueError('network message source/profile inputs changed during production')
    return inputs


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', nargs='?', type=Path)
    parser.add_argument('--build', action='store_true', help='build only this maintained owner')
    parser.add_argument('--guard', action='store_true')
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    try:
        if args.build and args.object:
            raise ValueError('choose --build or an existing object')
        if not args.build and args.object is None:
            raise ValueError('an object or --build is required')
        inputs = build() if args.build else None
        result = diagnose(ROOT/OBJECT if args.build else args.object)
        if inputs is not None:
            result.update(producer_inputs_sha256=inputs, producer_profile=PROFILE)
        if args.guard:
            guard(result)
        if args.json:
            print(json.dumps(result, indent=2))
        else:
            print('NON-CREDITING network message:', result['candidate_bytes'], '/', SIZE,
                  'physical bytes;', result['overlap_differences'], 'linked differences')
        return 0
    except (OSError, ValueError, KeyError, TypeError, ImportError, struct.error) as error:
        print('network message audit rejected:', error, file=sys.stderr)
        return 2


if __name__ == '__main__':
    raise SystemExit(main())
