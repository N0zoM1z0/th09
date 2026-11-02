#pragma once

#include "Float3.hpp"
#include "PlayerEnemyTrackingStateView.hpp"
#include "ZunTimer.hpp"
#include <stddef.h>

struct Effect;

// Shared target-observed Player window consumed by EnemyManager::OnUpdate
// and ECL's late side-state instructions. This is a bounded storage view;
// original Player class ownership and its untouched fields remain unknown.
struct EnemyCorePlayerView
{
    unsigned char unknown00000[0x364];
    Effect *focusEffect364;
    unsigned char unknown00368[0x1B88 - 0x368];
    Float3 position1B88;
    unsigned char unknown01B94[0x30364 - 0x1B94];
    PlayerEnemyTrackingStateView tracking30364;
    unsigned char unknown30380[0x303C8 - 0x30380];
    ZunTimer stateTimer303C8;
    unsigned char unknown303D4[0x303F4 - 0x303D4];
    int tableValue303F4;
};
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
