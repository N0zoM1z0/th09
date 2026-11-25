#include <d3d8.h>
#include <stdio.h>

typedef unsigned int u32;

char sCapsStart[] = "\x8C\xBB\x8D\xDD\x82\xCC\x83\x72\x83\x66\x83\x49\x83\x4A\x81\x5B\x83\x68\x81\x41\x8B\x79\x82\xD1\x83\x68\x83\x89\x83\x43\x83\x6F\x82\xCC\x94\x5C\x97\xCD\x8F\xDA\x8D\xD7\r\n"; // 現在のビデオカード、及びドライバの能力詳細\r\n
char sReadScanline[] = "\x81\x40\x91\x96\x8D\xB8\x90\xFC\x8E\xE6\x93\xBE\x94\x5C\x97\xCD : "; // 　走査線取得能力 :
char sWindowed[] = "\x81\x40\x83\x45\x83\x42\x83\x93\x83\x68\x83\x45\x83\x82\x81\x5B\x83\x68\x82\xCC\x83\x8C\x83\x93\x83\x5F\x83\x8A\x83\x93\x83\x4F : "; // 　ウィンドウモードのレンダリング :
char sImmediate[] = "\x81\x40\x83\x76\x83\x8C\x83\x5B\x83\x93\x83\x65\x81\x5B\x83\x56\x83\x87\x83\x93\x8A\xD4\x8A\x75\x81\x69\x92\xBC\x90\xDA\x81\x6A: "; // 　プレゼンテーション間隔（直接）:
char sVsync[] = "\x81\x40\x83\x76\x83\x8C\x83\x5B\x83\x93\x83\x65\x81\x5B\x83\x56\x83\x87\x83\x93\x8A\xD4\x8A\x75\x81\x69\x90\x82\x92\xBC\x93\xAF\x8A\xFA\x81\x6A: "; // 　プレゼンテーション間隔（垂直同期）:
char sDeviceStart[] = "\x81\x40-- \x83\x66\x83\x6F\x83\x43\x83\x58\x94\x5C\x97\xCD ------------------------------\r\n"; // 　-- デバイス能力 ------------------------------\r\n
char sNonlocalBlit[] = "\x81\x40System -> \x94\xF1\x83\x8D\x81\x5B\x83\x4A\x83\x8BVRAM\x83\x75\x83\x8A\x83\x62\x83\x67 : "; // 　System -> 非ローカルVRAMブリット :
char sHardwareTl[] = "\x81\x40\x83\x6E\x81\x5B\x83\x68\x83\x45\x83\x46\x83\x41 T&L : "; // 　ハードウェア T&L :
char sTexNonlocal[] = "\x81\x40\x94\xF1\x83\x8D\x81\x5B\x83\x4A\x83\x8BVRAM\x82\xA9\x82\xE7\x83\x65\x83\x4E\x83\x58\x83\x60\x83\x83\x8E\xE6\x93\xBE : "; // 　非ローカルVRAMからテクスチャ取得 :
char sTexSystem[] = "\x81\x40\x83\x56\x83\x58\x83\x65\x83\x80\x83\x81\x83\x82\x83\x8A\x82\xA9\x82\xE7\x83\x65\x83\x4E\x83\x58\x83\x60\x83\x83\x8E\xE6\x93\xBE : "; // 　システムメモリからテクスチャ取得 :
char sTexVideo[] = "\x81\x40VRAM \x82\xA9\x82\xE7\x83\x65\x83\x4E\x83\x58\x83\x60\x83\x83\x8E\xE6\x93\xBE : "; // 　VRAM からテクスチャ取得 :
char sVertexSystem[] = "\x81\x40\x92\xB8\x93\x5F\x83\x6F\x83\x62\x83\x74\x83\x40\x82\xC9\x83\x56\x83\x58\x83\x65\x83\x80\x83\x81\x83\x82\x83\x8A\x82\xF0\x8E\x67\x97\x70 : "; // 　頂点バッファにシステムメモリを使用 :
char sVertexVideo[] = "\x81\x40\x92\xB8\x93\x5F\x83\x6F\x83\x62\x83\x74\x83\x40\x82\xC9\x83\x72\x83\x66\x83\x49\x83\x81\x83\x82\x83\x8A\x82\xF0\x8E\x67\x97\x70 : "; // 　頂点バッファにビデオメモリを使用 :
char sPrimitiveStart[] = "\x81\x40-- \x83\x76\x83\x8A\x83\x7E\x83\x65\x83\x42\x83\x75\x94\x5C\x97\xCD ---------------------------\r\n"; // 　-- プリミティブ能力 ---------------------------\r\n
char sAlphaBlend[] = "\x81\x40\x94\xBC\x93\xA7\x96\xBE\x8F\x88\x97\x9D : "; // 　半透明処理 :
char sPointClip[] = "\x81\x40\x83\x7C\x83\x43\x83\x93\x83\x67\x82\xCC\x83\x4E\x83\x8A\x83\x62\x83\x73\x83\x93\x83\x4F\x8F\x88\x97\x9D : "; // 　ポイントのクリッピング処理 :
char sPrimitiveClip[] = "\x81\x40\x83\x76\x83\x8A\x83\x7E\x83\x65\x83\x42\x83\x75\x82\xCC\x83\x4E\x83\x8A\x83\x62\x83\x73\x83\x93\x83\x4F\x8F\x88\x97\x9D : "; // 　プリミティブのクリッピング処理 :
char sCullCcw[] = "\x81\x40\x96\x40\x90\xFC\x83\x4E\x83\x8A\x83\x62\x83\x76\x81\x69\x94\xBD\x8E\x9E\x8C\x76\x8E\xFC\x82\xE8\x81\x6A : "; // 　法線クリップ（反時計周り） :
char sCullCw[] = "\x81\x40\x96\x40\x90\xFC\x83\x4E\x83\x8A\x83\x62\x83\x76\x81\x69\x8E\x9E\x8C\x76\x8E\xFC\x82\xE8\x81\x6A : "; // 　法線クリップ（時計周り） :
char sCullNone[] = "\x81\x40\x96\x40\x90\xFC\x83\x4E\x83\x8A\x83\x62\x83\x76\x96\xB3\x82\xB5 : "; // 　法線クリップ無し :
char sMaskZ[] = "\x81\x40\x83\x66\x83\x76\x83\x58\x83\x65\x83\x58\x83\x67ON/OFF\x90\xD8\x82\xE8\x91\xD6\x82\xA6 : "; // 　デプステストON/OFF切り替え :
char sRasterStart[] = "\x81\x40-- \x83\x89\x83\x58\x83\x5E\x94\x5C\x97\xCD --------------------------------\r\n"; // 　-- ラスタ能力 --------------------------------\r\n
char sAnisotropy[] = "\x81\x40\x88\xD9\x95\xFB\x90\xAB\x83\x74\x83\x42\x83\x8B\x83\x5E\x83\x8A\x83\x93\x83\x4F : "; // 　異方性フィルタリング :
char sAntialias[] = "\x81\x40\x83\x41\x83\x93\x83\x60\x83\x47\x83\x43\x83\x8A\x83\x41\x83\x56\x83\x93\x83\x4F : "; // 　アンチエイリアシング :
char sDither[] = "\x81\x40\x83\x66\x83\x42\x83\x55\x8F\x88\x97\x9D : "; // 　ディザ処理 :
char sFogRange[] = "\x81\x40\x94\xCD\x88\xCD\x83\x78\x81\x5B\x83\x58\x82\xCC\x83\x74\x83\x48\x83\x4F : "; // 　範囲ベースのフォグ :
char sZFog[] = "\x81\x40Z\x83\x78\x81\x5B\x83\x58\x82\xCC\x83\x74\x83\x48\x83\x4F : "; // 　Zベースのフォグ :
char sFogTable[] = "\x81\x40\x83\x65\x81\x5B\x83\x75\x83\x8B\x83\x74\x83\x48\x83\x4F : "; // 　テーブルフォグ :
char sFogVertex[] = "\x81\x40\x92\xB8\x93\x5F\x83\x74\x83\x48\x83\x4F : "; // 　頂点フォグ :
char sZTest[] = "\x81\x40\x83\x66\x83\x76\x83\x58\x83\x65\x83\x58\x83\x67 : "; // 　デプステスト :
char sShadeStart[] = "\x81\x40-- \x83\x56\x83\x46\x81\x5B\x83\x66\x83\x42\x83\x93\x83\x4F\x94\x5C\x97\xCD -----------------------\r\n"; // 　-- シェーディング能力 -----------------------\r\n
char sGouraud[] = "\x81\x40\x83\x4F\x81\x5B\x83\x8D\x81\x5B\x83\x56\x83\x46\x81\x5B\x83\x66\x83\x42\x83\x93\x83\x4F : "; // 　グーローシェーディング :
char sAlphaGouraud[] = "\x81\x40\x83\xBF\x90\xAC\x95\xAA\x82\xCC\x83\x4F\x81\x5B\x83\x8D\x81\x5B\x83\x56\x83\x46\x81\x5B\x83\x66\x83\x42\x83\x93\x83\x4F : "; // 　α成分のグーローシェーディング :
char sFogGouraud[] = "\x81\x40\x83\x4F\x81\x5B\x83\x8D\x81\x5B\x83\x56\x83\x46\x81\x5B\x83\x66\x83\x42\x83\x93\x83\x4F\x82\xC5\x83\x74\x83\x48\x83\x4F : "; // 　グーローシェーディングでフォグ :
char sTextureStart[] = "\x81\x40-- \x83\x65\x83\x4E\x83\x58\x83\x60\x83\x83\x94\x5C\x97\xCD ---------------------------\r\n"; // 　-- テクスチャ能力 ---------------------------\r\n
char sMaxTexture[] = "\x81\x40\x8D\xC5\x91\xE5\x83\x65\x83\x4E\x83\x58\x83\x60\x83\x83\x83\x54\x83\x43\x83\x59 : (%d, %d)\r\n"; // 　最大テクスチャサイズ : (%d, %d)\r\n
char sTextureAlpha[] = "\x81\x40\x83\xBF\x95\x74\x82\xAB\x83\x65\x83\x4E\x83\x58\x83\x60\x83\x83 : "; // 　α付きテクスチャ :
char sTextureProjected[] = "\x81\x40\x83\x65\x83\x4E\x83\x58\x83\x60\x83\x83\x83\x67\x83\x89\x83\x93\x83\x58\x83\x74\x83\x48\x81\x5B\x83\x80 : "; // 　テクスチャトランスフォーム :
char sMagLinear[] = "\x81\x40\x83\x6F\x83\x43\x83\x8A\x83\x6A\x83\x41\x95\xE2\x8A\xD4\x81\x69\x8A\x67\x91\xE5\x81\x6A : "; // 　バイリニア補間（拡大） :
char sMinLinear[] = "\x81\x40\x83\x6F\x83\x43\x83\x8A\x83\x6A\x83\x41\x95\xE2\x8A\xD4\x81\x69\x8F\x6B\x8F\xAC\x81\x6A : "; // 　バイリニア補間（縮小） :
char sEnd[] = "--------------------------------------------\r\n"; // --------------------------------------------\r\n
char sCan[] = "\x89\xC2\r\n"; // 可\r\n
char sCannot[] = "\x95\x73\x89\xC2\r\n"; // 不可\r\n

// Capability reports append each formatted line to the original
// character buffer using ordinary pointer arithmetic.
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

char sD3DInit32Bit[] = "\x8F\x89\x89\xF1\x8B\x4E\x93\xAE\x81\x41\x89\xE6\x96\xCA\x82\xF0 32Bits \x82\xC5\x8F\x89\x8A\xFA\x89\xBB\x82\xB5\x82\xDC\x82\xB5\x82\xBD\r\n"; // 初回起動、画面を 32Bits で初期化しました\r\n
char sD3DSetRefresh60Hz[] = "\x83\x8A\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x8C\x81\x5B\x83\x67\x82\xF0" "60Hz\x82\xC9\x95\xCF\x8D\x58\x82\xF0\x8E\x8E\x82\xDD\x82\xDC\x82\xB7\r\n"; // リフレッシュレートを60Hzに変更を試みます\r\n
char sD3DTryAsyncVsync[] = "VSync\x94\xF1\x93\xAF\x8A\xFA\x89\xC2\x94\x5C\x82\xA9\x82\xC7\x82\xA4\x82\xA9\x82\xF0\x8E\x8E\x82\xDD\x82\xDC\x82\xB7\r\n"; // VSync非同期可能かどうかを試みます\r\n
char sD3DTlHalUnavailable[] = "T&L HAL \x82\xCD\x8E\x67\x97\x70\x82\xC5\x82\xAB\x82\xC8\x82\xA2\x82\xE6\x82\xA4\x82\xC5\x82\xB7\r\n"; // T&L HAL は使用できないようです\r\n
char sD3DHalUnavailable[] = "HAL \x82\xE0\x8E\x67\x97\x70\x82\xC5\x82\xAB\x82\xC8\x82\xA2\x82\xE6\x82\xA4\x82\xC5\x82\xB7\r\n"; // HAL も使用できないようです\r\n
char sD3DCannotSetRefresh[] = "\x83\x8A\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x8C\x81\x5B\x83\x67\x82\xAA\x95\xCF\x8D\x58\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\r\n"; // リフレッシュレートが変更できません\r\n
char sD3DAsyncVsyncUnsupported[] = "\x94\xF1\x93\xAF\x8A\xFA\x8D\x58\x90\x56\x82\xE0\x8D\x73\x82\xA6\x82\xDC\x82\xB9\x82\xF1\x81\x42\x88\xEA\x94\xD4\x89\x98\x82\xA2\x83\x82\x81\x5B\x83\x68\x82\xC9\x95\xCF\x8D\x58\x82\xB5\x82\xDC\x82\xB7\r\n"; // 非同期更新も行えません。一番汚いモードに変更します\r\n
char sD3DChangeRefreshRate[] = "*** \x83\x8A\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x8C\x81\x5B\x83\x67\x82\xF0" "60Hz\x82\xC9\x95\xCF\x8D\x58\x82\xB7\x82\xE9\x82\xB1\x82\xC6\x82\xF0\x90\x84\x8F\xA7\x82\xB5\x82\xDC\x82\xB7 ***\r\n"; // *** リフレッシュレートを60Hzに変更することを推奨します ***\r\n
char sD3DInitFailed[] = "Direct3D \x82\xCC\x8F\x89\x8A\xFA\x89\xBB\x82\xC9\x8E\xB8\x94\x73\x81\x41\x82\xB1\x82\xEA\x82\xC5\x82\xCD\x83\x51\x81\x5B\x83\x80\x82\xCD\x8F\x6F\x97\x88\x82\xDC\x82\xB9\x82\xF1\r\n"; // Direct3D の初期化に失敗、これではゲームは出来ません\r\n
char sD3DUsingRef[] = "REF \x82\xC5\x93\xAE\x8D\xEC\x82\xB5\x82\xDC\x82\xB7\x82\xAA\x81\x41\x8F\x64\x82\xB7\x82\xAC\x82\xC4\x8B\xB0\x82\xE7\x82\xAD\x83\x51\x81\x5B\x83\x80\x82\xC9\x82\xC8\x82\xE8\x82\xDC\x82\xB9\x82\xF1...\r\n"; // REF で動作しますが、重すぎて恐らくゲームになりません...\r\n
char sD3DUsingHal[] = "HAL \x82\xC5\x93\xAE\x8D\xEC\x82\xB5\x82\xDC\x82\xB7\r\n"; // HAL で動作します\r\n
char sD3DUsingTlHal[] = "T&L HAL \x82\xC5\x93\xAE\x8D\xEC\x82\xB5\x82\xDC\x81\x60\x82\xB7\r\n"; // T&L HAL で動作しま～す\r\n
char sD3DMaxTextureTooSmall[] = "512 \x88\xC8\x8F\xE3\x82\xCC\x83\x65\x83\x4E\x83\x58\x83\x60\x83\x83\x82\xF0\x83\x54\x83\x7C\x81\x5B\x83\x67\x82\xB5\x82\xC4\x82\xA2\x82\xDC\x82\xB9\x82\xF1\x81\x42\x96\x77\x82\xC7\x82\xCC\x8A\x47\x82\xAA\x83\x7B\x83\x50\x82\xC4\x95\x5C\x8E\xA6\x82\xB3\x82\xEA\x82\xDC\x82\xB7\x81\x42\r\n"; // 512 以上のテクスチャをサポートしていません。殆どの絵がボケて表示されます。\r\n
char sD3DA8R8G8B8Unsupported[] = "D3DFMT_A8R8G8B8 \x82\xF0\x83\x54\x83\x7C\x81\x5B\x83\x67\x82\xB5\x82\xC4\x82\xA2\x82\xDC\x82\xB9\x82\xF1\x81\x41\x8C\xB8\x90\x46\x83\x82\x81\x5B\x83\x68\x82\xC5\x93\xAE\x8D\xEC\x82\xB5\x82\xDC\x82\xB7\r\n"; // D3DFMT_A8R8G8B8 をサポートしていません、減色モードで動作します\r\n

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

// The +0x04 closing flag and +0x1C startup path byte belong to the
// single real GameWindow, also referenced from WinMain and Present.
typedef char GameWindowD3DInitClosingAt04[
    (offsetof(GameWindow, windowIsClosing04) == 0x04) ? 1 : -1];
typedef char GameWindowD3DInitPathAt1C[
    (offsetof(GameWindow, startupPathDiffers1C) == 0x1C) ? 1 : -1];

void __fastcall ResetRenderState();

typedef char D3DStartupNativeViewportStorage[
    (offsetof(Supervisor, ctorArray078) == 0x78 &&
     sizeof(SupervisorCtorArrayElement) == 0xF0 &&
     sizeof(((Supervisor *)0)->ctorArray078) == 3 * 0xF0) ? 1 : -1];

// The three fullscreen/gameplay D3D viewports share the genuine +0x78
// SupervisorCtorArrayElement[3] allocation with physical 0xF0 stride.
// Literal indices retain their original Win32 32-bit record addresses.
// The three original 0xF0-byte camera/viewport records are genuine
// SupervisorCtorArrayElement elements in one native C++ array.

int InitializeD3DRendering()
{
    D3DDISPLAYMODE displayMode;
    D3DPRESENT_PARAMETERS presentParameters;
    int failedToSetFramerate;
    unsigned char usingHardwareRenderer;
    char capabilitiesBuffer[0x2000];

    SupervisorD3DInitView *supervisor =
        reinterpret_cast<SupervisorD3DInitView *>(&g_Supervisor);
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

        if (g_GameWindow.startupPathDiffers1C)
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
                    g_GameWindow.window00,
                    D3DCREATE_HARDWARE_VERTEXPROCESSING,
                    &presentParameters,
                    &g_Supervisor.d3dDevice08) < 0)
            {
                if (failedToSetFramerate)
                    g_GameErrorContext.Log(sD3DTlHalUnavailable);

                if (g_Supervisor.d3d04->CreateDevice(
                        D3DADAPTER_DEFAULT,
                        D3DDEVTYPE_HAL,
                        g_GameWindow.window00,
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
                            g_GameWindow.window00,
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
    g_GameWindow.windowIsClosing04 = 0;
    g_Supervisor.lastFrameTime5B4 = 0;
    return 0;
}
