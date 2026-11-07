#pragma once

#include "ReplayDataOriginalView.hpp"
#include <stddef.h>

// The replay menu and replay-save renderer address the same Title allocation.
// Keep the *shared* replay-related prefix of that allocation in one view:
// original class spelling/complete trailing owner is not asserted here.
struct TitleScreenView
{
    int keyboardSelection;                     // +0x00000
    int replayNameCursor;                      // +0x00004
    unsigned char unknown00008[0x20];
    union { int replaySaveState; int currentScreenState; }; // +0x00028
    int stateTimer;                            // +0x0002C
    unsigned char unknown00030[0x5C];
    char replayName[9];                        // +0x0008C
    unsigned char unknown00095[0x28C - 0x95];
    char replayPaths[50][0x200];              // +0x0028C
    unsigned char unknown0668C[0x68E4 - 0x668C];
    ReplayDataView replays[50];               // +0x068E4
    unsigned char unknown0C8FC[0x0C];
    int selectedReplay;                        // +0x0C908
    unsigned char unknown0C90C[0x1B230 - 0xC90C];
    int stateTimer2;                           // +0x1B230

    int DrawReplayMenu();
    int DrawReplaySave();
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
     sizeof(TitleScreenView) == 0x1B234) ? 1 : -1];
