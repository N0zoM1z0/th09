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
