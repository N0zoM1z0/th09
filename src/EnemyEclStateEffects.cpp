#include "EffectManager.hpp"
#include "EnemyManager.hpp"

#include <stddef.h>

namespace Th09EclRunState
{

struct EnemyAttachedEffectsView
{
    unsigned char unknown0000[0x53B4];
    Effect *attachedEffects53B4[24];
    int attachedEffectCount5414;
};

typedef char EnemyAttachedEffectsArrayAt53B4[
    (offsetof(EnemyAttachedEffectsView, attachedEffects53B4) == 0x53B4) ? 1 : -1];
typedef char EnemyAttachedEffectsCountAt5414[
    (offsetof(EnemyAttachedEffectsView, attachedEffectCount5414) == 0x5414) ? 1 : -1];

void __fastcall ReleaseAttachedEffects(EnemyView *enemy)
{
    EnemyAttachedEffectsView *view =
        reinterpret_cast<EnemyAttachedEffectsView *>(enemy);

    for (int i = 0; i < view->attachedEffectCount5414; ++i)
    {
        Effect *effect = view->attachedEffects53B4[i];
        if (effect != 0)
        {
            reinterpret_cast<unsigned char *>(effect)[0xC6] = 1;
            view->attachedEffects53B4[i] = 0;
        }
    }

    view->attachedEffectCount5414 = 0;
}

}
