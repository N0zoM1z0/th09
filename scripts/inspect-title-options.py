#!/usr/bin/env python3
"""Read-only complete Options replay; diagnostic only, never match credit."""
import argparse
import capstone
import difflib
import hashlib
import importlib.util
from pathlib import Path
import struct
from diagnostic_branch_alignment import aligned_branch_conflicts, direct_control_flow

ROOT = Path(__file__).resolve().parents[1]
BASE, SIZE = 0x4276EB, 2045
SYMBOL = '?OnUpdateOptions@TitleScreenView@@QAEHXZ'
# Independently reviewed TH09 callees and literal/global operands, not fitted.
DESTINATIONS = {
    # Shared Supervisor owner and methods used by the maintained consumer.
    # The older view/field aliases below remain for historical objects.
    '?g_Supervisor@@3VSupervisor@@A': 0x4B3100,
    '?LoadMusic@Supervisor@@QAEHH@Z': 0x42FC20,
    '?PlayMusic@Supervisor@@QAEHHH@Z': 0x431930,
    '?StopAudio@Supervisor@@QAEHXZ': 0x42FD30,
    '?ChangeCurrentScreen@TitleScreenView@@QAEHH@Z': 0x422F39,
    '?DrawTitleHelpText@@YAXPAUTitleAnmManagerView@@PAUAnmVmView@@IHPBD@Z': 0x43BE50,
    '?ExecuteScriptArray@TitleAnmManagerView@@QAEXPAUAnmVmView@@H@Z': 0x439560,
    '?IsPressedScrolling@InputView@@QAEGH@Z': 0x423158,
    '?LoadMusic@TitleSupervisorView@@QAEHH@Z': 0x42FC20,
    '?MoveCursorVertical@TitleScreenView@@QAEHH@Z': 0x42457A,
    '?OnUpdateStartMenu@TitleScreenView@@QAEHXZ': 0x429E23,
    '?PlayMenuSound@TitleScreenView@@QAEHHH@Z': 0x422F17,
    '?PlayMusic@TitleSupervisorView@@QAEHHH@Z': 0x431930,
    '?PlaySoundByIdx@SoundPlayerView@@QAEXHH@Z': 0x43E2F0,
    '?QueueCommand@SoundPlayerView@@QAEXHHPAD@Z': 0x43EE40,
    '?SetInterruptArray@TitleAnmManagerView@@QAEXPAUAnmVmView@@HH@Z': 0x436DE0,
    '?SetMenuSelectionSprites@TitleScreenView@@QAEXHHH@Z': 0x424DFF,
    '?SetSprite@TitleAnmView@@QAEXPAUAnmVmView@@H@Z': 0x436AC0,
    '?StopAudio@TitleSupervisorView@@QAEHXZ': 0x42FD30,
    '?UpdateMenuSelection@TitleScreenView@@QAEXXZ': 0x4276D7,
    '?g_OptionA@@3EA': 0x4B3534,
    '?g_OptionB@@3EA': 0x4B3535,
    '?g_OptionC@@3EA': 0x4B3536,
    '?g_OptionD@@3EA': 0x4B3539,
    '?g_OptionUnknown3537@@3EA': 0x4B3537,
    '?g_SoundPlayer@@3USoundPlayerView@@A': 0x4DC698,
    '?g_TitleAnmManager@@3PAUTitleAnmManagerView@@A': 0x4DC550,
    '?g_TitleInput@@3UInputView@@A': 0x4ACF34,
    '?g_TitleInputFlags@@3IA': 0x4ACF3A,
    '?g_TitleMusicVolume@@3CA': 0x4B3542,
    '?g_TitleOptionsHelpText@@3PAPBDA': 0x4A1DDC,
    '?g_TitleSfxVolume@@3CA': 0x4B3543,
    '?g_TitleSupervisor@@3UTitleSupervisorView@@A': 0x4B3100,
    '??_C@_06EAGAPDIF@SetVol?$AA@': 0x48F518,
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
    for tag, a, b, x, y in matcher.get_opcodes():
        if tag != 'equal':
            print(tag, [(hex(i.address), i.mnemonic, i.op_str) for i in t[a:b]],
                  '=>', [(hex(i.address), i.mnemonic, i.op_str) for i in c[x:y]])


if __name__ == '__main__':
    main()
