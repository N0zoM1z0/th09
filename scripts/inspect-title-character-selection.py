#!/usr/bin/env python3
"""Inspect TH09 character-selection code; diagnostic, never exactness credit.

Read-only complete replay through independently reviewed destinations. Default
is mode-0; --mode123 selects its neighbor, --ordinary the 14-entry owner,
--screen16 the final-selection owner, --replay-menu its replay neighbor,
and --result the embedded result browser.
Requires Capstone. The canonical acceptance path remains the tracked match unit.
"""
import difflib
import hashlib
import importlib.util
from pathlib import Path
import struct
import capstone
from diagnostic_branch_alignment import aligned_branch_conflicts

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
mode.add_argument('--screen16', action='store_true')
mode.add_argument('--replay-menu', action='store_true')
mode.add_argument('--result', action='store_true')
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
elif args.screen16:
    base = 0x426334
    symbol = '?UpdateScreen16@TitleScreenView@@QAEHXZ'
    size = 897
elif args.replay_menu:
    base = 0x42689A
    symbol = '?OnUpdateReplayMenu@TitleScreenView@@QAEHXZ'
    size = 1387
elif args.result:
    base = 0x4266B5
    symbol = '?OnUpdateResult@TitleScreenView@@QAEHXZ'
    size = 485
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
    '?MoveCursorFourWay@TitleScreenView@@QAEHH@Z': 0x424671,
    '?UpdateScreen16SelectionVisuals@TitleScreenView@@QAEXHHPAD@Z': 0x425390,
    '?UpdateScreen8Mode0@TitleScreenView@@QAEHXZ': 0x4289DB,
    '?UpdateScreen8Mode123@TitleScreenView@@QAEHXZ': 0x425ACB,
    '?g_TitleScreen16Order@@3PADA': 0x4A1D9C,
    '?g_TitleScreen16Entries@@3PADA': 0x4AC8C4,
    '?g_TitleScreen16SelectionResult@@3HA': 0x4A7EC8,
    '?MoveCursorVertical@TitleScreenView@@QAEHH@Z': 0x42457A,
    '?PlaySoundByIdx@SoundPlayerView@@QAEXHH@Z': 0x43E2F0,
    '?g_SoundPlayer@@3USoundPlayerView@@A': 0x4DC698,
    '?LoadSurface@TitleAnmManagerView@@QAEHHPBD@Z': 0x43CAE0,
    '?OpenFile@FileSystem@@YIPAEPBDPAHH@Z': 0x42C970,
    '?LoadReplayData@ReplayManagerView@@SIPAUReplayDataView@@PAU2@H@Z': 0x4205E0,
    '?g_TitleNameTableIndex@@3HA': 0x4A7EAC,
    '?g_SelectedReplayPath@@3PADA': 0x4A7ED5,
    '?g_TitleGameFlags@@3IA': 0x4A7EC4,
    '_strcpy': 0x47C4E0,
    '_strcat': 0x47C4F0,
    '_sprintf': 0x47C47B,
    '_memset': 0x47C390,
    '_free': 0x47B163,
    '__mkdir': 0x47C71B,
    '__chdir': 0x47C5D8,
    '__imp__FindFirstFileA@8': 0x48E074,
    '__imp__FindNextFileA@8': 0x48E070,
    '__imp__FindClose@4': 0x48E06C,
    '??_C@_0BD@LOIKDJIG@title?1replay00?4png?$AA@': 0x48F6D4,
    '??_C@_0BG@BOKNDPOO@?4?1replay?1th9_?$CF?42d?4rpy?$AA@': 0x48F6BC,
    '??_C@_06HOJHLELG@replay?$AA@': 0x48F6B4,
    '??_C@_0P@JBFLNHEF@th9_ud?$DP?$DP?$DP?$DP?4rpy?$AA@': 0x48F6A4,
    '??_C@_07DAGALJGA@replay?1?$AA@': 0x48F69C,
    '??_C@_0M@JBOPCDDP@?4?1replay?1?$CFs?$AA@': 0x48F690,
    '??_C@_03LOJJKLGJ@?4?4?1?$AA@': 0x48F68C,
    '?MoveCursorHorizontal@TitleScreenView@@QAEHH@Z': 0x424601,
    '?SetCharacterCursorInactive@TitleScreenView@@QAEXHHHH@Z': 0x42505F,
    '?g_TitleResultUnlockStep@@3HA': 0x4AC8D4,
    '?g_TitleResultCharacterOrder@@3PADA': 0x4A1DAC,
    '??_C@_0BD@JGJJAHCD@title?1result00?4png?$AA@': 0x48F640,
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
# Normalized instruction similarity suppresses jump operands. Check real
# destinations separately so a same-sized but wrong exit cannot disappear.
# This correspondence is diagnostic only, never an acceptance byte mask.
branch_conflicts, unpaired_branches = aligned_branch_conflicts(
    t, c, matcher.get_matching_blocks(), capstone.x86.X86_OP_IMM
)
print('paired CFG branch conflicts:', [tuple(map(hex, row)) for row in branch_conflicts],
      'unpaired destinations:', unpaired_branches)
if len(code) == len(target):
    print('complete replay differences:',sum(a!=b for a,b in zip(code,target)))
for tag,a,b,x,y in matcher.get_opcodes():
    if tag != 'equal':
        print(tag, [(hex(i.address),i.mnemonic,i.op_str)for i in t[a:b]], '=>',[(hex(i.address),i.mnemonic,i.op_str)for i in c[x:y]])
