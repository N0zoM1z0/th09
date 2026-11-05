#pragma once

#include "PlayerEnemyTrackingStateView.hpp"
#include "PlayerLifecycleView.hpp"

// Enemy calc, cleanup and SHT callbacks use this same Player storage.
// Keep the observed component as a projection of the native opaque region;
// exposing a constructed Float3 member would change Player construction.
typedef char PlayerEnemyTrackingNativeStorageAt30364[
    (offsetof(PlayerLifecycleView, unknown30364) == 0x30364 &&
     sizeof(((PlayerLifecycleView *)0)->unknown30364) >=
         sizeof(PlayerEnemyTrackingStateView) &&
     offsetof(PlayerLifecycleView, unknown30364) +
         offsetof(PlayerEnemyTrackingStateView, homingTarget18) == 0x3037C)
         ? 1 : -1];
