#include <d3d8.h>
#include <stdio.h>

typedef unsigned int u32;

extern char sCapsStart[];
extern char sReadScanline[];
extern char sWindowed[];
extern char sImmediate[];
extern char sVsync[];
extern char sDeviceStart[];
extern char sNonlocalBlit[];
extern char sHardwareTl[];
extern char sTexNonlocal[];
extern char sTexSystem[];
extern char sTexVideo[];
extern char sVertexSystem[];
extern char sVertexVideo[];
extern char sPrimitiveStart[];
extern char sAlphaBlend[];
extern char sPointClip[];
extern char sPrimitiveClip[];
extern char sCullCcw[];
extern char sCullCw[];
extern char sCullNone[];
extern char sMaskZ[];
extern char sRasterStart[];
extern char sAnisotropy[];
extern char sAntialias[];
extern char sDither[];
extern char sFogRange[];
extern char sZFog[];
extern char sFogTable[];
extern char sFogVertex[];
extern char sZTest[];
extern char sShadeStart[];
extern char sGouraud[];
extern char sAlphaGouraud[];
extern char sFogGouraud[];
extern char sTextureStart[];
extern char sMaxTexture[];
extern char sTextureAlpha[];
extern char sTextureProjected[];
extern char sMagLinear[];
extern char sMinLinear[];
extern char sEnd[];
extern char sCan[];
extern char sCannot[];

static char *AppendD3DCapabilitySupportLine(
    char *capabilityName, u32 capabilityFlags, u32 mask, char *buf)
{
    buf += sprintf(buf, capabilityName);
    if ((capabilityFlags & mask) == 0)
        buf += sprintf(buf, sCannot);
    else
        buf += sprintf(buf, sCan);
    return buf;
}

static void FormatD3DCapabilities(D3DCAPS8 *caps, char *buf)
{
    char *strPos = buf;

    strPos += sprintf(strPos, sCapsStart);
    strPos = AppendD3DCapabilitySupportLine(sReadScanline, caps->Caps, D3DCAPS_READ_SCANLINE, strPos);
    strPos = AppendD3DCapabilitySupportLine(sWindowed, caps->Caps2, D3DCAPS2_CANRENDERWINDOWED, strPos);
    strPos = AppendD3DCapabilitySupportLine(sImmediate, caps->PresentationIntervals, D3DPRESENT_INTERVAL_IMMEDIATE, strPos);
    strPos = AppendD3DCapabilitySupportLine(sVsync, caps->PresentationIntervals, D3DPRESENT_INTERVAL_ONE, strPos);

    strPos += sprintf(strPos, sDeviceStart);
    strPos = AppendD3DCapabilitySupportLine(sNonlocalBlit, caps->DevCaps, D3DDEVCAPS_CANBLTSYSTONONLOCAL, strPos);
    strPos = AppendD3DCapabilitySupportLine(sHardwareTl, caps->DevCaps, D3DDEVCAPS_HWTRANSFORMANDLIGHT, strPos);
    strPos = AppendD3DCapabilitySupportLine(sTexNonlocal, caps->DevCaps, D3DDEVCAPS_TEXTURENONLOCALVIDMEM, strPos);
    strPos = AppendD3DCapabilitySupportLine(sTexSystem, caps->DevCaps, D3DDEVCAPS_TEXTURESYSTEMMEMORY, strPos);
    strPos = AppendD3DCapabilitySupportLine(sTexVideo, caps->DevCaps, D3DDEVCAPS_TEXTUREVIDEOMEMORY, strPos);
    strPos = AppendD3DCapabilitySupportLine(sVertexSystem, caps->DevCaps, D3DDEVCAPS_TLVERTEXSYSTEMMEMORY, strPos);
    strPos = AppendD3DCapabilitySupportLine(sVertexVideo, caps->DevCaps, D3DDEVCAPS_TLVERTEXVIDEOMEMORY, strPos);

    strPos += sprintf(strPos, sPrimitiveStart);
    strPos = AppendD3DCapabilitySupportLine(sAlphaBlend, caps->PrimitiveMiscCaps, D3DPMISCCAPS_BLENDOP, strPos);
    strPos = AppendD3DCapabilitySupportLine(sPointClip, caps->PrimitiveMiscCaps, D3DPMISCCAPS_CLIPPLANESCALEDPOINTS, strPos);
    strPos = AppendD3DCapabilitySupportLine(sPrimitiveClip, caps->PrimitiveMiscCaps, D3DPMISCCAPS_CLIPTLVERTS, strPos);
    strPos = AppendD3DCapabilitySupportLine(sCullCcw, caps->PrimitiveMiscCaps, D3DPMISCCAPS_CULLCCW, strPos);
    strPos = AppendD3DCapabilitySupportLine(sCullCw, caps->PrimitiveMiscCaps, D3DPMISCCAPS_CULLCW, strPos);
    strPos = AppendD3DCapabilitySupportLine(sCullNone, caps->PrimitiveMiscCaps, D3DPMISCCAPS_CULLNONE, strPos);
    strPos = AppendD3DCapabilitySupportLine(sMaskZ, caps->PrimitiveMiscCaps, D3DPMISCCAPS_MASKZ, strPos);

    strPos += sprintf(strPos, sRasterStart);
    strPos = AppendD3DCapabilitySupportLine(sAnisotropy, caps->RasterCaps, D3DPRASTERCAPS_ANISOTROPY, strPos);
    strPos = AppendD3DCapabilitySupportLine(sAntialias, caps->RasterCaps, D3DPRASTERCAPS_ANTIALIASEDGES, strPos);
    strPos = AppendD3DCapabilitySupportLine(sDither, caps->RasterCaps, D3DPRASTERCAPS_DITHER, strPos);
    strPos = AppendD3DCapabilitySupportLine(sFogRange, caps->RasterCaps, D3DPRASTERCAPS_FOGRANGE, strPos);
    strPos = AppendD3DCapabilitySupportLine(sZFog, caps->RasterCaps, D3DPRASTERCAPS_ZFOG, strPos);
    strPos = AppendD3DCapabilitySupportLine(sFogTable, caps->RasterCaps, D3DPRASTERCAPS_FOGTABLE, strPos);
    strPos = AppendD3DCapabilitySupportLine(sFogVertex, caps->RasterCaps, D3DPRASTERCAPS_FOGVERTEX, strPos);
    strPos = AppendD3DCapabilitySupportLine(sZTest, caps->RasterCaps, D3DPRASTERCAPS_ZTEST, strPos);

    strPos += sprintf(strPos, sShadeStart);
    strPos = AppendD3DCapabilitySupportLine(sGouraud, caps->ShadeCaps, D3DPSHADECAPS_COLORGOURAUDRGB, strPos);
    strPos = AppendD3DCapabilitySupportLine(sAlphaGouraud, caps->ShadeCaps, D3DPSHADECAPS_ALPHAGOURAUDBLEND, strPos);
    strPos = AppendD3DCapabilitySupportLine(sFogGouraud, caps->ShadeCaps, D3DPSHADECAPS_FOGGOURAUD, strPos);

    strPos += sprintf(strPos, sTextureStart);
    strPos += sprintf(strPos, sMaxTexture, caps->MaxTextureWidth, caps->MaxTextureHeight);
    strPos = AppendD3DCapabilitySupportLine(sTextureAlpha, caps->TextureCaps, D3DPTEXTURECAPS_ALPHA, strPos);
    strPos = AppendD3DCapabilitySupportLine(sTextureProjected, caps->TextureCaps, D3DPTEXTURECAPS_PROJECTED, strPos);
    strPos = AppendD3DCapabilitySupportLine(sMagLinear, caps->TextureFilterCaps, D3DPTFILTERCAPS_MAGFLINEAR, strPos);
    strPos = AppendD3DCapabilitySupportLine(sMinLinear, caps->TextureFilterCaps, D3DPTFILTERCAPS_MINFLINEAR, strPos);

    strPos += sprintf(strPos, sEnd);
}

#include "AsciiManager.hpp"
#include "GameErrorContext.hpp"
#include "GameWindow.hpp"
#include "ScreenEffect.hpp"
#include "Supervisor.hpp"

#include <stddef.h>
#include <string.h>

extern GameErrorContext g_GameErrorContext;

extern char sD3DInit32Bit[];
extern char sD3DSetRefresh60Hz[];
extern char sD3DTryAsyncVsync[];
extern char sD3DTlHalUnavailable[];
extern char sD3DHalUnavailable[];
extern char sD3DCannotSetRefresh[];
extern char sD3DAsyncVsyncUnsupported[];
extern char sD3DChangeRefreshRate[];
extern char sD3DInitFailed[];
extern char sD3DUsingRef[];
extern char sD3DUsingHal[];
extern char sD3DUsingTlHal[];
extern char sD3DMaxTextureTooSmall[];
extern char sD3DA8R8G8B8Unsupported[];

// Only the register-sensitive flags have a local receiver projection.
// Viewport records are the native SupervisorCtorArrayElement array.
struct SupervisorD3DInitView
{
    unsigned char unknown000[0x5D4];
    unsigned int flags5D4;
};

typedef char SupervisorD3DInitNativeFlags[
    (offsetof(SupervisorD3DInitView, flags5D4) == 0x5D4 &&
     offsetof(Supervisor, flags5D4) == 0x5D4) ? 1 : -1];

struct GameWindowD3DInitView
{
    HWND window00;
    int windowIsClosing04;
    unsigned char unknown008[0x1C - 0x08];
    unsigned char startupPathDiffers1C;
};

typedef char GameWindowD3DInitClosingAt04[
    (offsetof(GameWindowD3DInitView, windowIsClosing04) == 0x04) ? 1 : -1];
typedef char GameWindowD3DInitPathAt1C[
    (offsetof(GameWindowD3DInitView, startupPathDiffers1C) == 0x1C) ? 1 : -1];

void __fastcall ResetRenderState();

int InitializeD3DRendering()
{
    D3DDISPLAYMODE displayMode;
    D3DPRESENT_PARAMETERS presentParameters;
    int failedToSetFramerate;
    unsigned char usingHardwareRenderer;
    char capabilitiesBuffer[0x2000];

    SupervisorD3DInitView *supervisor =
        reinterpret_cast<SupervisorD3DInitView *>(&g_Supervisor);
    GameWindowD3DInitView *gameWindow =
        reinterpret_cast<GameWindowD3DInitView *>(&g_GameWindow);
    SupervisorCtorArrayElement (&viewports)[3] = g_Supervisor.ctorArray078;

    usingHardwareRenderer = 1;
    memset(&presentParameters, 0, sizeof(presentParameters));
    g_Supervisor.d3d04->GetAdapterDisplayMode(
        D3DADAPTER_DEFAULT, &displayMode);

    if (!g_Supervisor.config388.windowed)
    {
        if (g_Supervisor.Is16BitTexturesForced() == 1)
        {
            presentParameters.BackBufferFormat = D3DFMT_R5G6B5;
            g_Supervisor.config388.valueAD = 1;
        }
        else if (g_Supervisor.config388.valueAD == 0xff)
        {
            presentParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
            g_Supervisor.config388.valueAD = 0;
            g_GameErrorContext.Log(sD3DInit32Bit);
        }
        else
        {
            presentParameters.BackBufferFormat =
                g_Supervisor.config388.valueAD != 0 ?
                    D3DFMT_R5G6B5 : D3DFMT_X8R8G8B8;
        }

        if (gameWindow->startupPathDiffers1C)
            g_Supervisor.disableVsync = 1;

        if (!g_Supervisor.disableVsync)
        {
            presentParameters.FullScreen_RefreshRateInHz = 60;
            presentParameters.FullScreen_PresentationInterval =
                D3DPRESENT_INTERVAL_ONE;
            g_GameErrorContext.Log(sD3DSetRefresh60Hz);
            presentParameters.SwapEffect =
                g_Supervisor.config388.valueB2 != 0 ?
                    D3DSWAPEFFECT_COPY_VSYNC : D3DSWAPEFFECT_FLIP;
        }
        else
        {
            presentParameters.FullScreen_RefreshRateInHz =
                D3DPRESENT_RATE_DEFAULT;
            presentParameters.SwapEffect = D3DSWAPEFFECT_COPY;
            presentParameters.FullScreen_PresentationInterval =
                D3DPRESENT_INTERVAL_IMMEDIATE;
            g_GameErrorContext.Log(sD3DTryAsyncVsync);
        }
    }
    else
    {
        presentParameters.BackBufferFormat = displayMode.Format;
        presentParameters.SwapEffect = D3DSWAPEFFECT_COPY;
        presentParameters.Windowed = TRUE;
    }

    presentParameters.BackBufferWidth = 640;
    presentParameters.BackBufferHeight = 480;
    presentParameters.EnableAutoDepthStencil = TRUE;
    presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
    presentParameters.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    supervisor->flags5D4 |= 2;
    g_Supervisor.couldSetRefreshRate5B0 = 1;
    failedToSetFramerate = 0;

    for (;;)
    {
        if (g_Supervisor.IsReferenceRasterizerForced())
        {
            goto REFERENCE_RASTERIZER_MODE;
        }
        else
        {
            if (g_Supervisor.d3d04->CreateDevice(
                    D3DADAPTER_DEFAULT,
                    D3DDEVTYPE_HAL,
                    gameWindow->window00,
                    D3DCREATE_HARDWARE_VERTEXPROCESSING,
                    &presentParameters,
                    &g_Supervisor.d3dDevice08) < 0)
            {
                if (failedToSetFramerate)
                    g_GameErrorContext.Log(sD3DTlHalUnavailable);

                if (g_Supervisor.d3d04->CreateDevice(
                        D3DADAPTER_DEFAULT,
                        D3DDEVTYPE_HAL,
                        gameWindow->window00,
                        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                        &presentParameters,
                        &g_Supervisor.d3dDevice08) < 0)
                {
                    if (failedToSetFramerate)
                        g_GameErrorContext.Log(sD3DHalUnavailable);

                REFERENCE_RASTERIZER_MODE:
                    if (g_Supervisor.d3d04->CreateDevice(
                            D3DADAPTER_DEFAULT,
                            D3DDEVTYPE_REF,
                            gameWindow->window00,
                            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                            &presentParameters,
                            &g_Supervisor.d3dDevice08) < 0)
                    {
                        if (!g_Supervisor.disableVsync)
                        {
                            g_GameErrorContext.Log(sD3DCannotSetRefresh);
                            presentParameters.FullScreen_RefreshRateInHz =
                                D3DPRESENT_RATE_DEFAULT;
                            g_Supervisor.couldSetRefreshRate5B0 = 0;
                            failedToSetFramerate = 1;
                            continue;
                        }
                        else
                        {
                            if (presentParameters.FullScreen_PresentationInterval ==
                                D3DPRESENT_INTERVAL_IMMEDIATE)
                            {
                                g_GameErrorContext.Log(
                                    sD3DAsyncVsyncUnsupported);
                                g_GameErrorContext.Fatal(
                                    sD3DChangeRefreshRate);
                                presentParameters.FullScreen_PresentationInterval =
                                    D3DPRESENT_INTERVAL_ONE;
                                presentParameters.SwapEffect =
                                    D3DSWAPEFFECT_COPY;
                                continue;
                            }
                            else
                            {
                                g_GameErrorContext.Fatal(sD3DInitFailed);
                                if (g_Supervisor.d3d04 != NULL)
                                {
                                    g_Supervisor.d3d04->Release();
                                    g_Supervisor.d3d04 = NULL;
                                }
                                return 1;
                            }
                        }
                    }
                    else
                    {
                        g_GameErrorContext.Log(sD3DUsingRef);
                        supervisor->flags5D4 &= ~1u;
                        usingHardwareRenderer = 0;
                    }
                }
                else
                {
                    g_GameErrorContext.Log(sD3DUsingHal);
                    supervisor->flags5D4 &= ~1u;
                }
            }
            else
            {
                g_GameErrorContext.Log(sD3DUsingTlHal);
                supervisor->flags5D4 |= 1;
            }

            break;
        }
    }

    memcpy(
        &g_Supervisor.presentParameters350,
        &presentParameters,
        sizeof(presentParameters));

    viewports[2].cameraPosition00 =
        Float3(0.0f, 0.0f, 1000.0f);
    viewports[2].cameraLookAtOffset0C =
        Float3(0.0f, 0.0f, 0.0f);
    viewports[2].cameraUp18 =
        Float3(0.0f, 1.0f, 0.0f);
    viewports[2].fieldOfView48 = 0.52359879f;
    viewports[2].viewportCC.X = 0;
    viewports[2].viewportCC.Y = 0;
    viewports[2].viewportCC.Width = 640;
    viewports[2].viewportCC.Height = 480;
    viewports[2].viewportCC.MinZ = 0.0f;
    viewports[2].viewportCC.MaxZ = 1.0f;
    viewports[2].viewportModeE4 = 2;

    viewports[0].cameraPosition00 =
        Float3(0.0f, 0.0f, 1000.0f);
    viewports[0].cameraLookAtOffset0C =
        Float3(0.0f, 0.0f, 0.0f);
    viewports[0].cameraUp18 =
        Float3(0.0f, 1.0f, 0.0f);
    viewports[0].fieldOfView48 = 0.52359879f;
    viewports[0].viewportCC.X = 16;
    viewports[0].viewportCC.Y = 16;
    viewports[0].viewportCC.Width = 288;
    viewports[0].viewportCC.Height = 448;
    viewports[0].viewportCC.MinZ = 0.0f;
    viewports[0].viewportCC.MaxZ = 1.0f;
    viewports[0].viewportModeE4 = 0;

    viewports[1].cameraPosition00 =
        Float3(0.0f, 0.0f, 1000.0f);
    viewports[1].cameraLookAtOffset0C =
        Float3(0.0f, 0.0f, 0.0f);
    viewports[1].cameraUp18 =
        Float3(0.0f, 1.0f, 0.0f);
    viewports[1].fieldOfView48 = 0.52359879f;
    viewports[1].viewportCC.X = 336;
    viewports[1].viewportCC.Y = 16;
    viewports[1].viewportCC.Width = 288;
    viewports[1].viewportCC.Height = 448;
    viewports[1].viewportCC.MinZ = 0.0f;
    viewports[1].viewportCC.MaxZ = 1.0f;
    viewports[1].viewportModeE4 = 1;

    g_Supervisor.ConfigureScreenViewport(2);
    g_Supervisor.d3dDevice08->GetDeviceCaps(&g_Supervisor.d3dCaps5D8);

    if (g_Supervisor.d3dCaps5D8.MaxTextureWidth <= 256)
        g_GameErrorContext.Log(sD3DMaxTextureTooSmall);

    FormatD3DCapabilities(
        &g_Supervisor.d3dCaps5D8, capabilitiesBuffer);
    g_GameErrorContext.Log(capabilitiesBuffer);

    if (!g_Supervisor.Is16BitTexturesForced() && usingHardwareRenderer)
    {
        if (g_Supervisor.d3d04->CheckDeviceFormat(
                D3DADAPTER_DEFAULT,
                D3DDEVTYPE_HAL,
                presentParameters.BackBufferFormat,
                0,
                D3DRTYPE_TEXTURE,
                D3DFMT_A8R8G8B8) == D3D_OK)
        {
            supervisor->flags5D4 |= 4;
        }
        else
        {
            supervisor->flags5D4 &= ~4u;
            g_Supervisor.config388.options |= 1;
            g_GameErrorContext.Log(sD3DA8R8G8B8Unsupported);
        }
    }

    ResetRenderState();
    ScreenEffect::SetViewport(0xff000000);
    gameWindow->windowIsClosing04 = 0;
    g_Supervisor.lastFrameTime5B4 = 0;
    return 0;
}
