#pragma once

#include "EnemyManager.hpp"
#include "Float3.hpp"
#include "PlayerEnemyTrackingStateView.hpp"
#include "ZunTimer.hpp"
#include <stddef.h>

struct Effect;

// This is the same TH09 Player allocation as the native Enemy reward view.
// The full original Player class is still not assumed; only the independently
// observed shared fields and their offsets are represented here.
typedef EnemyRewardPlayerView EnemyCorePlayerView;
typedef char EnemyCorePlayerFocusAt364[
    (offsetof(EnemyCorePlayerView, focusEffect364) == 0x364) ? 1 : -1];
typedef char EnemyCorePlayerPositionAt1B88[
    (offsetof(EnemyCorePlayerView, position1B88) == 0x1B88) ? 1 : -1];
typedef char EnemyCorePlayerHomingAt3037C[
    (offsetof(EnemyCorePlayerView, tracking30364) +
     offsetof(PlayerEnemyTrackingStateView, homingTarget18) == 0x3037C) ? 1 : -1];

typedef char EnemyCorePlayerSideTableAt303F4[
    (offsetof(EnemyCorePlayerView, tableValue303F4) == 0x303F4) ? 1 : -1];
typedef char EnemyCorePlayerStateTimerAt303C8[
    (offsetof(EnemyCorePlayerView, stateTimer303C8) == 0x303C8) ? 1 : -1];
