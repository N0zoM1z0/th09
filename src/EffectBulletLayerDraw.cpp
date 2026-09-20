#include "EffectManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "AnmManager.hpp"

struct BulletDrawEffectManagerView
{
    int DrawBulletLayerEffects();
};

int BulletDrawEffectManagerView::DrawBulletLayerEffects()
{
    EffectManager *manager = reinterpret_cast<EffectManager *>(this);
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
            EffectVmView *vm = effect->vms;
            if (vmCount > 0)
            {
                int remaining = vmCount;
                do
                {
                    vm->position.x =
                        g_GameManager.TransformPopupX(effect->position.x);
                    vm->position.y =
                        g_GameManager.TransformPopupY(effect->position.y);
                    vm->position.z = 0.08f;
                    vm->position.Add(&vm->pos2);
                    g_AnmManager->Draw2D(reinterpret_cast<AnmVm *>(vm));
                    ++vm;
                }
                while (--remaining != 0);
            }
        }

        effect = effect->nextInDrawGroup;
    }

    return 1;
}
