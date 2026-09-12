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
    unsigned char unknown008[0x1C - 0x08];
    Float2 screenShakeOffset;
    unsigned char unknown024[0x12880 - 0x24];
    void *currentTexture;
    unsigned char unknown12884[2];
    unsigned char currentVertexShader;
    unsigned char unknown12887[0x128E4 - 0x12887];
    int spritesToDraw;
    unsigned char unknown128E8[0x2B28E8 - 0x128E8];
    VertexTex1DiffuseXyzrhw *vertexBufferEndPtr;
    VertexTex1DiffuseXyzrhw *vertexBufferStartPtr;
};

typedef char AnmManagerDrawInnerShakeAt1C[
    (offsetof(AnmManagerDrawInnerView, screenShakeOffset) == 0x1C) ? 1 : -1];
typedef char AnmManagerDrawInnerTextureAt12880[
    (offsetof(AnmManagerDrawInnerView, currentTexture) == 0x12880) ? 1 : -1];
typedef char AnmManagerDrawInnerShaderAt12886[
    (offsetof(AnmManagerDrawInnerView, currentVertexShader) == 0x12886) ? 1 : -1];

struct AnmManagerRenderMethodView
{
    void ApplyVmRenderState(AnmVm *vm);
};

unsigned char MixRenderColor(unsigned char first, unsigned char second);

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

    reinterpret_cast<AnmManagerRenderMethodView *>(this)->ApplyVmRenderState(vm);
    this->AddSpriteToDrawBuffer(g_AnmRenderQuad);
    return 0;
}
