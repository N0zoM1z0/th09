#include "AnmManager.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <d3d8.h>

class TextHelper
{
  public:
    static void RenderTextToTextureBold(
        int x, int y, int width, int height, int fontHeight, int fontWidth,
        unsigned long textColor, unsigned long outlineColor,
        const char *text, IDirect3DTexture8 *outTexture);
    static void RenderTextToTexture(
        int x, int y, int width, int height, int fontHeight, int fontWidth,
        unsigned long textColor, unsigned long outlineColor,
        const char *text, IDirect3DTexture8 *outTexture);
};

struct Float2UtilityView
{
    float x;
    float y;
};

struct AnmLoadedSpriteUtilityView
{
    int anmIndex;
    IDirect3DTexture8 *texture;
    Float2UtilityView startPixelInclusive;
    Float2UtilityView endPixelInclusive;
    float height;
    float width;
    Float2UtilityView uvStart;
    Float2UtilityView uvEnd;
    float heightPx;
    float widthPx;
    Float2UtilityView scaleFactor;
    unsigned int unknown40;
};

typedef char AnmUtilitySpriteSizeIs44[
    (sizeof(AnmLoadedSpriteUtilityView) == 0x44) ? 1 : -1];
typedef char AnmUtilitySpriteWidthPxAt34[
    (offsetof(AnmLoadedSpriteUtilityView, widthPx) == 0x34) ? 1 : -1];
typedef char AnmUtilitySpriteScaleAt38[
    (offsetof(AnmLoadedSpriteUtilityView, scaleFactor) == 0x38) ? 1 : -1];

struct AnmTextVmView
{
    unsigned char unknown000[0x1F8];
    unsigned int flagsWord;
    unsigned char unknown1FC[0x224 - 0x1FC];
    AnmLoadedSprite *loadedSprite;
    unsigned char unknown228[0x298 - 0x228];
    unsigned char fontWidth;
    unsigned char fontHeight;
};

typedef char AnmTextFlagsAt1F8[
    (offsetof(AnmTextVmView, flagsWord) == 0x1F8) ? 1 : -1];
typedef char AnmTextSpriteAt224[
    (offsetof(AnmTextVmView, loadedSprite) == 0x224) ? 1 : -1];
typedef char AnmTextFontWidthAt298[
    (offsetof(AnmTextVmView, fontWidth) == 0x298) ? 1 : -1];
typedef char AnmTextFontHeightAt299[
    (offsetof(AnmTextVmView, fontHeight) == 0x299) ? 1 : -1];

struct AnmSurfaceInfoView
{
    unsigned int Width;
    unsigned int Height;
    unsigned char unknown008[0x0C];
};

typedef char AnmSurfaceInfoSizeIs14[
    (sizeof(AnmSurfaceInfoView) == 0x14) ? 1 : -1];

struct AnmManagerSurfaceView
{
    unsigned char unknown000[0x123FC];
    IDirect3DSurface8 *surfaces[32];
    IDirect3DSurface8 *surfacesBis[32];
    unsigned char *surfaceData[32];
    int surfaceDataSizes[32];
    AnmSurfaceInfoView surfaceInfo[32];
};

typedef char AnmSurfacePrimaryAt123FC[
    (offsetof(AnmManagerSurfaceView, surfaces) == 0x123FC) ? 1 : -1];
typedef char AnmSurfaceSecondaryAt1247C[
    (offsetof(AnmManagerSurfaceView, surfacesBis) == 0x1247C) ? 1 : -1];
typedef char AnmSurfaceDataAt124FC[
    (offsetof(AnmManagerSurfaceView, surfaceData) == 0x124FC) ? 1 : -1];
typedef char AnmSurfaceDataSizesAt1257C[
    (offsetof(AnmManagerSurfaceView, surfaceDataSizes) == 0x1257C) ? 1 : -1];
typedef char AnmSurfaceInfoAt125FC[
    (offsetof(AnmManagerSurfaceView, surfaceInfo) == 0x125FC) ? 1 : -1];

struct AnmSurfaceTextureEntryView
{
    IDirect3DTexture8 *texture;
    unsigned char *rawData;
    int size;
};

typedef char AnmSurfaceTextureEntrySizeIs0C[
    (sizeof(AnmSurfaceTextureEntryView) == 0x0C) ? 1 : -1];

struct AnmSurfaceSlotView
{
    unsigned char unknown000[0x38];
    AnmSurfaceTextureEntryView *textures;
    unsigned char unknown03C[0x120 - 0x3C];
};

typedef char AnmSurfaceSlotSizeIs120[
    (sizeof(AnmSurfaceSlotView) == 0x120) ? 1 : -1];
typedef char AnmSurfaceSlotTexturesAt38[
    (offsetof(AnmSurfaceSlotView, textures) == 0x38) ? 1 : -1];

struct SupervisorUtilityView
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice;
    unsigned char unknown00C[0x350 - 0x0C];
    D3DPRESENT_PARAMETERS presentParameters;
};

typedef char SupervisorUtilityDeviceAt08[
    (offsetof(SupervisorUtilityView, d3dDevice) == 0x08) ? 1 : -1];
typedef char SupervisorUtilityPresentAt350[
    (offsetof(SupervisorUtilityView, presentParameters) == 0x350) ? 1 : -1];

HRESULT WINAPI D3DXLoadSurfaceFromSurface(
    IDirect3DSurface8 *destSurface, const PALETTEENTRY *destPalette,
    const RECT *destRect, IDirect3DSurface8 *srcSurface,
    const PALETTEENTRY *srcPalette, const RECT *srcRect,
    unsigned long filter, D3DCOLOR colorKey);

static __inline SupervisorUtilityView *UtilitySupervisor()
{
    return reinterpret_cast<SupervisorUtilityView *>(&g_Supervisor);
}

static __inline AnmManagerSurfaceView *SurfaceManager(AnmManager *manager)
{
    return reinterpret_cast<AnmManagerSurfaceView *>(manager);
}

static __inline AnmSurfaceSlotView *SurfaceSlots(AnmManager *manager)
{
    return reinterpret_cast<AnmSurfaceSlotView *>(manager);
}

void AnmManager::DrawTextInner(
    IDirect3DTexture8 *outTexture, int x, int y, int width, int height,
    int fontWidth, int fontHeight, unsigned long textColor,
    unsigned long outlineColor, const char *buffer, float scaleFactorX,
    float scaleFactorY)
{
    if (fontWidth <= 0)
        fontWidth = 15;
    if (fontHeight <= 0)
        fontHeight = 15;

    if (fontWidth > 8)
    {
        TextHelper::RenderTextToTextureBold(
            x, y, width, height, fontWidth * scaleFactorX,
            fontHeight * scaleFactorY, textColor, outlineColor, buffer,
            outTexture);
    }
    else
    {
        TextHelper::RenderTextToTexture(
            x, y, width, height, 8, 8, textColor, outlineColor, buffer,
            outTexture);
    }
}

void AnmManager::DrawTextLeft(
    AnmVm *vmObject, unsigned long textColor, unsigned long shadowColor,
    const char *format, ...)
{
    char buffer[128];
    AnmTextVmView *vm = reinterpret_cast<AnmTextVmView *>(vmObject);
    int fontWidth = vm->fontWidth;
    va_list args;

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    AnmLoadedSpriteUtilityView *sprite =
        reinterpret_cast<AnmLoadedSpriteUtilityView *>(vm->loadedSprite);
    this->DrawTextInner(
        sprite->texture, sprite->startPixelInclusive.x,
        sprite->startPixelInclusive.y, sprite->width, sprite->height,
        fontWidth, vm->fontHeight, textColor, shadowColor, buffer,
        sprite->scaleFactor.x, sprite->scaleFactor.y);
    vm->flagsWord |= 1;
}

void AnmManager::DrawTextCentered(
    AnmVm *vmObject, unsigned long textColor, unsigned long shadowColor,
    const char *format, ...)
{
    char buffer[68];
    AnmTextVmView *vm = reinterpret_cast<AnmTextVmView *>(vmObject);
    int fontWidth = vm->fontWidth <= 0 ? 15 : vm->fontWidth;
    va_list args;

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    unsigned int textLength = strlen(buffer);
    AnmLoadedSpriteUtilityView *sprite =
        reinterpret_cast<AnmLoadedSpriteUtilityView *>(vm->loadedSprite);
    int x = sprite->startPixelInclusive.x +
        sprite->widthPx * sprite->scaleFactor.x / 2.0f -
        static_cast<float>(textLength) * fontWidth *
            sprite->scaleFactor.x / 4.0f;
    this->DrawTextInner(
        sprite->texture, x, sprite->startPixelInclusive.y,
        sprite->width, sprite->height, fontWidth, vm->fontHeight,
        textColor, shadowColor, buffer, sprite->scaleFactor.x,
        sprite->scaleFactor.y);
    vm->flagsWord |= 1;
}

void AnmManager::ReleaseSurface(int surfaceIndex)
{
    AnmManagerSurfaceView *manager = SurfaceManager(this);
    if (manager->surfaces[surfaceIndex] != NULL)
    {
        manager->surfaces[surfaceIndex]->Release();
        manager->surfaces[surfaceIndex] = NULL;
    }
    if (manager->surfacesBis[surfaceIndex] != NULL)
    {
        manager->surfacesBis[surfaceIndex]->Release();
        manager->surfacesBis[surfaceIndex] = NULL;
    }
    if (manager->surfaceData[surfaceIndex] != NULL)
        g_ZunMemory.Free(manager->surfaceData[surfaceIndex]);
    manager->surfaceData[surfaceIndex] = NULL;
}

void AnmManager::CopySurfaceToBackbuffer(
    int surfaceIndex, int left, int top, int x, int y)
{
    if (reinterpret_cast<AnmManagerSurfaceView *>(this)->surfacesBis[surfaceIndex] == NULL)
        return;

    IDirect3DSurface8 *destSurface;
    if (reinterpret_cast<SupervisorUtilityView *>(&g_Supervisor)->d3dDevice->GetBackBuffer(
            0, D3DBACKBUFFER_TYPE_MONO, &destSurface) != D3D_OK)
        return;

    if (reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces[surfaceIndex] == NULL)
    {
        if (reinterpret_cast<SupervisorUtilityView *>(&g_Supervisor)->d3dDevice->CreateRenderTarget(
                reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo[surfaceIndex].Width,
                reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo[surfaceIndex].Height,
                reinterpret_cast<SupervisorUtilityView *>(&g_Supervisor)->presentParameters.BackBufferFormat,
                D3DMULTISAMPLE_NONE, TRUE,
                &reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces[surfaceIndex]) != D3D_OK)
        {
            if (reinterpret_cast<SupervisorUtilityView *>(&g_Supervisor)->d3dDevice->CreateImageSurface(
                    reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo[surfaceIndex].Width,
                    reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo[surfaceIndex].Height,
                    reinterpret_cast<SupervisorUtilityView *>(&g_Supervisor)->presentParameters.BackBufferFormat,
                    &reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces[surfaceIndex]) != D3D_OK)
            {
                destSurface->Release();
                return;
            }
        }
        if (D3DXLoadSurfaceFromSurface(
                reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces[surfaceIndex], NULL, NULL,
                reinterpret_cast<AnmManagerSurfaceView *>(this)->surfacesBis[surfaceIndex], NULL, NULL, 1, 0) != D3D_OK)
        {
            destSurface->Release();
            return;
        }
    }

    RECT sourceRect;
    POINT destPoint;
    sourceRect.left = left;
    sourceRect.top = top;
    sourceRect.right = reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo[surfaceIndex].Width;
    sourceRect.bottom = reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo[surfaceIndex].Height;
    destPoint.x = x;
    destPoint.y = y;
    reinterpret_cast<SupervisorUtilityView *>(&g_Supervisor)->d3dDevice->CopyRects(
        reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces[surfaceIndex], &sourceRect, 1, destSurface,
        &destPoint);
    destSurface->Release();
}

void AnmManager::CopySurfaceToBackbuffer2(
    int surfaceIndex, int rectX, int rectY, int rectLeft, int rectTop,
    int width, int height)
{
    if (reinterpret_cast<AnmManagerSurfaceView *>(this)->surfacesBis[surfaceIndex] == NULL)
        return;

    IDirect3DSurface8 *backbuffer;
    if (reinterpret_cast<SupervisorUtilityView *>(&g_Supervisor)->d3dDevice->GetBackBuffer(
            0, D3DBACKBUFFER_TYPE_MONO, &backbuffer) != D3D_OK)
        return;

    if (reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces[surfaceIndex] == NULL)
    {
        if (reinterpret_cast<SupervisorUtilityView *>(&g_Supervisor)->d3dDevice->CreateRenderTarget(
                reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo[surfaceIndex].Width,
                reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo[surfaceIndex].Height,
                reinterpret_cast<SupervisorUtilityView *>(&g_Supervisor)->presentParameters.BackBufferFormat,
                D3DMULTISAMPLE_NONE, TRUE,
                &reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces[surfaceIndex]) != D3D_OK)
        {
            if (reinterpret_cast<SupervisorUtilityView *>(&g_Supervisor)->d3dDevice->CreateImageSurface(
                    reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo[surfaceIndex].Width,
                    reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo[surfaceIndex].Height,
                    reinterpret_cast<SupervisorUtilityView *>(&g_Supervisor)->presentParameters.BackBufferFormat,
                    &reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces[surfaceIndex]) != D3D_OK)
            {
                backbuffer->Release();
                return;
            }
        }
        if (D3DXLoadSurfaceFromSurface(
                reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces[surfaceIndex], NULL, NULL,
                reinterpret_cast<AnmManagerSurfaceView *>(this)->surfacesBis[surfaceIndex], NULL, NULL, 1, 0) != D3D_OK)
        {
            backbuffer->Release();
            return;
        }
    }

    RECT rect;
    POINT point;
    rect.left = rectLeft;
    rect.top = rectTop;
    rect.right = rectLeft + width;
    rect.bottom = rectTop + height;
    point.x = rectX;
    point.y = rectY;
    reinterpret_cast<SupervisorUtilityView *>(&g_Supervisor)->d3dDevice->CopyRects(
        reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces[surfaceIndex], &rect, 1, backbuffer, &point);
    backbuffer->Release();
}

void AnmManager::CaptureToTexture(
    int captureAnmIndex, int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH)
{
    if (SurfaceSlots(this)[captureAnmIndex].textures[0].texture == NULL)
        return;

    this->FlushVertexBuffer();

    IDirect3DSurface8 *backbuffer;
    if (UtilitySupervisor()->d3dDevice->GetBackBuffer(
            0, D3DBACKBUFFER_TYPE_MONO, &backbuffer) != D3D_OK)
        return;

    IDirect3DSurface8 *textureSurface;
    if (SurfaceSlots(this)[captureAnmIndex].textures[0].texture->GetSurfaceLevel(
            0, &textureSurface) != D3D_OK)
    {
        backbuffer->Release();
        return;
    }

    RECT srcRect;
    RECT dstRect;
    srcRect.left = srcX;
    srcRect.top = srcY;
    srcRect.right = srcX + srcW;
    srcRect.bottom = srcY + srcH;
    dstRect.left = dstX;
    dstRect.top = dstY;
    dstRect.right = dstX + dstW;
    dstRect.bottom = dstY + dstH;

    if (D3DXLoadSurfaceFromSurface(
            textureSurface, NULL, &dstRect, backbuffer, NULL, &srcRect,
            static_cast<unsigned long>(-1), 0) != D3D_OK)
    {
        textureSurface->Release();
        backbuffer->Release();
        return;
    }

    textureSurface->Release();
    backbuffer->Release();
}

void AnmManager::CaptureToSurface(
    int captureSurfaceIndex, int srcX, int srcY, int srcW, int srcH,
    int dstX, int dstY, int dstW, int dstH)
{
    AnmManagerSurfaceView *manager = SurfaceManager(this);
    this->FlushVertexBuffer();

    if (manager->surfaces[captureSurfaceIndex] != NULL)
        this->ReleaseSurface(captureSurfaceIndex);

    RECT srcRect;
    RECT dstRect;
    srcRect.left = srcX;
    srcRect.top = srcY;
    srcRect.right = srcX + srcW;
    srcRect.bottom = srcY + srcH;
    dstRect.left = dstX;
    dstRect.top = dstY;
    dstRect.right = dstX + dstW;
    dstRect.bottom = dstY + dstH;

    IDirect3DSurface8 *backbuffer;
    if (UtilitySupervisor()->d3dDevice->GetBackBuffer(
            0, D3DBACKBUFFER_TYPE_MONO, &backbuffer) != D3D_OK)
        return;

    manager->surfaceInfo[captureSurfaceIndex].Width = dstW;
    manager->surfaceInfo[captureSurfaceIndex].Height = dstH;

    if (UtilitySupervisor()->d3dDevice->CreateRenderTarget(
            manager->surfaceInfo[captureSurfaceIndex].Width,
            manager->surfaceInfo[captureSurfaceIndex].Height,
            UtilitySupervisor()->presentParameters.BackBufferFormat,
            D3DMULTISAMPLE_NONE, TRUE,
            &manager->surfaces[captureSurfaceIndex]) != D3D_OK)
    {
        if (UtilitySupervisor()->d3dDevice->CreateImageSurface(
                manager->surfaceInfo[captureSurfaceIndex].Width,
                manager->surfaceInfo[captureSurfaceIndex].Height,
                UtilitySupervisor()->presentParameters.BackBufferFormat,
                &manager->surfaces[captureSurfaceIndex]) != D3D_OK)
            goto out;
    }

    if (UtilitySupervisor()->d3dDevice->CreateImageSurface(
            manager->surfaceInfo[captureSurfaceIndex].Width,
            manager->surfaceInfo[captureSurfaceIndex].Height,
            UtilitySupervisor()->presentParameters.BackBufferFormat,
            &manager->surfacesBis[captureSurfaceIndex]) != D3D_OK)
        goto out;

    if (D3DXLoadSurfaceFromSurface(
            manager->surfaces[captureSurfaceIndex], NULL, &dstRect,
            backbuffer, NULL, &srcRect, static_cast<unsigned long>(-1),
            0) != D3D_OK)
        goto out;

    D3DXLoadSurfaceFromSurface(
        manager->surfacesBis[captureSurfaceIndex], NULL, NULL,
        manager->surfaces[captureSurfaceIndex], NULL, NULL,
        static_cast<unsigned long>(-1), 0);

out:
    if (backbuffer != NULL)
    {
        backbuffer->Release();
        backbuffer = NULL;
    }
}
