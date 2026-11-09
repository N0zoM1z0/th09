#include "AnmManager.hpp"
#include "Supervisor.hpp"

#include <d3d8.h>
#include <stddef.h>

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

// Original +0x08 D3D8 device belongs to the full native Supervisor,
// already used by the neighboring Front-message draw callback.
typedef char FrontLineNativeSupervisorDeviceAt08[
    (offsetof(Supervisor, d3dDevice08) == 0x08) ? 1 : -1];

int __fastcall FrontDrawLineStrip(
    FrontLineVertex *vertices,
    int primitiveCount)
{
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
    g_Supervisor.d3dDevice08->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    g_Supervisor.d3dDevice08->DrawPrimitiveUP(
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

    g_Supervisor.d3dDevice08->SetTextureStageState(
        0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    g_Supervisor.d3dDevice08->SetTextureStageState(
        0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_Supervisor.d3dDevice08->SetTextureStageState(
        0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    return g_Supervisor.d3dDevice08->SetTextureStageState(
        0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
}
