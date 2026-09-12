// Target-bound TH09 TitleScreen reconstruction views. The physical TitleScreen
// owner is target-proved; original identifier spelling, TU partition, and data-definition ownership remain unresolved.

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

extern "C" int __cdecl sprintf(char *, const char *, ...);
extern "C" char *__cdecl strcpy(char *, const char *);
extern "C" void *__cdecl memset(void *, int, size_t);
extern "C" void __cdecl free(void *);
extern "C" int __cdecl _mkdir(const char *);

struct ReplayDataView {
    u32 magic;
    u8 bytes[0x1E8];
};

typedef char ReplayDataSizeIs1EC[(sizeof(ReplayDataView) == 0x1EC) ? 1 : -1];

struct TitleNameRecordView {
    char name[9];
    u8 unknown09[35];
};

typedef char TitleNameRecordSizeIs2C[(sizeof(TitleNameRecordView) == 0x2C) ? 1 : -1];

struct InputView {
    u16 currentInput;
    u16 IsPressedScrolling(i32 mask);
};

struct SoundPlayerView {
    void PlaySoundByIdx(i32 soundId, i32 unused);
};



struct ReplayManagerView {
    static ReplayDataView *LoadReplayData(ReplayDataView *data, i32 fileSize);
    static int SaveReplay(ReplayManagerView *replayManager, const char *replayPath, const char *replayName);
    void Release();
};

struct FileSystemView {
    static int CheckIfFileAlreadyExists(const char *path);
    static void *OpenFile(const char *path, i32 *fileSize, i32 loadFromDisk);
};

struct AnmVmView {
    AnmVmView();
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

typedef char AnmVmSizeIs2A4[(sizeof(AnmVmView) == 0x2A4) ? 1 : -1];
typedef char AnmVmColorAt1F0[(offsetof(AnmVmView, color1) == 0x1F0) ? 1 : -1];
typedef char AnmVmFlagsAt1F8[(offsetof(AnmVmView, flags) == 0x1F8) ? 1 : -1];
typedef char AnmVmInterruptAt1FE[(offsetof(AnmVmView, pendingInterrupt) == 0x1FE) ? 1 : -1];
typedef char AnmVmSpriteAt218[(offsetof(AnmVmView, baseSpriteIndex) == 0x218) ? 1 : -1];

struct TitleAnmView {
    void ExecuteAnmIdxArray(AnmVmView *vms, i32 start, i32 count);
    void SetSprite(AnmVmView *vm, i32 spriteIndex);
};

struct TitleAnmManagerView {
    int LoadSurface(i32 slot, const char *path);
    void SetInterruptArray(AnmVmView *vms, i32 count, i32 interrupt);
    void ExecuteScriptArray(AnmVmView *vms, i32 count);
};

struct TitleSideInputView {
    u32 currentInput;
    u16 pressedInput;
    u16 repeatInput;

    u16 IsPressedScrolling(u16 mask);
};

struct TitleSelectionRandomView {
    i32 NextU32();
};

struct TitleSupervisorView {
    void PlayMusic(i32 track, i32 unused);
    int PrepareResultScreen();
};

struct TitleMidiOutputView {
    void PlayFile(i32 track);
};

struct TitleScoreRecordView {
    u8 unknown00[0x0C];
    u32 score;
    u8 unknown10[4];
    u8 category0;
    u8 category1;
    u8 rank;
    u8 unknown17;
    char name[9];
    u8 unknown21[0x0B];

    int InsertIntoTable();
};

typedef char TitleScoreRecordSizeIs2C[(sizeof(TitleScoreRecordView) == 0x2C) ? 1 : -1];
typedef char TitleScoreRecordScoreAt0C[(offsetof(TitleScoreRecordView, score) == 0x0C) ? 1 : -1];
typedef char TitleScoreRecordCategory0At14[(offsetof(TitleScoreRecordView, category0) == 0x14) ? 1 : -1];
typedef char TitleScoreRecordCategory1At15[(offsetof(TitleScoreRecordView, category1) == 0x15) ? 1 : -1];
typedef char TitleScoreRecordRankAt16[(offsetof(TitleScoreRecordView, rank) == 0x16) ? 1 : -1];
typedef char TitleScoreRecordNameAt18[(offsetof(TitleScoreRecordView, name) == 0x18) ? 1 : -1];

struct TitleConfigSnapshotView {
    u32 words[0x33];
};

typedef char TitleConfigSnapshotSizeIsCC[(sizeof(TitleConfigSnapshotView) == 0xCC) ? 1 : -1];

extern void __cdecl DrawTitleHelpText(TitleAnmManagerView *manager, AnmVmView *vm,
                                      u32 color, i32 width, const char *text);

struct TitleScreenView {
    i32 keyboardSelection;                    // +0x00000
    i32 replayNameCursor;                     // +0x00004
    i32 nameBankIndex;                        // +0x00008
    i32 nameSlotIndex;                        // +0x0000C
    i32 side0CharacterCursor;                 // +0x00010
    i32 side1CharacterCursor;                 // +0x00014
    i32 side0CharacterConfirmed;              // +0x00018
    i32 side1CharacterConfirmed;              // +0x0001C
    u8 unknown00020[4];
    i32 previousCursor;                       // +0x00024
    union { i32 replaySaveState; i32 currentScreenState; }; // +0x00028
    i32 stateTimer;                           // +0x0002C
    u8 unknown00030[0x54];
    i32 previousScreen;                       // +0x00084
    u8 unknown00088[4];
    char replayName[9];                       // +0x0008C
    u8 unknown00095[0x1F7];
    char replayPaths[25][0x200];              // +0x0028C
    u8 unknown0348C[0x3458];
    ReplayDataView replays[50];               // +0x068E4
    u8 unknown0C8FC[0x0C];
    i32 selectedReplay;                       // +0x0C908
    i32 unknown0C90C;
    union { i32 replayUiFrameCounter; i32 screenFrameCounter; }; // +0x0C910
    u8 unknown0C914[4];
    i32 unknown0C918;                        // +0x0C918
    u8 unknown0C91C[4];
    i32 optionResetCounter;                   // +0x0C920
    u8 unknown0C924[0x5260];
    i32 side0CharacterSetting;                // +0x11B84
    i32 side1CharacterSetting;                // +0x11B88
    u8 unknown11B8C[4];
    TitleAnmView *titleAnm;                   // +0x11B90
    u8 unknown11B94[4];
    union { i32 uiVm; AnmVmView *vms; };      // +0x11B98
    union { i32 uiAux; AnmVmView *currentHelpTextVm; }; // +0x11B9C
    AnmVmView embeddedVms[57];                // +0x11BA0
    union { i32 uiScript; i32 vmCount; };     // +0x1B224
    i32 currentScreen;                        // +0x1B228
    i32 registrationContext;                  // +0x1B22C
    union { i32 phaseTimer; i32 stateTimer2; }; // +0x1B230
    i32 idleFrames;                           // +0x1B234
    i32 chainState;                           // +0x1B238
    union { i32 resultAction; i32 resumeState; }; // +0x1B23C
    union { i32 transitionId; i32 menuVmStart; }; // +0x1B240
    union { i32 transitionMode; i32 menuItemCount; }; // +0x1B244
    u8 unknown1B248[0x74];
    TitleConfigSnapshotView configSnapshot;   // +0x1B2BC

    int UpdateReplaySave();
    int OnUpdateStartMenu();
    int OnUpdateDifficultySelect();
    int OnUpdateModeSelect();
    int PlayMenuSound(i32 soundId, i32 unused);
    int ChangeCurrentScreen(i32 screen);
    int MoveTwoChoiceCursor(i32 count);
    int MoveCursorVertical(i32 count);
    int UpdateMenuSelection();
    int SetMenuSelectionSprites(i32 selected, i32 start, i32 count);
    int OnUpdateOptions();
    int OnUpdateCharacterSelect();
    int OnUpdateResultNameEntry();
    int MoveCharacterCursor(i32 side, i32 direction, char *order, i32 count);
    int MoveCharacterCursorHorizontal(i32 side, i32 count);
    int SetCharacterCursorActive(i32 selectedCharacter, i32 start, i32 count, i32 stride);
    int SetCharacterCursorInactive(i32 selectedCharacter, i32 start, i32 count, i32 stride);
    int UpdateCharacterSettings(i32 side0Setting, i32 side1Setting);
    int UpdateCharacterSelectionVisuals(i32 side, i32 selectedCharacter, i32 otherCharacter, char *order);
    int UpdateScreen8Mode0();
    int UpdateScreen8Mode123();
    int UpdateScreen16();
};

typedef char TitleSide0CursorAt10[(offsetof(TitleScreenView, side0CharacterCursor) == 0x10) ? 1 : -1];
typedef char TitleSide1CursorAt14[(offsetof(TitleScreenView, side1CharacterCursor) == 0x14) ? 1 : -1];
typedef char TitleSide0ConfirmedAt18[(offsetof(TitleScreenView, side0CharacterConfirmed) == 0x18) ? 1 : -1];
typedef char TitleSide1ConfirmedAt1C[(offsetof(TitleScreenView, side1CharacterConfirmed) == 0x1C) ? 1 : -1];
typedef char TitleStateAt28[(offsetof(TitleScreenView, replaySaveState) == 0x28) ? 1 : -1];
typedef char TitleReplayNameAt8C[(offsetof(TitleScreenView, replayName) == 0x8C) ? 1 : -1];
typedef char TitleReplayPathsAt28C[(offsetof(TitleScreenView, replayPaths) == 0x28C) ? 1 : -1];
typedef char TitleReplaysAt68E4[(offsetof(TitleScreenView, replays) == 0x68E4) ? 1 : -1];
typedef char TitleSelectedReplayAtC908[(offsetof(TitleScreenView, selectedReplay) == 0xC908) ? 1 : -1];
typedef char TitleUnknownC918AtC918[(offsetof(TitleScreenView, unknown0C918) == 0xC918) ? 1 : -1];
typedef char TitleUiVmAt11B98[(offsetof(TitleScreenView, uiVm) == 0x11B98) ? 1 : -1];
typedef char TitleUiScriptAt1B224[(offsetof(TitleScreenView, uiScript) == 0x1B224) ? 1 : -1];
typedef char TitlePhaseAt1B230[(offsetof(TitleScreenView, phaseTimer) == 0x1B230) ? 1 : -1];
typedef char TitlePrevCursorAt24[(offsetof(TitleScreenView, previousCursor) == 0x24) ? 1 : -1];
typedef char TitlePrevScreenAt84[(offsetof(TitleScreenView, previousScreen) == 0x84) ? 1 : -1];
typedef char TitleSide0SettingAt11B84[(offsetof(TitleScreenView, side0CharacterSetting) == 0x11B84) ? 1 : -1];
typedef char TitleSide1SettingAt11B88[(offsetof(TitleScreenView, side1CharacterSetting) == 0x11B88) ? 1 : -1];
typedef char TitleAnmAt11B90[(offsetof(TitleScreenView, titleAnm) == 0x11B90) ? 1 : -1];
typedef char TitleVmsAt11B98[(offsetof(TitleScreenView, vms) == 0x11B98) ? 1 : -1];
typedef char TitleHelpAt11BA0[(offsetof(TitleScreenView, embeddedVms) == 0x11BA0) ? 1 : -1];
typedef char TitleVmCountAt1B224[(offsetof(TitleScreenView, vmCount) == 0x1B224) ? 1 : -1];
typedef char TitleCurrentScreenAt1B228[(offsetof(TitleScreenView, currentScreen) == 0x1B228) ? 1 : -1];
typedef char TitleIdleAt1B234[(offsetof(TitleScreenView, idleFrames) == 0x1B234) ? 1 : -1];
typedef char TitleMenuStartAt1B240[(offsetof(TitleScreenView, menuVmStart) == 0x1B240) ? 1 : -1];
typedef char TitleMenuCountAt1B244[(offsetof(TitleScreenView, menuItemCount) == 0x1B244) ? 1 : -1];
typedef char TitleConfigAt1B2BC[(offsetof(TitleScreenView, configSnapshot) == 0x1B2BC) ? 1 : -1];
typedef char TitleScreenSizeIs1B388[(sizeof(TitleScreenView) == 0x1B388) ? 1 : -1];

extern InputView g_TitleInput;
extern u32 g_TitleInputFlags;
extern SoundPlayerView g_SoundPlayer;
extern ReplayManagerView *g_ReplayManager;
extern char *g_TitleAlphabet;
extern char g_ReplayName[];
extern TitleNameRecordView g_TitleNameTable[][5][5];
extern i32 g_TitleNameTableIndex;
extern TitleScoreRecordView g_TitleScoreRecord;
extern TitleScoreRecordView g_TitleScoreTable[16][5][5];
extern i32 g_GameMode;
extern u32 g_TitleGameFlags;
extern void **g_OptionPointers;
extern TitleAnmManagerView *g_TitleAnmManager;
extern TitleSupervisorView g_TitleSupervisor;
extern TitleMidiOutputView *g_TitleMidiOutput;
extern u8 g_TitleLockedMenuItem;
extern i32 g_TitleModeSelection;
extern u8 g_TitleDifficulty;
extern const char *g_DemoReplayPaths[3];
extern u8 g_DemoReplayIndex;
extern char g_SelectedReplayPath[];
extern i32 g_GameCurrentStage;
extern i32 g_GameConfigSelector;
extern i32 g_GameValueDB8;
extern i32 g_GameValueDF0;
extern i32 g_TitleGlobalMode;
extern i32 g_TitleTransitionCounter0;
extern i32 g_TitleTransitionCounter1;
extern u8 g_OptionA;
extern u8 g_OptionB;
extern u8 g_OptionC;
extern u8 g_OptionD;
extern const char *g_TitleHelpText[8];
extern TitleConfigSnapshotView g_TitleConfigSnapshot;
extern i32 g_GameSide0Value20;
extern i32 g_GameSide1Value20;
extern i32 g_GameSide0Value30;
extern i32 g_GameSide1Value30;
extern i32 g_GameSide0CharacterSetting;
extern i32 g_GameSide1CharacterSetting;
extern i32 g_TitleCharacterReturnSelection;
extern char g_TitleCharacterOrder[16];
extern u8 g_TitleCharacterUnlocked[16];
extern TitleSideInputView g_TitleSide0Input;
extern TitleSideInputView g_TitleSide1Input;
extern TitleSelectionRandomView g_TitleSelectionRandom;
extern void __fastcall PrepareTitleMode4Network(void *optionState);
extern void __fastcall ResetTitleMode4Supervisor(TitleSupervisorView *supervisor);
extern int SaveTitleScoreData();


int TitleScoreRecordView::InsertIntoTable()
{
    i32 rankIndex;
    for (rankIndex = 0; rankIndex < 5; rankIndex++)
    {
        if (score >= g_TitleScoreTable[category0][category1][rankIndex].score)
            break;
    }

    if (rankIndex < 5)
    {
        for (i32 index = 4; index > rankIndex; index--)
        {
            g_TitleScoreTable[category0][category1][index] =
                g_TitleScoreTable[category0][category1][index - 1];
            g_TitleScoreTable[category0][category1][index].rank = (u8)index;
        }

        g_TitleScoreTable[category0][category1][rankIndex] = *this;
        g_TitleScoreTable[category0][category1][rankIndex].rank = (u8)rankIndex;
        return rankIndex;
    }

    return 99;
}


int TitleScreenView::OnUpdateCharacterSelect()
{
    i32 value;

    switch (currentScreenState)
    {
    case 0:
    {
        if (stateTimer2 == 0)
        {
            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 13);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);

            {
                i32 visibleCount = 0;
                for (value = 0; value < 16; value++)
                {
                    i32 character = g_TitleCharacterOrder[value];
                    if (g_TitleCharacterUnlocked[character] || g_OptionPointers[42])
                    {
                        titleAnm->SetSprite(&vms[191 + visibleCount], character + 235);
                        vms[191 + visibleCount].flags |= 2;
                        visibleCount++;
                    }
                }
                for (value = visibleCount; value < 16; value++)
                    vms[191 + value].flags &= ~2u;
            }

            vms[189].flags |= 2;
            vms[190].flags |= 2;

            side0CharacterCursor = 0;
            while (g_TitleCharacterOrder[side0CharacterCursor] != g_GameSide0Value20)
                side0CharacterCursor++;
            side1CharacterCursor = 0;
            while (g_TitleCharacterOrder[side1CharacterCursor] != g_GameSide1Value20)
                side1CharacterCursor++;

            MoveCharacterCursor(0, 1, g_TitleCharacterOrder, 16);
            MoveCharacterCursor(1, 1, g_TitleCharacterOrder, 16);

            char side1Character = g_TitleCharacterOrder[side1CharacterCursor];
            value = g_TitleCharacterOrder[side0CharacterCursor];
            SetCharacterCursorActive(value, 92, 16, 2);
            SetCharacterCursorActive(g_TitleCharacterOrder[side1CharacterCursor], 93, 16, 2);
            SetCharacterCursorActive(g_TitleCharacterOrder[side0CharacterCursor], 60, 16, 2);
            SetCharacterCursorActive(g_TitleCharacterOrder[side1CharacterCursor], 61, 16, 2);

            unknown0C918 = 1;
            side1CharacterConfirmed = 0;
            side0CharacterConfirmed = 0;
            currentScreenState = 0;
            stateTimer = 0;
            uiAux = 0;
            side0CharacterSetting = g_GameSide0CharacterSetting;
            side1CharacterSetting = g_GameSide1CharacterSetting;
            UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
            UpdateCharacterSelectionVisuals(0, value, side1Character, g_TitleCharacterOrder);
            UpdateCharacterSelectionVisuals(1, side1Character, value, g_TitleCharacterOrder);
        }

        if (stateTimer2 == 8)
            currentScreenState = 1;
        break;
    }

    case 1:
    {
        char side0Character;
        char side1Character;

        if (g_TitleModeSelection == 4 && g_OptionPointers[42] == 0)
        {
            PlayMenuSound(11, 0);
            stateTimer = 0;
            if (currentScreen == 3)
                ChangeCurrentScreen(2);
            else if (currentScreen == 5)
                ChangeCurrentScreen(4);
            else
                ChangeCurrentScreen(7);
            return 1;
        }

        if (side0CharacterConfirmed == 0)
        {
            if ((g_TitleSide0Input.currentInput & 4) == 0)
            {
                value = MoveCharacterCursorHorizontal(0, 16);
                if (value != 0)
                {
                    MoveCharacterCursor(0, value, g_TitleCharacterOrder, 16);
                    side0Character = g_TitleCharacterOrder[side0CharacterCursor];
                    side1Character = g_TitleCharacterOrder[side1CharacterCursor];
                    if (value > 0)
                    {
                        SetCharacterCursorActive(side0Character, 92, 16, 2);
                        SetCharacterCursorActive(side0Character, 60, 16, 2);
                    }
                    else
                    {
                        SetCharacterCursorInactive(side0Character, 92, 16, 2);
                        SetCharacterCursorInactive(side0Character, 60, 16, 2);
                    }
                    UpdateCharacterSelectionVisuals(0, side0Character, side1Character, g_TitleCharacterOrder);
                }
            }
            else
            {
                if (g_TitleSide0Input.IsPressedScrolling(64))
                {
                    if (side0CharacterSetting > 1)
                        side0CharacterSetting--;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
                if (g_TitleSide0Input.IsPressedScrolling(128))
                {
                    if (side0CharacterSetting < 10)
                        side0CharacterSetting++;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
            }

            if (g_TitleSide0Input.currentInput & 0x100)
            {
                side0CharacterCursor += g_TitleSelectionRandom.NextU32() & 0xF;
                if (side0CharacterCursor >= 16)
                    side0CharacterCursor -= 16;
                MoveCharacterCursor(0, 1, g_TitleCharacterOrder, 16);
                side0Character = g_TitleCharacterOrder[side0CharacterCursor];
                side1Character = g_TitleCharacterOrder[side1CharacterCursor];
                SetCharacterCursorActive(side0Character, 92, 16, 2);
                SetCharacterCursorActive(side0Character, 60, 16, 2);
                UpdateCharacterSelectionVisuals(0, side0Character, side1Character, g_TitleCharacterOrder);
            }
        }

        if (side1CharacterConfirmed == 0)
        {
            if ((g_TitleSide1Input.currentInput & 4) == 0)
            {
                value = MoveCharacterCursorHorizontal(1, 16);
                if (value != 0)
                {
                    MoveCharacterCursor(1, value, g_TitleCharacterOrder, 16);
                    side0Character = g_TitleCharacterOrder[side0CharacterCursor];
                    side1Character = g_TitleCharacterOrder[side1CharacterCursor];
                    if (value < 0)
                    {
                        SetCharacterCursorActive(side1Character, 93, 16, 2);
                        SetCharacterCursorActive(side1Character, 61, 16, 2);
                    }
                    else
                    {
                        SetCharacterCursorInactive(side1Character, 93, 16, 2);
                        SetCharacterCursorInactive(side1Character, 61, 16, 2);
                    }
                    UpdateCharacterSelectionVisuals(1, side1Character, side0Character, g_TitleCharacterOrder);
                }
            }
            else
            {
                if (g_TitleSide1Input.IsPressedScrolling(64))
                {
                    if (side1CharacterSetting > 1)
                        side1CharacterSetting--;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
                if (g_TitleSide1Input.IsPressedScrolling(128))
                {
                    if (side1CharacterSetting < 10)
                        side1CharacterSetting++;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
            }

            if (g_TitleSide1Input.currentInput & 0x100)
            {
                side1CharacterCursor += g_TitleSelectionRandom.NextU32() & 0xF;
                if (side1CharacterCursor >= 16)
                    side1CharacterCursor -= 16;
                MoveCharacterCursor(1, 1, g_TitleCharacterOrder, 16);
                side0Character = g_TitleCharacterOrder[side0CharacterCursor];
                value = g_TitleCharacterOrder[side1CharacterCursor];
                SetCharacterCursorActive(value, 93, 16, 2);
                SetCharacterCursorActive(value, 61, 16, 2);
                UpdateCharacterSelectionVisuals(1, value, side0Character, g_TitleCharacterOrder);
            }
        }

        side0Character = g_TitleCharacterOrder[side0CharacterCursor];
        side1Character = g_TitleCharacterOrder[side1CharacterCursor];

        if ((g_TitleSide0Input.repeatInput & 0x1001) && side0CharacterConfirmed == 0)
        {
            PlayMenuSound(10, 0);
            side0CharacterConfirmed = 1;
            vms[92 + side0Character * 2].color1Bytes[2] = 0x80;
            vms[92 + side0Character * 2].color1Bytes[1] = 0x80;
            vms[92 + side0Character * 2].color1Bytes[0] = 0x80;
            vms[60 + side0Character * 2].color1Bytes[2] = 0x80;
            vms[60 + side0Character * 2].color1Bytes[1] = 0x80;
            vms[60 + side0Character * 2].color1Bytes[0] = 0x80;
            g_GameSide0Value30 = (g_TitleSide0Input.currentInput >> 2) & 1;
            if (side1CharacterConfirmed && side0Character == side1Character)
                g_GameSide0Value30 = 1 - g_GameSide1Value30;
        }

        if ((g_TitleSide1Input.repeatInput & 0x1001) && side1CharacterConfirmed == 0)
        {
            PlayMenuSound(10, 0);
            side1CharacterConfirmed = 1;
            vms[93 + side1Character * 2].color1Bytes[2] = 0x80;
            vms[93 + side1Character * 2].color1Bytes[1] = 0x80;
            vms[93 + side1Character * 2].color1Bytes[0] = 0x80;
            vms[61 + side1Character * 2].color1Bytes[2] = 0x80;
            vms[61 + side1Character * 2].color1Bytes[1] = 0x80;
            vms[61 + side1Character * 2].color1Bytes[0] = 0x80;
            g_GameSide1Value30 = (g_TitleSide1Input.currentInput >> 2) & 1;
            if (side0CharacterConfirmed && side0Character == side1Character)
                g_GameSide1Value30 = 1 - g_GameSide0Value30;
        }

        if (side0CharacterConfirmed && side1CharacterConfirmed)
        {
            g_GameSide0Value20 = side0Character;
            g_GameSide1Value20 = side1Character;
            g_GameSide0CharacterSetting = side0CharacterSetting;
            g_GameSide1CharacterSetting = side1CharacterSetting;
            PlayMenuSound(10, 0);
            ChangeCurrentScreen(16);
            UpdateScreen16();
            return 1;
        }

        if (((g_TitleSide0Input.repeatInput & 0xA) && side0CharacterConfirmed == 0) ||
            ((g_TitleSide1Input.repeatInput & 0xA) && side1CharacterConfirmed == 0))
        {
            g_TitleCharacterReturnSelection = keyboardSelection;
            PlayMenuSound(11, 0);
            stateTimer = 0;
            g_GameSide0Value20 = side0Character;
            g_GameSide1Value20 = side1Character;
            if (currentScreen == 3)
                ChangeCurrentScreen(2);
            else if (currentScreen == 5)
                ChangeCurrentScreen(4);
            else
                ChangeCurrentScreen(7);
            return 1;
        }

        if ((g_TitleSide0Input.repeatInput & 0xA) && side0CharacterConfirmed == 1)
        {
            side0CharacterConfirmed = 0;
            PlayMenuSound(11, 0);
            vms[92 + side0Character * 2].color1Bytes[2] = 0xFF;
            vms[92 + side0Character * 2].color1Bytes[1] = 0xFF;
            vms[92 + side0Character * 2].color1Bytes[0] = 0xFF;
            vms[60 + side0Character * 2].color1Bytes[2] = 0xFF;
            vms[60 + side0Character * 2].color1Bytes[1] = 0xFF;
            vms[60 + side0Character * 2].color1Bytes[0] = 0xFF;
        }

        if ((g_TitleSide1Input.repeatInput & 0xA) && side1CharacterConfirmed == 1)
        {
            side1CharacterConfirmed = 0;
            PlayMenuSound(11, 0);
            vms[93 + side1Character * 2].color1Bytes[2] = 0xFF;
            vms[93 + side1Character * 2].color1Bytes[1] = 0xFF;
            vms[93 + side1Character * 2].color1Bytes[0] = 0xFF;
            vms[61 + side1Character * 2].color1Bytes[2] = 0xFF;
            vms[61 + side1Character * 2].color1Bytes[1] = 0xFF;
            vms[61 + side1Character * 2].color1Bytes[0] = 0xFF;
        }
        break;
    }
    }

    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}

int TitleScreenView::OnUpdateResultNameEntry()
{
    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            g_TitleSupervisor.PrepareResultScreen();
            if (g_TitleAnmManager->LoadSurface(0, "title/result00.png"))
                return 0;

            if (vmCount == 0)
            {
                vmCount = 223;
                vms = new AnmVmView[vmCount];
                titleAnm->ExecuteAnmIdxArray(vms, 0, vmCount);
            }

            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 16);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);

            if (g_GameMode == 2)
            {
                stateTimer = 0;
                ChangeCurrentScreen(15);
                return 1;
            }

            nameBankIndex = g_GameSide0Value20;
            replayNameCursor = 0;
            SetCharacterCursorInactive(nameBankIndex, 60, 16, 2);
            keyboardSelection = 95;
            currentScreenState = 0;
            stateTimer = 0;
            uiAux = 0;
            nameSlotIndex = g_TitleScoreRecord.InsertIntoTable();
            if (nameSlotIndex >= 5)
            {
                keyboardSelection = 95;
            }
            else
            {
                strcpy(
                    g_TitleNameTable[nameBankIndex][g_TitleNameTableIndex][nameSlotIndex].name,
                    g_ReplayName);
            }
        }

        if (stateTimer2 == 8)
            currentScreenState = 1;
        break;

    case 1:
        if (nameSlotIndex < 5)
        {
            if (g_TitleInput.IsPressedScrolling(0x10))
            {
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                keyboardSelection -= 16;
                if (keyboardSelection < 0)
                    keyboardSelection += 96;
                if (keyboardSelection == 93)
                    keyboardSelection = 77;
                stateTimer = 0;
            }

            if (g_TitleInput.IsPressedScrolling(0x20))
            {
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                keyboardSelection += 16;
                if (keyboardSelection >= 96)
                    keyboardSelection -= 96;
                if (keyboardSelection == 93)
                    keyboardSelection = 13;
                stateTimer = 0;
            }

            if (g_TitleInput.IsPressedScrolling(0x40))
            {
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                if (keyboardSelection % 16 == 0)
                    keyboardSelection += 15;
                else
                {
                    keyboardSelection--;
                    if (keyboardSelection == 93)
                        keyboardSelection = 92;
                }
                stateTimer = 0;
            }

            if (g_TitleInput.IsPressedScrolling(0x80))
            {
                g_SoundPlayer.PlaySoundByIdx(12, 0);
                if (keyboardSelection % 16 == 15)
                    keyboardSelection -= 15;
                else
                {
                    keyboardSelection++;
                    if (keyboardSelection == 93)
                        keyboardSelection = 94;
                }
                stateTimer = 0;
            }
        }

        if (g_TitleInputFlags & 0x1001)
        {
            if (keyboardSelection == 95)
            {
                SaveTitleScoreData();
                PlayMenuSound(11, 0);
                stateTimer = 0;
                ChangeCurrentScreen(15);
                if (nameSlotIndex < 5)
                {
                    strcpy(
                        g_ReplayName,
                        g_TitleNameTable[nameBankIndex][g_TitleNameTableIndex][nameSlotIndex].name);
                }
                return 1;
            }

            char value;
            if (keyboardSelection == 94)
                value = ' ';
            else
                value = g_TitleAlphabet[keyboardSelection];
            g_TitleNameTable[nameBankIndex][g_TitleNameTableIndex][nameSlotIndex].name[replayNameCursor] = value;
            PlayMenuSound(10, 0);
            if (replayNameCursor < 7)
                replayNameCursor++;
            else
                keyboardSelection = 95;
            stateTimer = 0;
        }

        if (g_TitleInputFlags & 0xA)
        {
            PlayMenuSound(11, 0);
            if (replayNameCursor == 7 &&
                g_TitleNameTable[nameBankIndex][g_TitleNameTableIndex][nameSlotIndex].name[7] != ' ')
            {
                g_TitleNameTable[nameBankIndex][g_TitleNameTableIndex][nameSlotIndex].name[7] = ' ';
            }
            else if (replayNameCursor > 0)
            {
                replayNameCursor--;
                g_TitleNameTable[nameBankIndex][g_TitleNameTableIndex][nameSlotIndex].name[replayNameCursor] = ' ';
            }
            stateTimer = 0;
        }
        break;
    }

    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}


int TitleScreenView::UpdateReplaySave()
{
    char path[256];
    char source[64];
    i32 fileSize;
    ReplayDataView *loadedReplay;
    char *replayPath;
    i32 nextIndex;
    ReplayDataView *replayCursor;
    u32 inputFlags;

    switch (replaySaveState)
    {
    case 0:
        if (phaseTimer == 0)
        {
            if (g_TitleGameFlags & 0x2000)
            {
                stateTimer = 0;
                ChangeCurrentScreen(1);
                keyboardSelection = 0;
                return 1;
            }

            if (g_OptionPointers[42] != 0)
            {
                i32 replayIndex = 0;
                do
                {
                    nextIndex = replayIndex + 1;
                    sprintf(path, "./replay/th9_udn%.3d.rpy", replayIndex + 1);
                    if (!FileSystemView::CheckIfFileAlreadyExists(path))
                        break;
                    replayIndex = nextIndex;
                }
                while (nextIndex < 1000);

                if (replayIndex >= 1000)
                    replayIndex = 999;

                sprintf(path, "./replay/th9_udn%.3d.rpy", replayIndex + 1);
                ReplayManagerView::SaveReplay(g_ReplayManager, path, g_ReplayName);
                stateTimer = 0;
                ChangeCurrentScreen(1);
                keyboardSelection = 0;
                resultAction = 2;
                return 1;
            }

            if (g_TitleAnmManager->LoadSurface(0, "title/replay00.png"))
                return 0;

            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 19);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);
            keyboardSelection = 0;
            strcpy(replayName, g_ReplayName);
            replaySaveState = 0;
            stateTimer = 0;
            uiAux = 0;

            if (g_GameMode == 2)
                goto load_replays;

            transitionId = 125;
            transitionMode = 2;
            UpdateMenuSelection();
            _mkdir("replay");
        }

        if (phaseTimer == 8)
            replaySaveState = 1;
        break;

    case 1:
        if (MoveTwoChoiceCursor(2))
            UpdateMenuSelection();

        if (g_TitleInputFlags & 0xA)
        {
            if (keyboardSelection == 0)
            {
                PlayMenuSound(11, 0);
                keyboardSelection = 1;
                UpdateMenuSelection();
                break;
            }

        cancel_replay_save:
            PlayMenuSound(11, 0);
            stateTimer = 0;
            ChangeCurrentScreen(1);
            keyboardSelection = 0;
            strcpy(
                g_ReplayName,
                g_TitleNameTable[nameBankIndex][g_TitleNameTableIndex][nameSlotIndex].name);
            return 1;
        }

        if (g_TitleInputFlags & 0x1001)
        {
            if (keyboardSelection == 1)
                goto cancel_replay_save;

        load_replays:
            memset(replays, 0, 0x6018);
            replayCursor = replays;
            replayPath = replayPaths[0];
            for (i32 replayIndex = 0; replayIndex < 25; replayIndex++)
            {
                sprintf(source, "./replay/th9_%.2d.rpy", replayIndex + 1);
                void *fileData = FileSystemView::OpenFile(source, &fileSize, 1);
                if (fileData != 0)
                {
                    loadedReplay = ReplayManagerView::LoadReplayData((ReplayDataView *)fileData, fileSize);
                    if (loadedReplay != 0)
                    {
                        *replayCursor = *loadedReplay;
                        strcpy(replayPath, source);
                        free(loadedReplay);
                    }
                }
                replayCursor++;
                replayPath += 0x200;
            }
            replaySaveState = 2;
            phaseTimer = 0;
            keyboardSelection = 0;
            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 22);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);
        }
        break;

    case 2:
        if (phaseTimer < 6)
            break;

        if (MoveCursorVertical(25))
            UpdateMenuSelection();

        if (g_TitleInputFlags & 0xA)
        {
            PlayMenuSound(11, 0);
            keyboardSelection = 0;
            phaseTimer = 0;
            replaySaveState = 1;
            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 19);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);
            transitionId = 125;
            transitionMode = 2;
            UpdateMenuSelection();
            break;
        }

        if (g_TitleInputFlags & 0x1001)
        {
            PlayMenuSound(10, 0);
            selectedReplay = keyboardSelection;
            keyboardSelection = 0;
            if (replays[selectedReplay].magic != 0)
            {
                replaySaveState = 3;
                g_TitleAnmManager->SetInterruptArray(vms, vmCount, 21);
                g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);
                transitionId = 129;
                transitionMode = 2;
                UpdateMenuSelection();
            }
            else
            {
                keyboardSelection = 95;
                replaySaveState = 4;
                g_TitleAnmManager->SetInterruptArray(vms, vmCount, 23);
                g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);
            }
            phaseTimer = 0;
            strcpy(replayName, g_ReplayName);
        }
        break;

    case 3:
        if (phaseTimer < 10)
            break;

        if (MoveTwoChoiceCursor(2))
            UpdateMenuSelection();

        inputFlags = g_TitleInputFlags;
        if (!(inputFlags & 0xA))
        {
            if (!(inputFlags & 0x1001))
                break;
            if (keyboardSelection != 1)
            {
                PlayMenuSound(10, 0);
                keyboardSelection = 95;
                replaySaveState = 4;
                g_TitleAnmManager->SetInterruptArray(vms, vmCount, 23);
                g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);
                break;
            }
        }

        PlayMenuSound(11, 0);
        replaySaveState = 2;
        keyboardSelection = 0;
        g_TitleAnmManager->SetInterruptArray(vms, vmCount, 22);
        g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);
        break;

    case 4:
        if (phaseTimer < 10)
            break;

        if (g_TitleInput.IsPressedScrolling(0x10))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            keyboardSelection -= 16;
            if (keyboardSelection < 0)
                keyboardSelection += 96;
            if (keyboardSelection == 93)
                keyboardSelection = 77;
            stateTimer = 0;
        }

        if (g_TitleInput.IsPressedScrolling(0x20))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            keyboardSelection += 16;
            if (keyboardSelection >= 96)
                keyboardSelection -= 96;
            if (keyboardSelection == 93)
                keyboardSelection = 13;
            stateTimer = 0;
        }

        if (g_TitleInput.IsPressedScrolling(0x40))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            if (keyboardSelection % 16 == 0)
                keyboardSelection += 15;
            else
            {
                keyboardSelection--;
                if (keyboardSelection == 93)
                    keyboardSelection = 92;
            }
            stateTimer = 0;
        }

        if (g_TitleInput.IsPressedScrolling(0x80))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            if (keyboardSelection % 16 == 15)
                keyboardSelection -= 15;
            else
            {
                keyboardSelection++;
                if (keyboardSelection == 93)
                    keyboardSelection = 94;
            }
            stateTimer = 0;
        }

        if (g_TitleInputFlags & 0x1001)
        {
            if (keyboardSelection == 95)
            {
                PlayMenuSound(10, 0);
                sprintf(path, "replay/th9_%.2d.rpy", selectedReplay + 1);
                ReplayManagerView::SaveReplay(g_ReplayManager, path, replayName);
                stateTimer = 0;
                ChangeCurrentScreen(1);
                keyboardSelection = 0;
                strcpy(g_ReplayName, replayName);
                return 1;
            }

            char value;
            if (keyboardSelection == 94)
                value = ' ';
            else
                value = g_TitleAlphabet[keyboardSelection];
            replayName[replayNameCursor] = value;
            PlayMenuSound(10, 0);
            if (replayNameCursor < 7)
                replayNameCursor++;
            else
                keyboardSelection = 95;
            stateTimer = 0;
        }

        if (g_TitleInputFlags & 0xA)
        {
            PlayMenuSound(11, 0);
            if (replayNameCursor == 7 && replayName[7] != ' ')
            {
                replayName[7] = ' ';
            }
            else if (replayNameCursor > 0)
            {
                replayNameCursor--;
                replayName[replayNameCursor] = ' ';
            }
            else
            {
                keyboardSelection = 0;
                replaySaveState = 2;
                g_TitleAnmManager->SetInterruptArray(vms, vmCount, 22);
                g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);
                phaseTimer = 0;
            }
            stateTimer = 0;
        }
        break;
    }

    stateTimer++;
    replayUiFrameCounter++;
    phaseTimer++;
    return 1;
}


int TitleScreenView::OnUpdateStartMenu()
{
    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            if (previousScreen == 0 || previousScreen == 14 || previousScreen == 12 || previousScreen == 15)
                g_TitleSupervisor.PlayMusic(0, 0);

            if (previousScreen == 0 || previousScreen == 13 || previousScreen == 14 ||
                previousScreen == 11 || previousScreen == 15 || previousScreen == 12 ||
                previousScreen == 2 || previousScreen == 4 || previousScreen == 7 || previousScreen == 6)
            {
                if (g_TitleAnmManager->LoadSurface(0, "title/title00.png"))
                    return 0;
            }

            g_ReplayManager->Release();
            g_ReplayManager = 0;

            if (vmCount == 0)
            {
                vmCount = 223;
                vms = new AnmVmView[vmCount];
                titleAnm->ExecuteAnmIdxArray(vms, 0, vmCount);
            }

            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 2);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);
            if (!g_TitleLockedMenuItem)
                vms[2].color1 = 0x30908040;

            if (resumeState != 0)
            {
                g_TitleGameFlags &= ~8u;
                keyboardSelection = g_TitleModeSelection;
                ChangeCurrentScreen(6);
                g_TitleAnmManager->SetInterruptArray(vms, vmCount, 5);
                return 1;
            }

            menuVmStart = 1;
            menuItemCount = 8;
            UpdateMenuSelection();
            if (g_TitleGameFlags & 8)
            {
                ChangeCurrentScreen(11);
                g_TitleAnmManager->SetInterruptArray(vms, vmCount, 5);
                return 1;
            }
            g_TitleGameFlags &= ~2u;
        }

        if ((u32)stateTimer2 < 8)
        {
            DrawTitleHelpText(g_TitleAnmManager, &embeddedVms[stateTimer2], 0xFFF0E0, 0x300000,
                              g_TitleHelpText[stateTimer2]);
            stateTimer2++;
            return 1;
        }

        previousCursor = -1;
        stateTimer2 = 0;
        stateTimer = 0;
        currentScreenState = 1;
        idleFrames = 0;

    case 1:
        {
            i32 moved = MoveCursorVertical(8);
            if (moved != 0)
            {
                if (!g_TitleLockedMenuItem && keyboardSelection == 1)
                    keyboardSelection = moved + 1;
                UpdateMenuSelection();
            }
        }

        if (previousCursor != keyboardSelection)
        {
            currentHelpTextVm = &embeddedVms[keyboardSelection];
            currentHelpTextVm->pendingInterrupt = 1;
        }
        previousCursor = keyboardSelection;

        if (!g_TitleLockedMenuItem)
            vms[2].color1 = 0xB0B0B040;

        if (stateTimer2 >= 10)
        {
            if (g_TitleInput.currentInput != 0)
                idleFrames = 0;

            idleFrames++;
            if (idleFrames > 1500)
            {
                g_TitleGameFlags |= 2u;
                strcpy(g_SelectedReplayPath, g_DemoReplayPaths[g_DemoReplayIndex]);
                g_DemoReplayIndex++;
                g_DemoReplayIndex %= 3;
                g_TitleGameFlags |= 8u;
                g_GameCurrentStage = 9;
                g_GameConfigSelector = 3;
                g_GameValueDB8 = 0;
                g_GameValueDF0 = 1;
                g_TitleGlobalMode = 2;
                g_GameMode = 2;
                return 0;
            }

            if (g_TitleInputFlags & 0x1001)
            {
                switch (keyboardSelection)
                {
                case 0:
                    g_TitleGameFlags &= ~8u;
                    PlayMenuSound(10, 0);
                    if (keyboardSelection >= 4)
                        keyboardSelection = 2;
                    ChangeCurrentScreen(2);
                    g_TitleAnmManager->SetInterruptArray(vms, vmCount, 5);
                    g_TitleTransitionCounter0 = 10;
                    g_TitleTransitionCounter1 = 10;
                    break;
                case 1:
                    if (!g_TitleLockedMenuItem)
                    {
                        PlayMenuSound(39, 0);
                        break;
                    }
                    g_TitleGameFlags &= ~8u;
                    PlayMenuSound(10, 0);
                    if (keyboardSelection >= 4)
                        keyboardSelection = 2;
                    ChangeCurrentScreen(4);
                    g_TitleAnmManager->SetInterruptArray(vms, vmCount, 5);
                    g_TitleTransitionCounter0 = 10;
                    g_TitleTransitionCounter1 = 10;
                    break;
                case 2:
                    g_TitleGameFlags &= ~8u;
                    PlayMenuSound(10, 0);
                    keyboardSelection = g_TitleModeSelection;
                    ChangeCurrentScreen(6);
                    g_TitleAnmManager->SetInterruptArray(vms, vmCount, 5);
                    g_TitleTransitionCounter0 = 10;
                    g_TitleTransitionCounter1 = 10;
                    break;
                case 3:
                    PlayMenuSound(10, 0);
                    ChangeCurrentScreen(11);
                    g_TitleAnmManager->SetInterruptArray(vms, vmCount, 5);
                    currentHelpTextVm->pendingInterrupt = 2;
                    break;
                case 5:
                    PlayMenuSound(10, 0);
                    ChangeCurrentScreen(12);
                    g_TitleAnmManager->SetInterruptArray(vms, vmCount, 5);
                    currentHelpTextVm->pendingInterrupt = 2;
                    break;
                case 4:
                    PlayMenuSound(10, 0);
                    ChangeCurrentScreen(13);
                    g_TitleAnmManager->SetInterruptArray(vms, vmCount, 5);
                    return 1;
                case 6:
                    PlayMenuSound(10, 0);
                    currentScreenState = 0;
                    keyboardSelection = 0;
                    stateTimer2 = 0;
                    stateTimer = 0;
                    OnUpdateOptions();
                    SetMenuSelectionSprites(g_OptionA, 18, 5);
                    SetMenuSelectionSprites(g_OptionB, 21, 2);
                    SetMenuSelectionSprites(g_OptionC, 23, 3);
                    SetMenuSelectionSprites(g_OptionD, 34, 2);
                    currentScreenState = 3;
                    stateTimer = 0;
                    optionResetCounter = 0;
                    keyboardSelection = 0;
                    break;
                case 7:
                    currentScreenState = 2;
                    stateTimer = 0;
                    g_TitleAnmManager->SetInterruptArray(vms, vmCount, 1);
                    if (g_OptionC == 2)
                        g_TitleMidiOutput->PlayFile(30);
                    break;
                }
            }

            if (g_TitleInputFlags & 0xA)
            {
                titleAnm->SetSprite(
                    &vms[keyboardSelection + 1],
                    vms[keyboardSelection + 1].baseSpriteIndex + 1);
                keyboardSelection = 7;
                titleAnm->SetSprite(
                    &vms[keyboardSelection + 1],
                    vms[keyboardSelection + 1].baseSpriteIndex);
                PlayMenuSound(11, 0);
            }
        }
        break;

    case 2:
        if (stateTimer >= 60)
        {
            free(vms);
            vms = 0;
            vmCount = 0;
            stateTimer2 = 0;
            g_TitleGlobalMode = -1;
            return 0;
        }
        break;

    case 3:
        if (stateTimer >= 30)
        {
            ChangeCurrentScreen(9);
            keyboardSelection = 0;
            configSnapshot = g_TitleConfigSnapshot;
            return 1;
        }
        break;
    }

    screenFrameCounter++;
    stateTimer++;
    stateTimer2++;
    return 1;
}


int TitleScreenView::OnUpdateDifficultySelect()
{
    i32 oldScreen;

    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            if (g_TitleAnmManager->LoadSurface(0, "title/select00.png"))
                return 0;

            if (currentScreen == 4)
                g_TitleAnmManager->SetInterruptArray(vms, vmCount, 25);
            else
                g_TitleAnmManager->SetInterruptArray(vms, vmCount, 12);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);

            if (currentScreen == 7)
            {
                vms[131 + g_TitleModeSelection].pendingInterrupt = 9;
                if (resumeState == 1)
                {
                    ChangeCurrentScreen(8);
                    switch (g_TitleModeSelection)
                    {
                    case 0:
                        UpdateScreen8Mode0();
                        break;
                    case 1:
                    case 2:
                    case 3:
                        UpdateScreen8Mode123();
                        break;
                    case 4:
                    default:
                        break;
                    }
                    vms[136 + g_TitleDifficulty].pendingInterrupt = 10;
                    resumeState = 0;
                    return 1;
                }
                if (resumeState == 2)
                {
                    UpdateScreen8Mode0();
                    vms[136 + g_TitleDifficulty].pendingInterrupt = 10;
                    resumeState = 0;
                    ChangeCurrentScreen(8);
                    return 1;
                }
            }

            keyboardSelection = g_TitleDifficulty;
            if (currentScreen == 4)
                keyboardSelection = 4;
            else if (keyboardSelection >= 4)
                keyboardSelection = 1;

            menuVmStart = 136;
            menuItemCount = 4;
            UpdateMenuSelection();
            unknown0C918 = 1;
            currentScreenState = 0;
            stateTimer = 0;
            currentHelpTextVm = 0;
        }
        if (stateTimer2 == 8)
            currentScreenState = 1;
        break;

    case 1:
        if (currentScreen != 4)
        {
            if (MoveCursorVertical(4))
                UpdateMenuSelection();
        }
        else if (g_TitleInputFlags & 0x30)
        {
            PlayMenuSound(12, 0);
        }

        if (g_TitleInputFlags & 0x1001)
        {
            g_TitleDifficulty = (u8)keyboardSelection;
            PlayMenuSound(10, 0);
            if (currentScreen == 7)
            {
                ChangeCurrentScreen(8);
                switch (g_TitleModeSelection)
                {
                case 0:
                case 4:
                    UpdateScreen8Mode0();
                    break;
                case 1:
                case 2:
                case 3:
                    UpdateScreen8Mode123();
                    break;
                }
                vms[136 + g_TitleDifficulty].pendingInterrupt = 10;
                return 1;
            }

            if (currentScreen == 2)
                ChangeCurrentScreen(3);
            else
                ChangeCurrentScreen(5);
            OnUpdateCharacterSelect();
            vms[136 + g_TitleDifficulty].pendingInterrupt = 9;
            return 1;
        }

        if (g_TitleModeSelection == 4 && g_OptionPointers[42] == 0)
        {
            PlayMenuSound(11, 0);
            stateTimer = 0;
            if (currentScreen == 7)
            {
                ChangeCurrentScreen(6);
                return 1;
            }
            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 6);
            currentScreenState = 3;
        }

        if (g_OptionPointers[42] == 0 && (g_TitleInputFlags & 0xA))
        {
            g_TitleDifficulty = (u8)keyboardSelection;
            PlayMenuSound(11, 0);
            stateTimer = 0;
            if (currentScreen == 7)
            {
                ChangeCurrentScreen(6);
                return 1;
            }
            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 6);
            currentScreenState = 3;
        }
        break;

    case 3:
        if (stateTimer >= 20)
        {
            oldScreen = currentScreen;
            ChangeCurrentScreen(1);
            keyboardSelection = oldScreen != 2;
            return 1;
        }
        break;
    }

    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}


int TitleScreenView::OnUpdateModeSelect()
{
    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            if (previousScreen != 7 && g_TitleAnmManager->LoadSurface(0, "title/select00.png"))
                return 0;

            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 11);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);

            if (resumeState != 0)
            {
                ChangeCurrentScreen(7);
                OnUpdateDifficultySelect();
                vms[131 + g_TitleModeSelection].pendingInterrupt = 9;
                return 1;
            }

            keyboardSelection = g_TitleModeSelection;
            menuVmStart = 131;
            menuItemCount = 5;
            UpdateMenuSelection();
            unknown0C918 = 1;
            currentScreenState = 0;
            stateTimer = 0;
            currentHelpTextVm = 0;
        }

        if (stateTimer2 == 8)
            currentScreenState = 1;
        break;

    case 1:
        if (MoveCursorVertical(5))
            UpdateMenuSelection();

        {
            u32 inputFlags = g_TitleInputFlags;
            if (inputFlags & 0x1001)
            {
                g_TitleModeSelection = keyboardSelection;
                if (g_TitleModeSelection == 4)
                {
                    PrepareTitleMode4Network(g_OptionPointers);
                    ResetTitleMode4Supervisor(&g_TitleSupervisor);
                    g_TitleDifficulty = 1;
                    g_GameSide0Value20 = 0;
                    g_GameSide1Value20 = 1;
                }

                PlayMenuSound(10, 0);
                ChangeCurrentScreen(7);
                OnUpdateDifficultySelect();
                vms[131 + g_TitleModeSelection].pendingInterrupt = 9;
                return 1;
            }

            if (inputFlags & 0xA)
            {
                g_TitleModeSelection = keyboardSelection;
                PlayMenuSound(11, 0);
                currentScreenState = 3;
                stateTimer = 0;
                g_TitleAnmManager->SetInterruptArray(vms, vmCount, 6);
            }
        }
        break;

    case 3:
        if (stateTimer >= 20)
        {
            ChangeCurrentScreen(1);
            keyboardSelection = 2;
            return 1;
        }
        break;
    }

    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}
