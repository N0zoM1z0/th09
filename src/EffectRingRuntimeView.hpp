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

// The exact EffectManager and nonexact Enemy callers share the very same
// physical 0xD8-byte Effect. Preserve this name at existing ring/fan sites
// without defining a second contradictory 0xA0-byte state struct.
typedef Effect EffectRingRuntimeView;

typedef char EffectRingVertexSizeIs1C[(sizeof(EffectRingVertex) == 0x1C) ? 1 : -1];
typedef char EffectRingOriginAt48[(offsetof(EffectRingRuntimeView, origin48) == 0x48) ? 1 : -1];
typedef char EffectRingRadiusAt7C[(offsetof(EffectRingRuntimeView, radius7C) == 0x7C) ? 1 : -1];
typedef char EffectRingSegmentsAt8C[(offsetof(EffectRingRuntimeView, segments8C) == 0x8C) ? 1 : -1];
typedef char EffectRingFrequencyAt9C[(offsetof(EffectRingRuntimeView, frequency9C) == 0x9C) ? 1 : -1];
typedef char EffectRingPrefixSizeIsA0[(offsetof(EffectRingRuntimeView, unknown0A0) == 0xA0) ? 1 : -1];
