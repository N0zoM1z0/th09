#!/usr/bin/env python3
"""Inspect TH09 character-selection code; diagnostic, never exactness credit.

Read-only complete replay through independently reviewed destinations. Default
is mode-0; --mode123 selects its neighbor and --ordinary the 14-entry owner.
Requires Capstone. The canonical acceptance path remains the tracked match unit.
"""
import difflib
import hashlib
import importlib.util
from pathlib import Path
import struct
import capstone

root = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('coff', root / 'scripts/compare-coff-function.py')
coff = importlib.util.module_from_spec(spec)
spec.loader.exec_module(coff)
import argparse
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('object', type=Path)
mode = parser.add_mutually_exclusive_group()
mode.add_argument('--mode123', action='store_true')
mode.add_argument('--ordinary', action='store_true')
args = parser.parse_args()
base = 0x4289DB
symbol = '?UpdateScreen8Mode0@TitleScreenView@@QAEHXZ'
size = 2241
if args.mode123:
    base = 0x425ACB
    symbol = '?UpdateScreen8Mode123@TitleScreenView@@QAEHXZ'
    size = 2153
elif args.ordinary:
    base = 0x4254A7
    symbol = '?OnUpdateCharacterSelect@TitleScreenView@@QAEHXZ'
    size = 1572
destinations = {
    '?g_TitleSide0InputFlags@@3IA': 0x4ACE1E,
    '?ChangeCurrentScreen@TitleScreenView@@QAEHH@Z': 0x422F39,
    '?ExecuteScriptArray@TitleAnmManagerView@@QAEXPAUAnmVmView@@H@Z': 0x439560,
    '?IsPressedScrolling@TitleSideInputView@@QAEGG@Z': 0x423158,
    '?MoveCharacterCursor@TitleScreenView@@QAEPAXHHPADH@Z': 0x42304D,
    '?MoveCharacterCursorHorizontal@TitleScreenView@@QAEHHH@Z': 0x42474A,
    '?NextU32@TitleSelectionRandomView@@QAEHXZ': 0x42AE50,
    '?PlayMenuSound@TitleScreenView@@QAEHHH@Z': 0x422F17,
    '?SetCharacterCursorActive@TitleScreenView@@QAEXHHHH@Z': 0x424EDD,
    '?SetCharacterCursorReverse@TitleScreenView@@QAEXHHHH@Z': 0x424FB0,
    '?SetInterruptArray@TitleAnmManagerView@@QAEXPAUAnmVmView@@HH@Z': 0x436DE0,
    '?SetSprite@TitleAnmView@@QAEHPAUAnmVmView@@H@Z': 0x436AC0,
    '?UpdateCharacterSelectionVisuals@TitleScreenView@@QAEXHHHPAD@Z': 0x425233,
    '?UpdateCharacterSettings@TitleScreenView@@QAEHHH@Z': 0x425163,
    '?UpdateScreen16@TitleScreenView@@QAEHXZ': 0x426334,
    '?g_GameSide0CharacterSetting@@3HA': 0x4A8108,
    '?g_GameSide0Value20@@3HA': 0x4A7DB0,
    '?g_GameSide0Value30@@3HA': 0x4A7DC0,
    '?g_GameSide1CharacterSetting@@3HA': 0x4A810C,
    '?g_GameSide1Value20@@3HA': 0x4A7DE8,
    '?g_GameSide1Value30@@3HA': 0x4A7DF8,
    '?g_SupervisorNetworkState@@3PAUSupervisorNetworkState@@A': 0x4B42D0,
    '?g_TitleAnmManager@@3PAUTitleAnmManagerView@@A': 0x4DC550,
    '?g_TitleCharacterOrder@@3PADA': 0x4A1E2C,
    '?g_TitleCharacterReturnSelection@@3HA': 0x4A7EAC,
    '?g_TitleCharacterUnlocked@@3PAEA': 0x4A81CC,
    '?g_TitleModeSelection@@3HA': 0x4A7EA4,
    '?g_TitleSelectionRandom@@3UTitleSelectionRandomView@@A': 0x4ACE0C,
    '?g_TitleSide0Input@@3UTitleSideInputView@@A': 0x4ACE18,
    '?g_TitleSide1Input@@3UTitleSideInputView@@A': 0x4ACEA6,
    '?g_TitleCharacterOrderMode123@@3PADA': 0x4A1D8C,
    '?g_TitleInputFlags@@3IA': 0x4ACF3A,
    '?g_TitleInput@@3UInputView@@A': 0x4ACF34,
    '?IsPressedScrolling@InputView@@QAEGH@Z': 0x423158,
    '?MoveCharacterCursorHorizontalForInput@TitleScreenView@@QAEHHHH@Z': 0x4247F1,
    '?MoveCharacterCursorNormal@TitleScreenView@@QAEPAXHHPADH@Z': 0x4230A6,
    '?MoveCharacterCursorMode4@TitleScreenView@@QAEPAXHHPADH@Z': 0x4230FF,
    '?GetOptionState@TitleCharacterConfigView@@QAEHDH@Z': 0x4234A7,
    '?SetCharacterSettingIndicator@TitleScreenView@@QAEHHH@Z': 0x42520B,
    '?StopAudio@TitleSupervisorView@@QAEHXZ': 0x42FD30,
    '?g_TitleCharacterOrder14@@3PADA': 0x4A1D7C,
    '?g_TitleCharacterConfig@@3UTitleCharacterConfigView@@A': 0x4A8180,
    '?g_TitleCharacterUnlockedNormal@@3PAEA': 0x4A81DC,
    '?g_TitleCharacterUnlockedMode4@@3PAEA': 0x4A81EC,
    '?g_TitleDifficulty@@3EA': 0x4B3538,
    '?g_GameMode@@3HA': 0x4A7EA8,
    '?g_GameCurrentStage@@3HA': 0x4A7E8C,
    '?g_GameValueDB8@@3HA': 0x4A7DB8,
    '?g_GameValueDF0@@3HA': 0x4A7DF0,
    '?g_TitleGlobalMode@@3HA': 0x4B3690,
    # Historical nonexact view; the repaired ordinary owner uses GlobalMode.
    '?g_TitleLaunchState@@3HA': 0x4B3690,
    '?g_TitleSupervisor@@3UTitleSupervisorView@@A': 0x4B3100,
}
target = coff.pe_bytes_at(coff.verified_target(), base, size)
code, relocations = coff.object_function(args.object, symbol)
raw_hash = hashlib.sha256(code).hexdigest()
for row in relocations:
    dest = destinations[row['symbol']]
    value = dest + row['addend']
    if row['type'] == 'REL32':
        value -= base + row['offset'] + 4
    elif row['type'] != 'DIR32':
        raise ValueError('unreviewed relocation type')
    struct.pack_into('<I', code, row['offset'], value & 0xFFFFFFFF)
decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
decoder.detail = True
t = list(decoder.disasm(target, base))
c = list(decoder.disasm(code, base))
assert sum(i.size for i in t) == len(target)
assert sum(i.size for i in c) == len(code)
def normalized(i):
    if i.mnemonic.startswith('j') or i.mnemonic == 'call':
        return i.mnemonic, 'direct'
    return i.mnemonic, i.op_str
matcher = difflib.SequenceMatcher(None, list(map(normalized,t)), list(map(normalized,c)), autojunk=False)
def calls(ins):
    return [i.operands[0].imm for i in ins if i.mnemonic == 'call']
print('NON-CREDITING diagnostic:',len(code),'bytes',len(c),'instructions',len(relocations),'fields',raw_hash)
print('direct order agrees:', calls(t) == calls(c), 'normalized alignment:',sum(x.size for x in matcher.get_matching_blocks()),'/',len(t))
if len(code) == len(target):
    print('complete replay differences:',sum(a!=b for a,b in zip(code,target)))
for tag,a,b,x,y in matcher.get_opcodes():
    if tag != 'equal':
        print(tag, [(hex(i.address),i.mnemonic,i.op_str)for i in t[a:b]], '=>',[(hex(i.address),i.mnemonic,i.op_str)for i in c[x:y]])
