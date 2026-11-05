// SPDX-License-Identifier: GPL-3.0-only
// Adapted from Cirno-Labs/th09 d7653152, src/pl09.cpp; see THIRD_PARTY_NOTICES.md.
// Complete original-bound /Ob0 extent:2220 bytes,129 fields; exact codegen.
// Original nominal vector types and translation-unit ownership remain unknown.
#include "RuntimePolarMath.hpp"
#include "EffectRingRuntimeView.hpp"
#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "RuntimeAngleMath.hpp"
#include <d3dx8math.h>
typedef char Effect29SdkNativeVectorAbi[
    (sizeof(D3DXVECTOR3) == sizeof(Float3) && sizeof(Float3) == 12 &&
     offsetof(D3DXVECTOR3, x) == offsetof(Float3, x) &&
     offsetof(D3DXVECTOR3, y) == offsetof(Float3, y) &&
     offsetof(D3DXVECTOR3, z) == offsetof(Float3, z)) ? 1 : -1];
int __fastcall EffectDrawCallbackType29(Effect *effect)
{

    if (effect->unknown0C9)
    {
        int n = effect->segments8C;
        EffectRadialFanVertex *vtx = (EffectRadialFanVertex *)effect->vertices;
        EffectRadialFanVertex *fan;
        float step = 6.2831855f / (n - 1);
        int i;

        float angle = -3.1415927f;
        fan = vtx;
        vtx->pos = reinterpret_cast<const Float3 &>(effect->position);
        vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
        vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
        vtx->rhw = 1.0f;
        vtx->diffuse = 0x40600040;
        vtx++;
        for (i = 0; i < n; i++)
        {
            if (angle >= 3.1415927f)
            {
                angle -= 6.2831855f;
            }
            vtx->pos.z = 0.0f;
            SetPolar2D(reinterpret_cast<Float2 *>(&vtx->pos), angle, effect->radius7C);
            vtx->pos += reinterpret_cast<const Float3 &>(effect->position);
            vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
            vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
            vtx->rhw = 1.0f;
            vtx->diffuse = 0x70600040;
            vtx++;
            angle += step;
        }
        g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms), reinterpret_cast<VertexDiffuseXyzrhw *>(fan), n + 1);
        n /= 2;
        {
            float step = 6.2831855f / (n - 1);
            float angle = effect->angle80;
            D3DXVECTOR3 centre(0.0f, 0.0f, 0.0f);
            fan = vtx;
            SetPolar2D(reinterpret_cast<Float2 *>(&centre), angle, effect->radius7C + 24.0f);
            centre += reinterpret_cast<const D3DXVECTOR3 &>(effect->position);
            vtx->pos = reinterpret_cast<const Float3 &>(centre);
            vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
            vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
            vtx->rhw = 1.0f;
            vtx->diffuse = 0x40600040;
            vtx++;
            for (i = 0; i < n; i++)
            {
                if (angle >= 3.1415927f)
                {
                    angle -= 6.2831855f;
                }
                vtx->pos.z = 0.0f;
                SetPolar2D(reinterpret_cast<Float2 *>(&vtx->pos), angle, 24.0f);
                vtx->pos += reinterpret_cast<const Float3 &>(centre);
                vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
                vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
                vtx->rhw = 1.0f;
                vtx->diffuse = 0x70600040;
                vtx++;
                angle += step;
            }
            g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms), reinterpret_cast<VertexDiffuseXyzrhw *>(fan), n + 1);
        }
        {
            float step = 6.2831855f / (n - 1);
            float angle = effect->angle80;
            D3DXVECTOR3 centre(0.0f, 0.0f, 0.0f);
            angle = AddNormalizeAngle(angle, 1.0471976f);
            fan = vtx;
            SetPolar2D(reinterpret_cast<Float2 *>(&centre), angle, effect->radius7C + 24.0f);
            centre += reinterpret_cast<const D3DXVECTOR3 &>(effect->position);
            vtx->pos = reinterpret_cast<const Float3 &>(centre);
            vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
            vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
            vtx->rhw = 1.0f;
            vtx->diffuse = 0x40600040;
            vtx++;
            for (i = 0; i < n; i++)
            {
                if (angle >= 3.1415927f)
                {
                    angle -= 6.2831855f;
                }
                vtx->pos.z = 0.0f;
                SetPolar2D(reinterpret_cast<Float2 *>(&vtx->pos), angle, 24.0f);
                vtx->pos += reinterpret_cast<const Float3 &>(centre);
                vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
                vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
                vtx->rhw = 1.0f;
                vtx->diffuse = 0x70600040;
                vtx++;
                angle += step;
            }
            g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms), reinterpret_cast<VertexDiffuseXyzrhw *>(fan), n + 1);
        }
        {
            float step = 6.2831855f / (n - 1);
            float angle = effect->angle80;
            D3DXVECTOR3 centre(0.0f, 0.0f, 0.0f);
            angle = AddNormalizeAngle(angle, 2.0943952f);
            fan = vtx;
            SetPolar2D(reinterpret_cast<Float2 *>(&centre), angle, effect->radius7C + 24.0f);
            centre += reinterpret_cast<const D3DXVECTOR3 &>(effect->position);
            vtx->pos = reinterpret_cast<const Float3 &>(centre);
            vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
            vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
            vtx->rhw = 1.0f;
            vtx->diffuse = 0x40600040;
            vtx++;
            for (i = 0; i < n; i++)
            {
                if (angle >= 3.1415927f)
                {
                    angle -= 6.2831855f;
                }
                vtx->pos.z = 0.0f;
                SetPolar2D(reinterpret_cast<Float2 *>(&vtx->pos), angle, 24.0f);
                vtx->pos += reinterpret_cast<const Float3 &>(centre);
                vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
                vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
                vtx->rhw = 1.0f;
                vtx->diffuse = 0x70600040;
                vtx++;
                angle += step;
            }
            g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms), reinterpret_cast<VertexDiffuseXyzrhw *>(fan), n + 1);
        }
        {
            float step = 6.2831855f / (n - 1);
            float angle = effect->angle80;
            D3DXVECTOR3 centre(0.0f, 0.0f, 0.0f);
            angle = AddNormalizeAngle(angle, 3.1415927f);
            fan = vtx;
            SetPolar2D(reinterpret_cast<Float2 *>(&centre), angle, effect->radius7C + 24.0f);
            centre += reinterpret_cast<const D3DXVECTOR3 &>(effect->position);
            vtx->pos = reinterpret_cast<const Float3 &>(centre);
            vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
            vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
            vtx->rhw = 1.0f;
            vtx->diffuse = 0x40600040;
            vtx++;
            for (i = 0; i < n; i++)
            {
                if (angle >= 3.1415927f)
                {
                    angle -= 6.2831855f;
                }
                vtx->pos.z = 0.0f;
                SetPolar2D(reinterpret_cast<Float2 *>(&vtx->pos), angle, 24.0f);
                vtx->pos += reinterpret_cast<const Float3 &>(centre);
                vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
                vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
                vtx->rhw = 1.0f;
                vtx->diffuse = 0x70600040;
                vtx++;
                angle += step;
            }
            g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms), reinterpret_cast<VertexDiffuseXyzrhw *>(fan), n + 1);
        }
        {
            float step = 6.2831855f / (n - 1);
            float angle = effect->angle80;
            D3DXVECTOR3 centre(0.0f, 0.0f, 0.0f);
            angle = AddNormalizeAngle(angle, 4.1887903f);
            fan = vtx;
            SetPolar2D(reinterpret_cast<Float2 *>(&centre), angle, effect->radius7C + 24.0f);
            centre += reinterpret_cast<const D3DXVECTOR3 &>(effect->position);
            vtx->pos = reinterpret_cast<const Float3 &>(centre);
            vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
            vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
            vtx->rhw = 1.0f;
            vtx->diffuse = 0x40600040;
            vtx++;
            for (i = 0; i < n; i++)
            {
                if (angle >= 3.1415927f)
                {
                    angle -= 6.2831855f;
                }
                vtx->pos.z = 0.0f;
                SetPolar2D(reinterpret_cast<Float2 *>(&vtx->pos), angle, 24.0f);
                vtx->pos += reinterpret_cast<const Float3 &>(centre);
                vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
                vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
                vtx->rhw = 1.0f;
                vtx->diffuse = 0x70600040;
                vtx++;
                angle += step;
            }
            g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms), reinterpret_cast<VertexDiffuseXyzrhw *>(fan), n + 1);
        }
        {
            float step = 6.2831855f / (n - 1);
            float angle = effect->angle80;
            D3DXVECTOR3 centre(0.0f, 0.0f, 0.0f);
            angle = AddNormalizeAngle(angle, 5.2359877f);
            fan = vtx;
            SetPolar2D(reinterpret_cast<Float2 *>(&centre), angle, effect->radius7C + 24.0f);
            centre += reinterpret_cast<const D3DXVECTOR3 &>(effect->position);
            vtx->pos = reinterpret_cast<const Float3 &>(centre);
            vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
            vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
            vtx->rhw = 1.0f;
            vtx->diffuse = 0x40600040;
            vtx++;
            for (i = 0; i < n; i++)
            {
                if (angle >= 3.1415927f)
                {
                    angle -= 6.2831855f;
                }
                vtx->pos.z = 0.0f;
                SetPolar2D(reinterpret_cast<Float2 *>(&vtx->pos), angle, 24.0f);
                vtx->pos += reinterpret_cast<const Float3 &>(centre);
                vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
                vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
                vtx->rhw = 1.0f;
                vtx->diffuse = 0x70600040;
                vtx++;
                angle += step;
            }
            g_AnmManager->DrawTriangleFan(reinterpret_cast<AnmVm *>(effect->vms), reinterpret_cast<VertexDiffuseXyzrhw *>(fan), n + 1);
        }
    }
    return 1;
}
