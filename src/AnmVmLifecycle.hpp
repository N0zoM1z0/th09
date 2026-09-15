#pragma once

#include "ZunTimer.hpp"

#include <stddef.h>

struct AnmVmBase
{
    AnmVmBase();

    unsigned char unknown000[0x38];
    ZunTimer currentTimeInScript;
    ZunTimer waitTimer;
    ZunTimer interpolationCurrentTimers[7];
    ZunTimer interpolationEndTimers[7];
    unsigned char unknown0F8[0x228 - 0x0F8];
};

typedef char AnmVmBaseSizeIs228[(sizeof(AnmVmBase) == 0x228) ? 1 : -1];

struct AnmVm : AnmVmBase
{
    AnmVm();
    void SetInterrupt(short interrupt);

    ZunTimer interruptReturnTime;
    unsigned char unknown234[0x2A4 - 0x234];
};

typedef char AnmVmLifecycleSizeIs2A4[(sizeof(AnmVm) == 0x2A4) ? 1 : -1];
