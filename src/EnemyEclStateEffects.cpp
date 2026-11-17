#include "EffectManager.hpp"
#include "EnemyManager.hpp"
#include "EnemyAttachedEffectStateView.hpp"

#include <stddef.h>

namespace Th09EclRunState
{

// Release operates on the same concrete Enemy allocation as Enemy update.
// The effect's +0xC6 byte is the native two-byte field's first lane.
typedef char EnemyReleaseAttachedEffectsNativeLayout[
    (offsetof(EnemyView, attachedEffectState53B4) == 0x53B4 &&
     offsetof(EnemyAttachedEffectStateView, count60) == 0x60 &&
     offsetof(Effect, unknown0C6) == 0xC6) ? 1 : -1];

void __fastcall ReleaseAttachedEffects(EnemyView *enemy)
{
    for (int i = 0; i < enemy->attachedEffectState53B4.count60; ++i)
    {
        Effect *effect = enemy->attachedEffectState53B4.effects00[i];
        if (effect != 0)
        {
            effect->unknown0C6[0] = 1;
            enemy->attachedEffectState53B4.effects00[i] = 0;
        }
    }

    enemy->attachedEffectState53B4.count60 = 0;
}

}
