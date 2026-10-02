#!/usr/bin/env python3
"""Read-only MusicRoom owner replay; diagnostic, never exactness credit.

All destinations are independently reviewed TH09 callees/operands. Requires
Capstone. Unknown fields fail closed; the canonical match unit is acceptance.
"""
import argparse
import difflib
import hashlib
import importlib.util
from pathlib import Path
import struct
import capstone
from diagnostic_branch_alignment import aligned_branch_conflicts

ROOT = Path(__file__).resolve().parents[1]
BASE = 0x426E05
SIZE = 2258
SYMBOL = '?OnUpdateMusicRoom@TitleScreenView@@QAEHXZ'
DESTINATIONS = {
    '?ChangeCurrentScreen@TitleScreenView@@QAEHH@Z': 0x422F39,
    '?DrawTitleMusicText@@YAXPAUTitleAnmManagerView@@PAUAnmVmView@@IIPBDZZ': 0x43BDC0,
    '?ExecuteScript@TitleAnmManagerView@@QAEHPAUAnmVmView@@@Z': 0x436F30,
    '?ExecuteScriptArray@TitleAnmManagerView@@QAEXPAUAnmVmView@@H@Z': 0x439560,
    '?FadeOutMusic@TitleSupervisorView@@QAEHM@Z': 0x42FE20,
    '?LoadSurface@TitleAnmManagerView@@QAEHHPBD@Z': 0x43CAE0,
    '?MoveCursorVertical@TitleScreenView@@QAEHH@Z': 0x42457A,
    '?OpenFile@FileSystem@@YIPAEPBDPAHH@Z': 0x42C970,
    '?PlayAudio@TitleSupervisorView@@QAEHPADH@Z': 0x431A30,
    '?PlayMenuSound@TitleScreenView@@QAEHHH@Z': 0x422F17,
    '?QueueCommand@SoundPlayerView@@QAEXHHPAD@Z': 0x43EE40,
    '?SetAndExecuteScriptIdx@TitleAnmLoadedView@@QAEXPAUAnmVmView@@H@Z': 0x403E00,
    '?SetInterruptArray@TitleAnmManagerView@@QAEXPAUAnmVmView@@HH@Z': 0x436DE0,
    '?SetRangeSelectionInterrupts@TitleScreenView@@QAEXHHH@Z': 0x42510E,
    '?g_SoundPlayer@@3USoundPlayerView@@A': 0x4DC698,
    '?g_TitleAnmManager@@3PAUTitleAnmManagerView@@A': 0x4DC550,
    '?g_TitleBgmNotUnlockedWarning@@3PAPBDA': 0x4A1DBC,
    '?g_TitleBgmUnlocked@@3PAEA': 0x4A81AC,
    '?g_TitleInputFlags@@3IA': 0x4ACF3A,
    '?g_TitleMusicDescriptionAnm@@3PAUTitleAnmLoadedView@@A': 0x4B36CC,
    '?g_TitleSupervisor@@3UTitleSupervisorView@@A': 0x4B3100,
    '__real@41a00000': 0x48F7BC,
    '__real@42ba0000': 0x48F778,
    '__real@42d00000': 0x48F7C0,
    '_free': 0x47B163,
    '_memcpy': 0x47C750,
    '_memset': 0x47C390,
    '??_C@_05PEDNBBBD@Pause?$AA@': 0x48EF50,
    '??_C@_07OGJCKFMD@UnPause?$AA@': 0x48F510,
    '??_C@_0BC@IDOLHJBF@title?1music00?4png?$AA@': 0x48F7A8,
    '??_C@_0BC@OFOJFOID@sprt?1musiccmt?4txt?$AA@': 0x48F794,
    '??_C@_0BF@CKHJOOFD@?$CF5s?5?$IBH?$IBH?$IBH?$IBH?$IBH?$IBH?$IBH?$IBH?$AA@': 0x48F77C,
}

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
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
    assert sum(i.size for i in t) == len(target)
    assert sum(i.size for i in c) == len(code)
    def normalized(i):
        if i.mnemonic.startswith('j') or i.mnemonic == 'call':
            return i.mnemonic, 'direct'
        return i.mnemonic, i.op_str
    matcher = difflib.SequenceMatcher(None, list(map(normalized,t)), list(map(normalized,c)), autojunk=False)
    def calls(ins):
        return [i.operands[0].imm for i in ins if i.mnemonic == 'call']
    print('NON-CREDITING diagnostic:', len(code), 'bytes', len(c), 'instructions', len(rows), 'fields', raw_hash)
    print('direct order agrees:', calls(t) == calls(c), 'normalized alignment:',
          sum(b.size for b in matcher.get_matching_blocks()), '/', len(t))
    conflicts, unpaired = aligned_branch_conflicts(t, c, matcher.get_matching_blocks(), capstone.x86.X86_OP_IMM)
    print('paired CFG conflicts:', [tuple(map(hex,row)) for row in conflicts], 'unpaired destinations:', unpaired)
    if len(code) == len(target):
        print('complete replay differences:',sum(a!=b for a,b in zip(code,target)))
    for tag,a,b,x,y in matcher.get_opcodes():
        if tag != 'equal':
            print(tag, [(hex(i.address),i.mnemonic,i.op_str) for i in t[a:b]],
                  '=>', [(hex(i.address),i.mnemonic,i.op_str) for i in c[x:y]])

if __name__ == '__main__':
    main()
