// Target-bound TH09 TitleScreen reconstruction views. The physical TitleScreen
// owner is target-proved; original identifier spelling, TU partition, and data-definition ownership remain unresolved.

#include "Chain.hpp"
#include "ScreenEffect.hpp"

#include <stddef.h>
#include <windows.h>
#include <d3d8.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

extern "C" int __cdecl sprintf(char *, const char *, ...);
extern "C" char *__cdecl strcpy(char *, const char *);
extern "C" char *__cdecl strcat(char *, const char *);
extern "C" void *__cdecl memset(void *, int, size_t);
extern "C" void __cdecl free(void *);
extern "C" int __cdecl _mkdir(const char *);
extern "C" int __cdecl _chdir(const char *);

struct ReplayFrameDataStartView {
    u8 bytes[0x20];
};

struct ReplayDataView {
    u32 magic;
    u16 formatVersion;
    u8 value06;
    u8 value07;
    u8 unknown008[4];
    i32 fileSize;
    u32 checksum;
    u8 unknown14;
    u8 obfuscationKey;
    u8 unknown16[2];
    i32 compressedSize;
    i32 decompressedSize;
    ReplayFrameDataStartView *frameStart[3][10];
    u8 *fpsStart[10];
    u8 randomPayloadByte;
    u8 value0C1;
    u16 value0C2;
    char playTime[10];
    char playerName[8];
    u8 unknown0D6;
    u8 value0D7;
    u8 unknown0D8[4];
    u8 configSnapshot[0xCC];
    u8 unknown1A8[0x28];
    u32 value1D0;
    u32 value1D4;
    u32 value1D8;
    char exeVersion[6];
    u8 unknown1E2[2];
    u8 value1E4;
    u8 value1E5;
    u8 value1E6;
    u8 value1E7;
    u8 value1E8;
    u8 value1E9;
    u8 unknown1EA[2];
};

typedef char ReplayDataSizeIs1EC[(sizeof(ReplayDataView) == 0x1EC) ? 1 : -1];
typedef char ReplayDataFrame0At20[(offsetof(ReplayDataView, frameStart) == 0x20) ? 1 : -1];
typedef char ReplayDataValueD7AtD7[(offsetof(ReplayDataView, value0D7) == 0xD7) ? 1 : -1];
typedef char ReplayDataValue1E4At1E4[(offsetof(ReplayDataView, value1E4) == 0x1E4) ? 1 : -1];
typedef char ReplayDataValue1E5At1E5[(offsetof(ReplayDataView, value1E5) == 0x1E5) ? 1 : -1];

struct TitleNameRecordView {
    char name[9];
    u8 unknown09[35];
};

typedef char TitleNameRecordSizeIs2C[(sizeof(TitleNameRecordView) == 0x2C) ? 1 : -1];

struct InputView {
    u16 currentInput;
    u16 IsPressedScrolling(i32 mask);
};

struct TitleInputStatePhysicalView {
    u16 currentInput;
    u8 unknown002[0x8C];
    u16 IsPressedScrolling(i32 mask);
};
typedef char TitleInputStatePhysicalViewSize[(sizeof(TitleInputStatePhysicalView) == 0x8E) ? 1 : -1];

struct TitleInputStateTableView {
    TitleInputStatePhysicalView states[3];
};

struct TitleCharacterConfigView {
    u8 unknown000[0x7C];
    i32 optionValues[16][6];

    int GetOptionState(char character, i32 option);
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

struct TitleFloat3View {
    float x;
    float y;
    float z;
};

typedef char TitleFloat3ViewSizeIs0C[(sizeof(TitleFloat3View) == 0x0C) ? 1 : -1];

struct TitleZunTimerView {
    i32 previous;
    float subFrame;
    i32 current;

    void operator=(i32 value)
    {
        current = value;
        subFrame = (float)value;
        previous = -999999;
    }
};

struct TitleColorView {
    u8 b;
    u8 g;
    u8 r;
    u8 a;
};

struct AnmVmView {
    AnmVmView();
    u8 unknown000[0x50];
    TitleZunTimerView interpCurrentTimers[7];
    TitleZunTimerView interpEndTimers[7];
    u8 interpModes[7];
    u8 interpPadding;
    u8 unknown100[0xF0];
    union {
        u32 color1;
        u8 color1Bytes[4];
    };
    u8 unknown1F4[4];
    union {
        u32 flags;
        struct {
            u16 flagsLow;
            u16 flagsHigh;
        };
    };
    u8 unknown1FC[2];
    u16 pendingInterrupt;
    u8 unknown200[0x08];
    TitleFloat3View position;
    short activeSpriteIndex;
    u8 unknown216[2];
    short baseSpriteIndex;
    u8 unknown21A[0x1E];
    TitleFloat3View posInitial;
    TitleFloat3View posFinal;
    u8 unknown250[0x28];
    TitleColorView color1Initial;
    TitleColorView color1Final;
    u8 unknown280[0x18];
    u8 fontWidth;
    u8 fontHeight;
    u8 unknown29A[0x0A];

    char ConfigurePositionInterpolation(i32 duration, char mode,
                                        TitleFloat3View *start, TitleFloat3View *end);
    int ConfigureColorInterpolation(i32 duration, char mode,
                                    u32 startColor, u32 endColor);
};

typedef char AnmVmSizeIs2A4[(sizeof(AnmVmView) == 0x2A4) ? 1 : -1];
typedef char AnmVmColorAt1F0[(offsetof(AnmVmView, color1) == 0x1F0) ? 1 : -1];
typedef char AnmVmFlagsAt1F8[(offsetof(AnmVmView, flags) == 0x1F8) ? 1 : -1];
typedef char AnmVmInterruptAt1FE[(offsetof(AnmVmView, pendingInterrupt) == 0x1FE) ? 1 : -1];
typedef char AnmVmPositionAt208[(offsetof(AnmVmView, position) == 0x208) ? 1 : -1];
typedef char AnmVmActiveSpriteAt214[(offsetof(AnmVmView, activeSpriteIndex) == 0x214) ? 1 : -1];
typedef char AnmVmSpriteAt218[(offsetof(AnmVmView, baseSpriteIndex) == 0x218) ? 1 : -1];
typedef char AnmVmFontWidthAt298[(offsetof(AnmVmView, fontWidth) == 0x298) ? 1 : -1];
typedef char AnmVmInterpCurrentAt050[(offsetof(AnmVmView, interpCurrentTimers) == 0x050) ? 1 : -1];
typedef char AnmVmInterpEndAt0A4[(offsetof(AnmVmView, interpEndTimers) == 0x0A4) ? 1 : -1];
typedef char AnmVmInterpModeAt0F8[(offsetof(AnmVmView, interpModes) == 0x0F8) ? 1 : -1];
typedef char AnmVmPosInitialAt238[(offsetof(AnmVmView, posInitial) == 0x238) ? 1 : -1];
typedef char AnmVmPosFinalAt244[(offsetof(AnmVmView, posFinal) == 0x244) ? 1 : -1];
typedef char AnmVmColorInitialAt278[(offsetof(AnmVmView, color1Initial) == 0x278) ? 1 : -1];
typedef char AnmVmColorFinalAt27C[(offsetof(AnmVmView, color1Final) == 0x27C) ? 1 : -1];
typedef char AnmVmFontHeightAt299[(offsetof(AnmVmView, fontHeight) == 0x299) ? 1 : -1];

struct TitleAnmView {
    void ExecuteAnmIdx(AnmVmView *vm, i32 scriptIndex);
    void ExecuteAnmIdxArray(AnmVmView *vms, i32 start, i32 count);
    int SetSprite(AnmVmView *vm, i32 spriteIndex);
};

struct TitleAnmManagerView {
    u8 unknown000[8];
    i32 captureSurfaceIndex;

    TitleAnmView *PreloadAnm(i32 slot, const char *path);
    int PreloadSurface(i32 slot, const char *path);
    int LoadSurface(i32 slot, const char *path);
    void SetInterruptArray(AnmVmView *vms, i32 count, i32 interrupt);
    void ExecuteScriptArray(AnmVmView *vms, i32 count);
    int ExecuteScript(AnmVmView *vm);
    void ReleaseAnm(i32 slot);
    void ReleaseSurface(i32 slot);
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
    u8 unknown000[8];
    IDirect3DDevice8 *d3dDevice;               // +0x008
    u8 unknown00C[0x588];
    i32 transitionState594;                    // +0x594
    u8 unknown598[0x20];
    float frameRateMultiplier;                 // +0x5B8
    u8 unknown5BC[0x10];
    TitleAnmView *textAnm;                     // +0x5CC
    u8 unknown5D0[0xDC];
    i32 runningSubthreadHandle;                // +0x6AC
    u8 unknown6B0[4];
    i32 subthreadCloseRequestActive;           // +0x6B4
    i32 subthreadActive;                       // +0x6B8
    u8 unknown6BC[0x84];
    i32 loadingVmsHaveBeenSetup;               // +0x740
    u8 unknown744[0x38];
    i32 recordingFpsWarning77C;                 // +0x77C
    i32 playbackFpsWarning780;                  // +0x780
    i32 recordingFpsState784;                   // +0x784
    i32 recordingFpsState788;                   // +0x788
    i32 recordingFpsState78C;                   // +0x78C
    i32 recordingFpsState790;                   // +0x790

    int LoadMusic(i32 trackId);
    void PlayMusic(i32 track, i32 unused);
    int StopAudio();
    int ClearRecordingFpsWarningState();
    void SetupLoadingVms(TitleFloat3View *position);
    void SetupLoadingVmsAndInitCapture(TitleFloat3View *position);
    void StartEffect(i32 index);
    int ThreadStart(LPTHREAD_START_ROUTINE startFunction, void *startParam);
    void HideLoadingVms();
};

typedef char TitleSupervisorD3dAt008[(offsetof(TitleSupervisorView, d3dDevice) == 0x008) ? 1 : -1];
typedef char TitleSupervisorStateAt594[(offsetof(TitleSupervisorView, transitionState594) == 0x594) ? 1 : -1];
typedef char TitleSupervisorRateAt5B8[(offsetof(TitleSupervisorView, frameRateMultiplier) == 0x5B8) ? 1 : -1];
typedef char TitleSupervisorTextAnmAt5CC[(offsetof(TitleSupervisorView, textAnm) == 0x5CC) ? 1 : -1];
typedef char TitleSupervisorThreadAt6AC[(offsetof(TitleSupervisorView, runningSubthreadHandle) == 0x6AC) ? 1 : -1];
typedef char TitleSupervisorCloseAt6B4[(offsetof(TitleSupervisorView, subthreadCloseRequestActive) == 0x6B4) ? 1 : -1];
typedef char TitleSupervisorActiveAt6B8[(offsetof(TitleSupervisorView, subthreadActive) == 0x6B8) ? 1 : -1];
typedef char TitleSupervisorLoadingAt740[(offsetof(TitleSupervisorView, loadingVmsHaveBeenSetup) == 0x740) ? 1 : -1];
typedef char TitleSupervisorRecordingWarningAt77C[
    (offsetof(TitleSupervisorView, recordingFpsWarning77C) == 0x77C) ? 1 : -1];
typedef char TitleSupervisorPlaybackWarningAt780[
    (offsetof(TitleSupervisorView, playbackFpsWarning780) == 0x780) ? 1 : -1];
typedef char TitleSupervisorRecordingState790[
    (offsetof(TitleSupervisorView, recordingFpsState790) == 0x790) ? 1 : -1];

struct TitleAsciiManagerView {
    void Reset();
    void InitializeVms();
};

struct TitleNetworkStateView {
    u8 unknown000[0xA8];
    i32 activeA8;
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
    i32 practiceState;                         // +0x00088
    char replayName[9];                       // +0x0008C
    u8 unknown00095[0x1F7];
    char replayPaths[50][0x200];              // +0x0028C
    u8 unknown0668C[0x190];
    ReplayDataView *replayAllocations[50];    // +0x0681C
    ReplayDataView replays[50];               // +0x068E4
    ReplayDataView *currentReplay;             // +0x0C8FC
    i32 replayEnumerationResetState;           // +0x0C900
    i32 replaySlotCount;                       // +0x0C904
    i32 selectedReplay;                        // +0x0C908
    i32 unknown0C90C;
    union { i32 replayUiFrameCounter; i32 screenFrameCounter; }; // +0x0C910
    u8 unknown0C914[4];
    i32 unknown0C918;                        // +0x0C918
    u8 unknown0C91C[4];
    i32 optionResetCounter;                   // +0x0C920
    u8 unknown0C924[0x5260];
    i32 side0CharacterSetting;                // +0x11B84
    i32 side1CharacterSetting;                // +0x11B88
    i32 characterSettingInputActive;
    TitleAnmView *titleAnm;                   // +0x11B90
    TitleAnmView *resultTextAnm;               // +0x11B94
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
    ChainElem *calcChain;                      // +0x1B248
    ChainElem *drawChain;                      // +0x1B24C
    u8 unknown1B250[0x6C];
    TitleConfigSnapshotView configSnapshot;   // +0x1B2BC

    static void __cdecl TitleSetupThread(void *unused);
    int ActualAddedCallback();
    static int AddedCallback(TitleScreenView *titleScreen);
    int Release();
    static int DeletedCallback(TitleScreenView *titleScreen);

    int OnUpdateResult();
    int OnUpdateReplayMenu();
    int UpdateReplaySave();
    int OnUpdateStartMenu();
    int OnUpdateDifficultySelect();
    int OnUpdateModeSelect();
    int PlayMenuSound(i32 soundId, i32 unused);
    int ChangeCurrentScreen(i32 screen);
    int MoveTwoChoiceCursor(i32 count);
    int MoveCursorVertical(i32 count);
    int MoveCursorHorizontal(i32 count);
    int MoveCursorFourWay(i32 count);
    void UpdateMenuSelection();
    void SetMenuSelectionSprites(i32 selected, i32 start, i32 count);
    short *SwapDuplicateCharacterIndex(
        short *slots, short incoming, short replacement, int unusedMode);
    int SetTwoDigitVmValue(AnmVmView *digitVms, short value);
    void SetIndexedSpriteChoice(i32 choice, i32 vmIndex);
    void SetRangeSelectionInterrupts(i32 selected, i32 start, i32 count);
    int OnUpdateOptions();
    int OnUpdateCharacterSelect();
    int OnUpdateResultNameEntry();
    void *MoveCharacterCursor(i32 side, i32 direction, char *order, i32 count);
    void *MoveCharacterCursorNormal(i32 side, i32 direction, char *order, i32 count);
    void *MoveCharacterCursorMode4(i32 side, i32 direction, char *order, i32 count);
    int MoveCharacterCursorHorizontal(i32 side, i32 count);
    int MoveCharacterCursorHorizontalForInput(i32 inputIndex, i32 cursorIndex, i32 count);
    int SetCharacterCursorActive(i32 selectedCharacter, i32 start, i32 count, i32 stride);
    int SetCharacterCursorInactive(i32 selectedCharacter, i32 start, i32 count, i32 stride);
    int SetCharacterCursorReverse(i32 selectedCharacter, i32 start, i32 count, i32 stride);
    int SetCharacterSettingSprites(i32 side0Setting, i32 side1Setting);
    int UpdateCharacterSettings(i32 side0Setting, i32 side1Setting);
    int SetCharacterSettingIndicator(i32 side, i32 value);
    int UpdateCharacterSelectionVisuals(i32 side, i32 selectedCharacter, i32 otherCharacter, char *order);
    int UpdateScreen16SelectionVisuals(i32 side, i32 selectedCharacter, char *order);
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
typedef char TitleReplayAllocationsAt681C[(offsetof(TitleScreenView, replayAllocations) == 0x681C) ? 1 : -1];
typedef char TitleCurrentReplayAtC8FC[(offsetof(TitleScreenView, currentReplay) == 0xC8FC) ? 1 : -1];
typedef char TitleReplayEnumResetAtC900[(offsetof(TitleScreenView, replayEnumerationResetState) == 0xC900) ? 1 : -1];
typedef char TitleReplaySlotCountAtC904[(offsetof(TitleScreenView, replaySlotCount) == 0xC904) ? 1 : -1];
typedef char TitleReplaysAt68E4[(offsetof(TitleScreenView, replays) == 0x68E4) ? 1 : -1];
typedef char TitleSelectedReplayAtC908[(offsetof(TitleScreenView, selectedReplay) == 0xC908) ? 1 : -1];
typedef char TitleUnknownC918AtC918[(offsetof(TitleScreenView, unknown0C918) == 0xC918) ? 1 : -1];
typedef char TitleUiVmAt11B98[(offsetof(TitleScreenView, uiVm) == 0x11B98) ? 1 : -1];
typedef char TitleUiScriptAt1B224[(offsetof(TitleScreenView, uiScript) == 0x1B224) ? 1 : -1];
typedef char TitlePhaseAt1B230[(offsetof(TitleScreenView, phaseTimer) == 0x1B230) ? 1 : -1];
typedef char TitlePrevCursorAt24[(offsetof(TitleScreenView, previousCursor) == 0x24) ? 1 : -1];
typedef char TitlePrevScreenAt84[(offsetof(TitleScreenView, previousScreen) == 0x84) ? 1 : -1];
typedef char TitlePracticeStateAt88[(offsetof(TitleScreenView, practiceState) == 0x88) ? 1 : -1];
typedef char TitleSide0SettingAt11B84[(offsetof(TitleScreenView, side0CharacterSetting) == 0x11B84) ? 1 : -1];
typedef char TitleSide1SettingAt11B88[(offsetof(TitleScreenView, side1CharacterSetting) == 0x11B88) ? 1 : -1];
typedef char TitleAnmAt11B90[(offsetof(TitleScreenView, titleAnm) == 0x11B90) ? 1 : -1];
typedef char TitleResultAnmAt11B94[(offsetof(TitleScreenView, resultTextAnm) == 0x11B94) ? 1 : -1];
typedef char TitleVmsAt11B98[(offsetof(TitleScreenView, vms) == 0x11B98) ? 1 : -1];
typedef char TitleHelpAt11BA0[(offsetof(TitleScreenView, embeddedVms) == 0x11BA0) ? 1 : -1];
typedef char TitleVmCountAt1B224[(offsetof(TitleScreenView, vmCount) == 0x1B224) ? 1 : -1];
typedef char TitleCurrentScreenAt1B228[(offsetof(TitleScreenView, currentScreen) == 0x1B228) ? 1 : -1];
typedef char TitleIdleAt1B234[(offsetof(TitleScreenView, idleFrames) == 0x1B234) ? 1 : -1];
typedef char TitleMenuStartAt1B240[(offsetof(TitleScreenView, menuVmStart) == 0x1B240) ? 1 : -1];
typedef char TitleMenuCountAt1B244[(offsetof(TitleScreenView, menuItemCount) == 0x1B244) ? 1 : -1];
typedef char TitleCalcChainAt1B248[(offsetof(TitleScreenView, calcChain) == 0x1B248) ? 1 : -1];
typedef char TitleDrawChainAt1B24C[(offsetof(TitleScreenView, drawChain) == 0x1B24C) ? 1 : -1];
typedef char TitleConfigAt1B2BC[(offsetof(TitleScreenView, configSnapshot) == 0x1B2BC) ? 1 : -1];
typedef char TitleScreenSizeIs1B388[(sizeof(TitleScreenView) == 0x1B388) ? 1 : -1];

extern InputView g_TitleInput;
extern TitleInputStateTableView g_TitleInputStateTable;
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
extern TitleAsciiManagerView g_AsciiManager;
extern TitleNetworkStateView *g_SupervisorNetworkState;
extern Chain g_Chain;
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
extern i32 g_TitleLaunchState;
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
extern char g_TitleCharacterOrder14[16];
extern char g_TitleCharacterOrderMode123[16];
extern char g_TitleScreen16Order[16];
extern char g_TitleScreen16Entries[16];
extern i32 g_TitleScreen16SelectionResult;
extern TitleCharacterConfigView g_TitleCharacterConfig;
extern u8 g_TitleCharacterUnlocked[16];
extern u8 g_TitleCharacterUnlockedNormal[16];
extern u8 g_TitleCharacterUnlockedMode4[16];
extern i32 g_TitleResultUnlockStep;
extern ScreenEffect *g_TitleFadeReleaseEffect;
extern TitleSideInputView g_TitleSide0Input;
extern TitleSideInputView g_TitleSide1Input;
extern TitleSelectionRandomView g_TitleSelectionRandom;
extern void __fastcall PrepareTitleMode4Network(void *optionState);
extern void __fastcall ResetTitleMode4Supervisor(TitleSupervisorView *supervisor);
extern int SaveTitleScoreData();


extern TitleScreenView *g_TitleScreen;

void __cdecl TitleScreenView::TitleSetupThread(void *unused)
{
    (void)unused;

    while (g_TitleAnmManager->captureSurfaceIndex >= 0)
        Sleep(1);

    g_TitleScreen->titleAnm = g_TitleAnmManager->PreloadAnm(15, "title01.anm");
    if (g_TitleScreen->titleAnm == 0)
    {
        g_TitleScreen->chainState = 2;
        return;
    }

    g_TitleScreen->resultTextAnm = g_TitleAnmManager->PreloadAnm(17, "resulttext.anm");
    if (g_TitleScreen->resultTextAnm == 0)
    {
        g_TitleScreen->chainState = 2;
        return;
    }

    if (g_TitleSupervisor.subthreadCloseRequestActive)
        return;

    for (i32 i = 0; i < 14; i++)
    {
        AnmVmView *helpVm = &g_TitleScreen->embeddedVms[i];
        AnmVmView *infoVm = &g_TitleScreen->embeddedVms[35 + i];

        g_TitleSupervisor.textAnm->ExecuteAnmIdx(helpVm, 5);
        g_TitleSupervisor.textAnm->SetSprite(helpVm, helpVm->activeSpriteIndex + i);
        g_TitleSupervisor.textAnm->SetSprite(infoVm, i + 21);

        TitleFloat3View position = {
            64.0f,
            (float)i * 16.0f + (i <= 4 ? 352.0f : 362.0f),
            0.0f,
        };
        infoVm->flagsLow |= 0x1802;
        infoVm->position = position;
        infoVm->fontWidth = 15;
        infoVm->fontHeight = 15;
    }

    if (g_TitleSupervisor.subthreadCloseRequestActive)
        return;

    const char *surfacePath =
        g_TitleScreen->currentScreen == 1 ? "title/title00.png" : "title/result00.png";
    if (g_TitleAnmManager->PreloadSurface(0, surfacePath))
    {
        g_TitleScreen->chainState = 2;
        return;
    }

    if ((g_TitleGameFlags & 2) == 0)
    {
        if (g_TitleSupervisor.transitionState594 != 5)
            g_TitleSupervisor.LoadMusic(0);
        ScreenEffect::RegisterChain(SCREEN_EFFECT_FULL_FADE_IN, 70, 0xFFFFFF, 0, 0, 35, 2);
    }

    g_TitleScreen->currentHelpTextVm = &g_TitleScreen->embeddedVms[0];
    g_TitleScreen->chainState = 0;
    if (g_TitleFadeReleaseEffect != 0)
        g_TitleFadeReleaseEffect->BeginFadeRelease();
    g_TitleFadeReleaseEffect = 0;
    g_TitleSupervisor.HideLoadingVms();
    g_TitleSupervisor.runningSubthreadHandle = 0;
    g_TitleSupervisor.subthreadCloseRequestActive = 0;
    g_TitleSupervisor.subthreadActive = 0;
}

int TitleScreenView::ActualAddedCallback()
{
    g_ScreenEffectCounter = 0;
    g_AsciiManager.Reset();
    g_AsciiManager.InitializeVms();
    g_TitleSupervisor.frameRateMultiplier = 1.0f;

    if (g_TitleGameFlags & 8)
    {
        g_GameSide0Value20 = 0;
        g_GameSide1Value20 = 1;
    }
    if (g_TitleGameFlags & 2)
        g_TitleGameFlags &= ~8u;

    ChangeCurrentScreen(1);
    g_TitleSupervisor.ClearRecordingFpsWarningState();

    if (g_SupervisorNetworkState->activeA8 != 0 && g_GameMode != 2)
    {
        resumeState = 2;
        g_TitleModeSelection = 0;
        g_TitleDifficulty = 1;
    }

    switch (registrationContext)
    {
    case 0:
        if ((g_TitleGameFlags & 0xA) == 0)
        {
            if (g_GameMode == 0)
                keyboardSelection = 0;
            else if (g_GameMode == 1)
                keyboardSelection = 1;
            else if (g_GameMode == 2)
                resumeState = 1;
        }
        break;
    case 1:
        currentScreen = 14;
        break;
    default:
        keyboardSelection = 0;
        break;
    }

    practiceState = 0;
    g_TitleGameFlags &= ~1u;

    TitleFloat3View loadingPosition = {500.0f, 440.0f, 0.0f};
    if (g_TitleSupervisor.transitionState594 == 2)
    {
        g_TitleSupervisor.SetupLoadingVmsAndInitCapture(&loadingPosition);
        g_TitleSupervisor.StartEffect(0);
    }
    else if (g_TitleSupervisor.transitionState594 != 0)
    {
        g_TitleSupervisor.SetupLoadingVms(&loadingPosition);
    }

    g_TitleGameFlags &= ~2u;
    chainState = 1;
    g_TitleSupervisor.ThreadStart(
        reinterpret_cast<LPTHREAD_START_ROUTINE>(TitleScreenView::TitleSetupThread), 0);
    return 0;
}

int TitleScreenView::AddedCallback(TitleScreenView *titleScreen)
{
    return titleScreen->ActualAddedCallback();
}

int TitleScreenView::Release()
{
    if (currentReplay != 0)
    {
        free(currentReplay);
        currentReplay = 0;
    }

    if (vms != 0)
    {
        free(vms);
        vms = 0;
    }

    for (i32 i = 0; i < 50; i++)
    {
        if (replayAllocations[i] != 0)
            free(replayAllocations[i]);
        replayAllocations[i] = 0;
    }
    return 0;
}

int TitleScreenView::DeletedCallback(TitleScreenView *titleScreen)
{
    g_TitleSupervisor.d3dDevice->ResourceManagerDiscardBytes(0);
    g_TitleAnmManager->ReleaseAnm(15);
    g_TitleAnmManager->ReleaseAnm(17);
    g_TitleAnmManager->ReleaseSurface(0);
    g_Chain.Cut(titleScreen->drawChain);
    titleScreen->drawChain = 0;
    titleScreen->Release();
    free(titleScreen);
    return 0;
}



int TitleScreenView::MoveCursorVertical(i32 count)
{
    if (count == 0)
        return 0;

    if (g_TitleInput.IsPressedScrolling(0x10))
    {
        keyboardSelection--;
        g_SoundPlayer.PlaySoundByIdx(12, 0);
        if (keyboardSelection < 0)
            keyboardSelection = count - 1;
        if (keyboardSelection >= count)
            keyboardSelection = 0;
        return -1;
    }
    else if (g_TitleInput.IsPressedScrolling(0x20))
    {
        keyboardSelection++;
        g_SoundPlayer.PlaySoundByIdx(12, 0);
        if (keyboardSelection < 0)
            keyboardSelection = count - 1;
        if (keyboardSelection >= count)
            keyboardSelection = 0;
        return 1;
    }

    return 0;
}


int TitleScreenView::MoveCursorHorizontal(i32 count)
{
    if (count == 0)
        return 0;

    i32 direction;
    if (g_TitleInput.IsPressedScrolling(0x40))
    {
        if (--keyboardSelection < 0)
            keyboardSelection += count;
        direction = -1;
    }
    else if (g_TitleInput.IsPressedScrolling(0x80))
    {
        if (++keyboardSelection >= count)
            keyboardSelection -= count;
        direction = 1;
    }
    else
    {
        return 0;
    }

    g_SoundPlayer.PlaySoundByIdx(12, 0);
    return direction;
}


void TitleScreenView::SetMenuSelectionSprites(i32 selected, i32 start, i32 count)
{
    for (i32 index = start; index < start + count; index++)
    {
        titleAnm->SetSprite(&vms[index], vms[index].baseSpriteIndex + 1);
        vms[index].pendingInterrupt = 8;
    }

    i32 selectedIndex = selected + start;
    titleAnm->SetSprite(&vms[selectedIndex], vms[selectedIndex].baseSpriteIndex);
    vms[selectedIndex].pendingInterrupt = 7;
}


char AnmVmView::ConfigurePositionInterpolation(
    i32 duration, char mode, TitleFloat3View *start, TitleFloat3View *end)
{
    interpCurrentTimers[0] = 0;
    interpEndTimers[0] = duration;

    interpModes[0] = mode;
    posInitial = *start;
    posFinal = *end;
    return mode;
}


int AnmVmView::ConfigureColorInterpolation(
    i32 duration, char mode, u32 startColor, u32 endColor)
{
    interpCurrentTimers[1] = 0;
    interpEndTimers[1] = duration;

    interpModes[1] = mode;
    color1Initial.r = (startColor >> 16) & 0xFF;
    color1Initial.g = (startColor >> 8) & 0xFF;
    color1Initial.b = startColor & 0xFF;
    color1Final.r = (endColor >> 16) & 0xFF;
    color1Final.g = (endColor >> 8) & 0xFF;
    color1Final.b = endColor & 0xFF;
    return endColor;
}



int TitleScreenView::SetTwoDigitVmValue(AnmVmView *digitVms, short value)
{
    if (value < 0)
    {
        digitVms[0].flags &= ~2u;
        digitVms[1].flags &= ~2u;
    }
    else
    {
        titleAnm->SetSprite(
            &digitVms[0],
            digitVms[0].baseSpriteIndex + 2 * (value / 10));
        titleAnm->SetSprite(
            &digitVms[1],
            digitVms[1].baseSpriteIndex + 2 * (value % 10));
        digitVms[0].flags |= 2;
        digitVms[1].flags |= 2;
    }
    return 0;
}


void TitleScreenView::SetIndexedSpriteChoice(i32 choice, i32 vmIndex)
{
    titleAnm->SetSprite(
        &vms[vmIndex],
        vms[vmIndex].baseSpriteIndex + 2 * choice);
    vms[vmIndex].pendingInterrupt = 7;
}


void TitleScreenView::SetRangeSelectionInterrupts(i32 selected, i32 start, i32 count)
{
    for (i32 index = start; index < start + count; index++)
        vms[index].pendingInterrupt = 8;

    vms[start + selected].pendingInterrupt = 7;
}


void TitleScreenView::UpdateMenuSelection()
{
    SetMenuSelectionSprites(keyboardSelection, menuVmStart, menuItemCount);
}


int TitleScreenView::MoveCursorFourWay(i32 count)
{
    if (count == 0)
        return 0;

    if (g_TitleInput.IsPressedScrolling(0x40))
    {
        if (--keyboardSelection < 0)
            keyboardSelection += count;
        g_SoundPlayer.PlaySoundByIdx(12, 0);
        return -1;
    }

    if (g_TitleInput.IsPressedScrolling(0x80))
    {
        if (++keyboardSelection >= count)
            keyboardSelection -= count;
        g_SoundPlayer.PlaySoundByIdx(12, 0);
        return 1;
    }

    if (g_TitleInput.IsPressedScrolling(0x10))
    {
        keyboardSelection--;
        g_SoundPlayer.PlaySoundByIdx(12, 0);
        if (keyboardSelection < 0)
            keyboardSelection = count - 1;
        if (keyboardSelection >= count)
            keyboardSelection = 0;
        return -1;
    }

    if (g_TitleInput.IsPressedScrolling(0x20))
    {
        keyboardSelection++;
        g_SoundPlayer.PlaySoundByIdx(12, 0);
        if (keyboardSelection < 0)
            keyboardSelection = count - 1;
        if (keyboardSelection >= count)
            keyboardSelection = 0;
        return 1;
    }

    return 0;
}


void *TitleScreenView::MoveCharacterCursor(i32 side, i32 direction, char *order, i32 count)
{
    void *result = g_OptionPointers;
    if (g_OptionPointers[42] == 0)
    {
        i32 *cursor = &side0CharacterCursor + side;
        result = cursor;
        while (!g_TitleCharacterUnlocked[order[*cursor]])
        {
            *cursor += direction;
            if (*cursor < 0)
                *cursor += count;
            if (*cursor >= count)
                *cursor -= count;
        }
    }
    return result;
}


void *TitleScreenView::MoveCharacterCursorNormal(i32 side, i32 direction, char *order, i32 count)
{
    void *result = g_OptionPointers;
    if (g_OptionPointers[42] == 0)
    {
        i32 *cursor = &side0CharacterCursor + side;
        result = cursor;
        while (!g_TitleCharacterUnlockedNormal[order[*cursor]])
        {
            *cursor += direction;
            if (*cursor < 0)
                *cursor += count;
            if (*cursor >= count)
                *cursor -= count;
        }
    }
    return result;
}


void *TitleScreenView::MoveCharacterCursorMode4(i32 side, i32 direction, char *order, i32 count)
{
    void *result = g_OptionPointers;
    if (g_OptionPointers[42] == 0)
    {
        i32 *cursor = &side0CharacterCursor + side;
        result = cursor;
        while (!g_TitleCharacterUnlockedMode4[order[*cursor]])
        {
            *cursor += direction;
            if (*cursor < 0)
                *cursor += count;
            if (*cursor >= count)
                *cursor -= count;
        }
    }
    return result;
}


int TitleScreenView::MoveCharacterCursorHorizontal(i32 cursorIndex, i32 count)
{
    if (count == 0)
        return 0;

    TitleInputStatePhysicalView *input = &g_TitleInputStateTable.states[cursorIndex];
    if (input->IsPressedScrolling(0x40))
        goto moveLeft;
    if (input->IsPressedScrolling(0x80))
        goto moveRight;
    if (input->IsPressedScrolling(0x10))
        goto moveLeft;
    if (input->IsPressedScrolling(0x20))
        goto moveRight;
    return 0;

moveLeft:
    {
        i32 *cursor = &side0CharacterCursor + cursorIndex;
        --*cursor;
        if (*cursor < 0)
            *cursor += count;
        cursorIndex = -1;
    }
    goto moved;

moveRight:
    {
        i32 *cursor = &side0CharacterCursor + cursorIndex;
        ++*cursor;
        if (*cursor >= count)
            *cursor -= count;
        cursorIndex = 1;
    }

moved:
    g_SoundPlayer.PlaySoundByIdx(12, 0);
    return cursorIndex;
}


int TitleScreenView::MoveCharacterCursorHorizontalForInput(i32 inputIndex, i32 cursorIndex, i32 count)
{
    if (count == 0)
        return 0;

    TitleInputStatePhysicalView *input = &g_TitleInputStateTable.states[inputIndex];
    if (input->IsPressedScrolling(0x40))
        goto moveLeft;
    if (input->IsPressedScrolling(0x80))
        goto moveRight;
    if (input->IsPressedScrolling(0x10))
        goto moveLeft;
    if (input->IsPressedScrolling(0x20))
        goto moveRight;
    return 0;

moveLeft:
    {
        i32 *cursor = &side0CharacterCursor + cursorIndex;
        --*cursor;
        if (*cursor < 0)
            *cursor += count;
        cursorIndex = -1;
    }
    goto moved;

moveRight:
    {
        i32 *cursor = &side0CharacterCursor + cursorIndex;
        ++*cursor;
        if (*cursor >= count)
            *cursor -= count;
        cursorIndex = 1;
    }

moved:
    g_SoundPlayer.PlaySoundByIdx(12, 0);
    return cursorIndex;
}


int TitleScreenView::SetCharacterCursorActive(i32 selectedCharacter, i32 start, i32 count, i32 stride)
{
    i32 index = start;
    while (index < start + count * stride)
    {
        titleAnm->SetSprite(&vms[index], vms[index].baseSpriteIndex + 1);
        vms[index].pendingInterrupt = 8;
        g_TitleAnmManager->ExecuteScript(&vms[index]);
        index += stride;
    }

    index = start + stride * selectedCharacter;
    titleAnm->SetSprite(&vms[index], vms[index].baseSpriteIndex);
    vms[index].pendingInterrupt = 7;
    return g_TitleAnmManager->ExecuteScript(&vms[index]);
}


int TitleScreenView::SetCharacterCursorReverse(i32 selectedCharacter, i32 start, i32 count, i32 stride)
{
    i32 index = start;
    while (index < start + count * stride)
    {
        titleAnm->SetSprite(&vms[index], vms[index].baseSpriteIndex + 1);
        vms[index].pendingInterrupt = 15;
        index += stride;
    }

    index = start + stride * selectedCharacter;
    titleAnm->SetSprite(&vms[index], vms[index].baseSpriteIndex);
    vms[index].pendingInterrupt = 14;
    return (i32)vms;
}


int TitleScreenView::SetCharacterCursorInactive(i32 selectedCharacter, i32 start, i32 count, i32 stride)
{
    i32 index = start;
    while (index < start + count * stride)
    {
        titleAnm->SetSprite(&vms[index], vms[index].baseSpriteIndex + 1);
        vms[index].pendingInterrupt = 18;
        index += stride;
    }

    index = start + stride * selectedCharacter;
    titleAnm->SetSprite(&vms[index], vms[index].baseSpriteIndex);
    vms[index].pendingInterrupt = 17;
    return (i32)vms;
}


int TitleScreenView::SetCharacterSettingSprites(i32 side0Setting, i32 side1Setting)
{
    titleAnm->SetSprite(&vms[142], side0Setting == 0 ? 197 : (side0Setting == 1 ? 196 : 195));
    titleAnm->SetSprite(&vms[143], side0Setting <= 2 ? 197 : (side0Setting == 3 ? 196 : 195));
    titleAnm->SetSprite(&vms[144], side0Setting <= 4 ? 197 : (side0Setting == 5 ? 196 : 195));
    titleAnm->SetSprite(&vms[145], side0Setting <= 6 ? 197 : (side0Setting == 7 ? 196 : 195));
    titleAnm->SetSprite(&vms[146], side0Setting <= 8 ? 197 : (side0Setting == 9 ? 196 : 195));

    titleAnm->SetSprite(&vms[148], side1Setting == 0 ? 197 : (side1Setting == 1 ? 196 : 195));
    titleAnm->SetSprite(&vms[149], side1Setting <= 2 ? 197 : (side1Setting == 3 ? 196 : 195));
    titleAnm->SetSprite(&vms[150], side1Setting <= 4 ? 197 : (side1Setting == 5 ? 196 : 195));
    titleAnm->SetSprite(&vms[151], side1Setting <= 6 ? 197 : (side1Setting == 7 ? 196 : 195));
    return titleAnm->SetSprite(&vms[152], side1Setting <= 8 ? 197 : (side1Setting == 9 ? 196 : 195));
}


int TitleScreenView::UpdateCharacterSettings(i32 side0Setting, i32 side1Setting)
{
    vms[141].pendingInterrupt = 9;
    vms[147].pendingInterrupt = 9;
    vms[142].pendingInterrupt = 9;
    vms[143].pendingInterrupt = 9;
    vms[144].pendingInterrupt = 9;
    vms[145].pendingInterrupt = 9;
    vms[146].pendingInterrupt = 9;
    vms[148].pendingInterrupt = 9;
    vms[149].pendingInterrupt = 9;
    vms[150].pendingInterrupt = 9;
    vms[151].pendingInterrupt = 9;
    vms[152].pendingInterrupt = 9;
    return SetCharacterSettingSprites(side0Setting, side1Setting);
}


int TitleScreenView::SetCharacterSettingIndicator(i32 side, i32 value)
{
    value = value ? 9 : 10;
    vms[153 + side].pendingInterrupt = (u16)value;
    return value;
}


int TitleCharacterConfigView::GetOptionState(char character, i32 option)
{
    if (option >= 0)
        return optionValues[character][option];
    else
    {
        return optionValues[character][0] |
               optionValues[character][1] |
               optionValues[character][2] |
               optionValues[character][3];
    }
}


int TitleScreenView::UpdateCharacterSelectionVisuals(i32 side, i32 selectedCharacter,
                                                         i32 otherCharacter, char *order)
{
    i32 selectedVisibleIndex = 0;
    for (i32 orderIndex = 0; orderIndex < 16; orderIndex++)
    {
        i32 character = order[orderIndex];
        if (character == selectedCharacter)
            break;

        bool visible;
        if (currentScreen == 8)
            visible = g_TitleCharacterUnlocked[character] || g_OptionPointers[42];
        else if (currentScreen == 3)
            visible = g_TitleCharacterUnlockedNormal[character] != 0;
        else
            visible = g_TitleCharacterUnlockedMode4[character] != 0;
        if (visible)
            selectedVisibleIndex++;
    }

    TitleFloat3View targetPosition = vms[189 + side].position;
    targetPosition.y = selectedVisibleIndex * 16.0f + 128.0f;
    targetPosition.z = 0.0f;
    vms[189 + side].ConfigurePositionInterpolation(
        8, 4, &vms[189 + side].position, &targetPosition);

    targetPosition.x = vms[190].position.x;
    vms[190].ConfigurePositionInterpolation(8, 4, &vms[190].position, &targetPosition);

    i32 otherVisibleIndex = 0;
    for (i32 orderIndex = 0; orderIndex < 16; orderIndex++)
    {
        i32 character = order[orderIndex];
        if (character == otherCharacter)
            break;

        bool visible;
        if (currentScreen == 8)
            visible = g_TitleCharacterUnlocked[character] || g_OptionPointers[42];
        else if (currentScreen == 3)
            visible = g_TitleCharacterUnlockedNormal[character] != 0;
        else
            visible = g_TitleCharacterUnlockedMode4[character] != 0;
        if (visible)
            otherVisibleIndex++;
    }

    i32 visibleIndex = 0;
    i32 result = 0;
    for (i32 orderIndex = 0; orderIndex < 16; orderIndex++)
    {
        i32 character = order[orderIndex];
        if (g_TitleCharacterUnlocked[character] || g_OptionPointers[42])
        {
            AnmVmView *vm = &vms[191 + visibleIndex];
            u32 targetColor = (selectedVisibleIndex == visibleIndex || otherVisibleIndex == visibleIndex)
                ? 0xFFFFFFFFu
                : 0xFFA0A0A0u;
            result = vm->ConfigureColorInterpolation(8, 4, vm->color1, targetColor);
            visibleIndex++;
        }
    }
    return result;
}


int TitleScreenView::UpdateScreen16SelectionVisuals(i32 side, i32 selectedCharacter, char *order)
{
    i32 selectedVisibleIndex = 0;
    for (i32 orderIndex = 0; orderIndex < 16; orderIndex++)
    {
        i32 character = order[orderIndex];
        if (character == selectedCharacter)
            break;
        if (orderIndex > 13 || g_TitleCharacterUnlocked[character] || g_OptionPointers[42])
            selectedVisibleIndex++;
    }

    TitleFloat3View targetPosition = vms[189 + side].position;
    targetPosition.y = selectedVisibleIndex * 16.0f + 128.0f;
    targetPosition.z = 0.0f;
    vms[189 + side].ConfigurePositionInterpolation(
        8, 4, &vms[189 + side].position, &targetPosition);

    targetPosition.x = vms[190].position.x;
    vms[190].ConfigurePositionInterpolation(8, 4, &vms[190].position, &targetPosition);

    i32 visibleIndex = 0;
    i32 result = 0;
    for (i32 orderIndex = 0; orderIndex < 16; orderIndex++)
    {
        i32 character = order[orderIndex];
        if (g_TitleCharacterUnlocked[character] || orderIndex > 13 || g_OptionPointers[42])
        {
            AnmVmView *vm = &vms[207 + visibleIndex];
            u32 targetColor = selectedVisibleIndex == visibleIndex ? 0xFFFFFFFFu : 0xFFA0A0A0u;
            result = vm->ConfigureColorInterpolation(8, 4, vm->color1, targetColor);
            visibleIndex++;
        }
    }
    return result;
}


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
    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 13);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);

            i32 visibleCount = 0;
            for (i32 orderIndex = 0; orderIndex < 14; orderIndex++)
            {
                char character = g_TitleCharacterOrder14[orderIndex];
                bool available = g_TitleDifficulty == 4
                    ? g_TitleCharacterUnlockedMode4[character] != 0
                    : g_TitleCharacterUnlockedNormal[character] != 0;
                if (available)
                {
                    titleAnm->SetSprite(&vms[191 + visibleCount], character + 235);
                    vms[191 + visibleCount].flags |= 2;
                    visibleCount++;
                }
            }
            for (i32 index = visibleCount; index < 16; index++)
                vms[191 + index].flags &= ~2u;

            vms[189].flags |= 2;
            vms[190].flags &= ~2u;

            side0CharacterCursor = 0;
            while (g_TitleCharacterOrder14[side0CharacterCursor] != g_GameSide0Value20)
                side0CharacterCursor++;

            if (g_TitleDifficulty == 4)
                MoveCharacterCursorMode4(0, 1, g_TitleCharacterOrder14, 14);
            else
                MoveCharacterCursorNormal(0, 1, g_TitleCharacterOrder14, 14);

            char selectedCharacter = g_TitleCharacterOrder14[side0CharacterCursor];
            char otherCharacter = g_TitleCharacterOrder14[side1CharacterCursor];

            vms[93 + selectedCharacter * 2].color1Bytes[0] = 0xFF;
            vms[93 + selectedCharacter * 2].color1Bytes[1] = 0xFF;
            vms[93 + selectedCharacter * 2].color1Bytes[2] = 0xFF;
            vms[61 + selectedCharacter * 2].color1Bytes[0] = 0xFF;
            vms[61 + selectedCharacter * 2].color1Bytes[1] = 0xFF;
            vms[61 + selectedCharacter * 2].color1Bytes[2] = 0xFF;
            vms[93 + otherCharacter * 2].color1Bytes[0] = 0xFF;
            vms[93 + otherCharacter * 2].color1Bytes[1] = 0xFF;
            vms[93 + otherCharacter * 2].color1Bytes[2] = 0xFF;
            vms[61 + otherCharacter * 2].color1Bytes[0] = 0xFF;
            vms[61 + otherCharacter * 2].color1Bytes[1] = 0xFF;
            vms[61 + otherCharacter * 2].color1Bytes[2] = 0xFF;

            SetCharacterCursorActive(selectedCharacter, 92, 14, 2);
            SetCharacterCursorActive(selectedCharacter, 60, 14, 2);
            side0CharacterConfirmed = 0;
            currentScreenState = 0;
            stateTimer = 0;
            uiAux = 0;
            unknown0C918 = 1;
            side1CharacterConfirmed = 1;
            UpdateCharacterSelectionVisuals(0, selectedCharacter, selectedCharacter, g_TitleCharacterOrder14);

            characterSettingInputActive = 0;
            side0CharacterSetting = 10;
            side1CharacterSetting = 10;
            if (g_TitleInput.currentInput & 4)
            {
                characterSettingInputActive = 1;
                UpdateCharacterSettings(10, 10);
            }

            i32 optionState = g_TitleDifficulty == 4
                ? g_TitleCharacterConfig.optionValues[selectedCharacter][0]
                : g_TitleCharacterConfig.GetOptionState(selectedCharacter, g_TitleDifficulty);
            SetCharacterSettingIndicator(0, optionState);
        }

        if (stateTimer2 == 8)
            currentScreenState = 1;
        break;

    case 1:
    {
        if (side0CharacterConfirmed == 0)
        {
            if (g_TitleInput.currentInput & 4)
            {
                if (characterSettingInputActive)
                {
                    if (g_TitleInput.IsPressedScrolling(0x40))
                    {
                        if (side0CharacterSetting > 1)
                            side0CharacterSetting--;
                        UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                    }
                    if (g_TitleInput.IsPressedScrolling(0x80))
                    {
                        if (side0CharacterSetting < 10)
                            side0CharacterSetting++;
                        UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                    }
                }
            }
            else
            {
                i32 direction = MoveCharacterCursorHorizontalForInput(2, 0, 14);
                if (direction != 0)
                {
                    if (g_TitleDifficulty == 4)
                        MoveCharacterCursorMode4(0, direction, g_TitleCharacterOrder14, 14);
                    else
                        MoveCharacterCursorNormal(0, direction, g_TitleCharacterOrder14, 14);

                    char selectedCharacter = g_TitleCharacterOrder14[side0CharacterCursor];
                    i32 optionState = g_TitleDifficulty == 4
                        ? g_TitleCharacterConfig.optionValues[selectedCharacter][0]
                        : g_TitleCharacterConfig.GetOptionState(selectedCharacter, g_TitleDifficulty);
                    SetCharacterSettingIndicator(0, optionState);

                    if (direction > 0)
                    {
                        SetCharacterCursorActive(selectedCharacter, 92, 14, 2);
                        SetCharacterCursorActive(selectedCharacter, 60, 14, 2);
                    }
                    else
                    {
                        SetCharacterCursorReverse(selectedCharacter, 92, 14, 2);
                        SetCharacterCursorReverse(selectedCharacter, 60, 14, 2);
                    }
                    UpdateCharacterSelectionVisuals(0, selectedCharacter, selectedCharacter, g_TitleCharacterOrder14);
                }
            }
        }

        char selectedCharacter = g_TitleCharacterOrder14[side0CharacterCursor];
        if ((g_TitleInputFlags & 0x1001) && side0CharacterConfirmed == 0)
        {
            PlayMenuSound(10, 0);
            side0CharacterConfirmed = 1;
            vms[92 + selectedCharacter * 2].color1Bytes[0] = 0x80;
            vms[92 + selectedCharacter * 2].color1Bytes[1] = 0x80;
            vms[92 + selectedCharacter * 2].color1Bytes[2] = 0x80;
            vms[60 + selectedCharacter * 2].color1Bytes[0] = 0x80;
            vms[60 + selectedCharacter * 2].color1Bytes[1] = 0x80;
            vms[60 + selectedCharacter * 2].color1Bytes[2] = 0x80;
            g_GameSide0Value30 = (g_TitleInput.currentInput >> 2) & 1;
        }

        if (side0CharacterConfirmed && side1CharacterConfirmed)
        {
            g_TitleSupervisor.StopAudio();
            g_GameSide0Value20 = selectedCharacter;
            g_GameSide1Value20 = 0;
            g_GameSide1Value30 = (g_TitleInput.currentInput >> 14) & 1;
            g_TitleGlobalMode = 0;

            if (currentScreen == 3)
            {
                g_GameValueDB8 = 0;
                g_GameMode = 0;
                g_GameValueDF0 = 1;
                g_TitleLaunchState = 2;
            }
            else if (currentScreen == 5)
            {
                g_GameValueDB8 = 0;
                g_GameMode = 1;
                g_GameValueDF0 = 1;
                g_TitleLaunchState = 2;
            }
            else
            {
                switch (g_TitleModeSelection)
                {
                case 0:
                    g_GameValueDB8 = 0;
                    g_GameValueDF0 = 0;
                    g_GameMode = 2;
                    g_TitleLaunchState = 2;
                    break;
                case 1:
                    g_GameValueDB8 = 0;
                    g_GameMode = 2;
                    g_GameValueDF0 = 1;
                    g_TitleLaunchState = 2;
                    break;
                case 2:
                    g_GameValueDB8 = 1;
                    g_GameValueDF0 = 0;
                    g_GameMode = 2;
                    g_TitleLaunchState = 2;
                    break;
                case 3:
                    g_GameValueDB8 = 1;
                    g_GameMode = 2;
                    g_GameValueDF0 = 1;
                    g_TitleLaunchState = 2;
                    break;
                default:
                    break;
                }
            }

            g_TitleCharacterReturnSelection = g_TitleDifficulty;
            g_GameSide0CharacterSetting = side0CharacterSetting;
            g_GameSide1CharacterSetting = side1CharacterSetting;
            return 0;
        }

        if ((g_TitleInputFlags & 0xA) && side0CharacterConfirmed == 0)
        {
            g_TitleCharacterReturnSelection = keyboardSelection;
            PlayMenuSound(11, 0);
            stateTimer = 0;
            g_GameSide0Value20 = selectedCharacter;
            if (currentScreen == 3)
                ChangeCurrentScreen(2);
            else if (currentScreen == 5)
                ChangeCurrentScreen(4);
            else
                ChangeCurrentScreen(7);
            return 1;
        }
        break;
    }
    }

    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}


int TitleScreenView::UpdateScreen8Mode0()
{
    switch (currentScreenState)
    {
    case 0:
    {
        if (stateTimer2 == 0)
        {
            i32 value;

            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 13);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);

            {
                i32 visibleCount = 0;
                AnmVmView *visibleVm = &vms[191];
                for (value = 0; value < 16; value++)
                {
                    i32 character = g_TitleCharacterOrder[value];
                    if (g_TitleCharacterUnlocked[character] || g_OptionPointers[42])
                    {
                        titleAnm->SetSprite(visibleVm, character + 235);
                        visibleVm->flags |= 2;
                        ++visibleVm;
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
        i32 value;
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
                        SetCharacterCursorReverse(side0Character, 92, 16, 2);
                        SetCharacterCursorReverse(side0Character, 60, 16, 2);
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
                        SetCharacterCursorReverse(side1Character, 93, 16, 2);
                        SetCharacterCursorReverse(side1Character, 61, 16, 2);
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

int TitleScreenView::UpdateScreen8Mode123()
{
    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 13);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);

            i32 visibleCount = 0;
            for (i32 orderIndex = 0; orderIndex < 16; orderIndex++)
            {
                char character = g_TitleCharacterOrderMode123[orderIndex];
                if (g_TitleCharacterUnlocked[character])
                {
                    titleAnm->SetSprite(&vms[191 + visibleCount], character + 235);
                    vms[191 + visibleCount].flags |= 2;
                    visibleCount++;
                }
            }
            for (i32 index = visibleCount; index < 16; index++)
                vms[191 + index].flags &= ~2u;

            vms[189].flags |= 2;
            vms[190].flags &= ~2u;

            side0CharacterCursor = 0;
            while (g_TitleCharacterOrderMode123[side0CharacterCursor] != g_GameSide0Value20)
                side0CharacterCursor++;
            side1CharacterCursor = 0;
            while (g_TitleCharacterOrderMode123[side1CharacterCursor] != g_GameSide1Value20)
                side1CharacterCursor++;

            MoveCharacterCursor(0, 1, g_TitleCharacterOrderMode123, 16);
            MoveCharacterCursor(1, 1, g_TitleCharacterOrderMode123, 16);

            char side0Character = g_TitleCharacterOrderMode123[side0CharacterCursor];
            char side1Character = g_TitleCharacterOrderMode123[side1CharacterCursor];

            vms[92 + side0Character * 2].color1Bytes[0] = 0xFF;
            vms[92 + side0Character * 2].color1Bytes[1] = 0xFF;
            vms[92 + side0Character * 2].color1Bytes[2] = 0xFF;
            vms[60 + side0Character * 2].color1Bytes[0] = 0xFF;
            vms[60 + side0Character * 2].color1Bytes[1] = 0xFF;
            vms[60 + side0Character * 2].color1Bytes[2] = 0xFF;
            vms[93 + side1Character * 2].color1Bytes[0] = 0xFF;
            vms[93 + side1Character * 2].color1Bytes[1] = 0xFF;
            vms[93 + side1Character * 2].color1Bytes[2] = 0xFF;
            vms[61 + side1Character * 2].color1Bytes[0] = 0xFF;
            vms[61 + side1Character * 2].color1Bytes[1] = 0xFF;
            vms[61 + side1Character * 2].color1Bytes[2] = 0xFF;

            SetCharacterCursorActive(side0Character, 92, 16, 2);
            SetCharacterCursorActive(side1Character, 93, 16, 2);
            SetCharacterCursorActive(side0Character, 60, 16, 2);
            SetCharacterCursorActive(side1Character, 61, 16, 2);

            unknown0C918 = 1;
            side1CharacterConfirmed = 0;
            side0CharacterConfirmed = 0;
            currentScreenState = 0;
            stateTimer = 0;
            uiAux = 0;
            side0CharacterSetting = g_GameSide0CharacterSetting;
            side1CharacterSetting = g_GameSide1CharacterSetting;
            UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
            UpdateCharacterSelectionVisuals(
                0, side0Character, side0Character, g_TitleCharacterOrderMode123);
        }

        if (stateTimer2 == 8)
            currentScreenState = 1;
        break;

    case 1:
    {
        char side0Character = g_TitleCharacterOrderMode123[side0CharacterCursor];
        char side1Character = g_TitleCharacterOrderMode123[side1CharacterCursor];

        if (side0CharacterConfirmed == 0)
        {
            if (g_TitleInput.currentInput & 4)
            {
                if (g_TitleInput.IsPressedScrolling(0x40))
                {
                    if (side0CharacterSetting > 1)
                        side0CharacterSetting--;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
                if (g_TitleInput.IsPressedScrolling(0x80))
                {
                    if (side0CharacterSetting < 10)
                        side0CharacterSetting++;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
            }
            else
            {
                i32 direction = MoveCharacterCursorHorizontalForInput(2, 0, 16);
                if (direction != 0)
                {
                    MoveCharacterCursor(0, direction, g_TitleCharacterOrderMode123, 16);
                    side0Character = g_TitleCharacterOrderMode123[side0CharacterCursor];
                    side1Character = g_TitleCharacterOrderMode123[side1CharacterCursor];
                    if (direction > 0)
                    {
                        SetCharacterCursorActive(side0Character, 92, 16, 2);
                        SetCharacterCursorActive(side0Character, 60, 16, 2);
                    }
                    else
                    {
                        SetCharacterCursorReverse(side0Character, 92, 16, 2);
                        SetCharacterCursorReverse(side0Character, 60, 16, 2);
                    }
                    UpdateCharacterSelectionVisuals(
                        0, side0Character, side0Character, g_TitleCharacterOrderMode123);
                }
            }
        }
        else if (side1CharacterConfirmed == 0)
        {
            if (g_TitleInput.currentInput & 4)
            {
                if (g_TitleInput.IsPressedScrolling(0x40))
                {
                    if (side1CharacterSetting > 1)
                        side1CharacterSetting--;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
                if (g_TitleInput.IsPressedScrolling(0x80))
                {
                    if (side1CharacterSetting < 10)
                        side1CharacterSetting++;
                    UpdateCharacterSettings(side0CharacterSetting, side1CharacterSetting);
                }
            }
            else
            {
                i32 direction = MoveCharacterCursorHorizontalForInput(2, 1, 16);
                if (direction != 0)
                {
                    MoveCharacterCursor(1, direction, g_TitleCharacterOrderMode123, 16);
                    side0Character = g_TitleCharacterOrderMode123[side0CharacterCursor];
                    side1Character = g_TitleCharacterOrderMode123[side1CharacterCursor];
                    if (direction < 0)
                    {
                        SetCharacterCursorActive(side1Character, 93, 16, 2);
                        SetCharacterCursorActive(side1Character, 61, 16, 2);
                    }
                    else
                    {
                        SetCharacterCursorReverse(side1Character, 93, 16, 2);
                        SetCharacterCursorReverse(side1Character, 61, 16, 2);
                    }
                    UpdateCharacterSelectionVisuals(
                        1, side1Character, side0Character, g_TitleCharacterOrderMode123);
                }
            }
        }

        if ((g_TitleInputFlags & 0x1001) && side0CharacterConfirmed == 0)
        {
            PlayMenuSound(10, 0);
            side0CharacterConfirmed = 1;
            vms[92 + side0Character * 2].color1Bytes[0] = 0x80;
            vms[92 + side0Character * 2].color1Bytes[1] = 0x80;
            vms[92 + side0Character * 2].color1Bytes[2] = 0x80;
            vms[60 + side0Character * 2].color1Bytes[0] = 0x80;
            vms[60 + side0Character * 2].color1Bytes[1] = 0x80;
            vms[60 + side0Character * 2].color1Bytes[2] = 0x80;
            g_GameSide0Value30 = (g_TitleInput.currentInput >> 2) & 1;
            if (side1CharacterConfirmed && side0Character == side1Character)
                g_GameSide0Value30 = 1 - g_GameSide1Value30;
            vms[190].flags |= 2;
            UpdateCharacterSelectionVisuals(
                1, side1Character, side0Character, g_TitleCharacterOrderMode123);
        }
        else if ((g_TitleInputFlags & 0x1001) && side1CharacterConfirmed == 0)
        {
            PlayMenuSound(10, 0);
            side1CharacterConfirmed = 1;
            vms[93 + side1Character * 2].color1Bytes[0] = 0x80;
            vms[93 + side1Character * 2].color1Bytes[1] = 0x80;
            vms[93 + side1Character * 2].color1Bytes[2] = 0x80;
            vms[61 + side1Character * 2].color1Bytes[0] = 0x80;
            vms[61 + side1Character * 2].color1Bytes[1] = 0x80;
            vms[61 + side1Character * 2].color1Bytes[2] = 0x80;
            g_GameSide1Value30 = (g_TitleInput.currentInput >> 2) & 1;
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

        if ((g_TitleInputFlags & 0xA) && side0CharacterConfirmed == 0)
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

        if (g_TitleInputFlags & 0xA)
        {
            if (side1CharacterConfirmed)
            {
                side1CharacterConfirmed = 0;
                PlayMenuSound(11, 0);
                vms[93 + side1Character * 2].color1Bytes[0] = 0xFF;
                vms[93 + side1Character * 2].color1Bytes[1] = 0xFF;
                vms[93 + side1Character * 2].color1Bytes[2] = 0xFF;
                vms[61 + side1Character * 2].color1Bytes[0] = 0xFF;
                vms[61 + side1Character * 2].color1Bytes[1] = 0xFF;
                vms[61 + side1Character * 2].color1Bytes[2] = 0xFF;
            }
            else if (side0CharacterConfirmed)
            {
                side0CharacterConfirmed = 0;
                PlayMenuSound(11, 0);
                vms[92 + side0Character * 2].color1Bytes[0] = 0xFF;
                vms[92 + side0Character * 2].color1Bytes[1] = 0xFF;
                vms[92 + side0Character * 2].color1Bytes[2] = 0xFF;
                vms[60 + side0Character * 2].color1Bytes[0] = 0xFF;
                vms[60 + side0Character * 2].color1Bytes[1] = 0xFF;
                vms[60 + side0Character * 2].color1Bytes[2] = 0xFF;
                vms[190].flags &= ~2u;
                UpdateCharacterSelectionVisuals(
                    0, side0Character, side0Character, g_TitleCharacterOrderMode123);
            }
        }
        break;
    }
    }

    stateTimer++;
    screenFrameCounter++;
    stateTimer2++;
    return 1;
}


int TitleScreenView::UpdateScreen16()
{
    switch (currentScreenState)
    {
    case 0:
        if (stateTimer2 == 0)
        {
            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 26);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);

            i32 visibleCount = 0;
            for (i32 orderIndex = 0; orderIndex < 16; orderIndex++)
            {
                char character = g_TitleScreen16Order[orderIndex];
                if (character > 13 || g_TitleCharacterUnlocked[character] || g_OptionPointers[42])
                {
                    titleAnm->SetSprite(&vms[207 + visibleCount], character + 251);
                    vms[207 + visibleCount].flags |= 2;
                    g_TitleScreen16Entries[visibleCount] = character;
                    visibleCount++;
                }
            }

            keyboardSelection = visibleCount - 2;
            for (i32 index = visibleCount; index < 16; index++)
            {
                vms[207 + index].flags &= ~2u;
                g_TitleScreen16Entries[index] = -1;
            }

            vms[189].flags |= 2;
            vms[190].flags |= 2;
            currentScreenState = 0;
            stateTimer = 0;
            uiAux = 0;
            unknown0C918 = 1;
            UpdateScreen16SelectionVisuals(0, g_TitleScreen16Entries[keyboardSelection],
                                           g_TitleScreen16Order);
        }

        if (stateTimer2 == 8)
            currentScreenState = 1;
        break;

    case 1:
    {
        i32 direction = MoveCursorFourWay(16);
        if (direction != 0)
        {
            while (g_TitleScreen16Entries[keyboardSelection] < 0)
                keyboardSelection = (direction + keyboardSelection) % 16;
            UpdateScreen16SelectionVisuals(0, g_TitleScreen16Entries[keyboardSelection],
                                           g_TitleScreen16Order);
        }

        if (g_TitleInput.currentInput & 0x100)
        {
            keyboardSelection += g_TitleSelectionRandom.NextU32() & 0xF;
            if (keyboardSelection >= 16)
                keyboardSelection -= 16;
            UpdateScreen16SelectionVisuals(0, g_TitleScreen16Entries[keyboardSelection],
                                           g_TitleScreen16Order);
        }

        if (g_TitleInputFlags & 0x1001)
        {
            PlayMenuSound(10, 0);
            g_TitleSupervisor.StopAudio();
            g_TitleGlobalMode = 0;

            if (currentScreen == 3)
            {
                g_GameValueDF0 = 1;
                g_TitleLaunchState = 2;
                g_GameMode = 0;
                g_GameValueDB8 = 0;
            }
            else if (currentScreen == 5)
            {
                g_GameValueDF0 = 1;
                g_TitleLaunchState = 2;
                g_GameMode = 1;
                g_GameValueDB8 = 0;
            }
            else
            {
                switch (g_TitleModeSelection)
                {
                case 0:
                case 4:
                    g_GameValueDF0 = 0;
                    g_TitleLaunchState = 2;
                    g_GameMode = 2;
                    g_GameValueDB8 = 0;
                    break;
                case 1:
                    g_GameValueDF0 = 1;
                    g_TitleLaunchState = 2;
                    g_GameMode = 2;
                    g_GameValueDB8 = 0;
                    break;
                case 2:
                    g_GameValueDF0 = 0;
                    g_GameValueDB8 = 1;
                    g_TitleLaunchState = 2;
                    g_GameMode = 2;
                    break;
                case 3:
                    g_GameValueDF0 = 1;
                    g_GameValueDB8 = 1;
                    g_TitleLaunchState = 2;
                    g_GameMode = 2;
                    break;
                default:
                    break;
                }
            }

            g_TitleCharacterReturnSelection = g_TitleDifficulty;
            g_GameSide0CharacterSetting = side0CharacterSetting;
            g_GameSide1CharacterSetting = side1CharacterSetting;

            g_TitleScreen16SelectionResult = g_TitleScreen16Entries[keyboardSelection];
            if (g_TitleScreen16SelectionResult == 14)
                g_TitleScreen16SelectionResult = -1;
            else if (g_TitleScreen16SelectionResult == 15)
                g_TitleScreen16SelectionResult = -2;
            else
                g_TitleScreen16SelectionResult = g_TitleScreen16Entries[keyboardSelection];
            return 0;
        }

        if (g_TitleInputFlags & 0xA)
        {
            PlayMenuSound(11, 0);
            stateTimer = 0;
            ChangeCurrentScreen(8);
            if (g_TitleModeSelection == 0 || g_TitleModeSelection == 4)
                UpdateScreen8Mode0();
            else if (g_TitleModeSelection >= 1 && g_TitleModeSelection <= 3)
                UpdateScreen8Mode123();
            return 1;
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
            g_TitleSupervisor.StopAudio();
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


int TitleScreenView::OnUpdateResult()
{
    switch (currentScreenState)
    {
    case 0:
        if (phaseTimer == 0)
        {
            if (g_TitleAnmManager->LoadSurface(0, "title/result00.png"))
                return 0;

            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 16);
            g_TitleAnmManager->ExecuteScriptArray(vms, vmCount);
            keyboardSelection = 0;
            nameBankIndex = 0;
            SetCharacterCursorInactive(g_TitleCharacterOrder14[0], 60, 16, 2);
            currentScreenState = 0;
            stateTimer = 0;
            currentHelpTextVm = 0;
            nameSlotIndex = 0;
        }

        if (phaseTimer == 8)
            currentScreenState = 1;
        break;

    case 1:
    {
        if (g_TitleResultUnlockStep < 3)
        {
            if (g_TitleInputFlags & 0x2000)
                g_TitleResultUnlockStep++;
            else if ((u16)g_TitleInputFlags)
                g_TitleResultUnlockStep = 0;
        }
        else if (g_TitleResultUnlockStep < 7)
        {
            if (g_TitleInputFlags & 0x400)
                g_TitleResultUnlockStep++;
            else if ((u16)g_TitleInputFlags)
                g_TitleResultUnlockStep = 0;
        }
        else if (g_TitleResultUnlockStep < 8)
        {
            if (g_TitleInputFlags & 0x200)
                g_TitleResultUnlockStep++;
            else if ((u16)g_TitleInputFlags)
                g_TitleResultUnlockStep = 0;
        }
        else if (g_TitleResultUnlockStep < 10)
        {
            if (g_TitleInputFlags & 0x4000)
                g_TitleResultUnlockStep++;
            else if ((u16)g_TitleInputFlags)
                g_TitleResultUnlockStep = 0;
        }

        if (g_TitleResultUnlockStep >= 10)
        {
            for (i32 i = 0; i < 16; i++)
                g_TitleCharacterUnlocked[(char)i] = 1;
            for (i32 i = 0; i < 14; i++)
            {
                g_TitleCharacterUnlockedNormal[(char)i] = 1;
                g_TitleCharacterUnlockedMode4[(char)i] = 1;
            }
            g_SoundPlayer.PlaySoundByIdx(28, 0);
            g_TitleResultUnlockStep = 0;
        }

        i32 direction = MoveCursorHorizontal(14);
        if (direction != 0)
        {
            while (!g_TitleCharacterUnlockedNormal[keyboardSelection] &&
                   !g_TitleCharacterUnlockedMode4[keyboardSelection])
            {
                keyboardSelection += direction;
                if (keyboardSelection < 0)
                    keyboardSelection += 16;
                if (keyboardSelection >= 16)
                    keyboardSelection -= 16;
            }

            char character = g_TitleCharacterOrder14[keyboardSelection];
            SetCharacterCursorInactive(character, 60, 16, 2);
            nameBankIndex = character;
        }

        if (g_TitleInputFlags & 0xA)
        {
            PlayMenuSound(11, 0);
            stateTimer = 0;
            ChangeCurrentScreen(1);
            keyboardSelection = 4;
        }
        break;
    }
    }

    stateTimer++;
    screenFrameCounter++;
    phaseTimer++;
    return 1;
}


int TitleScreenView::OnUpdateReplayMenu()
{
    char fullPath[1024];
    WIN32_FIND_DATAA findData;
    char path[64];
    i32 fileSize;
    HANDLE findHandle;

    switch (currentScreenState)
    {
    case 0:
        if (phaseTimer == 0)
        {
            g_TitleGameFlags &= ~8u;
            if (previousScreen != 11 && g_TitleAnmManager->LoadSurface(0, "title/replay00.png"))
                return 0;

            g_TitleAnmManager->SetInterruptArray(vms, vmCount, 22);
            keyboardSelection = 0;
            currentScreenState = 0;
            stateTimer = 0;
            uiAux = 0;
            memset(replays, 0, sizeof(replays));

            for (i32 replayIndex = 0; replayIndex < 25; replayIndex++)
            {
                sprintf(path, "./replay/th9_%.2d.rpy", replayIndex + 1);
                ReplayDataView *fileData =
                    (ReplayDataView *)FileSystemView::OpenFile(path, &fileSize, 1);
                if (fileData != 0)
                {
                    ReplayDataView *loadedReplay =
                        ReplayManagerView::LoadReplayData(fileData, fileSize);
                    replayAllocations[replayIndex] = loadedReplay;
                    if (loadedReplay != 0)
                    {
                        replays[replayIndex] = *loadedReplay;
                        strcpy(replayPaths[replayIndex], path);
                    }
                }
            }

            _mkdir("replay");
            _chdir("replay");
            findHandle = FindFirstFileA("th9_ud????.rpy", &findData);
            if (findHandle != INVALID_HANDLE_VALUE)
            {
                for (i32 replayIndex = 0; replayIndex < 25; replayIndex++)
                {
                    strcpy(fullPath, "replay/");
                    strcat(fullPath, findData.cFileName);
                    ReplayDataView *fileData =
                        (ReplayDataView *)FileSystemView::OpenFile(fullPath, &fileSize, 1);
                    if (fileData == 0)
                        continue;

                    ReplayDataView *loadedReplay =
                        ReplayManagerView::LoadReplayData(fileData, fileSize);
                    replayAllocations[25 + replayIndex] = loadedReplay;
                    if (loadedReplay != 0)
                    {
                        replays[25 + replayIndex] = *loadedReplay;
                        sprintf(replayPaths[25 + replayIndex], "./replay/%s", findData.cFileName);
                    }

                    if (!FindNextFileA(findHandle, &findData))
                        break;
                }
            }
            FindClose(findHandle);
            _chdir("../");
            replaySlotCount = 50;
            replayEnumerationResetState = 0;
        }

        if (phaseTimer >= 8)
        {
            currentScreenState = 1;
            stateTimer = 0;
        }
        break;

    case 1:
        if (g_TitleInput.IsPressedScrolling(0x10))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            if (--keyboardSelection < 0)
                keyboardSelection += 50;
            stateTimer = 0;
        }
        if (g_TitleInput.IsPressedScrolling(0x20))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            if (++keyboardSelection >= 50)
                keyboardSelection -= 50;
            stateTimer = 0;
        }
        if (g_TitleInput.IsPressedScrolling(0x40))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            keyboardSelection -= 25;
            if (keyboardSelection < 0)
                keyboardSelection += 50;
            stateTimer = 0;
        }
        if (g_TitleInput.IsPressedScrolling(0x80))
        {
            g_SoundPlayer.PlaySoundByIdx(12, 0);
            keyboardSelection += 25;
            if (keyboardSelection >= 50)
                keyboardSelection -= 50;
            stateTimer = 0;
        }

        if (g_TitleInputFlags & 0xA)
        {
            PlayMenuSound(11, 0);
            stateTimer = 0;
            ChangeCurrentScreen(1);
            keyboardSelection = 3;
            for (i32 replayIndex = 0; replayIndex < 50; replayIndex++)
            {
                if (replayAllocations[replayIndex] != 0)
                    free(replayAllocations[replayIndex]);
                replayAllocations[replayIndex] = 0;
            }
            return 1;
        }

        if (g_TitleInputFlags & 0x1001)
        {
            if (replays[keyboardSelection].magic != 0)
            {
                PlayMenuSound(10, 0);
                selectedReplay = keyboardSelection;
                currentScreenState = 2;
                stateTimer = 0;
                keyboardSelection = 0;
                phaseTimer = 0;
                while (replays[selectedReplay].frameStart[0][keyboardSelection] == 0)
                    keyboardSelection++;
            }
            else
            {
                PlayMenuSound(39, 0);
            }
        }
        break;

    case 2:
    {
        i32 direction = MoveCursorVertical(10);
        if (direction != 0)
        {
            while (replays[selectedReplay].frameStart[0][keyboardSelection] == 0)
            {
                keyboardSelection += direction;
                if (keyboardSelection >= 10)
                    keyboardSelection -= 10;
                if (keyboardSelection < 0)
                    keyboardSelection += 10;
            }
        }

        if (g_TitleInputFlags & 0xA)
        {
            currentScreenState = 1;
            stateTimer = 0;
            keyboardSelection = selectedReplay;
            break;
        }

        if (g_TitleInputFlags & 0x1001)
        {
            g_GameSide0Value20 = side0CharacterCursor;
            g_GameSide1Value20 = 0;
            g_GameCurrentStage = keyboardSelection;
            g_TitleNameTableIndex = replays[selectedReplay].value0D7;
            strcpy(g_SelectedReplayPath, replayPaths[selectedReplay]);
            g_TitleGameFlags |= 8u;

            if (replays[selectedReplay].value1E4 == 0)
            {
                g_GameValueDF0 = 1;
                g_TitleLaunchState = 2;
                g_GameMode = 0;
                g_GameValueDB8 = 0;
            }
            else if (replays[selectedReplay].value1E4 == 1)
            {
                g_GameValueDF0 = 1;
                g_TitleLaunchState = 2;
                g_GameMode = 1;
                g_GameValueDB8 = 0;
            }
            else
            {
                switch (replays[selectedReplay].value1E5)
                {
                case 1:
                    g_GameValueDF0 = 1;
                    g_TitleLaunchState = 2;
                    g_GameMode = 2;
                    g_GameValueDB8 = 0;
                    break;
                case 2:
                    g_GameValueDF0 = 0;
                    g_GameValueDB8 = 1;
                    g_TitleLaunchState = 2;
                    g_GameMode = 2;
                    break;
                case 3:
                    g_GameValueDF0 = 1;
                    g_GameValueDB8 = 1;
                    g_TitleLaunchState = 2;
                    g_GameMode = 2;
                    break;
                case 0:
                case 4:
                    g_GameValueDF0 = 0;
                    g_TitleLaunchState = 2;
                    g_GameMode = 2;
                    g_GameValueDB8 = 0;
                    break;
                default:
                    break;
                }
            }

            for (i32 replayIndex = 0; replayIndex < 50; replayIndex++)
            {
                if (replayAllocations[replayIndex] != 0)
                    free(replayAllocations[replayIndex]);
                replayAllocations[replayIndex] = 0;
            }
            g_TitleNameTableIndex = g_TitleDifficulty;
            return 0;
        }
        break;
    }
    }

    replayUiFrameCounter++;
    stateTimer++;
    phaseTimer++;
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

int TitleSupervisorView::ClearRecordingFpsWarningState()
{
    this->recordingFpsWarning77C = 0;
    this->recordingFpsState784 = 0;
    this->recordingFpsState790 = 0;
    this->recordingFpsState788 = 0;
    this->recordingFpsState78C = 0;
    return 0;
}
