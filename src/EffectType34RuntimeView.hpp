#pragma once

#include <stddef.h>

// Effect template 34's original 0xEF8-byte allocation. Only the state,
// width and four strided random-offset rows are identified by its update.
// The 33rd float in each row is not written by that callback.
struct EffectType34VerticesView
{
    int state00;
    unsigned char unknown04[0x50 - 0x04];
    float width50;
    unsigned char unknown54[0xCE8 - 0x54];
    float jitterCE8[4][33];
};
typedef char EffectType34VerticesSizeIsEF8[
    (sizeof(EffectType34VerticesView) == 0xEF8) ? 1 : -1];
typedef char EffectType34JitterAtCE8[
    (offsetof(EffectType34VerticesView, jitterCE8) == 0xCE8) ? 1 : -1];

