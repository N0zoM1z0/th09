// Original Effect rows17/29/31 updates and rows31/37 draws.
// Lexical member of EclManager.cpp; Type31 draw uses the existing /Ob0 carrier.
#include "RuntimeAngleMath.hpp"

int __fastcall EffectUpdateCallbackType17(Effect *effect)
{
    EffectFanApertureView *aperture = reinterpret_cast<EffectFanApertureView *>(effect);
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    effect->unknown0C9 = 1;
    if (aperture->aperture84 < 1.5707964f)
        aperture->aperture84 += 0.052359879f;
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
    state->angle80 = AddNormalizeAngle(state->angle80, difference * 0.25f);
    return 1;
}

int __fastcall EffectUpdateCallbackType29(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    effect->unknown0C9 = 1;
    if (state->radius7C < 48.0f)
        state->radius7C += 4.0f;
    state->angle80 = AddNormalizeAngle(state->angle80, 0.052359879f);
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[effect->manager->sideIndex].player04->position1B88);
    return 1;
}

struct EffectFanPlayerVelocityView
{
    unsigned char unknown0000[0x1CCC];
    float velocityX1CCC;
};
typedef char EffectFanPlayerVelocityAt1CCC[
    (offsetof(EffectFanPlayerVelocityView, velocityX1CCC) == 0x1CCC) ? 1 : -1];

int __fastcall EffectUpdateCallbackType31(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    effect->unknown0C9 = 1;
    if (state->radius7C < 128.0f)
        state->radius7C += 6.0f;
    EffectManager *manager = effect->manager;
    const EffectFanPlayerVelocityView *player = reinterpret_cast<const EffectFanPlayerVelocityView *>(
        g_GameManager.sides[manager->sideIndex].player04);
    if (player->velocityX1CCC < 0.0f)
        state->angle80 += (-2.0943952f - state->angle80) * 0.02f;
    else if (player->velocityX1CCC > 0.0f)
        state->angle80 += (-1.0471976f - state->angle80) * 0.02f;
    else
        state->angle80 += (-1.5707964f - state->angle80) * 0.05f;
    effect->position = *reinterpret_cast<const EffectFloat3 *>(
        &g_GameManager.sides[manager->sideIndex].player04->position1B88);
    return 1;
}

int __fastcall EffectDrawCallbackType31(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    if (effect->unknown0C9 != 0)
    {
        float angle = state->angle80;
        Float3 points[10];
        points[0].FromAngleMagnitude(angle, state->radius7C);
        angle = AddNormalizeAngle(angle, 1.2566371f);
        points[2].FromAngleMagnitude(angle, state->radius7C);
        angle = AddNormalizeAngle(angle, 1.2566371f);
        points[4].FromAngleMagnitude(angle, state->radius7C);
        angle = AddNormalizeAngle(angle, 1.2566371f);
        points[6].FromAngleMagnitude(angle, state->radius7C);
        angle = AddNormalizeAngle(angle, 1.2566371f);
        points[8].FromAngleMagnitude(angle, state->radius7C);

        points[1] = (points[0] - points[4]) / 1.618f + points[4];
        points[3] = (points[2] - points[6]) / 1.618f + points[6];
        points[5] = (points[4] - points[8]) / 1.618f + points[8];
        points[7] = (points[6] - points[0]) / 1.618f + points[0];
        points[9] = (points[8] - points[2]) / 1.618f + points[2];

        EffectRadialFanVertex *vertex = static_cast<EffectRadialFanVertex *>(effect->vertices);
        const Float3 &origin = *reinterpret_cast<const Float3 *>(&effect->position);
        for (int i = 0; i < 10; ++i)
            points[i].z = 0.0f;
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x20600040;
        ++vertex;
        for (int i = 0; i < 10; ++i)
        {
            vertex->pos = points[i] + origin;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x70600040;
            ++vertex;
        }
        vertex->pos = points[0] + origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
    }
    g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
        static_cast<VertexDiffuseXyzrhw *>(effect->vertices), 12);
    return 1;
}

int __fastcall EffectDrawCallbackType37(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    if (effect->unknown0C9 != 0)
    {
        EffectRadialFanVertex *vertex = static_cast<EffectRadialFanVertex *>(effect->vertices);
        const Float3 &origin = *reinterpret_cast<const Float3 *>(&effect->position);
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x - state->radius7C);
        vertex->pos.y = g_GameManager.TransformPopupY(0.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x - state->radius7C);
        vertex->pos.y = g_GameManager.TransformPopupY(448.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(0.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x30600040;
        ++vertex;
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(448.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x30600040;
        ++vertex;
        const float &verticalRadius = state->radius7C;
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(verticalRadius + vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(0.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;
        vertex->pos = origin;
        vertex->pos.x = g_GameManager.TransformPopupX(verticalRadius + vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(448.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;
        g_AnmManager->DrawTriangleStrip(reinterpret_cast<AnmVm *>(effect->vms),
            static_cast<VertexDiffuseXyzrhw *>(effect->vertices), 6);
        EffectRadialFanVertex *start = vertex;
        vertex->pos = origin;
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y - state->radius7C);
        vertex->pos.x = g_GameManager.TransformPopupX(-144.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;
        vertex->pos = origin;
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y - state->radius7C);
        vertex->pos.x = g_GameManager.TransformPopupX(144.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;
        vertex->pos = origin;
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->pos.x = g_GameManager.TransformPopupX(-144.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x30600040;
        ++vertex;
        vertex->pos = origin;
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->pos.x = g_GameManager.TransformPopupX(144.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x30600040;
        ++vertex;
        const float &horizontalRadius = state->radius7C;
        vertex->pos = origin;
        vertex->pos.y = g_GameManager.TransformPopupY(horizontalRadius + vertex->pos.y);
        vertex->pos.x = g_GameManager.TransformPopupX(-144.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;
        vertex->pos = origin;
        vertex->pos.y = g_GameManager.TransformPopupY(horizontalRadius + vertex->pos.y);
        vertex->pos.x = g_GameManager.TransformPopupX(144.0f);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x70600040;
        ++vertex;
        g_AnmManager->DrawTriangleStrip(reinterpret_cast<AnmVm *>(effect->vms),
            reinterpret_cast<VertexDiffuseXyzrhw *>(start), 6);
    }
    return 1;
}
