// Original Effect rows17/29/30/31/32/37 initialization and bounded fan callbacks.
#include "RuntimeAngleMath.hpp"

struct EffectFanApertureView
{
    unsigned char unknown00[0x84];
    float aperture84;
};
typedef char EffectFanApertureAt84[
    (offsetof(EffectFanApertureView, aperture84) == 0x84) ? 1 : -1];

int __fastcall EffectInitializeCallbackType17(Effect *effect)
{
    effect->vertices = g_ZunMemory.Alloc(sizeof(EffectRadialFanVertex) * 33, "Effect");
    if (effect->vertices == 0)
        return -1;
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    state->origin48 = *reinterpret_cast<const Float3 *>(&effect->position);
    state->axis54.x = 0.0f;
    state->axis54.y = 0.0f;
    state->axis54.z = 1.0f;
    state->axis60.x = 0.0f;
    state->axis60.y = -1.0f;
    state->axis60.z = 0.0f;
    state->angle80 = AddNormalizeAngle(
        reinterpret_cast<const EffectFanPlayerMovementAngleView *>(
            static_cast<const AsciiGameManagerSideView *>(effect->manager->sideState)->player04)->movementAngle1CD8,
        3.1415927f);
    state->thickness88 = effect->velocity.z;
    state->ellipseY94 = 0.0f;
    state->rotation98 = 0.0f;
    reinterpret_cast<EffectFanApertureView *>(effect)->aperture84 = 0.0f;
    state->segments8C = 16;
    state->frequency9C = 1.0f;
    state->radius7C = 160.0f;
    effect->unknown0C9 = 1;
    return 0;
}

int __fastcall EffectInitializeCallbackType29(Effect *effect)
{
    effect->vertices = g_ZunMemory.Alloc(0xF78, "Effect");
    if (effect->vertices == 0)
        return -1;
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    state->origin48 = *reinterpret_cast<const Float3 *>(&effect->position);
    state->axis54.z = 1.0f;
    state->axis54.x = 0.0f;
    state->axis54.y = 0.0f;
    state->axis60.x = 0.0f;
    state->axis60.y = -1.0f;
    state->axis60.z = 0.0f;
    state->angle80 = 0.0f;
    state->thickness88 = effect->velocity.z;
    state->ellipseY94 = 0.0f;
    state->rotation98 = 0.0f;
    state->segments8C = 32;
    state->radius7C = 32.0f;
    state->frequency9C = 1.0f;
    effect->unknown0C9 = 1;
    return 0;
}

int __fastcall EffectInitializeCallbackType30(Effect *effect)
{
    effect->vertices = g_ZunMemory.Alloc(sizeof(EffectRadialFanVertex) * 33, "Effect");
    if (effect->vertices == 0)
        return -1;
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    state->origin48 = *reinterpret_cast<const Float3 *>(&effect->position);
    state->axis54.x = 0.0f;
    state->axis54.y = 0.0f;
    state->axis54.z = 1.0f;
    state->axis60.x = 0.0f;
    state->axis60.y = -1.0f;
    state->axis60.z = 0.0f;
    const AsciiGameManagerSideView *side =
        static_cast<const AsciiGameManagerSideView *>(effect->manager->sideState);
    AddNormalizeAngle(reinterpret_cast<const EffectFanPlayerMovementAngleView *>(side->player04)->movementAngle1CD8, 3.1415927f);
    state->thickness88 = effect->velocity.z;
    state->ellipseY94 = 0.0f;
    state->rotation98 = 0.0f;
    reinterpret_cast<EffectFanApertureView *>(effect)->aperture84 = 0.0f;
    state->segments8C = 16;
    state->frequency9C = 1.0f;
    state->radius7C = 128.0f;
    effect->unknown0C9 = 1;
    state->angle80 = -1.5707964f;
    return 0;
}

int __fastcall EffectInitializeCallbackType31(Effect *effect)
{
    effect->vertices = g_ZunMemory.Alloc(sizeof(EffectRadialFanVertex) * 33, "Effect");
    if (effect->vertices == 0)
        return -1;
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    state->origin48 = *reinterpret_cast<const Float3 *>(&effect->position);
    state->axis54.z = 1.0f;
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
    state->angle80 = -1.5707964f;
    return 0;
}

int __fastcall EffectInitializeCallbackType32(Effect *effect)
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
    state->radius7C = 640.0f;
    state->frequency9C = 1.0f;
    effect->unknown0C9 = 1;
    return 0;
}

int __fastcall EffectInitializeCallbackType37(Effect *effect)
{
    effect->vertices = g_ZunMemory.Alloc(0xF0, "Effect");
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

int __fastcall EffectUpdateCallbackType37(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    effect->unknown0C9 = 1;
    if (state->radius7C < 8.0f)
        state->radius7C += 0.5f;
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[effect->manager->sideIndex].player04->position1B88);
    return 1;
}

int __fastcall EffectUpdateCallbackType32(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    if (state->radius7C > 0.0f)
    {
        effect->unknown0C9 = 1;
        state->radius7C -= 32.0f;
    }
    else
        effect->unknown0C9 = 0;
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[effect->manager->sideIndex].player04->position1B88);
    return 1;
}

int __fastcall EffectUpdateCallbackType30(Effect *effect)
{
    EffectFanApertureView *aperture = reinterpret_cast<EffectFanApertureView *>(effect);
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    effect->unknown0C9 = 1;
    if (aperture->aperture84 < 1.5707964f)
        aperture->aperture84 += 0.031415928f;
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[effect->manager->sideIndex].player04->position1B88);
    state->angle80 = -1.5707964f;
    return 1;
}

int __fastcall EffectDrawCallbackType32(Effect *effect)
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
        g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
            static_cast<VertexDiffuseXyzrhw *>(effect->vertices), state->segments8C + 1);
    }
    return 1;
}

int __fastcall EffectDrawCallbackType17(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    if (effect->unknown0C9 != 0)
    {
        const float &aperture = *reinterpret_cast<const float *>(effect->unknown084);
        float angleStep = aperture / static_cast<float>(state->segments8C - 1);
        float angle = state->angle80 - aperture * 0.5f;
        EffectRadialFanVertex *vertex = static_cast<EffectRadialFanVertex *>(effect->vertices);
        const Float3 &origin = *reinterpret_cast<const Float3 *>(&effect->position);
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x30600040;
        ++vertex;
        for (int i = state->segments8C; i > 0; --i)
        {
            vertex->pos.z = 0.0f;
            angle = AddNormalizeAngle(angle, 0.0f);
            vertex->pos.FromAngleMagnitude(angle, state->radius7C);
            vertex->pos += origin;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            angle += angleStep;
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x50600040;
            ++vertex;
        }
    }
    g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
        static_cast<VertexDiffuseXyzrhw *>(effect->vertices), state->segments8C + 1);
    return 1;
}

int __fastcall EffectDrawCallbackType30(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    if (effect->unknown0C9 != 0)
    {
        const float &aperture = *reinterpret_cast<const float *>(effect->unknown084);
        float angleStep = aperture / static_cast<float>(state->segments8C - 1);
        float angle = state->angle80 - aperture * 0.5f;
        EffectRadialFanVertex *vertex = static_cast<EffectRadialFanVertex *>(effect->vertices);
        const Float3 &origin = *reinterpret_cast<const Float3 *>(&effect->position);
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x30600040;
        ++vertex;
        for (int i = state->segments8C; i > 0; --i)
        {
            vertex->pos.z = 0.0f;
            angle = AddNormalizeAngle(angle, 0.0f);
            vertex->pos.FromAngleMagnitude(angle, state->radius7C);
            vertex->pos += origin;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            angle += angleStep;
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x50600040;
            ++vertex;
        }
    }
    g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
        static_cast<VertexDiffuseXyzrhw *>(effect->vertices), state->segments8C + 1);
    return 1;
}
