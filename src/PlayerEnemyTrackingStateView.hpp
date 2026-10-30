#pragma once

#include "Float3.hpp"
#include <stddef.h>

struct EnemyView;

// Observed Player storage at +0x30364. Enemy calc publishes the nearest
// position and a separate homing pointer; Player stage D resets the position.
// The intervening twelve bytes have a separate position reset, but their role
// is not established by these consumers. Original class ownership is unknown.
struct PlayerEnemyTrackingStateView
{
    Float3 trackedEnemyPosition00;
    unsigned char unknown0C[0x0C];
    EnemyView *homingTarget18;
};

typedef char PlayerEnemyTrackingStateSizeIs1C[
    sizeof(PlayerEnemyTrackingStateView) == 0x1C ? 1 : -1];
typedef char PlayerEnemyTrackingHomingAt18[
    offsetof(PlayerEnemyTrackingStateView, homingTarget18) == 0x18 ? 1 : -1];
