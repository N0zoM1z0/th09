#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;

struct ReplayInputFrontState
{
    u16 current00;
    u16 previous02;
    u16 repeat04;
    u16 pressed06;
    u16 released08;
    u16 heldFrames0A[16];
    u8 unknown2A[0x8E - 0x2A];
};

typedef char ReplayInputFrontStateSizeIs8E[
    (sizeof(ReplayInputFrontState) == 0x8E) ? 1 : -1];

extern ReplayInputFrontState g_ReplayInputStates[3];

int __fastcall ApplyNetworkInput(int stateIndex, int input)
{
    ReplayInputFrontState *state = &g_ReplayInputStates[stateIndex];
    state->previous02 = state->current00;
    state->current00 = (u16)input;

    u16 repeatBit = 1;
    u16 shifted = (u16)input;
    state->repeat04 = 0;
    u16 *held = state->heldFrames0A;
    int remaining = 16;

    do
    {
        if ((shifted & 1) != 0)
        {
            ++*held;
            if (*held >= 26)
            {
                state->repeat04 |= repeatBit;
                *held -= 8;
            }
        }
        else
        {
            *held = 0;
        }

        shifted >>= 1;
        ++held;
        repeatBit <<= 1;
        --remaining;
    }
    while (remaining != 0);

    state->pressed06 =
        (state->current00 ^ state->previous02) & state->current00;
    state->released08 =
        (state->current00 ^ state->previous02) & ~state->current00;
    return input;
}
