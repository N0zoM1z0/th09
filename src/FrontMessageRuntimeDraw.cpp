#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"
#include "ZunTimer.hpp"

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

struct FrontMessageRuntimeView
{
    void *primary00;
    void *current04;
    int currentIndex08;
    void *secondary0C;
    int mode10;
    AnmVm vms14[11];
    unsigned long textColors1D20[4];
    unsigned long shadowColors1D30[4];
    unsigned char unknown1D40[0x10];
    int textSelectorCount1D50;
    int glyphSize1D54;
    ZunTimer boxTimer1D58;
    unsigned char colorIndex1D64;
    unsigned char flag1D65;
    unsigned char flag1D66;
    unsigned char flag1D67;
    unsigned char unknown1D68[4];
    unsigned char fontFlag1D6C;
    unsigned char boxVisible1D6D;
    unsigned char unknown1D6E[2];

    int Draw();
};

typedef char FrontMessageDrawVmsAt14[
    (offsetof(FrontMessageRuntimeView, vms14) == 0x14) ? 1 : -1];
typedef char FrontMessageDrawColorsAt1D20[
    (offsetof(FrontMessageRuntimeView, textColors1D20) == 0x1D20) ? 1 : -1];
typedef char FrontMessageDrawTimerAt1D58[
    (offsetof(FrontMessageRuntimeView, boxTimer1D58) == 0x1D58) ? 1 : -1];
typedef char FrontMessageDrawSelectorAt1D64[
    (offsetof(FrontMessageRuntimeView, colorIndex1D64) == 0x1D64) ? 1 : -1];
typedef char FrontMessageDrawVisibleAt1D6D[
    (offsetof(FrontMessageRuntimeView, boxVisible1D6D) == 0x1D6D) ? 1 : -1];
typedef char FrontMessageDrawSizeIs1D70[
    (sizeof(FrontMessageRuntimeView) == 0x1D70) ? 1 : -1];

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
        g_AnmManager->DrawNoRotation(&vms14[6]);
        g_AnmManager->DrawNoRotation(&vms14[7]);
        g_AnmManager->DrawNoRotation(&vms14[4]);
        g_AnmManager->DrawNoRotation(&vms14[5]);
        g_AnmManager->DrawNoRotation(&vms14[0]);
        g_AnmManager->DrawNoRotation(&vms14[1]);
    }
    else if (colorIndex1D64 == 2)
    {
        g_AnmManager->DrawNoRotation(&vms14[6]);
        g_AnmManager->DrawNoRotation(&vms14[7]);
        g_AnmManager->DrawNoRotation(&vms14[0]);
        g_AnmManager->DrawNoRotation(&vms14[1]);
        g_AnmManager->DrawNoRotation(&vms14[4]);
        g_AnmManager->DrawNoRotation(&vms14[5]);
    }
    else
    {
        g_AnmManager->DrawNoRotation(&vms14[4]);
        g_AnmManager->DrawNoRotation(&vms14[5]);
        g_AnmManager->DrawNoRotation(&vms14[0]);
        g_AnmManager->DrawNoRotation(&vms14[1]);
        g_AnmManager->DrawNoRotation(&vms14[6]);
        g_AnmManager->DrawNoRotation(&vms14[7]);
    }

    g_AnmManager->DrawNoRotationFlippedX(&vms14[2]);
    g_AnmManager->DrawNoRotationFlippedX(&vms14[3]);

    float height;
    if (boxTimer1D58 < 40)
        height = static_cast<float>(boxTimer1D58) * 1.5f;
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

    if (boxVisible1D6D != 0)
    {
        FrontMessageDrawSupervisorView *supervisor =
            FrontMessageDrawSupervisor();

        supervisor->d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
        supervisor->d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
        supervisor->d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        supervisor->d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

        g_Supervisor.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

        supervisor->d3dDevice08->SetVertexShader(
            D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
        supervisor->d3dDevice08->DrawPrimitiveUP(
            D3DPT_TRIANGLESTRIP,
            2,
            vertices,
            sizeof(FrontMessageDrawVertex));

        g_AnmManager->ClearVertexShader();
        g_AnmManager->ClearColorOp();
        g_AnmManager->ClearBlendMode();
        g_AnmManager->ClearZWrite();

        supervisor->d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        supervisor->d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        supervisor->d3dDevice08->SetTextureStageState(
            0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        supervisor->d3dDevice08->SetTextureStageState(
            0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        }
    }

    g_AnmManager->DrawNoRotation(&vms14[8]);
    g_AnmManager->DrawNoRotation(&vms14[9]);
    g_AnmManager->DrawNoRotation(&vms14[10]);
    return 0;
}
