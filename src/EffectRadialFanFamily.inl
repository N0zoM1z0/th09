// Original Effect radial-fan callbacks; the initializer is shared by seven rows.
#include "EffectRingRuntimeView.hpp"
#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "PlayerLifecycleView.hpp"
#include "ZunMemory.hpp"

struct EffectRadialFanVertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
};
typedef char EffectRadialFanVertexSizeIs14[(sizeof(EffectRadialFanVertex) == 0x14) ? 1 : -1];

int __fastcall EffectInitializeCallbackRadialFan(Effect *effect)
{
    effect->vertices = g_ZunMemory.Alloc(sizeof(EffectRadialFanVertex) * 33, "Effect");
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
    state->radius7C = 16.0f;
    state->frequency9C = 1.0f;
    effect->unknown0C9 = 1;
    return 0;
}

int __fastcall EffectUpdateCallbackPlayerOffsetFan(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    effect->unknown0C9 = 1;
    if (state->radius7C < 56.0f)
        state->radius7C += 4.0f;
    float yOffset = state->radius7C * 1.5f;
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[effect->manager->sideIndex].player04->position1B88);
    effect->position.y -= yOffset;
    return 1;
}

int __fastcall EffectDrawCallbackPlayerOffsetFan(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    if (effect->unknown0C9 != 0)
    {
        float angleStep = 6.2831855f / static_cast<float>(state->segments8C - 1);
        float angle = -3.1415927f;
        EffectRadialFanVertex *vertex = static_cast<EffectRadialFanVertex *>(effect->vertices);
        const Float3 &origin = *reinterpret_cast<const Float3 *>(&effect->position);
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x40600040;
        ++vertex;
        for (int i = state->segments8C; i > 0; --i)
        {
            if (angle >= 3.1415927f)
                angle -= 6.2831855f;
            vertex->pos.z = 0.0f;
            vertex->pos.FromAngleMagnitude(angle, state->radius7C);
            vertex->pos += origin;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            angle += angleStep;
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x70600040;
            ++vertex;
        }
    }
    g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
        static_cast<VertexDiffuseXyzrhw *>(effect->vertices), state->segments8C + 1);
    return 1;
}

// Original Effect template row18; independent complete callbacks.
int __fastcall EffectUpdateCallbackType18(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    effect->unknown0C9 = 1;
    if (state->radius7C < 144.0f)
        state->radius7C += 0.7f;
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[effect->manager->sideIndex].player04->position1B88);
    return 1;
}

int __fastcall EffectDrawCallbackType18(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    if (effect->unknown0C9 != 0)
    {
        float angleStep = 6.2831855f / static_cast<float>(state->segments8C - 1);
        float angle = -3.1415927f;
        EffectRadialFanVertex *vertex = static_cast<EffectRadialFanVertex *>(effect->vertices);
        const Float3 &origin = *reinterpret_cast<const Float3 *>(&effect->position);
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x40600040;
        ++vertex;
        for (int i = state->segments8C; i > 0; --i)
        {
            if (angle >= 3.1415927f)
                angle -= 6.2831855f;
            vertex->pos.z = 0.0f;
            vertex->pos.FromAngleMagnitude(angle, state->radius7C);
            vertex->pos += origin;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            angle += angleStep;
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x70600040;
            ++vertex;
        }
    }
    g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
        static_cast<VertexDiffuseXyzrhw *>(effect->vertices), state->segments8C + 1);
    return 1;
}

// Independently owned original Effect rows10,24,33.
int __fastcall EffectUpdateCallbackType10(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    effect->unknown0C9 = 1;
    if (state->radius7C < 96.0f)
        state->radius7C += 4.0f;
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[effect->manager->sideIndex].player04->position1B88);
    return 1;
}

int __fastcall EffectDrawCallbackType10(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    if (effect->unknown0C9 != 0)
    {
        float angleStep = 6.2831855f / static_cast<float>(state->segments8C - 1);
        float angle = -3.1415927f;
        EffectRadialFanVertex *vertex = static_cast<EffectRadialFanVertex *>(effect->vertices);
        const Float3 &origin = *reinterpret_cast<const Float3 *>(&effect->position);
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x40600040;
        ++vertex;
        for (int i = state->segments8C; i > 0; --i)
        {
            if (angle >= 3.1415927f)
                angle -= 6.2831855f;
            vertex->pos.z = 0.0f;
            vertex->pos.FromAngleMagnitude(angle, state->radius7C);
            vertex->pos += origin;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            angle += angleStep;
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x70600040;
            ++vertex;
        }
    }
    g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
        static_cast<VertexDiffuseXyzrhw *>(effect->vertices), state->segments8C + 1);
    return 1;
}

int __fastcall EffectUpdateCallbackType24(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    effect->unknown0C9 = 1;
    if (state->radius7C < 48.0f)
        state->radius7C += 8.0f;
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[effect->manager->sideIndex].player04->position1B88);
    return 1;
}

int __fastcall EffectDrawCallbackType24(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    if (effect->unknown0C9 != 0)
    {
        float angleStep = 6.2831855f / static_cast<float>(state->segments8C - 1);
        float angle = -3.1415927f;
        EffectRadialFanVertex *vertex = static_cast<EffectRadialFanVertex *>(effect->vertices);
        const Float3 &origin = *reinterpret_cast<const Float3 *>(&effect->position);
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x40600040;
        ++vertex;
        for (int i = state->segments8C; i > 0; --i)
        {
            if (angle >= 3.1415927f)
                angle -= 6.2831855f;
            vertex->pos.z = 0.0f;
            vertex->pos.FromAngleMagnitude(angle, state->radius7C);
            vertex->pos += origin;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            angle += angleStep;
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x70600040;
            ++vertex;
        }
    }
    g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
        static_cast<VertexDiffuseXyzrhw *>(effect->vertices), state->segments8C + 1);
    return 1;
}

int __fastcall EffectUpdateCallbackType33(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    effect->unknown0C9 = 1;
    if (state->radius7C < 96.0f)
        state->radius7C += 16.0f;
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[effect->manager->sideIndex].player04->position1B88);
    return 1;
}

int __fastcall EffectDrawCallbackType33(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    if (effect->unknown0C9 != 0)
    {
        float angleStep = 6.2831855f / static_cast<float>(state->segments8C - 1);
        float angle = -3.1415927f;
        EffectRadialFanVertex *vertex = static_cast<EffectRadialFanVertex *>(effect->vertices);
        const Float3 &origin = *reinterpret_cast<const Float3 *>(&effect->position);
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x40600040;
        ++vertex;
        for (int i = state->segments8C; i > 0; --i)
        {
            if (angle >= 3.1415927f)
                angle -= 6.2831855f;
            vertex->pos.z = 0.0f;
            vertex->pos.FromAngleMagnitude(angle, state->radius7C);
            vertex->pos += origin;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            angle += angleStep;
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x70600040;
            ++vertex;
        }
    }
    g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
        static_cast<VertexDiffuseXyzrhw *>(effect->vertices), state->segments8C + 1);
    return 1;
}
