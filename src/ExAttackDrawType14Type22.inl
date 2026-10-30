// Lexical member of the real ECL/native-polar carrier. Still nonexact.
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
    ExAttackType1422DrawRecord *record =
        reinterpret_cast<ExAttackType1422DrawRecord *>(base);
    ExAttackType1422DrawExtra *extra = record->extra34;

    if (extra->state00 >= 2)
    {
        float angle = -3.1415927f;
        AddNormalizeAngle(-3.1415927f, -1.5707964f);

        ExAttackType1422DrawVertex *vertex =
            &extra->centerFan54[0];

        unsigned char alpha;
        if (extra->state00 == 2)
        {
            alpha = 0xFF;
        }
        else
        {
            alpha = static_cast<unsigned char>(
                0xFF -
                static_cast<int>(record->timer10) *
                    0xFF / 30);
        }

        const Float3 *center = &record->position20;

        vertex->pos = *center;
        vertex->pos.x = g_GameManager.TransformPopupX(vertex->pos.x);
        vertex->pos.y = g_GameManager.TransformPopupY(vertex->pos.y);
        vertex->rhw = 1.0f;
        vertex->diffuse =
            (static_cast<unsigned long>(alpha) << 23) |
            0x00FFFFFFUL;

        ++vertex;
        unsigned long alphaBits =
            static_cast<unsigned long>(alpha) << 24;
        unsigned long ringColor =
            alphaBits | 0x00FF4040UL;
        int remaining = 32;
        do
        {
            if (angle >= 3.1415927f)
                angle -= 6.2831855f;

            vertex->pos.z = 0.0f;
            vertex->pos.FromAngleMagnitude(
                angle, extra->value50);
            vertex->pos += *center;
            vertex->pos.x =
                g_GameManager.TransformPopupX(vertex->pos.x);
            vertex->pos.y =
                g_GameManager.TransformPopupY(vertex->pos.y);
            vertex->rhw = 1.0f;
            vertex->diffuse = ringColor;

            ++vertex;
            angle += 0.2026834f;
        }
        while (--remaining != 0);

        record->dynamicData1C->color1 = 0xFFFFFFFFUL;
        g_AnmManager->DrawTriangleFan(
            record->dynamicData1C,
            reinterpret_cast<VertexDiffuseXyzrhw *>(
                extra->centerFan54),
            33);
        g_AnmManager->FlushVertexBuffer();

        ExAttackDrawDevice()->SetTextureStageState(
            0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        ExAttackDrawDevice()->SetTextureStageState(
            0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        ExAttackDrawDevice()->SetTextureStageState(
            0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        ExAttackDrawDevice()->SetTextureStageState(
            0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        ExAttackDrawDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        ExAttackDrawDevice()->SetRenderState(
            D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        unsigned long lineColor =
            alphaBits | 0x00FFFFFFUL;

        int groupIndex = 0;

        do
        {
            ExAttackType1422DrawVertex *lineVertex = extra->fanGroups2E8[groupIndex];
            float *randomValue = extra->randomD38[groupIndex];
            int lineRemaining = 32;
            do
            {
                if (angle >= 3.1415927f)
                    angle -= 6.2831855f;

                lineVertex->pos.z = 0.0f;
                float radius = *randomValue + extra->value50;
                lineVertex->pos.FromAngleMagnitude(
                    angle, radius);
                lineVertex->pos += *center;
                lineVertex->pos.x =
                    g_GameManager.TransformPopupX(
                        lineVertex->pos.x);
                lineVertex->pos.y =
                    g_GameManager.TransformPopupY(
                        lineVertex->pos.y);
                lineVertex->diffuse = lineColor;
                lineVertex->rhw = 1.0f;

                ++lineVertex;
                angle += 0.2026834f;
                ++randomValue;
            }
            while (--lineRemaining != 0);

            ExAttackDrawDevice()->SetVertexShader(
                D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
            ExAttackDrawDevice()->DrawPrimitiveUP(
                D3DPT_LINESTRIP,
                31,
                extra->fanGroups2E8[groupIndex],
                sizeof(ExAttackType1422DrawVertex));

            if (extra->state00 == 1)
                break;

            angle += 0.050670851f;
            ++groupIndex;
        }
        while (groupIndex < 4);

        g_AnmManager->ClearVertexShader();
        g_AnmManager->ClearSprite();
        g_AnmManager->ClearTexture();
        g_AnmManager->ClearColorOp();
        g_AnmManager->ClearBlendMode();
        g_AnmManager->ClearZWrite();

        ExAttackDrawDevice()->SetTextureStageState(
            0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        ExAttackDrawDevice()->SetTextureStageState(
            0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        ExAttackDrawDevice()->SetTextureStageState(
            0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        ExAttackDrawDevice()->SetTextureStageState(
            0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }

    extra->drawPending4C = 0;
    return 0;
}
