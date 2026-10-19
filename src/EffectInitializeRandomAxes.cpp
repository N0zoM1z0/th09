#include "EffectManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "RngRuntimeLeaves.hpp"
#include <stddef.h>

// Original Effect templates 1 and 2 share this initialization callback.
// This prefix view names only the fields accessed here; original C++ field
// names and the complete meaning of the two vectors remain unknown.
struct EffectRandomAxesSetupView
{
    unsigned char unknown00[0x0C];
    EffectFloat3 position0C;
    EffectFloat3 velocity18;
    unsigned char unknown24[0x3C - 0x24];
    Float3 axis3C;
    Float3 axis48;
    EffectFloat3 base54;
    EffectFloat3 transformed60;
    unsigned char unknown6C[0x98 - 0x6C];
    int state98;
    float random9C;
};
typedef char RandomAxesStateAt98[
    (offsetof(EffectRandomAxesSetupView, state98) == 0x98) ? 1 : -1];
typedef char RandomAxesValueAt9C[
    (offsetof(EffectRandomAxesSetupView, random9C) == 0x9C) ? 1 : -1];

extern RngRuntimeView g_Rng;
extern AsciiGameManagerView g_GameManager;

int __fastcall EffectInitializeCallbackType1Type2(Effect *effect)
{
    EffectRandomAxesSetupView *state =
        reinterpret_cast<EffectRandomAxesSetupView *>(effect);
    state->base54 = state->position0C;
    state->transformed60.x = g_GameManager.TransformPopupX(state->velocity18.x);
    state->transformed60.y = g_GameManager.TransformPopupY(state->velocity18.y);
    state->transformed60.z = state->velocity18.z;
    state->axis3C.FromAngleMagnitude(g_Rng.GetRandomF32SignedInRange(3.1415927f),
                                  g_Rng.GetRandomF32InRange(256.0f) + 64.0f);
    state->axis48.FromAngleMagnitude(g_Rng.GetRandomF32SignedInRange(3.1415927f),
                                  g_Rng.GetRandomF32InRange(256.0f) + 64.0f);
    state->random9C = g_Rng.GetRandomF32InRange(100.0f) + 60.0f;
    state->state98 = 0;
    return 0;
}
