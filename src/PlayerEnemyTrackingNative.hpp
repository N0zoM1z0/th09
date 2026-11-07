#pragma once

#include "PlayerEnemyTrackingStateView.hpp"
#include "PlayerLifecycleView.hpp"

// Enemy calc, cleanup and SHT callbacks use this same Player storage.
// This real POD allocation preserves the original Player constructor. The
// separate Float3-facing enemy/shot view remains for actual math operations.
typedef char PlayerEnemyTrackingNativeStorageAt30364[
    (offsetof(PlayerLifecycleView, tracking30364) == 0x30364 &&
     sizeof(((PlayerLifecycleView *)0)->tracking30364) == 0x20 &&
     offsetof(PlayerLifecycleView, tracking30364) +
         offsetof(PlayerEnemyTrackingNativeView, homingTarget18) == 0x3037C &&
     sizeof(PlayerEnemyTrackingStateView) == 0x1C) ? 1 : -1];
