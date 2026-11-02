#include "EnemyManager.hpp"
#include "EnemyChildEclBlocks.hpp"
#include "PlayerEnemyTrackingStateView.hpp"

#include <stddef.h>

// TH09 original-bound cleanup fields now point into the same native Enemy,
// manager and player allocations used by the full 3,900-byte Enemy update.
typedef char EnemyCleanupNativeSharedFields[
    (offsetof(EnemyView, bossSlot336B) == 0x336B &&
     offsetof(EnemyView, flags337C) == 0x337C &&
     offsetof(EnemyView, lifeCallbackThresholds33B0) == 0x33B0 &&
     offsetof(EnemyView, timerCallbackThreshold33D0) == 0x33D0 &&
     offsetof(EnemyView, attachedEffectState53B4) +
         offsetof(EnemyAttachedEffectStateView, count60) == 0x5414 &&
     offsetof(EnemyManagerView, remoteSlots2AC388) == 0x2AC388 &&
     offsetof(EnemyRewardPlayerView, tracking30364) +
         offsetof(PlayerEnemyTrackingStateView, homingTarget18) == 0x3037C)
         ? 1 : -1];

namespace Th09EclRunState
{
void __fastcall ReleaseAttachedEffects(EnemyView *enemy);
}

void EnemyView::CleanupAfterDeactivation()
{
    EnemyView *enemy = this;

    unsigned int flags = enemy->flags337C;
    if ((flags & 0x000E0000u) == 0)
        enemy->flags337C = flags & ~1u;

    if ((enemy->flags337C & 2u) != 0)
    {
        unsigned char slot = enemy->bossSlot336B;
        if (slot < 4)
        {
            enemy->manager00->remoteSlots2AC388[slot] = 0;
            enemy->flags337C &= ~2u;
        }
    }

    if (enemy->attachedEffectState53B4.count60 != 0)
        Th09EclRunState::ReleaseAttachedEffects(this);

    if ((enemy->flags337C & 2u) != 0)
        enemy->manager00->remoteSlots2AC388[enemy->bossSlot336B] = 0;

    enemy->lifeCallbackThresholds33B0[0] = -1;
    enemy->lifeCallbackThresholds33B0[1] = -1;
    enemy->lifeCallbackThresholds33B0[2] = -1;
    enemy->lifeCallbackThresholds33B0[3] = -1;

    int *timerThreshold = &enemy->timerCallbackThreshold33D0;
    *timerThreshold = -1;
    EnemyCoreReleaseChildEclBlocks(this);

    EnemyRewardPlayerView *player =
        enemy->manager00->sideState320->player04;
    if (player->tracking30364.homingTarget18 == this)
        player->tracking30364.homingTarget18 = 0;
}
