// Target-bound TH09 TitleScreen options reconstruction view.
// TH08 supplies the natural source family only; TH09 target evidence fixes
// the embedded owner, layout, menu domain, option globals, and control flow.

#include <stddef.h>

typedef signed char i8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

struct AnmVmView {
    u8 unknown000[0x1F0];
    union {
        u32 color1;
        u8 color1Bytes[4];
    };
    u8 unknown1F4[4];
    u32 flags;
    u8 unknown1FC[2];
    u16 pendingInterrupt;
    u8 unknown200[0x18];
    short baseSpriteIndex;
    u8 unknown21A[0x8A];
};

typedef char OptionsAnmVmSizeIs2A4[(sizeof(AnmVmView) == 0x2A4) ? 1 : -1];
typedef char OptionsAnmVmColorAt1F0[(offsetof(AnmVmView, color1) == 0x1F0) ? 1 : -1];
typedef char OptionsAnmVmFlagsAt1F8[(offsetof(AnmVmView, flags) == 0x1F8) ? 1 : -1];
typedef char OptionsAnmVmInterruptAt1FE[(offsetof(AnmVmView, pendingInterrupt) == 0x1FE) ? 1 : -1];
typedef char OptionsAnmVmSpriteAt218[(offsetof(AnmVmView, baseSpriteIndex) == 0x218) ? 1 : -1];

struct TitleAnmView {
    void SetSprite(AnmVmView *vm, i32 spriteIndex);
};

struct TitleAnmManagerView {
    void SetInterruptArray(AnmVmView *vms, i32 count, i32 interrupt);
    void ExecuteScriptArray(AnmVmView *vms, i32 count);
};

struct InputView {
    u16 currentInput;
    u16 IsPressedScrolling(i32 mask);
};

struct SoundPlayerView {
    u8 unknown0000[0x6218];
    i32 bgmVolume;
    i32 sfxVolume;

    void PlaySoundByIdx(i32 soundId, i32 unused);
    void QueueCommand(i32 opcode, i32 argument, char *path);
    int ProcessQueues();
};

struct TitleSupervisorView {
    int LoadMusic(i32 trackId);
    int PlayMusic(i32 trackId, i32 unused);
    int StopAudio();
};

struct TitleConfigSnapshotView {
    u8 bytes[0xCC];
};

typedef char OptionsConfigSnapshotSizeIsCC[(sizeof(TitleConfigSnapshotView) == 0xCC) ? 1 : -1];

extern InputView g_TitleInput;
extern u32 g_TitleInputFlags;
extern SoundPlayerView g_SoundPlayer;
extern TitleAnmManagerView *g_TitleAnmManager;
extern TitleSupervisorView g_TitleSupervisor;
extern const char *g_TitleOptionsHelpText[9];
extern u8 g_OptionA;
extern u8 g_OptionB;
extern u8 g_OptionC;
extern u8 g_OptionUnknown3537;
extern u8 g_OptionD;
extern i8 g_TitleMusicVolume;
extern i8 g_TitleSfxVolume;

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
    TitleAnmView *titleAnm;                   // +0x11B90
    u8 unknown11B94[4];
    AnmVmView *vms;                           // +0x11B98
    AnmVmView *currentHelpTextVm;             // +0x11B9C
    AnmVmView embeddedVms[57];                // +0x11BA0
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
    int UpdateMenuSelection();
    int SetMenuSelectionSprites(i32 selected, i32 start, i32 count);
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
    i32 i;

    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 3);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);
            currentScreenState = 0;
            stateTimer = 0;
            previousCursor = -1;
            menuVmStart = 9;
            menuItemCount = 9;
            UpdateMenuSelection();
            optionsUiReady = 1;
        }

        currentScreenState = 1;
        for (i = 0; i < 9; i++)
            DrawTitleHelpText(g_TitleAnmManager, &embeddedVms[i], 0xFFF0E0, 0x300000,
                              g_TitleOptionsHelpText[i]);

    case 1:
        if (g_TitleMusicVolume >= 100)
        {
            titleAnm->SetSprite(&vms[26], vms[26].baseSpriteIndex);
            vms[26].color1Bytes[3] = 0xFF;
            vms[26].flags |= 2u;
        }
        else
        {
            vms[26].color1Bytes[3] = 0;
            vms[26].flags &= ~2u;
        }

        if (g_TitleMusicVolume >= 10)
        {
            titleAnm->SetSprite(&vms[27], vms[27].baseSpriteIndex +
                                ((g_TitleMusicVolume / 10) % 10) * 2);
            vms[27].color1Bytes[3] = 0xFF;
            vms[27].flags |= 2u;
        }
        else
        {
            vms[27].color1Bytes[3] = 0;
            vms[27].flags &= ~2u;
        }

        titleAnm->SetSprite(&vms[28], vms[28].baseSpriteIndex + (g_TitleMusicVolume % 10) * 2);
        titleAnm->SetSprite(&vms[29], vms[29].baseSpriteIndex);

        if (g_TitleSfxVolume >= 100)
        {
            titleAnm->SetSprite(&vms[30], vms[30].baseSpriteIndex);
            vms[30].color1Bytes[3] = 0xFF;
            vms[30].flags |= 2u;
        }
        else
        {
            vms[30].color1Bytes[3] = 0;
            vms[30].flags &= ~2u;
        }

        if (g_TitleSfxVolume >= 10)
        {
            titleAnm->SetSprite(&vms[31], vms[31].baseSpriteIndex +
                                ((g_TitleSfxVolume / 10) % 10) * 2);
            vms[31].color1Bytes[3] = 0xFF;
            vms[31].flags |= 2u;
        }
        else
        {
            vms[31].color1Bytes[3] = 0;
            vms[31].flags &= ~2u;
        }
        titleAnm->SetSprite(&vms[32], vms[32].baseSpriteIndex + (g_TitleSfxVolume % 10) * 2);

        if (MoveCursorVertical(9))
            UpdateMenuSelection();

        if (previousCursor != keyboardSelection)
        {
            currentHelpTextVm = &embeddedVms[keyboardSelection];
            currentHelpTextVm->pendingInterrupt = 1;
        }
        previousCursor = keyboardSelection;

        if (stateTimer2 < 4)
            break;

        if (g_TitleInput.IsPressedScrolling(0x40))
        {
            switch (keyboardSelection)
            {
            case 0:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                if (g_OptionA)
                    g_OptionA--;
                else
                    g_OptionA = 2;
                SetMenuSelectionSprites(g_OptionA, 18, 3);
                break;
            case 1:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                if (g_OptionB)
                    g_OptionB--;
                else
                    g_OptionB = 1;
                SetMenuSelectionSprites(g_OptionB, 21, 2);
                break;
            case 2:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                g_TitleSupervisor.StopAudio();
                if (g_OptionC)
                    g_OptionC--;
                else
                    g_OptionC = 1;
                g_TitleSupervisor.LoadMusic(0);
                g_TitleSupervisor.PlayMusic(0, 0);
                SetMenuSelectionSprites(g_OptionC, 23, 3);
                break;
            case 5:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                if (g_OptionD)
                    g_OptionD--;
                else
                    g_OptionD = 1;
                SetMenuSelectionSprites(g_OptionD, 34, 2);
                break;
            default:
                goto left_scrolling_done;
            }
            PlayMenuSound(12, 0);
        }
    left_scrolling_done:

        if (g_TitleInputFlags & 0x40)
        {
            if (keyboardSelection == 3)
            {
                g_TitleMusicVolume -= 4;
                if (g_TitleMusicVolume < 0)
                    g_TitleMusicVolume = 0;
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
            }
            else if (keyboardSelection == 4)
            {
                g_TitleSfxVolume -= 4;
                if (g_TitleSfxVolume < 0)
                    g_TitleSfxVolume = 0;
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
            }
        }

        if (g_TitleInputFlags & 0x80)
        {
            if (keyboardSelection == 3)
            {
                g_TitleMusicVolume += 4;
                if (g_TitleMusicVolume > 100)
                    g_TitleMusicVolume = 100;
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
            }
            else if (keyboardSelection == 4)
            {
                g_TitleSfxVolume += 4;
                if (g_TitleSfxVolume > 100)
                    g_TitleSfxVolume = 100;
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
            }
        }

        if (g_TitleInputFlags & 0x40)
        {
            if (keyboardSelection == 3)
            {
                if (g_TitleMusicVolume > 0)
                    g_TitleMusicVolume--;
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
            }
            else if (keyboardSelection == 4)
            {
                if (g_TitleSfxVolume > 0)
                    g_TitleSfxVolume--;
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
            }
        }

        if (g_TitleInputFlags & 0x80)
        {
            if (keyboardSelection == 3)
            {
                if (g_TitleMusicVolume < 100)
                    g_TitleMusicVolume++;
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
            }
            else if (keyboardSelection == 4)
            {
                if (g_TitleSfxVolume < 100)
                    g_TitleSfxVolume++;
                g_SoundPlayer.QueueCommand(8, 0, "SetVol");
            }
        }

        g_SoundPlayer.bgmVolume = g_TitleMusicVolume;
        g_SoundPlayer.sfxVolume = g_TitleSfxVolume;

        if (g_TitleInput.IsPressedScrolling(0x80))
        {
            switch (keyboardSelection)
            {
            case 0:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                if (g_OptionA < 2)
                    g_OptionA++;
                else
                    g_OptionA = 0;
                SetMenuSelectionSprites(g_OptionA, 18, 3);
                break;
            case 1:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                g_OptionB = g_OptionB == 0;
                SetMenuSelectionSprites(g_OptionB, 21, 2);
                break;
            case 2:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                g_TitleSupervisor.StopAudio();
                g_OptionC = g_OptionC == 0;
                g_TitleSupervisor.LoadMusic(0);
                g_TitleSupervisor.PlayMusic(0, 0);
                SetMenuSelectionSprites(g_OptionC, 23, 3);
                break;
            case 5:
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                g_OptionD = g_OptionD == 0;
                SetMenuSelectionSprites(g_OptionD, 34, 2);
                break;
            default:
                goto right_scrolling_done;
            }
            PlayMenuSound(12, 0);
        }
    right_scrolling_done:

        if ((keyboardSelection == 3 || keyboardSelection == 4) && stateTimer2 % 50 == 0)
            g_SoundPlayer.PlaySoundByIdx(29, 0);

        optionsFrameCounter = 0;
        if (g_TitleInputFlags & 0x1001)
        {
            if (keyboardSelection == 6)
            {
                g_OptionA = 0;
                g_OptionC = 1;
                g_OptionUnknown3537 = 1;
                PlayMenuSound(10, 0);
                SetMenuSelectionSprites(g_OptionA, 18, 3);
                SetMenuSelectionSprites(g_OptionB, 21, 2);
                SetMenuSelectionSprites(g_OptionC, 23, 3);
                SetMenuSelectionSprites(g_OptionD, 34, 2);
            }
            else if (keyboardSelection == 7)
            {
                keyboardSelection = 0;
                ChangeCurrentScreen(10);
                PlayMenuSound(10, 0);
                return 1;
            }
            else if (keyboardSelection == 8)
            {
                goto leave_options;
            }
        }

        if (g_TitleInputFlags & 0xA)
        {
            if (keyboardSelection == 8)
                goto leave_options;

            titleAnm->SetSprite(&vms[keyboardSelection + 9],
                                vms[keyboardSelection + 9].baseSpriteIndex + 1);
            keyboardSelection = 8;
            titleAnm->SetSprite(&vms[17], vms[17].baseSpriteIndex);
            PlayMenuSound(11, 0);
        }
        break;

    default:
        break;
    }

common_tail:
    optionsFrameCounter++;
    stateTimer++;
    stateTimer2++;
    return 1;

leave_options:
    keyboardSelection = 6;
    ChangeCurrentScreen(1);
    OnUpdateStartMenu();
    PlayMenuSound(11, 0);
    if (configSnapshot.bytes[0xAD] != g_OptionB || configSnapshot.bytes[0xB1] != g_OptionD)
        return 5;
    return 1;
}

int TitleScreenView::PlayMenuSound(i32 soundId, i32 unused)
{
    g_SoundPlayer.PlaySoundByIdx(soundId, unused);
    return g_SoundPlayer.ProcessQueues();
}
