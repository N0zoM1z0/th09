#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "Supervisor.hpp"

#include <stddef.h>
#include <windows.h>
#include <d3d8.h>

struct VertexTex1DiffuseXyzrhw
{
    float x;
    float y;
    float z;
    float rhw;
    unsigned long diffuse;
    float u;
    float v;
};

typedef char VertexTex1DiffuseXyzrhwSizeIs1C[
    (sizeof(VertexTex1DiffuseXyzrhw) == 0x1C) ? 1 : -1];

extern VertexTex1DiffuseXyzrhw g_AnmRenderQuad[4];
static const float g_AnmHalfPixel = 0.5f;

struct SupervisorRenderDeviceView
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice;
};

struct AnmVmDrawAlphaView
{
    unsigned char unknown000[0x1F3];
    unsigned char colorAlpha;
};

typedef char AnmVmDrawAlphaAt1F3[
    (offsetof(AnmVmDrawAlphaView, colorAlpha) == 0x1F3) ? 1 : -1];

struct AnmManagerRenderView
{
    unsigned char unknown000000[0x18];
    int flushesThisFrame;
    unsigned char unknown00001C[0x128E4 - 0x1C];
    int spritesToDraw;
    unsigned char unknown0128E8[0x2B28E8 - 0x128E8];
    VertexTex1DiffuseXyzrhw *vertexBufferEndPtr;
    VertexTex1DiffuseXyzrhw *vertexBufferStartPtr;

};

typedef char AnmManagerFlushesAt18[
    (offsetof(AnmManagerRenderView, flushesThisFrame) == 0x18) ? 1 : -1];
typedef char AnmManagerSpritesToDrawAt128E4[
    (offsetof(AnmManagerRenderView, spritesToDraw) == 0x128E4) ? 1 : -1];
typedef char AnmManagerVertexEndAt2B28E8[
    (offsetof(AnmManagerRenderView, vertexBufferEndPtr) == 0x2B28E8) ? 1 : -1];
typedef char AnmManagerVertexStartAt2B28EC[
    (offsetof(AnmManagerRenderView, vertexBufferStartPtr) == 0x2B28EC) ? 1 : -1];

void AnmManager::FlushVertexBuffer()
{
    AnmManagerRenderView *anm = reinterpret_cast<AnmManagerRenderView *>(this);
    SupervisorRenderDeviceView *supervisor =
        reinterpret_cast<SupervisorRenderDeviceView *>(&g_Supervisor);

    if (anm->spritesToDraw == 0) {
        return;
    }

    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    supervisor->d3dDevice->SetVertexShader(
        D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    supervisor->d3dDevice->DrawPrimitiveUP(
        D3DPT_TRIANGLELIST, anm->spritesToDraw * 2,
        anm->vertexBufferStartPtr, sizeof(VertexTex1DiffuseXyzrhw));

    anm->vertexBufferStartPtr = anm->vertexBufferEndPtr;
    anm->spritesToDraw = 0;
    anm->flushesThisFrame++;
}

int AnmManager::DrawNoRotation(AnmVm *vm)
{
    float spriteHalfWidth;
    float spriteHalfHeight;
    AnmManagerRenderView *anm = reinterpret_cast<AnmManagerRenderView *>(this);
    AnmVmDrawAlphaView *drawVm = reinterpret_cast<AnmVmDrawAlphaView *>(vm);

    if (!vm->IsVisible()) {
        return -1;
    }
    if ((vm->flagsWord & 2) == 0) {
        return -1;
    }
    if (drawVm->colorAlpha == 0) {
        return -1;
    }

    spriteHalfWidth = (vm->spriteSize.x * vm->scale.x) / 2.0f;
    spriteHalfHeight = (vm->spriteSize.y * vm->scale.y) / 2.0f;

    if ((vm->anchor & 1) == 0) {
        g_AnmRenderQuad[0].x = g_AnmRenderQuad[2].x = vm->pos.x - spriteHalfWidth;
        g_AnmRenderQuad[1].x = g_AnmRenderQuad[3].x = spriteHalfWidth + vm->pos.x;
    } else {
        g_AnmRenderQuad[0].x = g_AnmRenderQuad[2].x = vm->pos.x;
        g_AnmRenderQuad[1].x = g_AnmRenderQuad[3].x =
            spriteHalfWidth + vm->pos.x + spriteHalfWidth;
    }

    if ((vm->anchor & 2) == 0) {
        g_AnmRenderQuad[0].y = g_AnmRenderQuad[1].y = vm->pos.y - spriteHalfHeight;
        g_AnmRenderQuad[2].y = g_AnmRenderQuad[3].y = spriteHalfHeight + vm->pos.y;
    } else {
        g_AnmRenderQuad[0].y = g_AnmRenderQuad[1].y = vm->pos.y;
        g_AnmRenderQuad[2].y = g_AnmRenderQuad[3].y =
            spriteHalfHeight + vm->pos.y + spriteHalfHeight;
    }

    g_AnmRenderQuad[0].z = g_AnmRenderQuad[1].z =
        g_AnmRenderQuad[2].z = g_AnmRenderQuad[3].z = vm->pos.z;

    return this->DrawInner(vm, 1);
}

union AnmRenderColor
{
    unsigned long value;
    struct
    {
        unsigned char b;
        unsigned char g;
        unsigned char r;
        unsigned char a;
    };
};

struct AnmVmDrawInnerLayout
{
    unsigned char unknown000[0x1F0];
    AnmRenderColor color1;
    AnmRenderColor color2;
    unsigned int flagsWord;
    unsigned char unknown1FC[0x28];
    AnmLoadedSprite *loadedSprite;
};

typedef char AnmVmDrawInnerColor1At1F0[
    (offsetof(AnmVmDrawInnerLayout, color1) == 0x1F0) ? 1 : -1];
typedef char AnmVmDrawInnerColor2At1F4[
    (offsetof(AnmVmDrawInnerLayout, color2) == 0x1F4) ? 1 : -1];
typedef char AnmVmDrawInnerFlagsAt1F8[
    (offsetof(AnmVmDrawInnerLayout, flagsWord) == 0x1F8) ? 1 : -1];
typedef char AnmVmDrawInnerSpriteAt224[
    (offsetof(AnmVmDrawInnerLayout, loadedSprite) == 0x224) ? 1 : -1];

struct AnmManagerDrawInnerView
{
    AnmRenderColor color;
    int useMixColor;
    unsigned char unknown008[0x14 - 0x08];
    int renderStateChangesThisFrame;
    unsigned char unknown018[0x1C - 0x18];
    Float2 screenShakeOffset;
    unsigned char unknown024[0x1287C - 0x24];
    unsigned long currentTextureFactor;
    void *currentTexture;
    unsigned char currentBlendMode;
    unsigned char unknown12885;
    unsigned char currentVertexShader;
    unsigned char disableZWrite;
    unsigned char cameraMode;
    unsigned char unknown12889[0x128E4 - 0x12889];
    int spritesToDraw;
    unsigned char unknown128E8[0x2B28E8 - 0x128E8];
    VertexTex1DiffuseXyzrhw *vertexBufferEndPtr;
    VertexTex1DiffuseXyzrhw *vertexBufferStartPtr;
};

typedef char AnmManagerDrawInnerShakeAt1C[
    (offsetof(AnmManagerDrawInnerView, screenShakeOffset) == 0x1C) ? 1 : -1];
typedef char AnmManagerDrawInnerTextureFactorAt1287C[
    (offsetof(AnmManagerDrawInnerView, currentTextureFactor) == 0x1287C) ? 1 : -1];
typedef char AnmManagerDrawInnerTextureAt12880[
    (offsetof(AnmManagerDrawInnerView, currentTexture) == 0x12880) ? 1 : -1];
typedef char AnmManagerDrawInnerShaderAt12886[
    (offsetof(AnmManagerDrawInnerView, currentVertexShader) == 0x12886) ? 1 : -1];
typedef char AnmManagerDrawInnerCameraModeAt12888[
    (offsetof(AnmManagerDrawInnerView, cameraMode) == 0x12888) ? 1 : -1];

static unsigned char MixRenderColor(unsigned char first, unsigned char second)
{
    unsigned int color = ((first * second) / 128U);
    if (color >= 256)
        color = 255;
    return (unsigned char)color;
}

void AnmManager::SetRenderStateForVm3D(AnmVm *vm)
{
    AnmManagerDrawInnerView *anm =
        reinterpret_cast<AnmManagerDrawInnerView *>(this);
    AnmVmDrawInnerLayout *drawVm =
        reinterpret_cast<AnmVmDrawInnerLayout *>(vm);
    SupervisorRenderDeviceView *supervisor =
        reinterpret_cast<SupervisorRenderDeviceView *>(&g_Supervisor);

    if (anm->currentBlendMode != vm->blendMode) {
        this->FlushVertexBuffer();
        anm->currentBlendMode = (unsigned char)vm->blendMode;
        switch (anm->currentBlendMode) {
        case 0:
            supervisor->d3dDevice->SetRenderState(
                D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            break;
        case 1:
            supervisor->d3dDevice->SetRenderState(
                D3DRS_DESTBLEND, D3DBLEND_ONE);
            break;
        }
    }

    {
        AnmRenderColor color;
        color.value = (drawVm->flagsWord & 0x20000) != 0
            ? drawVm->color2.value : drawVm->color1.value;
        if (anm->useMixColor != 0) {
            color.r = MixRenderColor(color.r, anm->color.r);
            color.g = MixRenderColor(color.g, anm->color.g);
            color.b = MixRenderColor(color.b, anm->color.b);
            color.a = MixRenderColor(color.a, anm->color.a);
        }

        if (anm->currentTextureFactor != color.value) {
            this->FlushVertexBuffer();
            anm->currentTextureFactor = color.value;
            supervisor->d3dDevice->SetRenderState(
                D3DRS_TEXTUREFACTOR, anm->currentTextureFactor);
        }
    }

    if (anm->disableZWrite != ((drawVm->flagsWord >> 13) & 1)) {
        this->FlushVertexBuffer();
        anm->disableZWrite =
            (unsigned char)((drawVm->flagsWord >> 13) & 1);
        if (!anm->disableZWrite) {
            supervisor->d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        } else {
            supervisor->d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        }
    }

    if (anm->cameraMode != ((drawVm->flagsWord >> 15) & 1)) {
        this->FlushVertexBuffer();
        anm->cameraMode = (unsigned char)((drawVm->flagsWord >> 15) & 1);
        if (!anm->cameraMode) {
            g_Supervisor.ApplyCameraMode0();
        } else {
            g_Supervisor.ApplyCameraMode1();
        }
    }

    anm->renderStateChangesThisFrame++;
}

void AnmManager::SetRenderStateForVm(AnmVm *vm)
{
    AnmManagerDrawInnerView *anm =
        reinterpret_cast<AnmManagerDrawInnerView *>(this);
    SupervisorRenderDeviceView *supervisor =
        reinterpret_cast<SupervisorRenderDeviceView *>(&g_Supervisor);

    if (anm->currentBlendMode != vm->blendMode) {
        this->FlushVertexBuffer();
        anm->currentBlendMode = (unsigned char)vm->blendMode;
        switch (anm->currentBlendMode) {
        case 0:
            supervisor->d3dDevice->SetRenderState(
                D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            break;
        case 1:
            supervisor->d3dDevice->SetRenderState(
                D3DRS_DESTBLEND, D3DBLEND_ONE);
            break;
        }
    }

    if (anm->disableZWrite != vm->disableZWrite) {
        anm->disableZWrite = (unsigned char)vm->disableZWrite;
        if (!anm->disableZWrite) {
            g_Supervisor.SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        } else {
            g_Supervisor.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        }
    }

    anm->renderStateChangesThisFrame++;
}

int AnmManager::AddSpriteToDrawBuffer(VertexTex1DiffuseXyzrhw *vertices)
{
    AnmManagerDrawInnerView *anm =
        reinterpret_cast<AnmManagerDrawInnerView *>(this);

    anm->vertexBufferEndPtr[0] = vertices[0];
    anm->vertexBufferEndPtr[1] = vertices[1];
    anm->vertexBufferEndPtr[2] = vertices[2];
    anm->vertexBufferEndPtr[3] = vertices[1];
    anm->vertexBufferEndPtr[4] = vertices[2];
    anm->vertexBufferEndPtr[5] = vertices[3];
    anm->vertexBufferEndPtr += 6;
    anm->spritesToDraw++;
    return 0;
}

int AnmManager::DrawInner(AnmVm *vm, int flags)
{
    AnmManagerDrawInnerView *anm =
        reinterpret_cast<AnmManagerDrawInnerView *>(this);
    AnmVmDrawInnerLayout *drawVm =
        reinterpret_cast<AnmVmDrawInnerLayout *>(vm);
    SupervisorRenderDeviceView *supervisor =
        reinterpret_cast<SupervisorRenderDeviceView *>(&g_Supervisor);
    AnmRenderColor color;
    float triangleX1;
    float triangleX2;
    float triangleY1;
    float triangleY2;

    g_AnmRenderQuad[0].x += anm->screenShakeOffset.x;
    g_AnmRenderQuad[0].y += anm->screenShakeOffset.y;
    g_AnmRenderQuad[1].x += anm->screenShakeOffset.x;
    g_AnmRenderQuad[1].y += anm->screenShakeOffset.y;
    g_AnmRenderQuad[2].x += anm->screenShakeOffset.x;
    g_AnmRenderQuad[2].y += anm->screenShakeOffset.y;
    g_AnmRenderQuad[3].x += anm->screenShakeOffset.x;
    g_AnmRenderQuad[3].y += anm->screenShakeOffset.y;

    if ((flags & 1) != 0) {
#if defined(_MSC_VER) && defined(_M_IX86)
        __asm {
            fld g_AnmRenderQuad[0 * TYPE g_AnmRenderQuad].x
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmRenderQuad[1 * TYPE g_AnmRenderQuad].x
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmRenderQuad[0 * TYPE g_AnmRenderQuad].y
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmRenderQuad[2 * TYPE g_AnmRenderQuad].y
            frndint
            fsub g_AnmHalfPixel
            fst g_AnmRenderQuad[2 * TYPE g_AnmRenderQuad].y
            fstp g_AnmRenderQuad[3 * TYPE g_AnmRenderQuad].y
            fst g_AnmRenderQuad[0 * TYPE g_AnmRenderQuad].y
            fstp g_AnmRenderQuad[1 * TYPE g_AnmRenderQuad].y
            fst g_AnmRenderQuad[1 * TYPE g_AnmRenderQuad].x
            fstp g_AnmRenderQuad[3 * TYPE g_AnmRenderQuad].x
            fst g_AnmRenderQuad[0 * TYPE g_AnmRenderQuad].x
            fstp g_AnmRenderQuad[2 * TYPE g_AnmRenderQuad].x
        }
#endif
    }

    g_AnmRenderQuad[0].u = g_AnmRenderQuad[2].u =
        drawVm->loadedSprite->uvStartX + vm->uvScrollPos.x;
    g_AnmRenderQuad[1].u = g_AnmRenderQuad[3].u =
        drawVm->loadedSprite->uvEndX + vm->uvScrollPos.x;
    g_AnmRenderQuad[0].v = g_AnmRenderQuad[1].v =
        drawVm->loadedSprite->uvStartY + vm->uvScrollPos.y;
    g_AnmRenderQuad[2].v = g_AnmRenderQuad[3].v =
        drawVm->loadedSprite->uvEndY + vm->uvScrollPos.y;

    triangleX1 = g_AnmRenderQuad[0].x > g_AnmRenderQuad[1].x
        ? g_AnmRenderQuad[0].x : g_AnmRenderQuad[1].x;
    triangleX1 = g_AnmRenderQuad[2].x > triangleX1
        ? g_AnmRenderQuad[2].x : triangleX1;
    triangleX1 = g_AnmRenderQuad[3].x > triangleX1
        ? g_AnmRenderQuad[3].x : triangleX1;

    triangleY1 = g_AnmRenderQuad[0].y > g_AnmRenderQuad[1].y
        ? g_AnmRenderQuad[0].y : g_AnmRenderQuad[1].y;
    triangleY1 = g_AnmRenderQuad[2].y > triangleY1
        ? g_AnmRenderQuad[2].y : triangleY1;
    triangleY1 = g_AnmRenderQuad[3].y > triangleY1
        ? g_AnmRenderQuad[3].y : triangleY1;

    triangleX2 = g_AnmRenderQuad[0].x < g_AnmRenderQuad[1].x
        ? g_AnmRenderQuad[0].x : g_AnmRenderQuad[1].x;
    triangleX2 = g_AnmRenderQuad[2].x < triangleX2
        ? g_AnmRenderQuad[2].x : triangleX2;
    triangleX2 = g_AnmRenderQuad[3].x < triangleX2
        ? g_AnmRenderQuad[3].x : triangleX2;

    triangleY2 = g_AnmRenderQuad[0].y < g_AnmRenderQuad[1].y
        ? g_AnmRenderQuad[0].y : g_AnmRenderQuad[1].y;
    triangleY2 = g_AnmRenderQuad[2].y < triangleY2
        ? g_AnmRenderQuad[2].y : triangleY2;
    triangleY2 = g_AnmRenderQuad[3].y < triangleY2
        ? g_AnmRenderQuad[3].y : triangleY2;

    if (triangleX1 < (float)g_RenderCoordinateOrigin->x ||
        triangleY1 < (float)g_RenderCoordinateOrigin->y ||
        triangleX2 > (float)(g_RenderCoordinateOrigin->x + g_RenderCoordinateOrigin->width) ||
        triangleY2 > (float)(g_RenderCoordinateOrigin->y + g_RenderCoordinateOrigin->height)) {
        return 0;
    }

    if (anm->currentTexture != drawVm->loadedSprite->texture) {
        anm->currentTexture = drawVm->loadedSprite->texture;
        this->FlushVertexBuffer();
        supervisor->d3dDevice->SetTexture(
            0, reinterpret_cast<IDirect3DBaseTexture8 *>(anm->currentTexture));
    }

    if (anm->currentVertexShader != 1) {
        this->FlushVertexBuffer();
        anm->currentVertexShader = 1;
    }

    if ((flags & 2) == 0) {
        color.value = (drawVm->flagsWord & 0x20000) != 0
            ? drawVm->color2.value : drawVm->color1.value;
        if (anm->useMixColor != 0) {
            color.r = MixRenderColor(color.r, anm->color.r);
            color.g = MixRenderColor(color.g, anm->color.g);
            color.b = MixRenderColor(color.b, anm->color.b);
            color.a = MixRenderColor(color.a, anm->color.a);
        }
        g_AnmRenderQuad[0].diffuse = color.value;
        g_AnmRenderQuad[1].diffuse = color.value;
        g_AnmRenderQuad[2].diffuse = color.value;
        g_AnmRenderQuad[3].diffuse = color.value;
    }

    this->SetRenderStateForVm(vm);
    this->AddSpriteToDrawBuffer(g_AnmRenderQuad);
    return 0;
}

int AnmManager::DrawInnerFlippedX(AnmVm *vm, int flags)
{
    AnmManagerDrawInnerView *anm =
        reinterpret_cast<AnmManagerDrawInnerView *>(this);
    AnmVmDrawInnerLayout *drawVm =
        reinterpret_cast<AnmVmDrawInnerLayout *>(vm);
    SupervisorRenderDeviceView *supervisor =
        reinterpret_cast<SupervisorRenderDeviceView *>(&g_Supervisor);
    AnmRenderColor color;
    float triangleX1;
    float triangleX2;
    float triangleY1;
    float triangleY2;

    g_AnmRenderQuad[0].x += anm->screenShakeOffset.x;
    g_AnmRenderQuad[0].y += anm->screenShakeOffset.y;
    g_AnmRenderQuad[1].x += anm->screenShakeOffset.x;
    g_AnmRenderQuad[1].y += anm->screenShakeOffset.y;
    g_AnmRenderQuad[2].x += anm->screenShakeOffset.x;
    g_AnmRenderQuad[2].y += anm->screenShakeOffset.y;
    g_AnmRenderQuad[3].x += anm->screenShakeOffset.x;
    g_AnmRenderQuad[3].y += anm->screenShakeOffset.y;

    if ((flags & 1) != 0) {
#if defined(_MSC_VER) && defined(_M_IX86)
        __asm {
            fld g_AnmRenderQuad[0 * TYPE g_AnmRenderQuad].x
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmRenderQuad[1 * TYPE g_AnmRenderQuad].x
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmRenderQuad[0 * TYPE g_AnmRenderQuad].y
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmRenderQuad[2 * TYPE g_AnmRenderQuad].y
            frndint
            fsub g_AnmHalfPixel
            fst g_AnmRenderQuad[2 * TYPE g_AnmRenderQuad].y
            fstp g_AnmRenderQuad[3 * TYPE g_AnmRenderQuad].y
            fst g_AnmRenderQuad[0 * TYPE g_AnmRenderQuad].y
            fstp g_AnmRenderQuad[1 * TYPE g_AnmRenderQuad].y
            fst g_AnmRenderQuad[1 * TYPE g_AnmRenderQuad].x
            fstp g_AnmRenderQuad[3 * TYPE g_AnmRenderQuad].x
            fst g_AnmRenderQuad[0 * TYPE g_AnmRenderQuad].x
            fstp g_AnmRenderQuad[2 * TYPE g_AnmRenderQuad].x
        }
#endif
    }

    g_AnmRenderQuad[0].u = g_AnmRenderQuad[2].u =
        drawVm->loadedSprite->uvEndX + vm->uvScrollPos.x;
    g_AnmRenderQuad[1].u = g_AnmRenderQuad[3].u =
        drawVm->loadedSprite->uvStartX + vm->uvScrollPos.x;
    g_AnmRenderQuad[0].v = g_AnmRenderQuad[1].v =
        drawVm->loadedSprite->uvStartY + vm->uvScrollPos.y;
    g_AnmRenderQuad[2].v = g_AnmRenderQuad[3].v =
        drawVm->loadedSprite->uvEndY + vm->uvScrollPos.y;

    triangleX1 = g_AnmRenderQuad[0].x > g_AnmRenderQuad[1].x
        ? g_AnmRenderQuad[0].x : g_AnmRenderQuad[1].x;
    triangleX1 = g_AnmRenderQuad[2].x > triangleX1
        ? g_AnmRenderQuad[2].x : triangleX1;
    triangleX1 = g_AnmRenderQuad[3].x > triangleX1
        ? g_AnmRenderQuad[3].x : triangleX1;

    triangleY1 = g_AnmRenderQuad[0].y > g_AnmRenderQuad[1].y
        ? g_AnmRenderQuad[0].y : g_AnmRenderQuad[1].y;
    triangleY1 = g_AnmRenderQuad[2].y > triangleY1
        ? g_AnmRenderQuad[2].y : triangleY1;
    triangleY1 = g_AnmRenderQuad[3].y > triangleY1
        ? g_AnmRenderQuad[3].y : triangleY1;

    triangleX2 = g_AnmRenderQuad[0].x < g_AnmRenderQuad[1].x
        ? g_AnmRenderQuad[0].x : g_AnmRenderQuad[1].x;
    triangleX2 = g_AnmRenderQuad[2].x < triangleX2
        ? g_AnmRenderQuad[2].x : triangleX2;
    triangleX2 = g_AnmRenderQuad[3].x < triangleX2
        ? g_AnmRenderQuad[3].x : triangleX2;

    triangleY2 = g_AnmRenderQuad[0].y < g_AnmRenderQuad[1].y
        ? g_AnmRenderQuad[0].y : g_AnmRenderQuad[1].y;
    triangleY2 = g_AnmRenderQuad[2].y < triangleY2
        ? g_AnmRenderQuad[2].y : triangleY2;
    triangleY2 = g_AnmRenderQuad[3].y < triangleY2
        ? g_AnmRenderQuad[3].y : triangleY2;

    if (triangleX1 < (float)g_RenderCoordinateOrigin->x ||
        triangleY1 < (float)g_RenderCoordinateOrigin->y ||
        triangleX2 > (float)(g_RenderCoordinateOrigin->x + g_RenderCoordinateOrigin->width) ||
        triangleY2 > (float)(g_RenderCoordinateOrigin->y + g_RenderCoordinateOrigin->height)) {
        return 0;
    }

    if (anm->currentTexture != drawVm->loadedSprite->texture) {
        anm->currentTexture = drawVm->loadedSprite->texture;
        this->FlushVertexBuffer();
        supervisor->d3dDevice->SetTexture(
            0, reinterpret_cast<IDirect3DBaseTexture8 *>(anm->currentTexture));
    }

    if (anm->currentVertexShader != 1) {
        this->FlushVertexBuffer();
        anm->currentVertexShader = 1;
    }

    if ((flags & 2) == 0) {
        color.value = (drawVm->flagsWord & 0x20000) != 0
            ? drawVm->color2.value : drawVm->color1.value;
        if (anm->useMixColor != 0) {
            color.r = MixRenderColor(color.r, anm->color.r);
            color.g = MixRenderColor(color.g, anm->color.g);
            color.b = MixRenderColor(color.b, anm->color.b);
            color.a = MixRenderColor(color.a, anm->color.a);
        }
        g_AnmRenderQuad[0].diffuse = color.value;
        g_AnmRenderQuad[1].diffuse = color.value;
        g_AnmRenderQuad[2].diffuse = color.value;
        g_AnmRenderQuad[3].diffuse = color.value;
    }

    this->SetRenderStateForVm(vm);
    this->AddSpriteToDrawBuffer(g_AnmRenderQuad);
    return 0;
}


struct AnmVmRotationZView
{
    unsigned char unknown000[0x08];
    float rotationZ;
};

typedef char AnmVmRotationZAt08[
    (offsetof(AnmVmRotationZView, rotationZ) == 0x08) ? 1 : -1];

// Neutral reconstruction name; original TH09 method spelling is not proven.
int AnmManager::DrawNoRotationFlippedX(AnmVm *vm)
{
    float spriteHalfWidth;
    float spriteHalfHeight;
    AnmVmDrawAlphaView *drawVm = reinterpret_cast<AnmVmDrawAlphaView *>(vm);

    if (!vm->IsVisible()) {
        return -1;
    }
    if ((vm->flagsWord & 2) == 0) {
        return -1;
    }
    if (drawVm->colorAlpha == 0) {
        return -1;
    }

    spriteHalfWidth = (vm->spriteSize.x * vm->scale.x) / 2.0f;
    spriteHalfHeight = (vm->spriteSize.y * vm->scale.y) / 2.0f;

    if ((vm->anchor & 1) == 0) {
        g_AnmRenderQuad[0].x = g_AnmRenderQuad[2].x = vm->pos.x - spriteHalfWidth;
        g_AnmRenderQuad[1].x = g_AnmRenderQuad[3].x = spriteHalfWidth + vm->pos.x;
    } else {
        g_AnmRenderQuad[0].x = g_AnmRenderQuad[2].x = vm->pos.x;
        g_AnmRenderQuad[1].x = g_AnmRenderQuad[3].x =
            spriteHalfWidth + vm->pos.x + spriteHalfWidth;
    }

    if ((vm->anchor & 2) == 0) {
        g_AnmRenderQuad[0].y = g_AnmRenderQuad[1].y = vm->pos.y - spriteHalfHeight;
        g_AnmRenderQuad[2].y = g_AnmRenderQuad[3].y = spriteHalfHeight + vm->pos.y;
    } else {
        g_AnmRenderQuad[0].y = g_AnmRenderQuad[1].y = vm->pos.y;
        g_AnmRenderQuad[2].y = g_AnmRenderQuad[3].y =
            spriteHalfHeight + vm->pos.y + spriteHalfHeight;
    }

    g_AnmRenderQuad[0].z = g_AnmRenderQuad[1].z =
        g_AnmRenderQuad[2].z = g_AnmRenderQuad[3].z = vm->pos.z;

    return this->DrawInnerFlippedX(vm, 1);
}

void AnmManager::TranslateRotation(
    VertexTex1DiffuseXyzrhw *vertex,
    float x,
    float y,
    float sine,
    float cosine,
    float xOffset,
    float yOffset)
{
    vertex->x = x * cosine - y * sine + xOffset;
    vertex->y = x * sine + y * cosine + yOffset;
}

int AnmManager::Draw2D(AnmVm *vm)
{
    float sine;
    float cosine;
    float rotation;
    float xOffset;
    float yOffset;
    float x;
    float y;
    AnmVmRotationZView *rotationVm = reinterpret_cast<AnmVmRotationZView *>(vm);
    AnmVmDrawAlphaView *drawVm = reinterpret_cast<AnmVmDrawAlphaView *>(vm);

    if (rotationVm->rotationZ == 0.0f) {
        return this->DrawNoRotation(vm);
    }
    if (!vm->IsVisible()) {
        return -1;
    }
    if ((vm->flagsWord & 2) == 0) {
        return -1;
    }
    if (drawVm->colorAlpha == 0) {
        return -1;
    }

    rotation = rotationVm->rotationZ;
// TH09 target uses one inline x87 FSINCOS here; ordinary C sin/cos calls do not reproduce it.
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm {
        fld rotation
        fsincos
        fstp cosine
        fstp sine
    }
#else
#error TH09 exact Draw2D requires the target's x87 FSINCOS sequence.
#endif

    xOffset = vm->pos.x;
    yOffset = vm->pos.y;
    x = (vm->spriteSize.x * vm->scale.x) / 2.0f;
    y = (vm->spriteSize.y * vm->scale.y) / 2.0f;

    this->TranslateRotation(&g_AnmRenderQuad[0], -x, -y, sine, cosine, xOffset, yOffset);
    this->TranslateRotation(&g_AnmRenderQuad[1], x, -y, sine, cosine, xOffset, yOffset);
    this->TranslateRotation(&g_AnmRenderQuad[2], -x, y, sine, cosine, xOffset, yOffset);
    this->TranslateRotation(&g_AnmRenderQuad[3], x, y, sine, cosine, xOffset, yOffset);

    g_AnmRenderQuad[0].z = g_AnmRenderQuad[1].z =
        g_AnmRenderQuad[2].z = g_AnmRenderQuad[3].z = vm->pos.z;

    if ((vm->anchor & 1) != 0) {
        g_AnmRenderQuad[0].x += x;
        g_AnmRenderQuad[1].x += x;
        g_AnmRenderQuad[2].x += x;
        g_AnmRenderQuad[3].x += x;
    }
    if ((vm->anchor & 2) != 0) {
        g_AnmRenderQuad[0].y += y;
        g_AnmRenderQuad[1].y += y;
        g_AnmRenderQuad[2].y += y;
        g_AnmRenderQuad[3].y += y;
    }

    return this->DrawInner(vm, 0);
}


// Adjacent-family reconstruction name; original TH09 method spelling is not independently proven.
int AnmManager::Draw2DRotatedOrAxisAligned(AnmVm *vm)
{
    float rotation;
    float sine;
    float cosine;
    float halfWidth;
    float halfHeight;
    float xOffset;
    float yOffset;
    float zeroHalfWidth;
    float zeroHalfHeight;
    AnmVmRotationZView *rotationVm = reinterpret_cast<AnmVmRotationZView *>(vm);
    AnmVmDrawAlphaView *drawVm = reinterpret_cast<AnmVmDrawAlphaView *>(vm);

    if (!vm->IsVisible()) {
        return -1;
    }
    if ((vm->flagsWord & 2) == 0) {
        return -1;
    }
    if (drawVm->colorAlpha == 0) {
        return -1;
    }

    rotation = rotationVm->rotationZ;
    if (rotation != 0.0f) {
#if defined(_MSC_VER) && defined(_M_IX86)
        __asm {
            fld rotation
            fsincos
            fstp cosine
            fstp sine
        }
#else
#error TH09 Draw2DRotatedOrAxisAligned requires the target's x87 FSINCOS sequence.
#endif
        xOffset = vm->pos.x;
        yOffset = vm->pos.y;
        halfWidth = (vm->spriteSize.x * vm->scale.x) / 2.0f;
        halfHeight = (vm->spriteSize.y * vm->scale.y) / 2.0f;

        this->TranslateRotation(&g_AnmRenderQuad[0], -halfWidth, -halfHeight,
                                sine, cosine, xOffset, yOffset);
        this->TranslateRotation(&g_AnmRenderQuad[1], halfWidth, -halfHeight,
                                sine, cosine, xOffset, yOffset);
        this->TranslateRotation(&g_AnmRenderQuad[2], -halfWidth, halfHeight,
                                sine, cosine, xOffset, yOffset);
        this->TranslateRotation(&g_AnmRenderQuad[3], halfWidth, halfHeight,
                                sine, cosine, xOffset, yOffset);

        g_AnmRenderQuad[3].z = vm->pos.z;
        g_AnmRenderQuad[2].z = g_AnmRenderQuad[3].z;
        g_AnmRenderQuad[1].z = g_AnmRenderQuad[2].z;
        g_AnmRenderQuad[0].z = g_AnmRenderQuad[1].z;

        if ((vm->anchor & 1) != 0) {
            g_AnmRenderQuad[0].x += halfWidth;
            g_AnmRenderQuad[1].x += halfWidth;
            g_AnmRenderQuad[2].x += halfWidth;
            g_AnmRenderQuad[3].x += halfWidth;
        }
        if ((vm->anchor & 2) != 0) {
            g_AnmRenderQuad[0].y += halfHeight;
            g_AnmRenderQuad[1].y += halfHeight;
            g_AnmRenderQuad[2].y += halfHeight;
            g_AnmRenderQuad[3].y += halfHeight;
        }
    } else {
        zeroHalfWidth = (vm->spriteSize.x * vm->scale.x) / 2.0f;
        zeroHalfHeight = (vm->spriteSize.y * vm->scale.y) / 2.0f;

        if ((vm->anchor & 1) == 0) {
            g_AnmRenderQuad[2].x = vm->pos.x - zeroHalfWidth;
            g_AnmRenderQuad[0].x = g_AnmRenderQuad[2].x;
            g_AnmRenderQuad[3].x = zeroHalfWidth + vm->pos.x;
            g_AnmRenderQuad[1].x = g_AnmRenderQuad[3].x;
        } else {
            g_AnmRenderQuad[2].x = vm->pos.x;
            g_AnmRenderQuad[0].x = g_AnmRenderQuad[2].x;
            g_AnmRenderQuad[3].x = zeroHalfWidth + vm->pos.x + zeroHalfWidth;
            g_AnmRenderQuad[1].x = g_AnmRenderQuad[3].x;
        }

        if ((vm->anchor & 2) == 0) {
            g_AnmRenderQuad[1].y = vm->pos.y - zeroHalfHeight;
            g_AnmRenderQuad[0].y = g_AnmRenderQuad[1].y;
            g_AnmRenderQuad[3].y = zeroHalfHeight + vm->pos.y;
            g_AnmRenderQuad[2].y = g_AnmRenderQuad[3].y;
        } else {
            g_AnmRenderQuad[1].y = vm->pos.y;
            g_AnmRenderQuad[0].y = g_AnmRenderQuad[1].y;
            g_AnmRenderQuad[3].y = zeroHalfHeight + vm->pos.y + zeroHalfHeight;
            g_AnmRenderQuad[2].y = g_AnmRenderQuad[3].y;
        }
    }

    return this->DrawInner(vm, 0);
}


int AnmManager::DrawNoRotationNoRound(AnmVm *vm)
{
    float spriteHalfWidth;
    float spriteHalfHeight;
    AnmVmDrawAlphaView *drawVm = reinterpret_cast<AnmVmDrawAlphaView *>(vm);
    if (!vm->IsVisible()) return -1;
    if ((vm->flagsWord & 2) == 0) return -1;
    if (drawVm->colorAlpha == 0) return -1;
    spriteHalfWidth = (vm->spriteSize.x * vm->scale.x) / 2.0f;
    spriteHalfHeight = (vm->spriteSize.y * vm->scale.y) / 2.0f;
    if ((vm->anchor & 1) == 0) {
        g_AnmRenderQuad[0].x = g_AnmRenderQuad[2].x = vm->pos.x - spriteHalfWidth;
        g_AnmRenderQuad[1].x = g_AnmRenderQuad[3].x = spriteHalfWidth + vm->pos.x;
    } else {
        g_AnmRenderQuad[0].x = g_AnmRenderQuad[2].x = vm->pos.x;
        g_AnmRenderQuad[1].x = g_AnmRenderQuad[3].x = spriteHalfWidth + vm->pos.x + spriteHalfWidth;
    }
    if ((vm->anchor & 2) == 0) {
        g_AnmRenderQuad[0].y = g_AnmRenderQuad[1].y = vm->pos.y - spriteHalfHeight;
        g_AnmRenderQuad[2].y = g_AnmRenderQuad[3].y = spriteHalfHeight + vm->pos.y;
    } else {
        g_AnmRenderQuad[0].y = g_AnmRenderQuad[1].y = vm->pos.y;
        g_AnmRenderQuad[2].y = g_AnmRenderQuad[3].y = spriteHalfHeight + vm->pos.y + spriteHalfHeight;
    }
    g_AnmRenderQuad[0].z = g_AnmRenderQuad[1].z = g_AnmRenderQuad[2].z = g_AnmRenderQuad[3].z = vm->pos.z;
    return this->DrawInner(vm, 0);
}

int AnmManager::DrawCameraFacingQuad(AnmVm *vm)
{
    AnmVmDrawAlphaView *drawVm = reinterpret_cast<AnmVmDrawAlphaView *>(vm);
    if (!vm->IsVisible()) return -1;
    if ((vm->flagsWord & 2) == 0) return -1;
    if (drawVm->colorAlpha == 0) return -1;
    if (this->ProjectCameraFacingQuad(vm) != 0) return -1;
    return this->DrawInner(vm, 0);
}

int AnmManager::DrawProjected3DQuad(AnmVm *vm)
{
    AnmVmDrawAlphaView *drawVm = reinterpret_cast<AnmVmDrawAlphaView *>(vm);
    if (!vm->IsVisible()) return -1;
    if ((vm->flagsWord & 2) == 0) return -1;
    if (drawVm->colorAlpha == 0) return -1;
    this->Project3DQuad(vm);
    return this->DrawInner(vm, 0);
}
