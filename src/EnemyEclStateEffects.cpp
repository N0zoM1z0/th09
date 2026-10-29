#include "EffectManager.hpp"
#include "EnemyManager.hpp"
#include "EnemyAttachedEffectStateView.hpp"

#include <stddef.h>

namespace Th09EclRunState
{

struct EnemyAttachedEffectsView
{
    unsigned char unknown0000[0x53B4];
    EnemyAttachedEffectStateView attachedEffectState53B4;
};

typedef char EnemyAttachedEffectsArrayAt53B4[
    (offsetof(EnemyAttachedEffectsView, attachedEffectState53B4) == 0x53B4) ? 1 : -1];
typedef char EnemyAttachedEffectsCountAt5414[
    (offsetof(EnemyAttachedEffectsView, attachedEffectState53B4) +
     offsetof(EnemyAttachedEffectStateView, count60) == 0x5414) ? 1 : -1];

void __fastcall ReleaseAttachedEffects(EnemyView *enemy)
{
    EnemyAttachedEffectsView *view =
        reinterpret_cast<EnemyAttachedEffectsView *>(enemy);

    for (int i = 0; i < view->attachedEffectState53B4.count60; ++i)
    {
        Effect *effect = view->attachedEffectState53B4.effects00[i];
        if (effect != 0)
        {
            reinterpret_cast<unsigned char *>(effect)[0xC6] = 1;
            view->attachedEffectState53B4.effects00[i] = 0;
        }
    }

    view->attachedEffectState53B4.count60 = 0;
}

}
