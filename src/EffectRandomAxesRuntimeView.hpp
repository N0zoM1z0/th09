#pragma once

#include "EffectManager.hpp"
#include "AsciiManager.hpp"

#include <stddef.h>

// Original Effect template families 1/2 and 3/4 share this runtime prefix.
// The original C++ names and the full lifetime model remain unknown.
struct EffectRandomAxesRuntimeView
{
    unsigned char unknown00[0x0C];
    EffectFloat3 position0C;
    EffectFloat3 velocity18;
    unsigned char unknown24[0x3C - 0x24];
    Float3 axis3C;
    Float3 axis48;
    EffectFloat3 base54;
    EffectFloat3 transformed60;
    unsigned char unknown6C[0x98 - 0x6C];
    float delay98;
    float duration9C;
    float speedA0;
    union {
        short bulletTypeA4;
        short subroutineA4;
    };
    short colorA6;
    short sideIndexA8;
    short levelAA;
};

typedef char EffectAxesDelayAt98[
    (offsetof(EffectRandomAxesRuntimeView, delay98) == 0x98) ? 1 : -1];
typedef char EffectAxesLevelAtAA[
    (offsetof(EffectRandomAxesRuntimeView, levelAA) == 0xAA) ? 1 : -1];
