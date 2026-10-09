#!/usr/bin/env python3
"""Read-only complete DrawResult replay; diagnostic only, never match credit."""
import argparse
import capstone
import difflib
import hashlib
import importlib.util
from pathlib import Path
import struct
from diagnostic_branch_alignment import aligned_branch_conflicts, direct_control_flow

ROOT = Path(__file__).resolve().parents[1]
BASE, SIZE = 0x423D16, 1939
SYMBOL = '?DrawResult@TitleScreenView@@QAEHXZ'
# TH09-local target operands and literal contents reviewed independently.
DESTINATIONS = {
    '?AddFormatText@AsciiManager@@QAAXPAUFloat3@@PBDZZ': 0x434330,
    '?AddString@AsciiManager@@QAEXPAUFloat3@@PBD@Z': 0x4342A0,
    '?g_AsciiManager@@3VAsciiManager@@A': 0x4CE458,
    '?g_GameManager@@3UGameManagerModeView@@A': 0x4A7D90,
    '?g_TitleAlphabet@@3PADA': 0x4A1CF4,
    # Retained only to inspect source-bound historical objects. Current source
    # addresses the same +0x11C field through the reviewed GameManager root.
    '?g_TitleNameTableIndex@@3HA': 0x4A7EAC,
    '?g_TitleRankLabels@@3PAPBDA': 0x4A1D68,
    '?g_TitleScoreTable@@3PAY144UResultDrawScoreRecord@@A': 0x4A8380,
    '??_C@_01IDAFKMJL@_?$AA@': 0x48F5A8,
    '??_C@_04FOHLOOIG@?$CF?48s?$AA@': 0x48F5A0,
    '??_C@_0BA@EHBBDLHE@Lunatic?5Ranking?$AA@': 0x48F5E8,
    '??_C@_0N@JAEECOBI@Hard?5Ranking?$AA@': 0x48F5F8,
    '??_C@_0N@MELGJLPO@Easy?5Ranking?$AA@': 0x48F628,
    '??_C@_0O@POHKDEEG@Extra?5Ranking?$AA@': 0x48F5D8,
    '??_C@_0P@COFLBEKD@?$CFs?5?$CF?48s?5?$CF?49d?$CFd?$AA@': 0x48F618,
    '??_C@_0P@KJMNBHBM@Normal?5Ranking?$AA@': 0x48F608,
    '__real@3ccccccd': 0x48F598,
    '__real@3f800000': 0x48E2A4,
    '__real@3f99999a': 0x48E830,
    '__real@40000000': 0x48EF5C,
    '__real@41400000': 0x48F59C,
    '__real@41800000': 0x48E470,
    '__real@c1000000': 0x48F594,
}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    parser.add_argument('--instructions', action='store_true')
    parser.add_argument('--summary', action='store_true')
    args = parser.parse_args()
    spec = importlib.util.spec_from_file_location('coff', ROOT / 'scripts/compare-coff-function.py')
    coff = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(coff)
    target = coff.pe_bytes_at(coff.verified_target(), BASE, SIZE)
    code, rows = coff.object_function(args.object, SYMBOL)
    raw_hash = hashlib.sha256(code).hexdigest()
    for row in rows:
        value = DESTINATIONS[row['symbol']] + row['addend']
        if row['type'] == 'REL32':
            value -= BASE + row['offset'] + 4
        elif row['type'] != 'DIR32':
            raise ValueError('unreviewed relocation type')
        struct.pack_into('<I', code, row['offset'], value & 0xFFFFFFFF)
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    t, c = list(decoder.disasm(target, BASE)), list(decoder.disasm(code, BASE))
    if sum(i.size for i in t) != len(target) or sum(i.size for i in c) != len(code):
        raise ValueError('incomplete function decode')
    def normalized(i):
        if i.mnemonic.startswith('j') or i.mnemonic == 'call':
            return i.mnemonic, 'direct'
        return i.mnemonic, i.op_str
    matcher = difflib.SequenceMatcher(None, list(map(normalized, t)), list(map(normalized, c)), autojunk=False)
    calls = lambda ins: [i.operands[0].imm for i in ins if i.mnemonic == 'call']
    print('NON-CREDITING:', len(code), 'bytes', len(c), 'instructions', len(rows), 'fields', raw_hash)
    first = next((n for n, (a, b) in enumerate(zip(target, code)) if a != b), None)
    print('complete owned bytes agree:', code == target,
          'length delta:', len(code) - len(target), 'first overlapping difference:', first)
    print('ordered calls agree:', calls(t) == calls(c), 'alignment:',
          sum(b.size for b in matcher.get_matching_blocks()), '/', len(t))
    conflicts, unpaired = aligned_branch_conflicts(t, c, matcher.get_matching_blocks(), capstone.x86.X86_OP_IMM)
    print('paired CFG conflicts:', [tuple(map(hex, row)) for row in conflicts], 'unpaired:', unpaired)
    target_cfg = direct_control_flow(t, capstone.x86.X86_OP_IMM)
    candidate_cfg = direct_control_flow(c, capstone.x86.X86_OP_IMM)
    print('complete direct block graphs:', len(target_cfg), len(candidate_cfg),
          'edges/per-block calls/return cleanup agree:', target_cfg == candidate_cfg)
    if len(code) == len(target):
        print('complete replay differences:', sum(a != b for a, b in zip(code, target)))
    if args.summary:
        return
    if args.instructions:
        for n, i in enumerate(c):
            print(n, hex(i.address), i.mnemonic, i.op_str)
    else:
        for tag, a, b, x, y in matcher.get_opcodes():
            if tag != 'equal':
                print(tag, [(hex(i.address), i.mnemonic, i.op_str) for i in t[a:b]],
                      '=>', [(hex(i.address), i.mnemonic, i.op_str) for i in c[x:y]])


if __name__ == '__main__':
    main()
