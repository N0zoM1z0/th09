// Original Effect rows26/28, included after the shared radial fan vertex.
#include "RuntimeAngleMath.hpp"

struct EffectFanPlayerMovementAngleView
{
    unsigned char unknown00[0x1CD8];
    float movementAngle1CD8;
};
typedef char EffectFanPlayerMovementAngleAt1CD8[
    (offsetof(EffectFanPlayerMovementAngleView, movementAngle1CD8) == 0x1CD8) ? 1 : -1];

int __fastcall EffectUpdateCallbackType26(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    effect->unknown0C9 = 1;
    if (state->radius7C < 112.0f)
        state->radius7C += 8.0f;
    EffectManager *manager = effect->manager;
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[manager->sideIndex].player04->position1B88);
    float targetAngle = AddNormalizeAngle(
        reinterpret_cast<const EffectFanPlayerMovementAngleView *>(
            static_cast<const AsciiGameManagerSideView *>(manager->sideState)->player04)->movementAngle1CD8,
        3.1415927f);
    float difference = targetAngle - state->angle80;
    if (targetAngle > state->angle80)
    {
        if (difference > 3.1415927f)
            difference -= 6.2831855f;
    }
    else if (difference < -3.1415927f)
        difference += 6.2831855f;
    state->angle80 = AddNormalizeAngle(state->angle80, difference * 0.125f);
    return 1;
}

int __fastcall EffectUpdateCallbackType28(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    effect->unknown0C9 = 1;
    if (state->radius7C < 320.0f)
        state->radius7C += 8.0f;
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[effect->manager->sideIndex].player04->position1B88);
    state->angle80 = 0.0f;
    return 1;
}

int __fastcall EffectDrawCallbackType26(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    if (effect->unknown0C9 != 0)
    {
        float angleStep = 2.0943952f / static_cast<float>(state->segments8C / 2 - 1);
        float angle = AddNormalizeAngle(state->angle80, 0.52359879f);
        float offsetAngle = AddNormalizeAngle(state->angle80, -1.5707964f);
        EffectRadialFanVertex *vertex = static_cast<EffectRadialFanVertex *>(effect->vertices);
        const Float3 &origin = *reinterpret_cast<const Float3 *>(&effect->position);
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x40600040;
        ++vertex;
        Float3 offset;
        offset.FromAngleMagnitude(offsetAngle, state->radius7C * 0.5f);
        for (int i = state->segments8C; i > 0; i -= 2)
        {
            if (angle >= 3.1415927f)
                angle -= 6.2831855f;
            vertex->pos.FromAngleMagnitude(angle, state->radius7C);
            vertex->pos += origin + offset;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            angle += angleStep;
            vertex->pos.z = 0.0f;
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x70600040;
            ++vertex;
        }
        angle = AddNormalizeAngle(angle, 1.0471976f - angleStep);
        for (int i = state->segments8C; i > 0; i -= 2)
        {
            if (angle >= 3.1415927f)
                angle -= 6.2831855f;
            vertex->pos.FromAngleMagnitude(angle, state->radius7C);
            vertex->pos += origin - offset;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            angle += angleStep;
            vertex->pos.z = 0.0f;
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x70600040;
            ++vertex;
        }
    }
    g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
        static_cast<VertexDiffuseXyzrhw *>(effect->vertices), state->segments8C + 1);
    return 1;
}

int __fastcall EffectDrawCallbackType28(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    if (effect->unknown0C9 != 0)
    {
        float angleStep = 1.0471976f / static_cast<float>(state->segments8C / 2 - 1);
        float angle = AddNormalizeAngle(state->angle80, 1.0471976f);
        float offsetAngle = AddNormalizeAngle(state->angle80, -1.5707964f);
        EffectRadialFanVertex *vertex = static_cast<EffectRadialFanVertex *>(effect->vertices);
        const Float3 &origin = *reinterpret_cast<const Float3 *>(&effect->position);
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x40600040;
        ++vertex;
        Float3 offset;
        offset.FromAngleMagnitude(offsetAngle, state->radius7C * 0.8660254f);
        for (int i = state->segments8C; i > 0; i -= 2)
        {
            if (angle >= 3.1415927f)
                angle -= 6.2831855f;
            vertex->pos.FromAngleMagnitude(angle, state->radius7C);
            vertex->pos += origin + offset;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            angle += angleStep;
            vertex->pos.z = 0.0f;
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x70600040;
            ++vertex;
        }
        angle = AddNormalizeAngle(angle, 2.0943952f - angleStep);
        for (int i = state->segments8C; i > 0; i -= 2)
        {
            if (angle >= 3.1415927f)
                angle -= 6.2831855f;
            vertex->pos.FromAngleMagnitude(angle, state->radius7C);
            vertex->pos += origin - offset;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            angle += angleStep;
            vertex->pos.z = 0.0f;
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x70600040;
            ++vertex;
        }
    }
    g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
        static_cast<VertexDiffuseXyzrhw *>(effect->vertices), state->segments8C + 1);
    return 1;
}
