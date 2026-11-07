#pragma once

#include "PlayerLifecycleView.hpp"

#include <stddef.h>

// Legacy zero-storage source-facing role. The exact 51-byte method is now
// actually declared on PlayerLifecycleView, whose native +0x30388 resource
// scalar is the same allocation consumed by ChargeAttack.
struct PlayerResourceSubtractView : PlayerLifecycleView
{
};

typedef char PlayerResourceAt30388[
    (sizeof(PlayerResourceSubtractView) == sizeof(PlayerLifecycleView) &&
     offsetof(PlayerResourceSubtractView, scalar30388) == 0x30388) ? 1 : -1];
