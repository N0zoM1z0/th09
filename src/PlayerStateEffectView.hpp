#pragma once

#include "ZunTimer.hpp"

#include <stddef.h>

struct Effect;
struct EffectManager;

struct PlayerStateEffectSideView
{
    unsigned char unknown00[0x0C];
    EffectManager *effectManager0C;
};

struct PlayerStateEffectPosition
{
    float x;
    float y;
    float z;
};

// Shared target-observed Player prefix used by the three state-3 effects.
// Calls return the Effect produced by the native EffectManager implementation.
struct PlayerStateEffectView
{
    unsigned char unknown00000[0x0C];
    PlayerStateEffectSideView *sideState0C;
    unsigned char unknown00010[0x1B88 - 0x10];
    PlayerStateEffectPosition position1B88;
    unsigned char unknown01B94[0x303C8 - 0x1B94];
    ZunTimer timer303C8;

    Effect *ConfigureState3Effect13();
    Effect *ConfigureState3Effect14();
    Effect *ConfigureState3Effect15();
};

typedef char PlayerStateEffectSideAt0C[
    (offsetof(PlayerStateEffectView, sideState0C) == 0x0C) ? 1 : -1];
typedef char PlayerStateEffectPositionAt1B88[
    (offsetof(PlayerStateEffectView, position1B88) == 0x1B88) ? 1 : -1];
typedef char PlayerStateEffectTimerAt303C8[
    (offsetof(PlayerStateEffectView, timer303C8) == 0x303C8) ? 1 : -1];
typedef char PlayerStateEffectManagerAt0C[
    (offsetof(PlayerStateEffectSideView, effectManager0C) == 0x0C) ? 1 : -1];
typedef char PlayerStateEffectPrefixSizeIs303D4[
    (sizeof(PlayerStateEffectView) == 0x303D4) ? 1 : -1];
