#pragma once

#include "AsciiManager.hpp"
#include <stddef.h>

// Effect template 34 owns this original 0xEF8-byte allocation. The fan,
// line groups and random rows are fixed by the original draw/update accesses.
// The final 33rd random float in each row is not written by the update.
struct VertexDiffuseXyzrhw
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
};
struct EffectType34VerticesView
{
    int state00;
    unsigned char unknown04[0x4C - 4];
    unsigned int unknown4C;
    float width50;
    VertexDiffuseXyzrhw fan54[33];
    VertexDiffuseXyzrhw lines2E8[4][32];
    float jitterCE8[4][33];
};
typedef char EffectType34BufferSize[(sizeof(EffectType34VerticesView)==0xEF8)?1:-1];
typedef char EffectType34JitterOffset[(offsetof(EffectType34VerticesView,jitterCE8)==0xCE8)?1:-1];
typedef char EffectType34FanAt54[
    (offsetof(EffectType34VerticesView, fan54) == 0x54) ? 1 : -1];
typedef char EffectType34LinesAt2E8[
    (offsetof(EffectType34VerticesView, lines2E8) == 0x2E8) ? 1 : -1];
