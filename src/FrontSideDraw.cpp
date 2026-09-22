#include "FrontSide.hpp"
#include "AnmManager.hpp"

#include <d3d8.h>
#include <stddef.h>

typedef unsigned char u8;
typedef unsigned int u32;

struct Float3
{
    Float3(float x, float y, float z);
    float x;
    float y;
    float z;
};
typedef char FrontSideDrawFloat3SizeIs0C[
    (sizeof(Float3) == 0x0C) ? 1 : -1];

struct FrontSideDrawVmCursor
{
    Float3 pos00;
    u8 unknown00C[0x80 - 0x0C];
    Float3 pos80;
    u8 unknown08C[0x2A4 - 0x8C];
};
typedef char FrontSideDrawVmCursorSizeIs2A4[
    (sizeof(FrontSideDrawVmCursor) == 0x2A4) ? 1 : -1];

struct FrontSideDrawPlayerView
{
    u8 unknown0000[0x1B88];
    PlayerPositionView position1B88;
};
typedef char FrontSideDrawPlayerPositionAt1B88[
    (offsetof(FrontSideDrawPlayerView, position1B88) == 0x1B88) ? 1 : -1];

struct FrontSideDrawSideView
{
    u8 unknown00[0x04];
    FrontSideDrawPlayerView *player04;
};
typedef char FrontSideDrawSidePlayerAt04[
    (offsetof(FrontSideDrawSideView, player04) == 0x04) ? 1 : -1];

struct FrontSideDrawFloat2
{
    float x;
    float y;
};

struct FrontSideDrawViewportConfiguration
{
    u8 unknown000[0xE8];
    FrontSideDrawFloat2 screenShakeOffsetE8;
};
typedef char FrontSideDrawViewportSizeIsF0[
    (sizeof(FrontSideDrawViewportConfiguration) == 0xF0) ? 1 : -1];

class Supervisor
{
  public:
    void ConfigureScreenViewport(int index);
    void SetRenderState(int state, int value);

    u8 unknown000[0x08];
    IDirect3DDevice8 *d3dDevice08;
    u8 unknown00C[0x78 - 0x0C];
    FrontSideDrawViewportConfiguration viewports78[3];
};
typedef char FrontSideDrawSupervisorDeviceAt08[
    (offsetof(Supervisor, d3dDevice08) == 0x08) ? 1 : -1];
typedef char FrontSideDrawSupervisorViewportsAt78[
    (offsetof(Supervisor, viewports78) == 0x78) ? 1 : -1];

struct AsciiGameManagerView
{
    float TransformPopupX(float value);
    float TransformPopupY(float value);
};

struct FrontSideDrawVertex
{
    FrontSideDrawVertex();
    Float3 pos;
    float rhw;
    u32 diffuse;
};
typedef char FrontSideDrawVertexSizeIs14[
    (sizeof(FrontSideDrawVertex) == 0x14) ? 1 : -1];

extern Supervisor g_Supervisor;
extern AsciiGameManagerView g_GameManager;

static __forceinline FrontSideDrawVmCursor *FrontSideDrawCursor(AnmVm *vm)
{
    return reinterpret_cast<FrontSideDrawVmCursor *>(
        reinterpret_cast<u8 *>(vm) + 0x208);
}

static __forceinline AnmVm *FrontSideDrawBase(FrontSideDrawVmCursor *cursor)
{
    return reinterpret_cast<AnmVm *>(
        reinterpret_cast<u8 *>(cursor) - 0x208);
}

static __forceinline FrontSideDrawSideView *FrontSideDrawSide(FrontSide *frontSide)
{
    return reinterpret_cast<FrontSideDrawSideView *>(frontSide->sideStateA670);
}

int FrontSide::OnDraw(FrontSide *frontSide)
{
    int i;

    g_Supervisor.viewports78[frontSide->sideIndexA66C].screenShakeOffsetE8.x = 0.0f;
    g_Supervisor.viewports78[frontSide->sideIndexA66C].screenShakeOffsetE8.y = 0.0f;
    g_Supervisor.ConfigureScreenViewport(frontSide->sideIndexA66C);

    FrontSideDrawVmCursor *cursor = FrontSideDrawCursor(&frontSide->statusVms0000[0]);
    for (i = 0; i < 11; ++i, ++cursor)
    {
        cursor->pos00.x = g_GameManager.TransformPopupX(cursor->pos80.x);
        cursor->pos00.y = g_GameManager.TransformPopupY(cursor->pos80.y);
        cursor->pos00.z = 0.0f;
        g_AnmManager->Draw2D(FrontSideDrawBase(cursor));
    }

    cursor = FrontSideDrawCursor(&frontSide->stockVms1D0C[0]);
    for (i = 0; i < 5; ++i, ++cursor)
    {
        cursor->pos00.x = g_GameManager.TransformPopupX(cursor->pos80.x);
        cursor->pos00.y = g_GameManager.TransformPopupY(cursor->pos80.y);
        cursor->pos00.z = 0.0f;
        g_AnmManager->Draw2D(FrontSideDrawBase(cursor));
    }

    cursor = FrontSideDrawCursor(&frontSide->gaugeVms2A40[0]);
    for (i = 0; i < 7; ++i, ++cursor)
    {
        cursor->pos00.x = g_GameManager.TransformPopupX(cursor->pos80.x);
        cursor->pos00.y = g_GameManager.TransformPopupY(cursor->pos80.y);
        cursor->pos00.z = 0.0f;
        g_AnmManager->Draw2D(FrontSideDrawBase(cursor));
    }

    cursor = FrontSideDrawCursor(&frontSide->auxA678.vms0C[0]);
    for (i = 0; i < 2; ++i, ++cursor)
    {
        cursor->pos00.x = g_GameManager.TransformPopupX(cursor->pos80.x);
        cursor->pos00.y = g_GameManager.TransformPopupY(cursor->pos80.y);
        cursor->pos00.z = 0.0f;
        g_AnmManager->DrawNoRotation(FrontSideDrawBase(cursor));
    }

    cursor = FrontSideDrawCursor(&frontSide->vm3CBC);
    cursor->pos00.x = g_GameManager.TransformPopupX(cursor->pos80.x);
    cursor->pos00.y = g_GameManager.TransformPopupY(cursor->pos80.y);
    cursor->pos00.z = 0.0f;
    g_AnmManager->DrawNoRotation(FrontSideDrawBase(cursor));

    cursor = FrontSideDrawCursor(&frontSide->scoreVms3F60[0]);
    for (i = 0; i < 10; ++i, ++cursor)
    {
        cursor->pos00.x = g_GameManager.TransformPopupX(cursor->pos80.x);
        cursor->pos00.y = g_GameManager.TransformPopupY(cursor->pos80.y);
        cursor->pos00.z = 0.0f;
        g_AnmManager->DrawNoRotation(FrontSideDrawBase(cursor));
    }

    cursor = FrontSideDrawCursor(&frontSide->vm59C8);
    cursor->pos00.x = g_GameManager.TransformPopupX(cursor->pos80.x);
    cursor->pos00.y = g_GameManager.TransformPopupY(cursor->pos80.y);
    g_AnmManager->DrawNoRotation(FrontSideDrawBase(cursor));

    cursor = FrontSideDrawCursor(&frontSide->meterVms5C6C[0]);
    for (i = 0; i < 4; ++i, ++cursor)
    {
        cursor->pos00.x = g_GameManager.TransformPopupX(
            FrontSideDrawSide(frontSide)->player04->position1B88.x +
            cursor->pos80.x);
        cursor->pos00.y = g_GameManager.TransformPopupY(
            FrontSideDrawSide(frontSide)->player04->position1B88.y +
            cursor->pos80.y);
        g_AnmManager->DrawNoRotation(FrontSideDrawBase(cursor));
    }

    cursor = FrontSideDrawCursor(&frontSide->markerVms66FC[0]);
    for (i = 0; i < 3; ++i, ++cursor)
    {
        cursor->pos00.x = g_GameManager.TransformPopupX(
            FrontSideDrawSide(frontSide)->player04->position1B88.x +
            cursor->pos80.x);
        cursor->pos00.y = g_GameManager.TransformPopupY(
            FrontSideDrawSide(frontSide)->player04->position1B88.y +
            cursor->pos80.y);
        if (frontSide->transitionStateA65C != 0)
            cursor->pos00.y -= 24.0f;
        g_AnmManager->DrawNoRotation(FrontSideDrawBase(cursor));
    }

    cursor = FrontSideDrawCursor(&frontSide->timingVms76D4[0]);
    for (i = 0; i < 6; ++i, ++cursor)
    {
        cursor->pos00.x = g_GameManager.TransformPopupX(
            FrontSideDrawSide(frontSide)->player04->position1B88.x +
            cursor->pos80.x);
        cursor->pos00.y = g_GameManager.TransformPopupY(
            FrontSideDrawSide(frontSide)->player04->position1B88.y +
            cursor->pos80.y);
        g_AnmManager->DrawNoRotation(FrontSideDrawBase(cursor));
    }

    cursor = FrontSideDrawCursor(&frontSide->stateVms6EE8[0]);
    for (i = 0; i < 2; ++i, ++cursor)
    {
        cursor->pos00.x = g_GameManager.TransformPopupX(
            FrontSideDrawSide(frontSide)->player04->position1B88.x +
            cursor->pos80.x);
        cursor->pos00.y = g_GameManager.TransformPopupY(
            FrontSideDrawSide(frontSide)->player04->position1B88.y +
            cursor->pos80.y);
        g_AnmManager->DrawNoRotation(FrontSideDrawBase(cursor));
    }

    cursor = FrontSideDrawCursor(&frontSide->vm7430);
    FrontSideDrawSideView *singleSide = FrontSideDrawSide(frontSide);
    cursor->pos00.x = g_GameManager.TransformPopupX(
        singleSide->player04->position1B88.x +
        cursor->pos80.x);
    cursor->pos00.y = g_GameManager.TransformPopupY(
        singleSide->player04->position1B88.y +
        cursor->pos80.y);
    g_AnmManager->DrawNoRotation(FrontSideDrawBase(cursor));

    cursor = FrontSideDrawCursor(&frontSide->rankVms86AC[0]);
    for (i = 0; i < 7; ++i, ++cursor)
    {
        cursor->pos00.x = g_GameManager.TransformPopupX(cursor->pos80.x);
        cursor->pos00.y = g_GameManager.TransformPopupY(cursor->pos80.y);
        g_AnmManager->DrawNoRotation(FrontSideDrawBase(cursor));
    }

    cursor = FrontSideDrawCursor(&frontSide->counterVms9928[0]);
    for (i = 0; i < 5; ++i, ++cursor)
    {
        cursor->pos00.x = g_GameManager.TransformPopupX(cursor->pos80.x);
        cursor->pos00.y = g_GameManager.TransformPopupY(cursor->pos80.y);
        g_AnmManager->Draw2D(FrontSideDrawBase(cursor));
    }

    if (frontSide->auxA678.unknown554 != 0)
    {
        FrontSideDrawVertex vertices[4];

        float transitionTime;
        if (static_cast<float>(frontSide->transitionTimerABD0) >= 30.0f)
            transitionTime = 30.0f;
        else
            transitionTime = static_cast<float>(frontSide->transitionTimerABD0);

        float direction = 1.0f;
        if (frontSide->auxA678.unknown554 != 1)
            direction = -1.0f;

        g_AnmManager->FlushVertexBuffer();

        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        g_Supervisor.d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        g_Supervisor.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        g_Supervisor.d3dDevice08->SetVertexShader(
            D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

        transitionTime *= 14.933333f;
        float edgeX = direction * -144.0f;

        vertices[0].pos = Float3(
            g_GameManager.TransformPopupX(edgeX),
            g_GameManager.TransformPopupY(0.0f),
            0.0f);

        direction *= 144.0f;
        vertices[1].pos = Float3(
            g_GameManager.TransformPopupX(direction),
            g_GameManager.TransformPopupY(0.0f),
            0.0f);
        vertices[2].pos = Float3(
            g_GameManager.TransformPopupX(edgeX),
            g_GameManager.TransformPopupY(transitionTime),
            0.0f);

        vertices[0].diffuse =
            vertices[1].diffuse =
            vertices[2].diffuse = 0xFF000000u;
        vertices[0].rhw =
            vertices[1].rhw =
            vertices[2].rhw = 1.0f;

        g_Supervisor.d3dDevice08->DrawPrimitiveUP(
            D3DPT_TRIANGLESTRIP, 1, vertices, sizeof(FrontSideDrawVertex));

        transitionTime =
            448.0f -
            static_cast<float>(frontSide->transitionTimerABD0) * 14.933333f;

        vertices[0].pos = Float3(
            g_GameManager.TransformPopupX(edgeX),
            g_GameManager.TransformPopupY(448.0f),
            0.0f);
        vertices[1].pos = Float3(
            g_GameManager.TransformPopupX(direction),
            g_GameManager.TransformPopupY(448.0f),
            0.0f);
        vertices[2].pos = Float3(
            g_GameManager.TransformPopupX(direction),
            g_GameManager.TransformPopupY(transitionTime),
            0.0f);

        vertices[0].diffuse =
            vertices[1].diffuse =
            vertices[2].diffuse = 0xFF000000u;
        vertices[0].rhw =
            vertices[1].rhw =
            vertices[2].rhw = 1.0f;

        g_Supervisor.d3dDevice08->DrawPrimitiveUP(
            D3DPT_TRIANGLESTRIP, 1, vertices, sizeof(FrontSideDrawVertex));

        transitionTime =
            static_cast<float>(frontSide->transitionTimerABD0) * 9.6000004f -
            144.0f;

        float edgeY;
        if (frontSide->auxA678.unknown554 == 1)
        {
            vertices[0].pos = Float3(
                g_GameManager.TransformPopupX(-144.0f),
                g_GameManager.TransformPopupY(0.0f),
                0.0f);
            vertices[1].pos = Float3(
                g_GameManager.TransformPopupX(-144.0f),
                g_GameManager.TransformPopupY(448.0f),
                0.0f);
            edgeY = 448.0f;
        }
        else
        {
            vertices[0].pos = Float3(
                g_GameManager.TransformPopupX(-144.0f),
                g_GameManager.TransformPopupY(448.0f),
                0.0f);
            vertices[1].pos = Float3(
                g_GameManager.TransformPopupX(-144.0f),
                g_GameManager.TransformPopupY(0.0f),
                0.0f);
            edgeY = 0.0f;
        }
        vertices[2].pos = Float3(
            g_GameManager.TransformPopupX(transitionTime),
            g_GameManager.TransformPopupY(edgeY),
            0.0f);

        vertices[0].diffuse =
            vertices[1].diffuse =
            vertices[2].diffuse = 0xFF000000u;
        vertices[0].rhw =
            vertices[1].rhw =
            vertices[2].rhw = 1.0f;

        g_Supervisor.d3dDevice08->DrawPrimitiveUP(
            D3DPT_TRIANGLESTRIP, 1, vertices, sizeof(FrontSideDrawVertex));

        transitionTime =
            144.0f -
            static_cast<float>(frontSide->transitionTimerABD0) * 9.6000004f;

        if (frontSide->auxA678.unknown554 == 1)
        {
            vertices[0].pos = Float3(
                g_GameManager.TransformPopupX(144.0f),
                g_GameManager.TransformPopupY(448.0f),
                0.0f);
            vertices[1].pos = Float3(
                g_GameManager.TransformPopupX(144.0f),
                g_GameManager.TransformPopupY(0.0f),
                0.0f);
            edgeY = 0.0f;
        }
        else
        {
            vertices[0].pos = Float3(
                g_GameManager.TransformPopupX(144.0f),
                g_GameManager.TransformPopupY(0.0f),
                0.0f);
            vertices[1].pos = Float3(
                g_GameManager.TransformPopupX(144.0f),
                g_GameManager.TransformPopupY(448.0f),
                0.0f);
            edgeY = 448.0f;
        }
        vertices[2].pos = Float3(
            g_GameManager.TransformPopupX(transitionTime),
            g_GameManager.TransformPopupY(edgeY),
            0.0f);

        vertices[0].diffuse =
            vertices[1].diffuse =
            vertices[2].diffuse = 0xFF000000u;
        vertices[0].rhw =
            vertices[1].rhw =
            vertices[2].rhw = 1.0f;

        g_Supervisor.d3dDevice08->DrawPrimitiveUP(
            D3DPT_TRIANGLESTRIP, 1, vertices, sizeof(FrontSideDrawVertex));

        g_AnmManager->ClearVertexShader();
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

    return 1;
}
