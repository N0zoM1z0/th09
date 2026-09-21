#include <stddef.h>

typedef unsigned short u16;

struct ReplayInputState
{
    u16 currentInput;
    u16 word02;
    u16 repeatOutput;
    u16 word06;
    u16 word08;
    unsigned char unknown0A[0x20];
    u16 auxiliary2A;
    u16 historyCurrent;
    u16 historyPrevious;
    u16 historyRepeat;
    u16 historyPressed;
    u16 historyReleased;
    u16 unknown36;
    u16 heldFrames[16];
    short keyCodes58[27];

    u16 IsHeld(u16 mask);
};

typedef char ReplayInputIsHeldHistoryAt2C[
    (offsetof(ReplayInputState, historyCurrent) == 0x2C) ? 1 : -1];

u16 ReplayInputState::IsHeld(u16 mask)
{
    return historyCurrent & mask;
}
