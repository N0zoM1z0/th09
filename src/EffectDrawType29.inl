// Lexical member of EclManager.cpp after EffectRadialFanFamily.inl.
// Reuses its reviewed 20-byte EffectRadialFanVertex and the unchanged existing
// Float3 polar primitive. Original C++ names and TU partition remain unknown.
// SOURCE-PRESENT NON-EXACT: 2205 candidate bytes versus 2220 original bytes;
// see scripts/inspect-effect-type29.py. No partial or exact match credit.
#include "EffectRingRuntimeView.hpp"
#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "RuntimeAngleMath.hpp"

int __fastcall EffectDrawCallbackType29(Effect *effect)
{
    EffectRingRuntimeView *state = reinterpret_cast<EffectRingRuntimeView *>(effect);
    if (effect->unknown0C9 != 0)
    {
        int count = state->segments8C;
        float angleStep = 6.2831855f / static_cast<float>(count - 1);
        float angle = -3.1415927f;
        EffectRadialFanVertex *vertex = static_cast<EffectRadialFanVertex *>(effect->vertices);
        EffectRadialFanVertex *start = vertex;
        vertex->pos = *reinterpret_cast<const Float3 *>(&effect->position);
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->rhw = 1.0f;
        vertex->diffuse = 0x40600040;
        ++vertex;
        for (int i = count; i > 0; --i)
        {
            if (angle >= 3.1415927f)
                angle -= 6.2831855f;
            vertex->pos.z = 0.0f;
            vertex->pos.FromAngleMagnitude(angle, state->radius7C);
            vertex->pos += *reinterpret_cast<const Float3 *>(&effect->position);
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            angle += angleStep;
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x70600040;
            ++vertex;
        }
        g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
            reinterpret_cast<VertexDiffuseXyzrhw *>(start), count + 1);
        count /= 2;
        {
            float angleStep = 6.2831855f / static_cast<float>(count - 1);
            float angle = state->angle80;
            Float3 origin(0.0f, 0.0f, 0.0f);
            EffectRadialFanVertex *start = vertex;
            origin.FromAngleMagnitude(angle, state->radius7C + 24.0f);
            origin += *reinterpret_cast<const Float3 *>(&effect->position);
            vertex->pos = origin;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x40600040;
            ++vertex;
            for (int i = count; i > 0; --i)
            {
                if (angle >= 3.1415927f)
                    angle -= 6.2831855f;
                vertex->pos.z = 0.0f;
                vertex->pos.FromAngleMagnitude(angle, 24.0f);
                vertex->pos += origin;
                vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
                vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
                angle += angleStep;
                vertex->rhw = 1.0f;
                vertex->diffuse = 0x70600040;
                ++vertex;
            }
            g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
                reinterpret_cast<VertexDiffuseXyzrhw *>(start), count + 1);
        }
        {
            float angleStep = 6.2831855f / static_cast<float>(count - 1);
            float angle = state->angle80;
            Float3 origin(0.0f, 0.0f, 0.0f);
            angle = AddNormalizeAngle(angle, 1.0471976f);
            EffectRadialFanVertex *start = vertex;
            origin.FromAngleMagnitude(angle, state->radius7C + 24.0f);
            origin += *reinterpret_cast<const Float3 *>(&effect->position);
            vertex->pos = origin;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x40600040;
            ++vertex;
            for (int i = count; i > 0; --i)
            {
                if (angle >= 3.1415927f)
                    angle -= 6.2831855f;
                vertex->pos.z = 0.0f;
                vertex->pos.FromAngleMagnitude(angle, 24.0f);
                vertex->pos += origin;
                vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
                vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
                angle += angleStep;
                vertex->rhw = 1.0f;
                vertex->diffuse = 0x70600040;
                ++vertex;
            }
            g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
                reinterpret_cast<VertexDiffuseXyzrhw *>(start), count + 1);
        }
        {
            float angleStep = 6.2831855f / static_cast<float>(count - 1);
            float angle = state->angle80;
            Float3 origin(0.0f, 0.0f, 0.0f);
            angle = AddNormalizeAngle(angle, 2.0943952f);
            EffectRadialFanVertex *start = vertex;
            origin.FromAngleMagnitude(angle, state->radius7C + 24.0f);
            origin += *reinterpret_cast<const Float3 *>(&effect->position);
            vertex->pos = origin;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x40600040;
            ++vertex;
            for (int i = count; i > 0; --i)
            {
                if (angle >= 3.1415927f)
                    angle -= 6.2831855f;
                vertex->pos.z = 0.0f;
                vertex->pos.FromAngleMagnitude(angle, 24.0f);
                vertex->pos += origin;
                vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
                vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
                angle += angleStep;
                vertex->rhw = 1.0f;
                vertex->diffuse = 0x70600040;
                ++vertex;
            }
            g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
                reinterpret_cast<VertexDiffuseXyzrhw *>(start), count + 1);
        }
        {
            float angleStep = 6.2831855f / static_cast<float>(count - 1);
            float angle = state->angle80;
            Float3 origin(0.0f, 0.0f, 0.0f);
            angle = AddNormalizeAngle(angle, 3.1415927f);
            EffectRadialFanVertex *start = vertex;
            origin.FromAngleMagnitude(angle, state->radius7C + 24.0f);
            origin += *reinterpret_cast<const Float3 *>(&effect->position);
            vertex->pos = origin;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x40600040;
            ++vertex;
            for (int i = count; i > 0; --i)
            {
                if (angle >= 3.1415927f)
                    angle -= 6.2831855f;
                vertex->pos.z = 0.0f;
                vertex->pos.FromAngleMagnitude(angle, 24.0f);
                vertex->pos += origin;
                vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
                vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
                angle += angleStep;
                vertex->rhw = 1.0f;
                vertex->diffuse = 0x70600040;
                ++vertex;
            }
            g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
                reinterpret_cast<VertexDiffuseXyzrhw *>(start), count + 1);
        }
        {
            float angleStep = 6.2831855f / static_cast<float>(count - 1);
            float angle = state->angle80;
            Float3 origin(0.0f, 0.0f, 0.0f);
            angle = AddNormalizeAngle(angle, 4.1887903f);
            EffectRadialFanVertex *start = vertex;
            origin.FromAngleMagnitude(angle, state->radius7C + 24.0f);
            origin += *reinterpret_cast<const Float3 *>(&effect->position);
            vertex->pos = origin;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x40600040;
            ++vertex;
            for (int i = count; i > 0; --i)
            {
                if (angle >= 3.1415927f)
                    angle -= 6.2831855f;
                vertex->pos.z = 0.0f;
                vertex->pos.FromAngleMagnitude(angle, 24.0f);
                vertex->pos += origin;
                vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
                vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
                angle += angleStep;
                vertex->rhw = 1.0f;
                vertex->diffuse = 0x70600040;
                ++vertex;
            }
            g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
                reinterpret_cast<VertexDiffuseXyzrhw *>(start), count + 1);
        }
        {
            float angleStep = 6.2831855f / static_cast<float>(count - 1);
            float angle = state->angle80;
            Float3 origin(0.0f, 0.0f, 0.0f);
            angle = AddNormalizeAngle(angle, 5.2359877f);
            EffectRadialFanVertex *start = vertex;
            origin.FromAngleMagnitude(angle, state->radius7C + 24.0f);
            origin += *reinterpret_cast<const Float3 *>(&effect->position);
            vertex->pos = origin;
            vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
            vertex->rhw = 1.0f;
            vertex->diffuse = 0x40600040;
            ++vertex;
            for (int i = count; i > 0; --i)
            {
                if (angle >= 3.1415927f)
                    angle -= 6.2831855f;
                vertex->pos.z = 0.0f;
                vertex->pos.FromAngleMagnitude(angle, 24.0f);
                vertex->pos += origin;
                vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
                vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
                angle += angleStep;
                vertex->rhw = 1.0f;
                vertex->diffuse = 0x70600040;
                ++vertex;
            }
            g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms),
                reinterpret_cast<VertexDiffuseXyzrhw *>(start), count + 1);
        }
    }
    return 1;
}
