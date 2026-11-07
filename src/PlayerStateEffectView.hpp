#pragma once

#include "PlayerResourceSubtractView.hpp"

#include <stddef.h>

struct Effect;

// The three original-exact State3 effects now belong to the actual native
// Player, along with its resource clamp. Retain the zero-extra-storage
// derived source interface for the still-open ChargeAttack method.
struct PlayerStateEffectView : PlayerResourceSubtractView
{
};

typedef char PlayerStateEffectNativeStorage[
    (sizeof(PlayerStateEffectView) == sizeof(PlayerLifecycleView) &&
     offsetof(PlayerStateEffectView, sideState) == 0x0C &&
     offsetof(PlayerStateEffectView, position1B88) == 0x1B88 &&
     offsetof(PlayerStateEffectView, timer303C8) == 0x303C8 &&
     offsetof(PlayerSideStateView, effectManager0C) == 0x0C) ? 1 : -1];
