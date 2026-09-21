#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "AsciiManager.hpp"
#include "ExAttackController.hpp"

#include <d3d8.h>
#include <stddef.h>

struct ExAttackType1422DrawVertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
};
typedef char ExAttackType1422DrawVertexSizeIs14[
    (sizeof(ExAttackType1422DrawVertex) == 0x14) ? 1 : -1];

struct ExAttackType1422DrawExtra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 point28;
    Float3 point34;
    Float3 spawn40;
    int drawPending4C;
    float value50;
    ExAttackType1422DrawVertex centerFan54[33];
    ExAttackType1422DrawVertex fanGroups2E8[4][33];
    float randomD38[4][33];
};
typedef char ExAttackType1422DrawExtraSizeIsF48[
    (sizeof(ExAttackType1422DrawExtra) == 0xF48) ? 1 : -1];

struct ExAttackType1422DrawRecord
{
    int unknown00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    AnmVm *dynamicData1C;
    Float3 position20;
    int vmCount2C;
    int unknown30;
    ExAttackType1422DrawExtra *extra34;
};

struct ExAttackType1422DrawVmView
{
    unsigned char unknown000[0x1F0];
    int field1F0;
};

struct ExAttackType1422DrawSupervisorView
{
    unsigned char unknown00[0x08];
    IDirect3DDevice8 *d3dDevice08;
};

struct ExAttackType1422TimerCurrentView
{
    unsigned char unknown00[8];
    int current08;

    int GetCurrent();
};

extern ExAttackType1422DrawSupervisorView g_Supervisor;
float __stdcall AddNormalizeAngle(float angle, float delta);

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
                reinterpret_cast<ExAttackType1422TimerCurrentView *>(
                    &record->timer10)->GetCurrent() *
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

        reinterpret_cast<ExAttackType1422DrawVmView *>(
            record->dynamicData1C)->field1F0 = -1;
        g_AnmManager->DrawTriangleFan(
            record->dynamicData1C,
            reinterpret_cast<VertexDiffuseXyzrhw *>(
                extra->centerFan54),
            33);
        g_AnmManager->FlushVertexBuffer();

        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        g_Supervisor.d3dDevice08->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        g_Supervisor.d3dDevice08->SetRenderState(
            D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        unsigned long lineColor =
            alphaBits | 0x00FFFFFFUL;

        ExAttackType1422DrawVertex *group =
            &extra->fanGroups2E8[0][0];
        float *random = &extra->randomD38[0][0];
        int groupIndex = 0;

        do
        {
            ExAttackType1422DrawVertex *lineVertex = group;
            float *randomValue = random;
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

            g_Supervisor.d3dDevice08->SetVertexShader(
                D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
            g_Supervisor.d3dDevice08->DrawPrimitiveUP(
                D3DPT_LINESTRIP,
                31,
                group,
                sizeof(ExAttackType1422DrawVertex));

            if (extra->state00 == 1)
                break;

            angle += 0.050670851f;
            ++groupIndex;
            random += 33;
            group += 33;
        }
        while (groupIndex < 4);

        g_AnmManager->ClearVertexShader();
        g_AnmManager->ClearSprite();
        g_AnmManager->ClearTexture();
        g_AnmManager->ClearColorOp();
        g_AnmManager->ClearBlendMode();
        g_AnmManager->ClearZWrite();

        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    }

    extra->drawPending4C = 0;
    return 0;
}
