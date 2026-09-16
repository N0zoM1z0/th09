#include "ScreenEffect.hpp"

struct SupervisorStartEffectView
{
    void StartEffect(int index);
};

extern ScreenEffect *g_SupervisorScreenEffect;

void SupervisorStartEffectView::StartEffect(int index)
{
    if (g_SupervisorScreenEffect == NULL)
    {
        g_SupervisorScreenEffect = ScreenEffect::RegisterChain(
            (ScreenEffectType)(index + SCREEN_EFFECT_FULL_FADE_HOLD),
            60, 0, 0, 0, 1, 2);
    }
}
