#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"
#include "ZunTimer.hpp"
#include "FrontMessageRuntimeView.hpp"

#include <d3d8.h>
#include <stddef.h>

struct FrontMessageDrawVertex
{
    FrontMessageDrawVertex();

    Float3 pos;
    float rhw;
    unsigned long diffuse;
};
typedef char FrontMessageDrawVertexSizeIs14[
    (sizeof(FrontMessageDrawVertex) == 0x14) ? 1 : -1];

struct FrontMessageDrawSupervisorView
{
    unsigned char unknown00[0x08];
    IDirect3DDevice8 *d3dDevice08;
};

static __inline FrontMessageDrawSupervisorView *FrontMessageDrawSupervisor()
{
    return reinterpret_cast<FrontMessageDrawSupervisorView *>(&g_Supervisor);
}

int FrontMessageRuntimeView::Draw()
{
    if (currentIndex08 < 0)
        return -1;

    if (colorIndex1D64 < 2)
    {
        g_AnmManager->DrawNoRotation(&messageVms14[6]);
        g_AnmManager->DrawNoRotation(&messageVms14[7]);
        g_AnmManager->DrawNoRotation(&messageVms14[4]);
        g_AnmManager->DrawNoRotation(&messageVms14[5]);
        g_AnmManager->DrawNoRotation(&messageVms14[0]);
        g_AnmManager->DrawNoRotation(&messageVms14[1]);
    }
    else if (colorIndex1D64 == 2)
    {
        g_AnmManager->DrawNoRotation(&messageVms14[6]);
        g_AnmManager->DrawNoRotation(&messageVms14[7]);
        g_AnmManager->DrawNoRotation(&messageVms14[0]);
        g_AnmManager->DrawNoRotation(&messageVms14[1]);
        g_AnmManager->DrawNoRotation(&messageVms14[4]);
        g_AnmManager->DrawNoRotation(&messageVms14[5]);
    }
    else
    {
        g_AnmManager->DrawNoRotation(&messageVms14[4]);
        g_AnmManager->DrawNoRotation(&messageVms14[5]);
        g_AnmManager->DrawNoRotation(&messageVms14[0]);
        g_AnmManager->DrawNoRotation(&messageVms14[1]);
        g_AnmManager->DrawNoRotation(&messageVms14[6]);
        g_AnmManager->DrawNoRotation(&messageVms14[7]);
    }

    g_AnmManager->DrawNoRotationFlippedX(&messageVms14[2]);
    g_AnmManager->DrawNoRotationFlippedX(&messageVms14[3]);

    float height;
    if (secondaryTimer1D58 < 40)
        height = static_cast<float>(secondaryTimer1D58) * 1.5f;
    else
        height = 60.0f;

    if (height > 0.0f)
    {
        FrontMessageDrawVertex vertices[4];

    vertices[0].pos = Float3(64.0f, 364.0f, 0.0f);
    vertices[1].pos = Float3(576.0f, 364.0f, 0.0f);

    height += 364.0f;
    vertices[2].pos = Float3(64.0f, height, 0.0f);
    vertices[3].pos = Float3(576.0f, height, 0.0f);

    vertices[0].diffuse = vertices[1].diffuse = 0xD0000000UL;
    vertices[2].diffuse = vertices[3].diffuse = 0x90000000UL;

    vertices[0].rhw =
        vertices[1].rhw =
        vertices[2].rhw =
        vertices[3].rhw = 1.0f;

    g_AnmManager->FlushVertexBuffer();

    if (textBoxVisible1D6D != 0)
    {
        reinterpret_cast<FrontMessageDrawSupervisorView *>(&g_Supervisor)->d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        reinterpret_cast<FrontMessageDrawSupervisorView *>(&g_Supervisor)->d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        reinterpret_cast<FrontMessageDrawSupervisorView *>(&g_Supervisor)->d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        reinterpret_cast<FrontMessageDrawSupervisorView *>(&g_Supervisor)->d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

        g_Supervisor.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

        reinterpret_cast<FrontMessageDrawSupervisorView *>(&g_Supervisor)->d3dDevice08->SetVertexShader(
            D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
        reinterpret_cast<FrontMessageDrawSupervisorView *>(&g_Supervisor)->d3dDevice08->DrawPrimitiveUP(
            D3DPT_TRIANGLESTRIP,
            2,
            vertices,
            sizeof(FrontMessageDrawVertex));

        g_AnmManager->ClearVertexShader();
        g_AnmManager->ClearColorOp();
        g_AnmManager->ClearBlendMode();
        g_AnmManager->ClearZWrite();

        reinterpret_cast<FrontMessageDrawSupervisorView *>(&g_Supervisor)->d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        reinterpret_cast<FrontMessageDrawSupervisorView *>(&g_Supervisor)->d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        reinterpret_cast<FrontMessageDrawSupervisorView *>(&g_Supervisor)->d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        reinterpret_cast<FrontMessageDrawSupervisorView *>(&g_Supervisor)->d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        }
    }

    g_AnmManager->DrawNoRotation(&textVms1534[0]);
    g_AnmManager->DrawNoRotation(&textVms1534[1]);
    g_AnmManager->DrawNoRotation(&introVm1A7C);
    return 0;
}
