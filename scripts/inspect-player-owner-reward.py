#!/usr/bin/env python3
"""Complete TH09 ApplyReward diagnostic; no acceptance or partial byte credit.

Independently reviewed callees/operands and PE literals are bound below.
The physical owner includes one post-return alignment byte and a five-entry
compiler table. Callback identity is the observed [ESI+0x40] operand only.
Original source spelling, native linkage and unique storage owners are unknown.
"""
import argparse
import capstone
import difflib
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
BASE, CODE_SIZE, PHYSICAL_SIZE = 0x41D150, 1519, 1540
SYMBOL = '?ApplyReward@PlayerOwnerStateView@@QAEXPAUPlayerPositionView@@HHHH@Z'
DESTINATIONS = {
    '?IsRewardBlocked@PlayerRewardSharedRuntimeView@@QAEHXZ': 0x40D4F0,
    '?GetTransitionBlockFlag@PlayerRewardSharedRuntimeView@@QAEHXZ': 0x40D4F0,
    '?AddScore@PlayerRewardSideScoreView@@QAEXH@Z': 0x40F860,
    '?Configure@PlayerRewardAttackOwnerView@@QAEXHHHPBD@Z': 0x403E50,
    '?CheckMode@PlayerRewardAttackTargetView@@QAEHH@Z': 0x40F7D0,
    '?FindActiveEnemyBySideCategory@PlayerRewardAttackTargetView@@QAEPAUEnemyView@@H@Z': 0x40F7D0,
    '?CreateScorePopup@AsciiManager@@QAEXHPAUFloat3@@HK@Z': 0x4346A0,
    '?TransformPopupX@PlayerRewardGameManagerView@@QAEMM@Z': 0x401680,
    '?TransformPopupY@PlayerRewardGameManagerView@@QAEMM@Z': 0x4016B0,
    '?GetRandomF32SignedInRange@RngRuntimeView@@QAEMM@Z': 0x406200,
    '?GetRandomF32InRange@RngRuntimeView@@QAEMM@Z': 0x404910,
    '?SelectSide@PlayerRewardSupervisorView@@QAEXH@Z': 0x401440,
    '?SpawnEffectWithVelocity@EffectManager@@QAEPAUEffect@@HPBUEffectFloat3@@0HI@Z': 0x40CCC0,
    '??MZunTimer@@QAEIH@Z': 0x401540,
    '??YZunTimer@@QAEXH@Z': 0x406640,
    '?GetCurrent@PlayerRewardTimerCurrentView@@QAEHXZ': 0x435F00,
    '??4ZunTimer@@QAEXH@Z': 0x401500,
    '?g_PlayerSharedRuntime@@3PAUPlayerRewardSharedRuntimeView@@A': 0x4A7E38,
    '?g_PlayerRewardBaseValue@@3HA': 0x4A7E44,
    '?g_PlayerRewardModeValue@@3HA': 0x4A7EAC,
    '?g_PlayerGameManagerRuntime@@3UPlayerRewardGameManagerView@@A': 0x4A7D90,
    '?g_PlayerSupervisorRuntime@@3UPlayerRewardSupervisorView@@A': 0x4B3100,
    '?g_ReplayRng@@3URngRuntimeView@@A': 0x4ACE0C,
    '?g_PlayerRewardEffectManager@@3PAUEffectManager@@A': 0x4A7E0C,
    '?g_PlayerRewardVelocitySpan@@3MA': 0x4A80E8,
    '?g_AsciiManager@@3VAsciiManager@@A': 0x4CE458,
    '__real@3f000000': 0x490F54,
    '__real@41000000': 0x48E5D8,
    '__real@3c888889': 0x48E5D4,
    '__real@3ada740e': 0x48EF90,
    '__real@3d23d70a': 0x48EF8C,
    '__real@3f666666': 0x48EF88,
    '__real@3d4ccccd': 0x48EF84,
    '__real@3f8ccccd': 0x48EF80,
    '__real@3da3d70a': 0x48EF7C,
    '__real@3fa66666': 0x48EF78,
    '__real@3de147ae': 0x48EF74,
    '__real@3fcccccd': 0x48EF70,
}

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--symbol', default=SYMBOL)
    parser.add_argument('--instructions', action='store_true')
    parser.add_argument('--diff', action='store_true')
    args = parser.parse_args()
    def module(name, path):
        spec = importlib.util.spec_from_file_location(name, path)
        value = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(value)
        return value
    coff = module('reward_coff', ROOT / 'scripts/compare-coff-function.py')
    movement = module('reward_coff_symbols', ROOT / 'scripts/inspect-player-movement.py')
    target = coff.pe_bytes_at(coff.verified_target(), BASE, PHYSICAL_SIZE)
    raw, rows = coff.object_function(args.object, args.symbol)
    symbols = movement.coff_symbols(
        args.object, coff, {args.symbol} | {r['symbol'] for r in rows})
    section, owner = symbols[args.symbol]
    replay = bytearray(raw)
    internal = []
    for row in rows:
        name, offset, addend = row['symbol'], row['offset'], row['addend']
        if name in DESTINATIONS:
            destination = DESTINATIONS[name]
        elif name in symbols and symbols[name][0] == section:
            destination = BASE + symbols[name][1] - owner
            internal.append(offset)
        else:
            raise ValueError('unreviewed external relocation: ' + name)
        if row['type'] == 'REL32':
            addend = struct.unpack('<i', struct.pack('<I', addend))[0]
            value = destination + addend - (BASE + offset + 4)
        elif row['type'] == 'DIR32':
            value = destination + addend
        else:
            raise ValueError('unreviewed relocation type')
        struct.pack_into('<I', replay, offset, value & 0xffffffff)
    table = len(raw) - 20
    if len(internal) != 6 or sorted(o for o in internal if o >= table) != list(range(table, len(raw), 4)):
        raise ValueError('five-entry switch table coverage is incomplete')
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    def decode(blob):
        ins = list(decoder.disasm(blob, BASE))
        if sum(i.size for i in ins) != len(blob):
            raise ValueError('incomplete decoding')
        return ins
    prefix = decode(replay[:table])
    returns = [i for i in prefix if i.mnemonic == 'ret']
    if len(returns) != 1:
        raise ValueError('expected one shared return')
    body = returns[-1].address - BASE + returns[-1].size
    if not 0 <= table - body < 16 or any(i.mnemonic not in ('nop', 'lea', 'mov', 'int3') for i in prefix if i.address >= BASE + body):
        raise ValueError('unreviewed alignment')
    t, c = decode(target[:CODE_SIZE]), decode(replay[:body])
    for blob, ins, start in [(target, t, CODE_SIZE + 1), (replay, c, table)]:
        positions = {i.address for i in ins}
        entries = struct.unpack_from('<5I', blob, start)
        if any(a not in positions for a in entries):
            raise ValueError('table entry outside decoded owner')
        jumps = [i for i in ins if i.mnemonic == 'jmp' and i.operands[0].type == capstone.x86.X86_OP_MEM]
        if len(jumps) != 1:
            raise ValueError('expected one switch dispatch')
        op = jumps[0].operands[0].mem
        if op.segment or op.base or op.index != capstone.x86.X86_REG_EAX or op.scale != 4 or op.disp != BASE + start:
            raise ValueError('unreviewed switch dispatch operand')
    def call_id(i):
        op = i.operands[0]
        if op.type == capstone.x86.X86_OP_IMM:
            return ('direct', op.imm)
        if op.type == capstone.x86.X86_OP_MEM:
            m = op.mem
            if not m.segment and m.base == capstone.x86.X86_REG_ESI and not m.index and m.scale == 1 and m.disp == 0x40:
                return ('owner-callback-field', 'esi', 0x40)
        raise ValueError('unreviewed call operand')
    calls = lambda ins: [call_id(i) for i in ins if i.mnemonic == 'call']
    def normalized(i):
        if i.mnemonic.startswith('j') or i.mnemonic == 'call':
            return i.mnemonic, 'control-transfer'
        return i.mnemonic, i.op_str
    matcher = difflib.SequenceMatcher(None, list(map(normalized, t)), list(map(normalized, c)), autojunk=False)
    print(json.dumps({
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_code_bytes': CODE_SIZE, 'target_physical_bytes': PHYSICAL_SIZE,
        'candidate_code_bytes': body, 'candidate_physical_bytes': len(raw),
        'candidate_alignment_bytes': table - body, 'compiler_table_bytes': 20,
        'target_instructions': len(t), 'candidate_instructions': len(c),
        'relocation_fields': len(rows), 'internal_fields': len(internal),
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
        'ordered_call_operands_agree': calls(t) == calls(c),
        'target_calls': len(calls(t)), 'candidate_calls': len(calls(c)),
        'complete_physical_bytes_agree': replay == target,
        'code_length_delta': body - CODE_SIZE,
        'first_overlapping_difference': next((n for n,(a,b) in enumerate(zip(target,replay)) if a != b), None),
        'complete_differences_if_same_extent': sum(a != b for a,b in zip(target,replay)) if len(target)==len(replay) else None,
        'normalized_aligned_instructions': sum(b.size for b in matcher.get_matching_blocks()),
        'cfg_verdict': 'not evaluated: indirect switch dispatch; no direct-only graph claim',
    }, indent=2))
    if calls(t) != calls(c):
        print('TARGET CALLS', calls(t), 'CANDIDATE CALLS', calls(c))
    if args.instructions:
        for i in c: print(hex(i.address), i.mnemonic, i.op_str)
    if args.diff:
        for tag,a,b,x,y in matcher.get_opcodes():
            if tag != 'equal':
                print(tag, [(hex(i.address),i.mnemonic,i.op_str) for i in t[a:b]],
                      '=>', [(hex(i.address),i.mnemonic,i.op_str) for i in c[x:y]])

if __name__ == '__main__':
    main()
