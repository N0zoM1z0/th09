#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"
#include "TextHelper.hpp"

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <d3d8.h>


// DrawTextLeft/Centered traverse the same 0x2A4 ANM VM and 0x44 sprite
// that the exact 9776-byte interpreter, loader and SetSprite already use.
typedef char AnmNativeTextSpriteAndVmOffsets[
    (sizeof(AnmVm) == 0x2A4 &&
     sizeof(AnmLoadedSprite) == 0x44 &&
     offsetof(AnmVm, flagsWord) == 0x1F8 &&
     offsetof(AnmVm, loadedSprite) == 0x224 &&
     offsetof(AnmVm, fontWidth) == 0x298 &&
     offsetof(AnmVm, fontHeight) == 0x299 &&
     offsetof(AnmLoadedSprite, startPixelX08) == 0x08 &&
     offsetof(AnmLoadedSprite, textureWidth1C) == 0x1C &&
     offsetof(AnmLoadedSprite, widthPx) == 0x34 &&
     offsetof(AnmLoadedSprite, scaleX38) == 0x38) ? 1 : -1];

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

HRESULT WINAPI D3DXLoadSurfaceFromSurface(
    IDirect3DSurface8 *destSurface, const PALETTEENTRY *destPalette,
    const RECT *destRect, IDirect3DSurface8 *srcSurface,
    const PALETTEENTRY *srcPalette, const RECT *srcRect,
    unsigned long filter, D3DCOLOR colorKey);

static __inline AnmManagerSurfaceView *SurfaceManager(AnmManager *manager)
{
    return reinterpret_cast<AnmManagerSurfaceView *>(manager);
}

static __inline AnmSurfaceSlotView *SurfaceSlots(AnmManager *manager)
{
    return reinterpret_cast<AnmSurfaceSlotView *>(manager);
}

// The four original Manager surface arrays have different real bases and
// strides (0x04-byte pointers versus 0x14-byte D3D descriptors). Keep every
// repeated source read, but use Win32 modulo-32 address math for the signed
// surface selector instead of out-of-bounds C++ indexing.
template <typename T>
static __forceinline T &NativeAnmSurfaceSlot(T *records, int index)
{
    return *reinterpret_cast<T *>(
        reinterpret_cast<unsigned int>(records) +
        static_cast<unsigned int>(index) * sizeof(T));
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
    AnmVm *vm = vmObject;
    int fontWidth = vm->fontWidth;
    va_list args;

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    AnmLoadedSprite *sprite = vm->loadedSprite;
    this->DrawTextInner(
        static_cast<IDirect3DTexture8 *>(sprite->texture), sprite->startPixelX08,
        sprite->startPixelY0C, sprite->textureWidth1C, sprite->textureHeight18,
        fontWidth, vm->fontHeight, textColor, shadowColor, buffer,
        sprite->scaleX38, sprite->scaleY3C);
    vm->flagsWord |= 1;
}

void AnmManager::DrawTextCentered(
    AnmVm *vmObject, unsigned long textColor, unsigned long shadowColor,
    const char *format, ...)
{
    char buffer[68];
    AnmVm *vm = vmObject;
    int fontWidth = vm->fontWidth <= 0 ? 15 : vm->fontWidth;
    va_list args;

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    unsigned int textLength = strlen(buffer);
    AnmLoadedSprite *sprite = vm->loadedSprite;
    int x = sprite->startPixelX08 +
        sprite->widthPx * sprite->scaleX38 / 2.0f -
        static_cast<float>(textLength) * fontWidth *
            sprite->scaleX38 / 4.0f;
    this->DrawTextInner(
        static_cast<IDirect3DTexture8 *>(sprite->texture), x, sprite->startPixelY0C,
        sprite->textureWidth1C, sprite->textureHeight18, fontWidth, vm->fontHeight,
        textColor, shadowColor, buffer, sprite->scaleX38,
        sprite->scaleY3C);
    vm->flagsWord |= 1;
}

void AnmManager::ReleaseSurface(int surfaceIndex)
{
    AnmManagerSurfaceView *manager = SurfaceManager(this);
    if (NativeAnmSurfaceSlot(manager->surfaces, surfaceIndex) != NULL)
    {
        NativeAnmSurfaceSlot(manager->surfaces, surfaceIndex)->Release();
        NativeAnmSurfaceSlot(manager->surfaces, surfaceIndex) = NULL;
    }
    if (NativeAnmSurfaceSlot(manager->surfacesBis, surfaceIndex) != NULL)
    {
        NativeAnmSurfaceSlot(manager->surfacesBis, surfaceIndex)->Release();
        NativeAnmSurfaceSlot(manager->surfacesBis, surfaceIndex) = NULL;
    }
    if (NativeAnmSurfaceSlot(manager->surfaceData, surfaceIndex) != NULL)
        g_ZunMemory.Free(NativeAnmSurfaceSlot(manager->surfaceData, surfaceIndex));
    NativeAnmSurfaceSlot(manager->surfaceData, surfaceIndex) = NULL;
}

void AnmManager::CopySurfaceToBackbuffer(
    int surfaceIndex, int left, int top, int x, int y)
{
    if (NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfacesBis, surfaceIndex) == NULL)
        return;

    IDirect3DSurface8 *destSurface;
    if (g_Supervisor.d3dDevice08->GetBackBuffer(
            0, D3DBACKBUFFER_TYPE_MONO, &destSurface) != D3D_OK)
        return;

    if (NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces, surfaceIndex) == NULL)
    {
        if (g_Supervisor.d3dDevice08->CreateRenderTarget(
                NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo, surfaceIndex).Width,
                NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo, surfaceIndex).Height,
                g_Supervisor.presentParameters350.BackBufferFormat,
                D3DMULTISAMPLE_NONE, TRUE,
                &NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces, surfaceIndex)) != D3D_OK)
        {
            if (g_Supervisor.d3dDevice08->CreateImageSurface(
                    NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo, surfaceIndex).Width,
                    NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo, surfaceIndex).Height,
                    g_Supervisor.presentParameters350.BackBufferFormat,
                    &NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces, surfaceIndex)) != D3D_OK)
            {
                destSurface->Release();
                return;
            }
        }
        if (D3DXLoadSurfaceFromSurface(
                NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces, surfaceIndex), NULL, NULL,
                NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfacesBis, surfaceIndex), NULL, NULL, 1, 0) != D3D_OK)
        {
            destSurface->Release();
            return;
        }
    }

    RECT sourceRect;
    POINT destPoint;
    sourceRect.left = left;
    sourceRect.top = top;
    sourceRect.right = NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo, surfaceIndex).Width;
    sourceRect.bottom = NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo, surfaceIndex).Height;
    destPoint.x = x;
    destPoint.y = y;
    g_Supervisor.d3dDevice08->CopyRects(
        NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces, surfaceIndex), &sourceRect, 1, destSurface,
        &destPoint);
    destSurface->Release();
}

void AnmManager::CopySurfaceToBackbuffer2(
    int surfaceIndex, int rectX, int rectY, int rectLeft, int rectTop,
    int width, int height)
{
    if (NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfacesBis, surfaceIndex) == NULL)
        return;

    IDirect3DSurface8 *backbuffer;
    if (g_Supervisor.d3dDevice08->GetBackBuffer(
            0, D3DBACKBUFFER_TYPE_MONO, &backbuffer) != D3D_OK)
        return;

    if (NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces, surfaceIndex) == NULL)
    {
        if (g_Supervisor.d3dDevice08->CreateRenderTarget(
                NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo, surfaceIndex).Width,
                NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo, surfaceIndex).Height,
                g_Supervisor.presentParameters350.BackBufferFormat,
                D3DMULTISAMPLE_NONE, TRUE,
                &NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces, surfaceIndex)) != D3D_OK)
        {
            if (g_Supervisor.d3dDevice08->CreateImageSurface(
                    NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo, surfaceIndex).Width,
                    NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaceInfo, surfaceIndex).Height,
                    g_Supervisor.presentParameters350.BackBufferFormat,
                    &NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces, surfaceIndex)) != D3D_OK)
            {
                backbuffer->Release();
                return;
            }
        }
        if (D3DXLoadSurfaceFromSurface(
                NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces, surfaceIndex), NULL, NULL,
                NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfacesBis, surfaceIndex), NULL, NULL, 1, 0) != D3D_OK)
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
    g_Supervisor.d3dDevice08->CopyRects(
        NativeAnmSurfaceSlot(reinterpret_cast<AnmManagerSurfaceView *>(this)->surfaces, surfaceIndex), &rect, 1, backbuffer, &point);
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
    if (g_Supervisor.d3dDevice08->GetBackBuffer(
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
    if (g_Supervisor.d3dDevice08->GetBackBuffer(
            0, D3DBACKBUFFER_TYPE_MONO, &backbuffer) != D3D_OK)
        return;

    manager->surfaceInfo[captureSurfaceIndex].Width = dstW;
    manager->surfaceInfo[captureSurfaceIndex].Height = dstH;

    if (g_Supervisor.d3dDevice08->CreateRenderTarget(
            manager->surfaceInfo[captureSurfaceIndex].Width,
            manager->surfaceInfo[captureSurfaceIndex].Height,
            g_Supervisor.presentParameters350.BackBufferFormat,
            D3DMULTISAMPLE_NONE, TRUE,
            &manager->surfaces[captureSurfaceIndex]) != D3D_OK)
    {
        if (g_Supervisor.d3dDevice08->CreateImageSurface(
                manager->surfaceInfo[captureSurfaceIndex].Width,
                manager->surfaceInfo[captureSurfaceIndex].Height,
                g_Supervisor.presentParameters350.BackBufferFormat,
                &manager->surfaces[captureSurfaceIndex]) != D3D_OK)
            goto out;
    }

    if (g_Supervisor.d3dDevice08->CreateImageSurface(
            manager->surfaceInfo[captureSurfaceIndex].Width,
            manager->surfaceInfo[captureSurfaceIndex].Height,
            g_Supervisor.presentParameters350.BackBufferFormat,
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
