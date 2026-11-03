#include "EffectManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"

typedef char EffectBulletLayerNativeVmCoordinates[
    (sizeof(AnmVm) == 0x2A4 &&
     offsetof(AnmVm, pos) == 0x208 &&
     offsetof(AnmVm, pos2) == 0x288) ? 1 : -1];

int EffectManager::DrawBulletLayerEffects()
{
    EffectManager *manager = this;
    Effect *effect = manager->drawSentinel1.nextInDrawGroup;

    while (effect != NULL)
    {
        if (effect->drawCallback != NULL)
        {
            effect->drawCallback(effect);
        }
        else
        {
            int vmCount = effect->vmCount;
            AnmVm *vm = reinterpret_cast<AnmVm *>(effect->vms);
            if (vmCount > 0)
            {
                int remaining = vmCount;
                do
                {
                    vm->pos.x =
                        g_GameManager.TransformPopupX(effect->position.x);
                    vm->pos.y =
                        g_GameManager.TransformPopupY(effect->position.y);
                    vm->pos.z = 0.08f;
                    vm->pos += vm->pos2;
                    g_AnmManager->Draw2D(vm);
                    ++vm;
                }
                while (--remaining != 0);
            }
        }

        effect = effect->nextInDrawGroup;
    }

    return 1;
}
