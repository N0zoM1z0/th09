#include "EffectManager.hpp"
#include "EffectType34RuntimeView.hpp"
#include "RngRuntimeLeaves.hpp"

extern RngRuntimeView g_Rng;

int __fastcall EffectUpdateCallbackType34(Effect *effect)
{
    EffectType34VerticesView *vertices =
        static_cast<EffectType34VerticesView *>(effect->vertices);
    if (vertices->state00 == 0)
    {
        float remaining = 1.0f - static_cast<float>(effect->timer) * 0.02f;
        vertices->width50 = (1.0f - remaining * remaining) * 64.0f;
        if (effect->timer >= 50)
        {
            vertices->state00 = 1;
            effect->timer = 0;
        }
    }
    else
    {
        ZunTimer &timer = effect->timer;
        float *row = &vertices->jitterCE8[0][0];
        for (int layer = 0; layer < 4; ++layer)
        {
            float *value = row;
            for (int index = 0; index < 32; ++index)
                *value++ = g_Rng.GetRandomF32SignedInRange(static_cast<float>(timer));
            row += 33;
        }
        if (timer >= 30)
            return 0;
    }
    return 1;
}
