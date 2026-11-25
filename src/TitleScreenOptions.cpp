// SPDX-License-Identifier: GPL-3.0-only
// Adapted from Cirno-Labs/th09 src/mainmenu.cpp, MainMenu::UpdateState9,
// commit d7653152becabb8c86f2f43d49015ecc11bd0652.
// Original MIT portions retain their grant in LICENSE; see THIRD_PARTY_NOTICES.md.
// Target-bound TH09 TitleScreen options reconstruction view.
// TH08 supplies the natural source family only; TH09 target evidence fixes
// the embedded owner, layout, menu domain, option globals, and control flow.

#include "Supervisor.hpp"
#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "SoundPlayer.hpp"
#include "ReplayInputState.hpp"
#include "TitleScreenReplayView.hpp"
#include "TitleScreenStates.hpp"

#include <stddef.h>

typedef signed char i8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

typedef char OptionsAnmVmSizeIs2A4[(sizeof(AnmVm) == 0x2A4) ? 1 : -1];
typedef char OptionsRealBgmVolumeAt6218[(offsetof(SoundPlayer, bgmVolume) == 0x6218) ? 1 : -1];
typedef char OptionsRealSfxVolumeAt621C[(offsetof(SoundPlayer, sfxVolume) == 0x621C) ? 1 : -1];
typedef char OptionsAnmVmColorAt1F0[(offsetof(AnmVm, color1) == 0x1F0) ? 1 : -1];
typedef char OptionsAnmVmFlagsAt1F8[(offsetof(AnmVm, flagsWord) == 0x1F8) ? 1 : -1];
typedef char OptionsAnmVmInterruptAt1FE[(offsetof(AnmVm, pendingInterrupt) == 0x1FE) ? 1 : -1];
typedef char OptionsAnmVmSpriteAt218[(offsetof(AnmVm, baseSpriteIndex) == 0x218) ? 1 : -1];

// DrawTitleHelpText is a separate, independently addressed helper whose
// original C++ declaration is not yet reconstructed. Retain its ABI-facing
// argument tag without inventing ownership of the real ANM manager.
struct TitleAnmManagerView;
struct AnmVmView; // Opaque argument tag for DrawTitleHelpText only.

// The options compiler profile requires inlining the scrolling helper, but
// the receiver is the genuine complete 0x8E-byte native replay input slot.
struct InputView : ReplayInputState
{
    u16 IsPressedScrolling(i32 mask)
    {
        u16 pressed = word06 & mask;
        return pressed != 0 || ((pressed = repeatOutput & mask) != 0);
    }
};
typedef char OptionsInputWholeNativeRecord[
    (sizeof(InputView) == sizeof(ReplayInputState) &&
     sizeof(InputView) == 0x8E &&
     offsetof(InputView, currentInput) == 0 &&
     offsetof(InputView, repeatOutput) == 4 &&
     offsetof(InputView, word06) == 6) ? 1 : -1];

typedef char OptionsConfigvalueACAtAC[(offsetof(GameConfiguration, valueAC) == 0xAC) ? 1 : -1];
typedef char OptionsConfigvalueADAtAD[(offsetof(GameConfiguration, valueAD) == 0xAD) ? 1 : -1];
typedef char OptionsConfigmusicModeAtAE[(offsetof(GameConfiguration, musicMode) == 0xAE) ? 1 : -1];
typedef char OptionsConfigvalueAFAtAF[(offsetof(GameConfiguration, valueAF) == 0xAF) ? 1 : -1];
typedef char OptionsConfigwindowedAtB1[(offsetof(GameConfiguration, windowed) == 0xB1) ? 1 : -1];
typedef char OptionsConfigmusicVolumeAtBA[(offsetof(GameConfiguration, musicVolume) == 0xBA) ? 1 : -1];
typedef char OptionsConfigsfxVolumeAtBB[(offsetof(GameConfiguration, sfxVolume) == 0xBB) ? 1 : -1];

// Title's stored 0xCC-byte snapshot is the same serialized native
// GameConfiguration as Supervisor::config388, not another private prefix.
typedef char OptionsConfigSnapshotNative[
    (sizeof(TitleConfigSnapshotView) == sizeof(GameConfiguration) &&
     sizeof(TitleConfigSnapshotView) == 0xCC &&
     offsetof(TitleConfigSnapshotView, valueAD) == 0xAD &&
     offsetof(TitleConfigSnapshotView, windowed) == 0xB1) ? 1 : -1];


extern const char *g_TitleOptionsHelpText[9];

extern void __cdecl DrawTitleHelpText(TitleAnmManagerView *manager, AnmVmView *vm,
                                      u32 color, i32 width, const char *text);



typedef char OptionsPreviousCursorAt24[(offsetof(TitleScreenView, previousCursor) == 0x24) ? 1 : -1];
typedef char OptionsStateAt28[(offsetof(TitleScreenView, currentScreenState) == 0x28) ? 1 : -1];
typedef char OptionsFrameAtC910[(offsetof(TitleScreenView, optionsFrameCounter) == 0xC910) ? 1 : -1];
typedef char OptionsUiReadyAtC918[(offsetof(TitleScreenView, optionsUiReady) == 0xC918) ? 1 : -1];
typedef char OptionsAnmAt11B90[(offsetof(TitleScreenView, titleAnm) == 0x11B90) ? 1 : -1];
typedef char OptionsVmsAt11B98[(offsetof(TitleScreenView, vms) == 0x11B98) ? 1 : -1];
typedef char OptionsHelpAt11BA0[(offsetof(TitleScreenView, embeddedVms) == 0x11BA0) ? 1 : -1];
typedef char OptionsVmCountAt1B224[(offsetof(TitleScreenView, vmCount) == 0x1B224) ? 1 : -1];
typedef char OptionsTimerAt1B230[(offsetof(TitleScreenView, stateTimer2) == 0x1B230) ? 1 : -1];
typedef char OptionsMenuStartAt1B240[(offsetof(TitleScreenView, menuVmStart) == 0x1B240) ? 1 : -1];
typedef char OptionsMenuCountAt1B244[(offsetof(TitleScreenView, menuItemCount) == 0x1B244) ? 1 : -1];
typedef char OptionsConfigAt1B2BC[(offsetof(TitleScreenView, configSnapshot) == 0x1B2BC) ? 1 : -1];
typedef char OptionsTitleScreenSizeIs1B388[(sizeof(TitleScreenView) == 0x1B388) ? 1 : -1];

// Music and SFX volumes are genuine GameConfiguration byte members.
// Read them with signed-char semantics for display and bounds checks;
// write those native members directly, without aliasing byte references.
// The real TitleScreen owns 0x2A4-byte ANM VMs and a separate
// four-byte help-text pointer table. These live selectors form original
// Win32 byte addresses without a new clamp, copy or auxiliary allocation.
// The original Title object owns native help VMs and script VMs,
// and the help-text pointers belong to a real four-byte global table.

int TitleScreenView::OnUpdateOptions()
{
    int i;

    switch (this->currentScreenState)
    {
    case TITLE_OPTIONS_STATE_INIT:
        if (this->stateTimer2 == 0)
        {
            g_AnmManager->SetInterruptArray(this->vms, this->vmCount, 3);
            g_AnmManager->ExecuteScriptArray(this->vms, this->vmCount);
            this->currentScreenState = TITLE_OPTIONS_STATE_INIT;
            this->stateTimer = 0;
            this->previousCursor = -1;
            this->menuItemCount = this->menuVmStart = 9;
            this->UpdateMenuSelection();
            this->optionsUiReady = 1;
        }
        this->currentScreenState = TITLE_OPTIONS_STATE_ACTIVE;
        for (i = 0; i < sizeof(g_TitleOptionsHelpText) / sizeof(g_TitleOptionsHelpText[0]); i++)
        {
            DrawTitleHelpText(reinterpret_cast<TitleAnmManagerView *>(g_AnmManager), &this->embeddedVms[i], 0xfff0e0, 0x300000, g_TitleOptionsHelpText[i]);
        }
    case TITLE_OPTIONS_STATE_ACTIVE:
        if (static_cast<i8>(g_Supervisor.config388.musicVolume) >= 100)
        {
            this->titleAnm->SetSprite(&this->vms[26], this->vms[26].baseSpriteIndex);
            this->vms[26].color1Script.a = 0xff;
            this->vms[26].drawEnabled = 1;
        }
        else
        {
            this->vms[26].color1Script.a = 0;
            this->vms[26].drawEnabled = 0;
        }
        if (static_cast<i8>(g_Supervisor.config388.musicVolume) >= 10)
        {
            this->titleAnm->SetSprite(&this->vms[27],
                                 this->vms[27].baseSpriteIndex + static_cast<i8>(g_Supervisor.config388.musicVolume) / 10 % 10 * 2);
            this->vms[27].color1Script.a = 0xff;
            this->vms[27].drawEnabled = 1;
        }
        else
        {
            this->vms[27].color1Script.a = 0;
            this->vms[27].drawEnabled = 0;
        }
        this->titleAnm->SetSprite(&this->vms[28], this->vms[28].baseSpriteIndex + static_cast<i8>(g_Supervisor.config388.musicVolume) % 10 * 2);
        this->titleAnm->SetSprite(&this->vms[29], this->vms[29].baseSpriteIndex);
        if (static_cast<i8>(g_Supervisor.config388.sfxVolume) >= 100)
        {
            this->titleAnm->SetSprite(&this->vms[30], this->vms[30].baseSpriteIndex);
            this->vms[30].color1Script.a = 0xff;
            this->vms[30].drawEnabled = 1;
        }
        else
        {
            this->vms[30].color1Script.a = 0;
            this->vms[30].drawEnabled = 0;
        }
        if (static_cast<i8>(g_Supervisor.config388.sfxVolume) >= 10)
        {
            this->titleAnm->SetSprite(&this->vms[31],
                                 this->vms[31].baseSpriteIndex + static_cast<i8>(g_Supervisor.config388.sfxVolume) / 10 % 10 * 2);
            this->vms[31].color1Script.a = 0xff;
            this->vms[31].drawEnabled = 1;
        }
        else
        {
            this->vms[31].color1Script.a = 0;
            this->vms[31].drawEnabled = 0;
        }
        if (this->MoveCursorVertical(9) != 0)
        {
            this->UpdateMenuSelection();
        }
        if (this->previousCursor != this->keyboardSelection)
        {
            this->currentHelpTextVm = &this->embeddedVms[this->keyboardSelection];
            this->currentHelpTextVm->pendingInterrupt = 1;
        }
        this->previousCursor = this->keyboardSelection;
        if (this->stateTimer2 < 4)
        {
            break;
        }
        if (reinterpret_cast<InputView *>(&g_ReplayInputStates[2])->IsPressedScrolling(0x40))
        {
            switch (this->keyboardSelection)
            {
            case TITLE_OPTIONS_ITEM_STARTING_LIVES:
                g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
                if (g_Supervisor.config388.valueAC == 0)
                {
                    g_Supervisor.config388.valueAC = 2;
                }
                else
                {
                    g_Supervisor.config388.valueAC--;
                }
                this->SetMenuSelectionSprites(g_Supervisor.config388.valueAC, 0x12, 3);
                break;
            case TITLE_OPTIONS_ITEM_COLOR_DEPTH:
                g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
                if (g_Supervisor.config388.valueAD == 0)
                {
                    g_Supervisor.config388.valueAD = 1;
                }
                else
                {
                    g_Supervisor.config388.valueAD--;
                }
                this->SetMenuSelectionSprites(g_Supervisor.config388.valueAD, 0x15, 2);
                break;
            case TITLE_OPTIONS_ITEM_BGM_MODE:
                g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
                g_Supervisor.StopAudio();
                if (g_Supervisor.config388.musicMode == 0)
                {
                    g_Supervisor.config388.musicMode = 1;
                }
                else
                {
                    g_Supervisor.config388.musicMode--;
                }
                g_Supervisor.LoadMusic(0);
                g_Supervisor.PlayMusic(0, 0);
                this->SetMenuSelectionSprites(g_Supervisor.config388.musicMode, 0x17, 3);
                break;
            case TITLE_OPTIONS_ITEM_WINDOW_MODE:
                g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
                if (g_Supervisor.config388.windowed == 0)
                {
                    g_Supervisor.config388.windowed = 1;
                }
                else
                {
                    g_Supervisor.config388.windowed--;
                }
                this->SetMenuSelectionSprites(g_Supervisor.config388.windowed, 0x22, 2);
                break;
            default:
                goto volume;
            }
            this->PlayMenuSound(SOUND_MOVE_MENU, 0);
        }
    volume:
        if (g_ReplayInputStates[2].word06 & 0x40)
        {
            switch (this->keyboardSelection)
            {
            case TITLE_OPTIONS_ITEM_BGM_VOLUME:
                g_Supervisor.config388.musicVolume -= 4u;
                if (static_cast<i8>(g_Supervisor.config388.musicVolume) < 0)
                {
                    g_Supervisor.config388.musicVolume = 0;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            case TITLE_OPTIONS_ITEM_SFX_VOLUME:
                g_Supervisor.config388.sfxVolume -= 4u;
                if (static_cast<i8>(g_Supervisor.config388.sfxVolume) < 0)
                {
                    g_Supervisor.config388.sfxVolume = 0;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            }
        }
        if (g_ReplayInputStates[2].word06 & 0x80)
        {
            switch (this->keyboardSelection)
            {
            case TITLE_OPTIONS_ITEM_BGM_VOLUME:
                g_Supervisor.config388.musicVolume += 4u;
                if (static_cast<i8>(g_Supervisor.config388.musicVolume) > 100)
                {
                    g_Supervisor.config388.musicVolume = 100;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            case TITLE_OPTIONS_ITEM_SFX_VOLUME:
                g_Supervisor.config388.sfxVolume += 4u;
                if (static_cast<i8>(g_Supervisor.config388.sfxVolume) > 100)
                {
                    g_Supervisor.config388.sfxVolume = 100;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            }
        }
        if (g_ReplayInputStates[2].word06 & 0x40)
        {
            switch (this->keyboardSelection)
            {
            case TITLE_OPTIONS_ITEM_BGM_VOLUME:
                if (static_cast<i8>(g_Supervisor.config388.musicVolume) > 0)
                {
                    g_Supervisor.config388.musicVolume--;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            case TITLE_OPTIONS_ITEM_SFX_VOLUME:
                if (static_cast<i8>(g_Supervisor.config388.sfxVolume) > 0)
                {
                    g_Supervisor.config388.sfxVolume--;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            }
        }
        if (g_ReplayInputStates[2].word06 & 0x80)
        {
            switch (this->keyboardSelection)
            {
            case TITLE_OPTIONS_ITEM_BGM_VOLUME:
                if (static_cast<i8>(g_Supervisor.config388.musicVolume) < 100)
                {
                    g_Supervisor.config388.musicVolume++;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            case TITLE_OPTIONS_ITEM_SFX_VOLUME:
                if (static_cast<i8>(g_Supervisor.config388.sfxVolume) < 100)
                {
                    g_Supervisor.config388.sfxVolume++;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            }
        }
        g_SoundPlayer.bgmVolume = static_cast<i8>(g_Supervisor.config388.musicVolume);
        g_SoundPlayer.sfxVolume = static_cast<i8>(g_Supervisor.config388.sfxVolume);
        if (reinterpret_cast<InputView *>(&g_ReplayInputStates[2])->IsPressedScrolling(0x80))
        {
            switch (this->keyboardSelection)
            {
            case TITLE_OPTIONS_ITEM_STARTING_LIVES:
                g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
                if (g_Supervisor.config388.valueAC >= 2)
                {
                    g_Supervisor.config388.valueAC = 0;
                }
                else
                {
                    g_Supervisor.config388.valueAC++;
                }
                this->SetMenuSelectionSprites(g_Supervisor.config388.valueAC, 0x12, 3);
                break;
            case TITLE_OPTIONS_ITEM_COLOR_DEPTH:
                g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
                if (g_Supervisor.config388.valueAD >= 1)
                {
                    g_Supervisor.config388.valueAD = 0;
                }
                else
                {
                    g_Supervisor.config388.valueAD++;
                }
                this->SetMenuSelectionSprites(g_Supervisor.config388.valueAD, 0x15, 2);
                break;
            case TITLE_OPTIONS_ITEM_BGM_MODE:
                g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
                g_Supervisor.StopAudio();
                if (g_Supervisor.config388.musicMode >= 1)
                {
                    g_Supervisor.config388.musicMode = 0;
                }
                else
                {
                    g_Supervisor.config388.musicMode++;
                }
                g_Supervisor.LoadMusic(0);
                g_Supervisor.PlayMusic(0, 0);
                this->SetMenuSelectionSprites(g_Supervisor.config388.musicMode, 0x17, 3);
                break;
            case TITLE_OPTIONS_ITEM_WINDOW_MODE:
                g_SoundPlayer.PlaySoundByIdx(SOUND_MOVE_MENU, 0);
                if (g_Supervisor.config388.windowed >= 1)
                {
                    g_Supervisor.config388.windowed = 0;
                }
                else
                {
                    g_Supervisor.config388.windowed++;
                }
                this->SetMenuSelectionSprites(g_Supervisor.config388.windowed, 0x22, 2);
                break;
            default:
                goto idle;
            }
            this->PlayMenuSound(SOUND_MOVE_MENU, 0);
        }
    idle:
        if (this->keyboardSelection >= TITLE_OPTIONS_ITEM_BGM_VOLUME &&
            this->keyboardSelection <= TITLE_OPTIONS_ITEM_SFX_VOLUME &&
            this->stateTimer2 % 50 == 0)
        {
            g_SoundPlayer.PlaySoundByIdx(SOUND_SFX_TEST, 0);
        }
        this->optionsFrameCounter = 0;
        if (g_ReplayInputStates[2].word06 & 0x1001)
        {
            switch (this->keyboardSelection)
            {
            case TITLE_OPTIONS_ITEM_RESET_DEFAULTS:
                g_Supervisor.config388.valueAC = 0;
                g_Supervisor.config388.musicMode = 1;
                g_Supervisor.config388.valueAF = 1;
                this->PlayMenuSound(SOUND_SELECT, 0);
                this->SetMenuSelectionSprites(g_Supervisor.config388.valueAC, 0x12, 3);
                this->SetMenuSelectionSprites(g_Supervisor.config388.valueAD, 0x15, 2);
                this->SetMenuSelectionSprites(g_Supervisor.config388.musicMode, 0x17, 3);
                this->SetMenuSelectionSprites(g_Supervisor.config388.windowed, 0x22, 2);
                break;
            case TITLE_OPTIONS_ITEM_KEY_CONFIG:
                this->keyboardSelection = 0;
                this->ChangeCurrentScreen(TITLE_SCREEN_KEY_CONFIG);
                this->PlayMenuSound(SOUND_SELECT, 0);
                return 1;
            case TITLE_OPTIONS_ITEM_QUIT:
                goto leave;
            }
        }
        if (g_ReplayInputStates[2].word06 & 0xa)
        {
            if (this->keyboardSelection == TITLE_OPTIONS_ITEM_QUIT)
            {
            leave:
                this->keyboardSelection = TITLE_START_MENU_OPTIONS;
                this->ChangeCurrentScreen(TITLE_SCREEN_START_MENU);
                this->OnUpdateStartMenu();
                this->PlayMenuSound(SOUND_BACK, 0);
                if (this->configSnapshot.valueAD != g_Supervisor.config388.valueAD ||
                    this->configSnapshot.windowed != g_Supervisor.config388.windowed)
                {
                    return 5;
                }
                return 1;
            }
            this->titleAnm->SetSprite(&this->vms[static_cast<int>(static_cast<u32>(this->keyboardSelection) + 9u)], this->vms[static_cast<int>(static_cast<u32>(this->keyboardSelection) + 9u)].baseSpriteIndex + 1);
            this->keyboardSelection = 8;
            this->titleAnm->SetSprite(&this->vms[static_cast<int>(static_cast<u32>(this->keyboardSelection) + 9u)], this->vms[static_cast<int>(static_cast<u32>(this->keyboardSelection) + 9u)].baseSpriteIndex);
            this->PlayMenuSound(SOUND_BACK, 0);
        }
    }
    // Advance the three original Title screen/timer members directly.
    ++this->optionsFrameCounter;
    ++this->stateTimer;
    ++this->stateTimer2;
    return 1;
}

int TitleScreenView::PlayMenuSound(i32 soundId, i32 unused)
{
    g_SoundPlayer.PlaySoundByIdx(soundId, unused);
    return g_SoundPlayer.ProcessQueues();
}
