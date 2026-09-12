#include "AnmManager.hpp"
#include "AsciiManager.hpp"
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

    int DrawInner(AnmVm *vm, int roundToPixels);
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

    return anm->DrawInner(vm, 1);
}
