#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;

#include "ReplayInputState.hpp"

// This source callback writes the original native ReplayInputState's
// network-specific counter window at +0x0A, not Replay heldFrames at +0x38.
typedef char ApplyNetworkInputNativeOwner[
    (sizeof(ReplayInputState) == 0x8E &&
     offsetof(ReplayInputState, networkHeldFrames0A) == 0x0A &&
     offsetof(ReplayInputState, heldFrames) == 0x38) ? 1 : -1];

int __fastcall ApplyNetworkInput(int stateIndex, int input)
{
    ReplayInputState *state = reinterpret_cast<ReplayInputState *>(
        reinterpret_cast<unsigned char *>(&g_ReplayInputStates[0]) +
        static_cast<unsigned int>(stateIndex) * sizeof(g_ReplayInputStates[0]));
    state->networkPrevious02 = state->currentInput;
    state->currentInput = (u16)input;

    u16 repeatBit = 1;
    u16 shifted = (u16)input;
    state->networkRepeat04 = 0;
    u16 *held = state->networkHeldFrames0A;
    int remaining = 16;

    do
    {
        if ((shifted & 1) != 0)
        {
            ++*held;
            if (*held >= 26)
            {
                state->networkRepeat04 |= repeatBit;
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

    state->networkPressed06 =
        (state->currentInput ^ state->networkPrevious02) & state->currentInput;
    state->networkReleased08 =
        (state->currentInput ^ state->networkPrevious02) & ~state->currentInput;
    return input;
}
