#pragma once
#include "ReplayDataOriginalView.hpp"
#include "AnmManager.hpp"
#include "Chain.hpp"
#include <stddef.h>

// One genuine 0x1B388-byte Title object also backs the two replay draw
// callbacks. Original nominal class spelling remains source-facing.
typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;
// The complete source native ANM loaded resource adds no storage.
struct TitleAnmView : AnmLoaded {};
struct TitleFloat3View;

struct AnmVmView : AnmVm {
    AnmVmView();
    char ConfigurePositionInterpolation(i32 duration, char mode,
                                        TitleFloat3View *start, TitleFloat3View *end);
    int ConfigureColorInterpolation(i32 duration, char mode,
                                    u32 startColor, u32 endColor);
};

// Original Title options store a 0xCC-byte serialized GameConfiguration
// snapshot. Keep it POD: the real GameConfiguration has a constructor and
// would alter TitleScreen construction if embedded as an object.
struct TitleConfigSnapshotView {
    union {
        u32 words[0x33];
        struct {
            u8 unknown00[0xAD];
            u8 valueAD;
            u8 unknownAE[0xB1-0xAE];
            u8 windowed;
            u8 unknownB2[0xCC-0xB2];
        };
    };
};

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
    union { i32 replayUiFrameCounter; i32 screenFrameCounter; i32 optionsFrameCounter; }; // +0x0C910
    u8 unknown0C914[4];
    union { i32 unknown0C918; i32 optionsUiReady; }; // +0x0C918
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
    void SetCharacterCursorActive(i32 selectedCharacter, i32 start, i32 count, i32 stride);
    void SetCharacterCursorInactive(i32 selectedCharacter, i32 start, i32 count, i32 stride);
    void SetCharacterCursorReverse(i32 selectedCharacter, i32 start, i32 count, i32 stride);
    int SetCharacterSettingSprites(i32 side0Setting, i32 side1Setting);
    int UpdateCharacterSettings(i32 side0Setting, i32 side1Setting);
    int SetCharacterSettingIndicator(i32 side, i32 value);
    void UpdateCharacterSelectionVisuals(i32 side, i32 selectedCharacter, i32 otherCharacter, char *order);
    void UpdateScreen16SelectionVisuals(i32 unusedSide, i32 selectedCharacter, char *order);
    int UpdateScreen8Mode0();
    int UpdateScreen8Mode123();
    int UpdateScreen16();
    int DrawReplayMenu();
    int DrawReplaySave();
    int DrawMusicRoom();
    int DrawResult();
};



// The main Title runtime and both draw callbacks independently consume these
// byte coordinates. A different local overlay may not silently reinterpret
// either the 0x1EC-byte serialized replay entries or the 0x200-byte paths.
typedef char TitleReplaySharedCoordinates[
    (offsetof(TitleScreenView, replayNameCursor) == 0x04 &&
     offsetof(TitleScreenView, replaySaveState) == 0x28 &&
     offsetof(TitleScreenView, currentScreenState) == 0x28 &&
     offsetof(TitleScreenView, stateTimer) == 0x2C &&
     offsetof(TitleScreenView, replayName) == 0x8C &&
     offsetof(TitleScreenView, replayPaths) == 0x28C &&
     sizeof(((TitleScreenView *)0)->replayPaths[0]) == 0x200 &&
     offsetof(TitleScreenView, replays) == 0x68E4 &&
     sizeof(((TitleScreenView *)0)->replays[0]) == 0x1EC &&
     offsetof(TitleScreenView, selectedReplay) == 0xC908 &&
     offsetof(TitleScreenView, stateTimer2) == 0x1B230 &&
     sizeof(TitleScreenView) == 0x1B388) ? 1 : -1];

// Complete Player/Menu/Replay/VM/Game config physical Title owner extent.
typedef char TitleNativeOptionsSnapshotFields[
    (sizeof(TitleConfigSnapshotView) == 0xCC &&
     offsetof(TitleConfigSnapshotView, valueAD) == 0xAD &&
     offsetof(TitleConfigSnapshotView, windowed) == 0xB1 &&
     offsetof(TitleScreenView, optionsFrameCounter) == 0xC910 &&
     offsetof(TitleScreenView, optionsUiReady) == 0xC918) ? 1 : -1];

typedef char TitleReplayNativeFullOwnerExtent[
    (sizeof(TitleScreenView) == 0x1B388 &&
     offsetof(TitleScreenView, replayAllocations) == 0x681C &&
     offsetof(TitleScreenView, replays) == 0x68E4 &&
     offsetof(TitleScreenView, currentReplay) == 0xC8FC &&
     offsetof(TitleScreenView, selectedReplay) == 0xC908 &&
     offsetof(TitleScreenView, embeddedVms) == 0x11BA0 &&
     offsetof(TitleScreenView, stateTimer2) == 0x1B230 &&
     offsetof(TitleScreenView, configSnapshot) == 0x1B2BC) ? 1 : -1];
