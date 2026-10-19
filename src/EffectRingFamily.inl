// Original Effect rows 6/8/13/14/15 share these two callbacks.
// Member names describe the observed runtime prefix; full ownership is unknown.
#include "EffectRingRuntimeView.hpp"
#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "PlayerLifecycleView.hpp"
#include "ZunMemory.hpp"
#include <math.h>

extern void __fastcall RotatePlayerCollisionVector(
    PlayerPositionView *outVector, PlayerPositionView *point, float angle);

int __fastcall EffectInitializeCallbackRing(Effect *effect)
{
    effect->vertices = g_ZunMemory.Alloc(sizeof(EffectRingVertex) * 258, "Effect");
    if (effect->vertices == 0)
        return -1;
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    state->origin48 = *reinterpret_cast<const Float3 *>(&effect->position);
    state->angle80 = effect->velocity.x;
    state->segments8C = 3;
    state->axis54.x = 0.0f;
    state->axis54.y = 0.0f;
    state->axis54.z = 1.0f;
    state->axis60.x = 0.0f;
    state->axis60.y = -1.0f;
    state->axis60.z = 0.0f;
    state->radius7C = effect->velocity.y;
    state->thickness88 = effect->velocity.z;
    g_AnmManager->InitializeHorizontalTextureStrip(
        reinterpret_cast<AnmVm *>(effect->vms),
        static_cast<VertexTex1DiffuseXyzrhw *>(effect->vertices), 6);
    state->ellipseY94 = 0.0f;
    state->rotation98 = 0.0f;
    state->frequency9C = 0.0f;
    effect->unknown0C9 = 1;
    state->segments8C = 24;
    return 0;
}

int __fastcall EffectDrawCallbackRing(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    int i;
    float innerRadius;
    EffectRingVertex *vertex;
    float angleStep;
    float halfWidth;
    if (effect->unknown0C9 != 0)
    {
        angleStep = 6.2831855f / static_cast<float>(state->segments8C);
        halfWidth = state->thickness88 / sinf((3.1415927f - angleStep) * 0.5f);
        vertex = static_cast<EffectRingVertex *>(effect->vertices);
        g_AnmManager->InitializeVerticalTextureStrip(
            reinterpret_cast<AnmVm *>(effect->vms),
            reinterpret_cast<VertexTex1DiffuseXyzrhw *>(vertex), 2 * (state->segments8C + 1));

        if (state->ellipseY94 == 0.0f)
        {
            innerRadius = state->radius7C - halfWidth;
            float angle = state->angle80;
            halfWidth += state->radius7C;
            for (i = state->segments8C + 1; i > 0; --i)
            {
                if (angle >= 3.1415927f)
                    angle -= 6.2831855f;
                vertex->pos.z = 0.0f;
                vertex->pos.FromAngleMagnitude(angle, halfWidth);
                vertex->pos += state->origin48;
                vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
                vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
                ++vertex;
                vertex->pos.z = 0.0f;
                vertex->pos.FromAngleMagnitude(angle, innerRadius);
                vertex->pos += state->origin48;
                vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
                vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
                ++vertex;
                angle += angleStep;
            }
        }
        else if (state->frequency9C == 0.0f)
        {
            float outerY = halfWidth + state->ellipseY94;
            float angle = 0.0f;
            float innerY = state->ellipseY94 - halfWidth;
            innerRadius = state->radius7C - halfWidth;
            halfWidth += state->radius7C;
            for (i = state->segments8C + 1; i > 0; --i)
            {
                PlayerPositionView point;
                EffectPolarScaleView &polar = *reinterpret_cast<EffectPolarScaleView *>(&point);
                polar.SetFromAngleAxes(angle, halfWidth, outerY);
                RotatePlayerCollisionVector(reinterpret_cast<PlayerPositionView *>(&vertex->pos), reinterpret_cast<PlayerPositionView *>(&polar), state->rotation98);
                vertex->pos += state->origin48;
                vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
                vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
                vertex->pos.z = 0.0f;
                ++vertex;
                polar.SetFromAngleAxes(angle, innerRadius, innerY);
                RotatePlayerCollisionVector(reinterpret_cast<PlayerPositionView *>(&vertex->pos), reinterpret_cast<PlayerPositionView *>(&polar), state->rotation98);
                vertex->pos += state->origin48;
                vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
                vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
                vertex->pos.z = 0.0f;
                ++vertex;
                angle += angleStep;
            }
        }
        else
        {
            float waveAngle;
            float angle;
            float waveStep;
            float ripple;
            waveAngle = state->rotation98;
            angle = state->angle80;
            waveStep = 6.2831855f * state->frequency9C / static_cast<float>(state->segments8C);
            innerRadius = state->radius7C - halfWidth;
            halfWidth += state->radius7C;
            for (i = state->segments8C + 1; i > 0; --i)
            {
                if (angle >= 3.1415927f)
                    angle -= 6.2831855f;
                if (waveAngle >= 3.1415927f)
                    waveAngle -= 6.2831855f;
                ripple = cosf(waveAngle) * state->ellipseY94;
                vertex->pos.FromAngleMagnitude(angle, ripple + halfWidth);
                vertex->pos += state->origin48;
                vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
                vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
                vertex->pos.z = 0.0f;
                ++vertex;
                vertex->pos.FromAngleMagnitude(angle, ripple + innerRadius);
                vertex->pos += state->origin48;
                vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
                vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
                vertex->pos.z = 0.0f;
                ++vertex;
                angle += angleStep;
                waveAngle = waveStep + waveAngle;
            }
        }
        effect->unknown0C9 = 0;
    }
    g_AnmManager->DrawVertices(reinterpret_cast<AnmVm *>(effect->vms),
        static_cast<VertexTex1DiffuseXyzrhw *>(effect->vertices), 2 * (state->segments8C + 1));
    return 1;
}
