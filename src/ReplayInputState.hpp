#pragma once

#include <stddef.h>

struct ReplayInputState
{
    unsigned short currentInput;
    union { unsigned short word02; unsigned short networkPrevious02; };
    union { unsigned short repeatOutput; unsigned short networkRepeat04; };
    union { unsigned short word06; unsigned short networkPressed06; };
    union { unsigned short word08; unsigned short networkReleased08; };
    // The network input path uses 16 word counters in this *first* +0x20
    // window. Replay/Controller owners independently use another set of
    // heldFrames[16] at +0x38. Do not confuse these two physical arrays.
    union {
        unsigned char unknown0A[0x20];
        unsigned short networkHeldFrames0A[16];
    };
    unsigned short auxiliary2A;
    unsigned short historyCurrent;
    unsigned short historyPrevious;
    unsigned short historyRepeat;
    unsigned short historyPressed;
    unsigned short historyReleased;
    unsigned short unknown36;
    unsigned short heldFrames[16];
    short keyCodes58[27];

    ReplayInputState();
    void Update();
    unsigned short IsHeld(unsigned short mask);
    unsigned short WasPressed(unsigned short buttons);
};

typedef char ReplayInputStateSizeIs8E[
    (sizeof(ReplayInputState) == 0x8E) ? 1 : -1];
typedef char ReplayInputNativeNetworkInputFields[
    (offsetof(ReplayInputState, currentInput) == 0x00 &&
     offsetof(ReplayInputState, networkPrevious02) == 0x02 &&
     offsetof(ReplayInputState, networkRepeat04) == 0x04 &&
     offsetof(ReplayInputState, networkPressed06) == 0x06 &&
     offsetof(ReplayInputState, networkReleased08) == 0x08 &&
     offsetof(ReplayInputState, networkHeldFrames0A) == 0x0A &&
     sizeof(((ReplayInputState *)0)->networkHeldFrames0A) == 0x20 &&
     offsetof(ReplayInputState, heldFrames) == 0x38) ? 1 : -1];
typedef char ReplayInputAuxAt2A[
    (offsetof(ReplayInputState, auxiliary2A) == 0x2A) ? 1 : -1];
typedef char ReplayInputHistoryAt2C[
    (offsetof(ReplayInputState, historyCurrent) == 0x2C) ? 1 : -1];
typedef char ReplayInputPressedAt32[
    (offsetof(ReplayInputState, historyPressed) == 0x32) ? 1 : -1];

extern ReplayInputState g_ReplayInputStates[3];
