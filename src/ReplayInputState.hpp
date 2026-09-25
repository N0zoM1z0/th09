#pragma once

#include <stddef.h>

struct ReplayInputState
{
    unsigned short currentInput;
    unsigned short word02;
    unsigned short repeatOutput;
    unsigned short word06;
    unsigned short word08;
    unsigned char unknown0A[0x20];
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
};

typedef char ReplayInputStateSizeIs8E[
    (sizeof(ReplayInputState) == 0x8E) ? 1 : -1];
typedef char ReplayInputAuxAt2A[
    (offsetof(ReplayInputState, auxiliary2A) == 0x2A) ? 1 : -1];
typedef char ReplayInputHistoryAt2C[
    (offsetof(ReplayInputState, historyCurrent) == 0x2C) ? 1 : -1];

extern ReplayInputState g_ReplayInputStates[3];
