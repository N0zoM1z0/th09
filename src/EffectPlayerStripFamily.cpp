#include "EffectRingRuntimeView.hpp"
#include "AsciiGameManagerView.hpp"
#include "PlayerLifecycleView.hpp"
#include "ZunMemory.hpp"

int __fastcall EffectInitializeCallbackStripType27Type36(Effect *effect)
{
    effect->vertices = g_ZunMemory.Alloc(0x78, "Effect");
    if (effect->vertices == 0)
        return -1;
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    state->origin48 = *reinterpret_cast<const Float3 *>(&effect->position);
    state->axis54.z = 1.0f;
    state->angle80 = effect->velocity.x;
    state->axis54.x = 0.0f;
    state->axis54.y = 0.0f;
    state->axis60.x = 0.0f;
    state->axis60.y = -1.0f;
    state->axis60.z = 0.0f;
    state->thickness88 = effect->velocity.z;
    state->ellipseY94 = 0.0f;
    state->rotation98 = 0.0f;
    state->segments8C = 32;
    state->radius7C = 0.0f;
    state->frequency9C = 1.0f;
    effect->unknown0C9 = 1;
    return 0;
}

int __fastcall EffectUpdateCallbackType27(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    effect->unknown0C9 = 1;
    if (state->radius7C < 16.0f)
        state->radius7C += 0.5f;
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[effect->manager->sideIndex].player04->position1B88);
    return 1;
}

int __fastcall EffectUpdateCallbackType36(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    effect->unknown0C9 = 1;
    if (state->radius7C < 24.0f)
        state->radius7C += 0.5f;
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[effect->manager->sideIndex].player04->position1B88);
    return 1;
}
