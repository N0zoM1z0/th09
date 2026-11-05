#include "RuntimePolarMath.hpp"
// SPDX-License-Identifier: GPL-3.0-only
// Adapted from Cirno-Labs/th09 src/pl04.cpp DrawExAttackTypes14And22,
// commit d7653152becabb8c86f2f43d49015ecc11bd0652.
// Original MIT portions retain their grant in LICENSE; see THIRD_PARTY_NOTICES.md.
// Reference-adapted lexical member of the real ECL/native-polar carrier.
#pragma once

#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "AsciiManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackType1422ExtraView.hpp"
#include "Supervisor.hpp"
#include "RuntimeAngleMath.hpp"

#include <d3d8.h>
#include <stddef.h>



typedef char ExAttackDrawVmColorAt1F0[
    (offsetof(AnmVm, color1) == 0x1F0) ? 1 : -1];

static __inline IDirect3DDevice8 *ExAttackDrawDevice()
{
    return g_Supervisor.d3dDevice08;
}

int __fastcall ExAttackDrawCallbackType14_22(ExAttackRecord *base)
{
    ExAttackType1422DrawRecord *attack = reinterpret_cast<ExAttackType1422DrawRecord *>(base);
    ExAttackType1422DrawExtra *data = (ExAttackType1422DrawExtra *)attack->extra34;

    if (data->state00 >= 2)
    {
        float angle = -3.1415927f;
        ExAttackType1422DrawVertex *vtx;
        unsigned char alpha;
        int i;
        int j;

        AddNormalizeAngle(angle, -1.5707964f);
        vtx = data->centerFan54;
        if (data->state00 == 2)
        {
            alpha = 0xff;
        }
        else
        {
            alpha = 0xff - static_cast<int>(attack->timer10) * 0xff / 30;
        }
        vtx->pos = attack->position20;
        vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
        vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
        vtx->rhw = 1.0f;
        vtx->diffuse = (alpha / 2 << 24) | 0xffffff;
        vtx++;
        for (i = 0; i < 0x20; i++)
        {
            if (angle >= 3.1415927f)
            {
                angle -= 6.2831855f;
            }
            vtx->pos.z = 0.0f;
            SetPolar2D(reinterpret_cast<Float2 *>(&(vtx->pos)), angle, data->value50);
            vtx->pos += attack->position20;
            vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
            vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
            vtx->rhw = 1.0f;
            vtx->diffuse = (alpha << 24) | 0xff4040;
            vtx++;
            angle += 0.2026834f;
        }
        attack->dynamicData1C->color1 = 0xffffffff;
        g_AnmManager->DrawTriangleFan(attack->dynamicData1C, reinterpret_cast<VertexDiffuseXyzrhw *>(data->centerFan54), 0x21);
        g_AnmManager->FlushVertexBuffer();
        g_Supervisor.d3dDevice08->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        g_Supervisor.d3dDevice08->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        g_Supervisor.d3dDevice08->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        g_Supervisor.d3dDevice08->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        g_Supervisor.d3dDevice08->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        g_Supervisor.d3dDevice08->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        for (i = 0; i < 4; i++)
        {
            vtx = data->fanGroups2E8[i];
            for (j = 0; j < 0x20; j++)
            {
                if (angle >= 3.1415927f)
                {
                    angle -= 6.2831855f;
                }
                vtx->pos.z = 0.0f;
                SetPolar2D(reinterpret_cast<Float2 *>(&(vtx->pos)), angle, data->randomD38[i][j] + data->value50);
                vtx->pos += attack->position20;
                vtx->pos.x = g_GameManager.TransformPopupX(vtx->pos.x);
                vtx->pos.y = g_GameManager.TransformPopupY(vtx->pos.y);
                vtx->diffuse = (alpha << 24) | 0xffffff;
                vtx->rhw = 1.0f;
                vtx++;
                angle += 0.2026834f;
            }
            g_Supervisor.d3dDevice08->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
            g_Supervisor.d3dDevice08->DrawPrimitiveUP(D3DPT_LINESTRIP, 0x1f, data->fanGroups2E8[i], sizeof(ExAttackType1422DrawVertex));
            if (data->state00 == 1)
            {
                break;
            }
            angle += 0.05067085f;
        }
        g_AnmManager->ClearVertexShader();
        g_AnmManager->ClearSprite();
        g_AnmManager->ClearTexture();
        g_AnmManager->ClearColorOp();
        g_AnmManager->ClearBlendMode();
        g_AnmManager->ClearZWrite();
        g_Supervisor.d3dDevice08->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        g_Supervisor.d3dDevice08->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        g_Supervisor.d3dDevice08->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        g_Supervisor.d3dDevice08->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }
    data->drawPending4C = 0;
    return 0;
}
