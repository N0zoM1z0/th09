#pragma once

#include "ReplayInputState.hpp"
#include <stddef.h>

// Title's pointer at 0x4ACF34 is the last 0x8E-byte element of the physical
// replay input array at 0x4ACE18. Its independently exact scrolling method
// keeps its original InputView ABI, while sharing native input storage.
struct InputView : ReplayInputState
{
    unsigned short IsPressedScrolling(int mask);
};
typedef char TitleInputNativeReplayStorage[
    (sizeof(InputView) == sizeof(ReplayInputState) &&
     sizeof(InputView) == 0x8E &&
     offsetof(InputView, currentInput) == 0x00 &&
     offsetof(InputView, word02) == 0x02 &&
     offsetof(InputView, repeatOutput) == 0x04 &&
     offsetof(InputView, word06) == 0x06) ? 1 : -1];

extern InputView g_TitleInput;
