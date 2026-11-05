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

// Bounded scrolling-helper prefix of the native ReplayInputState slot.
struct InputView {
    u16 held00;
    u16 previous02;
    u16 repeatOutput04;
    u16 frontState06;
    u16 IsPressedScrolling(i32 mask)
    {
        u16 pressed = frontState06 & mask;
        return pressed != 0 || ((pressed = repeatOutput04 & mask) != 0);
    }
};

typedef char OptionsInputSizeIs08[(sizeof(InputView) == 8) ? 1 : -1];
typedef char OptionsInputRepeatAt04[(offsetof(InputView, repeatOutput04) == 4) ? 1 : -1];
typedef char OptionsInputFrontAt06[(offsetof(InputView, frontState06) == 6) ? 1 : -1];

typedef char OptionsNativeInputSizeIs8E[(sizeof(ReplayInputState) == 0x8E) ? 1 : -1];
typedef char OptionsNativeInputWord06At06[(offsetof(ReplayInputState, word06) == 6) ? 1 : -1];
typedef char OptionsNativeInputWordIs16[(sizeof(((ReplayInputState *)0)->word06) == 2) ? 1 : -1];
typedef char OptionsInputPrefixMatchesNative[
    (offsetof(InputView, held00) == offsetof(ReplayInputState, currentInput) &&
     offsetof(InputView, previous02) == offsetof(ReplayInputState, word02) &&
     offsetof(InputView, repeatOutput04) == offsetof(ReplayInputState, repeatOutput) &&
     offsetof(InputView, frontState06) == offsetof(ReplayInputState, word06)) ? 1 : -1];

typedef char OptionsConfigvalueACAtAC[(offsetof(GameConfiguration, valueAC) == 0xAC) ? 1 : -1];
typedef char OptionsConfigvalueADAtAD[(offsetof(GameConfiguration, valueAD) == 0xAD) ? 1 : -1];
typedef char OptionsConfigmusicModeAtAE[(offsetof(GameConfiguration, musicMode) == 0xAE) ? 1 : -1];
typedef char OptionsConfigvalueAFAtAF[(offsetof(GameConfiguration, valueAF) == 0xAF) ? 1 : -1];
typedef char OptionsConfigwindowedAtB1[(offsetof(GameConfiguration, windowed) == 0xB1) ? 1 : -1];
typedef char OptionsConfigmusicVolumeAtBA[(offsetof(GameConfiguration, musicVolume) == 0xBA) ? 1 : -1];
typedef char OptionsConfigsfxVolumeAtBB[(offsetof(GameConfiguration, sfxVolume) == 0xBB) ? 1 : -1];

struct TitleConfigSnapshotView {
    u8 unknown00[0xAD];
    u8 valueAD;
    u8 unknownAE[3];
    u8 windowed;
    u8 unknownB2[0x1A];
};

typedef char OptionsConfigSnapshotSizeIsCC[(sizeof(TitleConfigSnapshotView) == 0xCC) ? 1 : -1];
typedef char OptionsSnapshotValueADAtAD[(offsetof(TitleConfigSnapshotView, valueAD) == 0xAD) ? 1 : -1];
typedef char OptionsSnapshotWindowedAtB1[(offsetof(TitleConfigSnapshotView, windowed) == 0xB1) ? 1 : -1];


extern const char *g_TitleOptionsHelpText[9];

extern void __cdecl DrawTitleHelpText(TitleAnmManagerView *manager, AnmVmView *vm,
                                      u32 color, i32 width, const char *text);

struct TitleScreenView {
    i32 keyboardSelection;                    // +0x00000
    u8 unknown00004[0x20];
    i32 previousCursor;                       // +0x00024
    i32 currentScreenState;                   // +0x00028
    i32 stateTimer;                           // +0x0002C
    u8 unknown00030[0xC8E0];
    union {
        i32 screenFrameCounter;
        i32 optionsFrameCounter;
    };                                        // +0x0C910
    u8 unknown0C914[4];
    i32 optionsUiReady;                       // +0x0C918
    u8 unknown0C91C[0x5274];
    AnmLoaded *titleAnm;                   // +0x11B90
    u8 unknown11B94[4];
    AnmVm *vms;                           // +0x11B98
    AnmVm *currentHelpTextVm;             // +0x11B9C
    AnmVm embeddedVms[57];                // +0x11BA0
    i32 vmCount;                              // +0x1B224
    i32 currentScreen;                        // +0x1B228
    i32 registrationContext;                  // +0x1B22C
    i32 stateTimer2;                          // +0x1B230
    i32 idleFrames;                           // +0x1B234
    i32 chainState;                           // +0x1B238
    i32 resumeState;                          // +0x1B23C
    i32 menuVmStart;                          // +0x1B240
    i32 menuItemCount;                        // +0x1B244
    u8 unknown1B248[0x74];
    TitleConfigSnapshotView configSnapshot;   // +0x1B2BC

    int PlayMenuSound(i32 soundId, i32 unused);
    int ChangeCurrentScreen(i32 screen);
    int MoveCursorVertical(i32 count);
    void UpdateMenuSelection();
    void SetMenuSelectionSprites(i32 selected, i32 start, i32 count);
    int OnUpdateStartMenu();
    int OnUpdateOptions();
};

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

int TitleScreenView::OnUpdateOptions()
{
    int i;

    switch (this->currentScreenState)
    {
    case 0:
        if (this->stateTimer2 == 0)
        {
            g_AnmManager->SetInterruptArray(reinterpret_cast<AnmVm *>(this->vms), this->vmCount, 3);
            g_AnmManager->ExecuteScriptArray(reinterpret_cast<AnmVm *>(this->vms), this->vmCount);
            this->currentScreenState = 0;
            this->stateTimer = 0;
            this->previousCursor = -1;
            this->menuItemCount = this->menuVmStart = 9;
            this->UpdateMenuSelection();
            this->optionsUiReady = 1;
        }
        this->currentScreenState = 1;
        for (i = 0; i < sizeof(g_TitleOptionsHelpText) / sizeof(g_TitleOptionsHelpText[0]); i++)
        {
            DrawTitleHelpText(reinterpret_cast<TitleAnmManagerView *>(g_AnmManager), reinterpret_cast<AnmVmView *>(&this->embeddedVms[i]), 0xfff0e0, 0x300000, g_TitleOptionsHelpText[i]);
        }
    case 1:
        if (reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume) >= 100)
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
        if (reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume) >= 10)
        {
            this->titleAnm->SetSprite(&this->vms[27],
                                 this->vms[27].baseSpriteIndex + reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume) / 10 % 10 * 2);
            this->vms[27].color1Script.a = 0xff;
            this->vms[27].drawEnabled = 1;
        }
        else
        {
            this->vms[27].color1Script.a = 0;
            this->vms[27].drawEnabled = 0;
        }
        this->titleAnm->SetSprite(&this->vms[28], this->vms[28].baseSpriteIndex + reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume) % 10 * 2);
        this->titleAnm->SetSprite(&this->vms[29], this->vms[29].baseSpriteIndex);
        if (reinterpret_cast<i8 &>(g_Supervisor.config388.sfxVolume) >= 100)
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
        if (reinterpret_cast<i8 &>(g_Supervisor.config388.sfxVolume) >= 10)
        {
            this->titleAnm->SetSprite(&this->vms[31],
                                 this->vms[31].baseSpriteIndex + reinterpret_cast<i8 &>(g_Supervisor.config388.sfxVolume) / 10 % 10 * 2);
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
            case 0:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
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
            case 1:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
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
            case 2:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
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
            case 5:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
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
            this->PlayMenuSound(12, 0);
        }
    volume:
        if (g_ReplayInputStates[2].word06 & 0x40)
        {
            switch (this->keyboardSelection)
            {
            case 3:
                reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume) -= 4;
                if (reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume) < 0)
                {
                    reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume) = 0;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            case 4:
                reinterpret_cast<i8 &>(g_Supervisor.config388.sfxVolume) -= 4;
                if (reinterpret_cast<i8 &>(g_Supervisor.config388.sfxVolume) < 0)
                {
                    reinterpret_cast<i8 &>(g_Supervisor.config388.sfxVolume) = 0;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            }
        }
        if (g_ReplayInputStates[2].word06 & 0x80)
        {
            switch (this->keyboardSelection)
            {
            case 3:
                reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume) += 4;
                if (reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume) > 100)
                {
                    reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume) = 100;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            case 4:
                reinterpret_cast<i8 &>(g_Supervisor.config388.sfxVolume) += 4;
                if (reinterpret_cast<i8 &>(g_Supervisor.config388.sfxVolume) > 100)
                {
                    reinterpret_cast<i8 &>(g_Supervisor.config388.sfxVolume) = 100;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            }
        }
        if (g_ReplayInputStates[2].word06 & 0x40)
        {
            switch (this->keyboardSelection)
            {
            case 3:
                if (reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume) > 0)
                {
                    reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume)--;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            case 4:
                if (reinterpret_cast<i8 &>(g_Supervisor.config388.sfxVolume) > 0)
                {
                    reinterpret_cast<i8 &>(g_Supervisor.config388.sfxVolume)--;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            }
        }
        if (g_ReplayInputStates[2].word06 & 0x80)
        {
            switch (this->keyboardSelection)
            {
            case 3:
                if (reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume) < 100)
                {
                    reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume)++;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            case 4:
                if (reinterpret_cast<i8 &>(g_Supervisor.config388.sfxVolume) < 100)
                {
                    reinterpret_cast<i8 &>(g_Supervisor.config388.sfxVolume)++;
                }
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
                break;
            }
        }
        g_SoundPlayer.bgmVolume = reinterpret_cast<i8 &>(g_Supervisor.config388.musicVolume);
        g_SoundPlayer.sfxVolume = reinterpret_cast<i8 &>(g_Supervisor.config388.sfxVolume);
        if (reinterpret_cast<InputView *>(&g_ReplayInputStates[2])->IsPressedScrolling(0x80))
        {
            switch (this->keyboardSelection)
            {
            case 0:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
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
            case 1:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
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
            case 2:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
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
            case 5:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
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
            this->PlayMenuSound(12, 0);
        }
    idle:
        if (this->keyboardSelection >= 3 && this->keyboardSelection <= 4 && this->stateTimer2 % 50 == 0)
        {
            g_SoundPlayer.PlaySoundByIdx(0x1d, 0);
        }
        this->optionsFrameCounter = 0;
        if (g_ReplayInputStates[2].word06 & 0x1001)
        {
            switch (this->keyboardSelection)
            {
            case 6:
                g_Supervisor.config388.valueAC = 0;
                g_Supervisor.config388.musicMode = 1;
                g_Supervisor.config388.valueAF = 1;
                this->PlayMenuSound(10, 0);
                this->SetMenuSelectionSprites(g_Supervisor.config388.valueAC, 0x12, 3);
                this->SetMenuSelectionSprites(g_Supervisor.config388.valueAD, 0x15, 2);
                this->SetMenuSelectionSprites(g_Supervisor.config388.musicMode, 0x17, 3);
                this->SetMenuSelectionSprites(g_Supervisor.config388.windowed, 0x22, 2);
                break;
            case 7:
                this->keyboardSelection = 0;
                this->ChangeCurrentScreen(10);
                this->PlayMenuSound(10, 0);
                return 1;
            case 8:
                goto leave;
            }
        }
        if (g_ReplayInputStates[2].word06 & 0xa)
        {
            if (this->keyboardSelection == 8)
            {
            leave:
                this->keyboardSelection = 6;
                this->ChangeCurrentScreen(1);
                this->OnUpdateStartMenu();
                this->PlayMenuSound(11, 0);
                if (this->configSnapshot.valueAD != g_Supervisor.config388.valueAD ||
                    this->configSnapshot.windowed != g_Supervisor.config388.windowed)
                {
                    return 5;
                }
                return 1;
            }
            this->titleAnm->SetSprite(&this->vms[this->keyboardSelection + 9], this->vms[this->keyboardSelection + 9].baseSpriteIndex + 1);
            this->keyboardSelection = 8;
            this->titleAnm->SetSprite(&this->vms[this->keyboardSelection + 9], this->vms[this->keyboardSelection + 9].baseSpriteIndex);
            this->PlayMenuSound(11, 0);
        }
    }
    this->optionsFrameCounter++;
    this->stateTimer++;
    this->stateTimer2++;
    return 1;
}

int TitleScreenView::PlayMenuSound(i32 soundId, i32 unused)
{
    g_SoundPlayer.PlaySoundByIdx(soundId, unused);
    return g_SoundPlayer.ProcessQueues();
}
