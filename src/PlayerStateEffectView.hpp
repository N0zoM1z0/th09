#pragma once

#include "PlayerResourceSubtractView.hpp"

#include <stddef.h>

struct Effect;

// These state-3 effect methods operate on the native Player storage. Preserve
// their separately original-bound method ABI, but do not reconstruct a second
// 0x303D4-byte stand-in for the same Side, position and timer members.
struct PlayerStateEffectView : PlayerResourceSubtractView
{
    Effect *ConfigureState3Effect13();
    Effect *ConfigureState3Effect14();
    Effect *ConfigureState3Effect15();
};

typedef char PlayerStateEffectNativeStorage[
    (sizeof(PlayerStateEffectView) == sizeof(PlayerLifecycleView) &&
     offsetof(PlayerStateEffectView, sideState) == 0x0C &&
     offsetof(PlayerStateEffectView, position1B88) == 0x1B88 &&
     offsetof(PlayerStateEffectView, timer303C8) == 0x303C8 &&
     offsetof(PlayerSideStateView, effectManager0C) == 0x0C) ? 1 : -1];
