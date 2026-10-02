#!/usr/bin/env python3
"""Complete target-bound type01 diagnostic; never acceptance or byte credit."""
import argparse
import capstone
import difflib
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
from diagnostic_branch_alignment import direct_control_flow

ROOT = Path(__file__).resolve().parents[1]
BASE, SIZE = 0x441100, 475
SYMBOL = '?ExAttackUpdateCallbackType01@@YIHPAUExAttackRecord@@@Z'
# Target-local immediate calls, global operands and verified float identities.
DESTINATIONS = {
    '??0Float3@@QAE@MMM@Z': 0x4010B0,
    '??OZunTimer@@QAEIH@Z': 0x403DE0,
    '??4ZunTimer@@QAEXH@Z': 0x401500,
    '??BZunTimer@@QAEMXZ': 0x4014F0,
    '??YFloat3@@QAEAAU0@ABU0@@Z': 0x405730,
    '?CheckBulletCollision@ExAttackUpdatePlayerView@@QAEHPAUPlayerPositionView@@0PAX@Z': 0x41DFF0,
    '?CheckBulletCollision@ExAttackUpdatePlayerView@@QAEHPAUPlayerPositionView@@0PAUBullet@@@Z': 0x41DFF0,
    '?SetInterrupt@ExAttackUpdateAnmVmView@@QAEXF@Z': 0x406790,
    '?AppendCircleRecord@ExAttackCollisionCircleView@@QAEXPBUPlayerPositionView@@MH@Z': 0x440D30,
    '?ExAttackInterpolate2D@@YIPAMPAM0000MM@Z': 0x42AF80,
    '?ExAttackInterpolate2D@@YIXPAM0000MM@Z': 0x42AF80,
    '?ExecuteAnmIdx@AnmLoaded@@QAEXPAUAnmVm@@H@Z': 0x401560,
    '?g_GameManager@@3UExAttackUpdateGameManagerView@@A': 0x4A7D90,
    '__real@00000000': 0x48E314,
    '__real@c3100000': 0x48E808,
    '__real@43100000': 0x48E804,
    '__real@3d23d70a': 0x48EF8C,
    '__real@40400000': 0x48E2A0,
    '__real@43f00000': 0x4915A8,
}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--diff', action='store_true')
    parser.add_argument('--instructions', action='store_true')
    args = parser.parse_args()
    spec = importlib.util.spec_from_file_location('coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    pe = coff.verified_target()
    target = coff.pe_bytes_at(pe, BASE, SIZE)
    raw, rows = coff.object_function(args.object, SYMBOL)
    code = bytearray(raw)
    for row in rows:
        value = DESTINATIONS[row['symbol']] + row['addend']
        if row['type'] == 'REL32':
            if row['addend']:
                raise ValueError('unreviewed nonzero call addend')
            value -= BASE + row['offset'] + 4
        elif row['type'] != 'DIR32':
            raise ValueError('unreviewed relocation type')
        struct.pack_into('<I', code, row['offset'], value & 0xffffffff)
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True

    def decode(blob):
        ins = list(decoder.disasm(blob, BASE))
        if sum(i.size for i in ins) != len(blob) or ins[-1].mnemonic != 'ret':
            raise ValueError('incomplete code owner')
        return ins

    t, c = decode(target), decode(code)
    def calls(ins):
        result = []
        for i in ins:
            if i.mnemonic == 'call':
                if i.operands[0].type != capstone.x86.X86_OP_IMM:
                    raise ValueError('unreviewed indirect call')
                result.append(i.operands[0].imm)
        return result
    def normalized(i):
        if i.mnemonic.startswith('j') or i.mnemonic == 'call':
            return i.mnemonic, 'control-transfer'
        return i.mnemonic, i.op_str
    matcher = difflib.SequenceMatcher(None, list(map(normalized, t)),
                                     list(map(normalized, c)), autojunk=False)
    target_cfg = direct_control_flow(t, capstone.x86.X86_OP_IMM)
    candidate_cfg = direct_control_flow(c, capstone.x86.X86_OP_IMM)
    print(json.dumps({
        'diagnostic_only': True, 'exactness_credit': 'none',
        'target_bytes': SIZE, 'candidate_bytes': len(raw),
        'target_instructions': len(t), 'candidate_instructions': len(c),
        'relocations': len(rows),
        'raw_sha256': hashlib.sha256(raw).hexdigest(),
        'complete_bytes_agree': code == target,
        'complete_differences_if_same_extent': sum(a != b for a,b in zip(code,target))
                                               if len(code) == SIZE else None,
        'ordered_calls_agree': calls(t) == calls(c),
        'target_calls': len(calls(t)), 'candidate_calls': len(calls(c)),
        'direct_blocks': [len(target_cfg), len(candidate_cfg)],
        'complete_direct_graphs_agree': target_cfg == candidate_cfg,
        'normalized_aligned_instructions': sum(b.size for b in matcher.get_matching_blocks()),
    }, indent=2))
    if args.diff:
        for tag,a,b,x,y in matcher.get_opcodes():
            if tag != 'equal':
                print(tag, [(hex(i.address),i.mnemonic,i.op_str) for i in t[a:b]],
                      '=>', [(hex(i.address),i.mnemonic,i.op_str) for i in c[x:y]])
    if args.instructions:
        for i in c:
            print(hex(i.address), i.mnemonic, i.op_str)


if __name__ == '__main__':
    main()
