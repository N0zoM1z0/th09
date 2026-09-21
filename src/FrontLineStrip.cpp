#include "AnmManager.hpp"

#include <d3d8.h>

struct FrontLineVertex
{
    float x;
    float y;
    float z;
    float rhw;
    unsigned long diffuse;
};
typedef char FrontLineVertexSizeIs14[
    (sizeof(FrontLineVertex) == 0x14) ? 1 : -1];

struct FrontLineSupervisorView
{
    unsigned char unknown00[0x08];
    IDirect3DDevice8 *d3dDevice08;
};

extern FrontLineSupervisorView g_Supervisor;

#define FRONT_LINE_DEVICE (g_Supervisor.d3dDevice08)

int __fastcall FrontDrawLineStrip(
    FrontLineVertex *vertices,
    int primitiveCount)
{
    g_AnmManager->FlushVertexBuffer();

    FRONT_LINE_DEVICE->SetTextureStageState(
        0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    FRONT_LINE_DEVICE->SetTextureStageState(
        0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    FRONT_LINE_DEVICE->SetTextureStageState(
        0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    FRONT_LINE_DEVICE->SetTextureStageState(
        0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    FRONT_LINE_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    FRONT_LINE_DEVICE->SetRenderState(
        D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    FRONT_LINE_DEVICE->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    FRONT_LINE_DEVICE->DrawPrimitiveUP(
        D3DPT_LINESTRIP,
        primitiveCount,
        vertices,
        sizeof(FrontLineVertex));

    g_AnmManager->ClearVertexShader();
    g_AnmManager->ClearSprite();
    g_AnmManager->ClearTexture();
    g_AnmManager->ClearColorOp();
    g_AnmManager->ClearBlendMode();
    g_AnmManager->ClearZWrite();

    FRONT_LINE_DEVICE->SetTextureStageState(
        0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    FRONT_LINE_DEVICE->SetTextureStageState(
        0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    FRONT_LINE_DEVICE->SetTextureStageState(
        0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    return FRONT_LINE_DEVICE->SetTextureStageState(
        0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
}
