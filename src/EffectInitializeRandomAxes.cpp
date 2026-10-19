#include "EffectRandomAxesRuntimeView.hpp"
#include "AsciiGameManagerView.hpp"
#include "RngRuntimeLeaves.hpp"

extern RngRuntimeView g_Rng;
extern AsciiGameManagerView g_GameManager;

int __fastcall EffectInitializeCallbackType1Type2(Effect *effect)
{
    EffectRandomAxesRuntimeView *state =
        reinterpret_cast<EffectRandomAxesRuntimeView *>(effect);
    state->base54 = state->position0C;
    state->transformed60.x = g_GameManager.TransformPopupX(state->velocity18.x);
    state->transformed60.y = g_GameManager.TransformPopupY(state->velocity18.y);
    state->transformed60.z = state->velocity18.z;
    state->axis3C.FromAngleMagnitude(g_Rng.GetRandomF32SignedInRange(3.1415927f),
                                  g_Rng.GetRandomF32InRange(256.0f) + 64.0f);
    state->axis48.FromAngleMagnitude(g_Rng.GetRandomF32SignedInRange(3.1415927f),
                                  g_Rng.GetRandomF32InRange(256.0f) + 64.0f);
    state->duration9C = g_Rng.GetRandomF32InRange(100.0f) + 60.0f;
    state->delay98 = 0.0f;
    return 0;
}
