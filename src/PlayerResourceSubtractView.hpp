#pragma once

#include "PlayerLifecycleView.hpp"

#include <stddef.h>

// The 51-byte original resource clamp acts on the same native Player's
// +0x30388 scalar. Keep its historically bound method ABI while borrowing
// the real storage instead of a separate 0x30388-byte shadow prefix.
struct PlayerResourceSubtractView : PlayerLifecycleView
{
    void SubtractResourceClamped(float amount);
};

typedef char PlayerResourceAt30388[
    (sizeof(PlayerResourceSubtractView) == sizeof(PlayerLifecycleView) &&
     offsetof(PlayerResourceSubtractView, scalar30388) == 0x30388) ? 1 : -1];
