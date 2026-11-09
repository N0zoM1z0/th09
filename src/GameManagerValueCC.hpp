#pragma once

#include "GameManagerRuntimeView.hpp"

#include <stddef.h>

// The original value operation uses this 0x20-byte window rooted at the
// actual constructed GameManager +0xB0. It writes the real +0xCC dword.
// The historic nominal owner type remains unproven; no separate allocation.
struct GameManagerValueCCView
{
    unsigned char unknown00[0x1C];
    int value1C;

    void SubtractSaturating(int amount);
};

typedef char GameManagerValueCCNativeOffsets[
    (sizeof(GameManagerValueCCView) == 0x20 &&
     offsetof(GameManagerValueCCView, value1C) == 0x1C &&
     offsetof(AsciiGameManagerView, valueB0) == 0xB0 &&
     offsetof(AsciiGameManagerView, valueCC) == 0xCC) ? 1 : -1];
