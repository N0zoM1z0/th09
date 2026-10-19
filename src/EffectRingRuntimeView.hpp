#pragma once

#include "AsciiManager.hpp"
#include "EffectManager.hpp"
#include <stddef.h>

struct EffectRingVertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
    float u;
    float v;
};

struct EffectRingRuntimeView
{
    unsigned char unknown00[0x48];
    Float3 origin48;
    Float3 axis54;
    Float3 axis60;
    unsigned char unknown6C[0x10];
    float radius7C;
    float angle80;
    unsigned char unknown84[4];
    float thickness88;
    int segments8C;
    unsigned char unknown90[4];
    float ellipseY94;
    float rotation98;
    float frequency9C;
};

typedef char EffectRingVertexSizeIs1C[(sizeof(EffectRingVertex) == 0x1C) ? 1 : -1];
typedef char EffectRingOriginAt48[(offsetof(EffectRingRuntimeView, origin48) == 0x48) ? 1 : -1];
typedef char EffectRingRadiusAt7C[(offsetof(EffectRingRuntimeView, radius7C) == 0x7C) ? 1 : -1];
typedef char EffectRingSegmentsAt8C[(offsetof(EffectRingRuntimeView, segments8C) == 0x8C) ? 1 : -1];
typedef char EffectRingFrequencyAt9C[(offsetof(EffectRingRuntimeView, frequency9C) == 0x9C) ? 1 : -1];
typedef char EffectRingPrefixSizeIsA0[(sizeof(EffectRingRuntimeView) == 0xA0) ? 1 : -1];
