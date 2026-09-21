#include "AnmManager.hpp"
#include "Supervisor.hpp"

#include <d3d8.h>

struct ResetRenderSupervisorView
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice08;
    unsigned char unknown00C[0x5FC - 0x0C];
    DWORD rasterCaps5FC;
};

struct ResetRenderBackgroundView
{
    unsigned char unknown000[0xCC8];
    unsigned char deviceResetFlagCC8;
};

struct ResetRenderSideView
{
    ResetRenderBackgroundView *subsystem0;
    unsigned char unknown04[0x38 - 0x04];
};

struct ResetRenderGameManagerView
{
    ResetRenderSideView sides[2];
};

extern ResetRenderGameManagerView g_GameManager;

#define RESET_RENDER_DEVICE     (reinterpret_cast<ResetRenderSupervisorView *>(&g_Supervisor)->d3dDevice08)

void ResetRenderState()
{
    float fogValue;

    RESET_RENDER_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
    RESET_RENDER_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);
    RESET_RENDER_DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    RESET_RENDER_DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    RESET_RENDER_DEVICE->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    RESET_RENDER_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    RESET_RENDER_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    RESET_RENDER_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    RESET_RENDER_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    RESET_RENDER_DEVICE->SetRenderState(D3DRS_ALPHAREF, 4);
    RESET_RENDER_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

    if (!g_Supervisor.IsFogDisabled())
        RESET_RENDER_DEVICE->SetRenderState(D3DRS_FOGENABLE, TRUE);
    else
        RESET_RENDER_DEVICE->SetRenderState(D3DRS_FOGENABLE, FALSE);

    fogValue = 1.0f;
    RESET_RENDER_DEVICE->SetRenderState(
        D3DRS_FOGDENSITY, *reinterpret_cast<DWORD *>(&fogValue));
    RESET_RENDER_DEVICE->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
    RESET_RENDER_DEVICE->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    RESET_RENDER_DEVICE->SetRenderState(D3DRS_FOGCOLOR, 0xffa0a0a0);

    fogValue = 1000.0f;
    RESET_RENDER_DEVICE->SetRenderState(
        D3DRS_FOGSTART, *reinterpret_cast<DWORD *>(&fogValue));
    fogValue = 5000.0f;
    RESET_RENDER_DEVICE->SetRenderState(
        D3DRS_FOGEND, *reinterpret_cast<DWORD *>(&fogValue));

    if (reinterpret_cast<ResetRenderSupervisorView *>(&g_Supervisor)
            ->rasterCaps5FC |
        D3DPRASTERCAPS_ANTIALIASEDGES)
    {
        RESET_RENDER_DEVICE->SetRenderState(D3DRS_EDGEANTIALIAS, FALSE);
    }

    RESET_RENDER_DEVICE->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

    RESET_RENDER_DEVICE->SetTextureStageState(
        0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    RESET_RENDER_DEVICE->SetTextureStageState(
        0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    RESET_RENDER_DEVICE->SetTextureStageState(
        0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
    RESET_RENDER_DEVICE->SetTextureStageState(
        0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    RESET_RENDER_DEVICE->SetTextureStageState(
        0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    RESET_RENDER_DEVICE->SetTextureStageState(
        0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    RESET_RENDER_DEVICE->SetTextureStageState(
        0, D3DTSS_MIPFILTER, D3DTEXF_NONE);
    RESET_RENDER_DEVICE->SetTextureStageState(
        0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
    RESET_RENDER_DEVICE->SetTextureStageState(
        0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
    RESET_RENDER_DEVICE->SetTextureStageState(
        0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    RESET_RENDER_DEVICE->SetTextureStageState(
        0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
    RESET_RENDER_DEVICE->SetTextureStageState(
        0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
    RESET_RENDER_DEVICE->SetTextureStageState(
        0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);

    if (g_AnmManager != NULL)
    {
        g_AnmManager->ClearBlendMode();
        g_AnmManager->ClearColorOp();
        g_AnmManager->ClearVertexShader();
        g_AnmManager->ClearTexture();
        g_AnmManager->ClearCameraSettings();
    }

    if (g_GameManager.sides[0].subsystem0 != NULL)
        g_GameManager.sides[0].subsystem0->deviceResetFlagCC8 = 1;
}
