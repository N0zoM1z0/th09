#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"

#include <stddef.h>
#include <string.h>
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

struct VertexDiffuseXyzrhw
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
};

struct VertexTex1Xyz
{
    Float3 pos;
    Float2 textureUV;
};

typedef char PrimitiveTexturedVertexSizeIs1C[(sizeof(VertexTex1DiffuseXyzrhw) == 0x1C) ? 1 : -1];
typedef char PrimitiveDiffuseVertexSizeIs14[(sizeof(VertexDiffuseXyzrhw) == 0x14) ? 1 : -1];
typedef char PrimitiveXyzTex1VertexSizeIs14[(sizeof(VertexTex1Xyz) == 0x14) ? 1 : -1];

struct AnmVmPrimitiveView
{
    unsigned char unknown000[0x1F3];
    unsigned char colorAlpha;
    unsigned char unknown1F4[0x1F8 - 0x1F4];
    unsigned int flagsWord;
    unsigned char unknown1FC[0x224 - 0x1FC];
    AnmLoadedSprite *loadedSprite;
};

typedef char PrimitiveVmAlphaAt1F3[(offsetof(AnmVmPrimitiveView, colorAlpha) == 0x1F3) ? 1 : -1];
typedef char PrimitiveVmFlagsAt1F8[(offsetof(AnmVmPrimitiveView, flagsWord) == 0x1F8) ? 1 : -1];
typedef char PrimitiveVmSpriteAt224[(offsetof(AnmVmPrimitiveView, loadedSprite) == 0x224) ? 1 : -1];

struct SupervisorPrimitiveView
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice;
};

struct AnmManagerPrimitiveView
{
    unsigned char unknown000000[0x12880];
    IDirect3DTexture8 *currentTexture;
    unsigned char currentBlendMode;
    unsigned char currentColorOp;
    unsigned char currentVertexShader;
    unsigned char disableZWrite;
    unsigned char unknown12888[0x12890 - 0x12888];
    IDirect3DVertexBuffer8 *quadVertexBuffer;
    VertexDiffuseXyzrhw untexturedVector[4];
    int spritesToDraw;
};

typedef char PrimitiveCurrentTextureAt12880[(offsetof(AnmManagerPrimitiveView, currentTexture) == 0x12880) ? 1 : -1];
typedef char PrimitiveBlendAt12884[(offsetof(AnmManagerPrimitiveView, currentBlendMode) == 0x12884) ? 1 : -1];
typedef char PrimitiveColorOpAt12885[(offsetof(AnmManagerPrimitiveView, currentColorOp) == 0x12885) ? 1 : -1];
typedef char PrimitiveShaderAt12886[(offsetof(AnmManagerPrimitiveView, currentVertexShader) == 0x12886) ? 1 : -1];
typedef char PrimitiveZWriteAt12887[(offsetof(AnmManagerPrimitiveView, disableZWrite) == 0x12887) ? 1 : -1];
typedef char PrimitiveVertexBufferAt12890[(offsetof(AnmManagerPrimitiveView, quadVertexBuffer) == 0x12890) ? 1 : -1];
typedef char PrimitiveSoftwareVerticesAt12894[(offsetof(AnmManagerPrimitiveView, untexturedVector) == 0x12894) ? 1 : -1];
typedef char PrimitiveSpritesToDrawAt128E4[(offsetof(AnmManagerPrimitiveView, spritesToDraw) == 0x128E4) ? 1 : -1];

extern VertexTex1Xyz g_BackgroundQuadVertices[4];

static __inline AnmManagerPrimitiveView *PrimitiveManager(AnmManager *manager)
{
    return reinterpret_cast<AnmManagerPrimitiveView *>(manager);
}

static __inline SupervisorPrimitiveView *PrimitiveSupervisor()
{
    return reinterpret_cast<SupervisorPrimitiveView *>(&g_Supervisor);
}

void AnmManager::ClearBlendMode() { PrimitiveManager(this)->currentBlendMode = 3; }
void AnmManager::ClearColorOp() { PrimitiveManager(this)->currentColorOp = 0xff; }
void AnmManager::ClearVertexShader() { PrimitiveManager(this)->currentVertexShader = 0xff; }
void AnmManager::ClearZWrite() { PrimitiveManager(this)->disableZWrite = 0xff; }

int AnmManager::DrawVertices(AnmVm *vmObject, VertexTex1DiffuseXyzrhw *vertices, int vertexCount)
{
    AnmManagerPrimitiveView *anm = PrimitiveManager(this);
    AnmVmPrimitiveView *vm = reinterpret_cast<AnmVmPrimitiveView *>(vmObject);
    SupervisorPrimitiveView *supervisor = PrimitiveSupervisor();
    if (!vmObject->IsVisible()) return -1;
    if ((vm->flagsWord & 2) == 0) return -1;
    if (vm->colorAlpha == 0) return -1;
    if (anm->spritesToDraw != 0) this->FlushVertexBuffer();
    if (anm->currentTexture != vm->loadedSprite->texture) {
        anm->currentTexture = reinterpret_cast<IDirect3DTexture8 *>(vm->loadedSprite->texture);
        supervisor->d3dDevice->SetTexture(0, anm->currentTexture);
    }
    if (anm->currentVertexShader != 3) {
        supervisor->d3dDevice->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
        anm->currentVertexShader = 3;
    }
    this->SetRenderStateForVm(vmObject);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    supervisor->d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, vertexCount - 2, vertices, sizeof(VertexTex1DiffuseXyzrhw));
    return 0;
}

int AnmManager::QueueSpriteQuad(AnmVm *vmObject, VertexTex1DiffuseXyzrhw *vertices)
{
    AnmManagerPrimitiveView *anm = PrimitiveManager(this);
    AnmVmPrimitiveView *vm = reinterpret_cast<AnmVmPrimitiveView *>(vmObject);
    SupervisorPrimitiveView *supervisor = PrimitiveSupervisor();
    if (!vmObject->IsVisible()) return -1;
    if ((vm->flagsWord & 2) == 0) return -1;
    if (vm->colorAlpha == 0) return -1;
    if (anm->currentTexture != vm->loadedSprite->texture) {
        anm->currentTexture = reinterpret_cast<IDirect3DTexture8 *>(vm->loadedSprite->texture);
        this->FlushVertexBuffer();
        supervisor->d3dDevice->SetTexture(0, anm->currentTexture);
    }
    if (anm->currentVertexShader != 1) {
        this->FlushVertexBuffer();
        anm->currentVertexShader = 1;
    }
    this->SetRenderStateForVm(vmObject);
    this->AddSpriteToDrawBuffer(vertices);
    return 0;
}

int AnmManager::DrawTriangleStrip(AnmVm *vm, VertexDiffuseXyzrhw *vertices, int vertexCount)
{
    AnmManagerPrimitiveView *anm = PrimitiveManager(this);
    SupervisorPrimitiveView *supervisor = PrimitiveSupervisor();
    if (anm->spritesToDraw != 0) this->FlushVertexBuffer();
    if (anm->currentVertexShader != 4) {
        supervisor->d3dDevice->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
        anm->currentVertexShader = 4;
    }
    this->SetRenderStateForVm(vm);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    g_Supervisor.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    supervisor->d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, vertexCount - 2, vertices, sizeof(VertexDiffuseXyzrhw));
    g_AnmManager->ClearVertexShader();
    g_AnmManager->ClearColorOp();
    g_AnmManager->ClearBlendMode();
    g_AnmManager->ClearZWrite();
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    return 0;
}

int AnmManager::DrawTriangleFan(AnmVm *vm, VertexDiffuseXyzrhw *vertices, int vertexCount)
{
    AnmManagerPrimitiveView *anm = PrimitiveManager(this);
    SupervisorPrimitiveView *supervisor = PrimitiveSupervisor();
    if (anm->spritesToDraw != 0) this->FlushVertexBuffer();
    if (anm->currentVertexShader != 4) {
        supervisor->d3dDevice->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
        anm->currentVertexShader = 4;
    }
    this->SetRenderStateForVm(vm);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    g_Supervisor.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    supervisor->d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, vertexCount - 2, vertices, sizeof(VertexDiffuseXyzrhw));
    g_AnmManager->ClearVertexShader();
    g_AnmManager->ClearColorOp();
    g_AnmManager->ClearBlendMode();
    g_AnmManager->ClearZWrite();
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    return 0;
}

int AnmManager::DrawTexturedTriangleFan(AnmVm *vmObject, VertexTex1DiffuseXyzrhw *vertices, int vertexCount)
{
    AnmManagerPrimitiveView *anm = PrimitiveManager(this);
    AnmVmPrimitiveView *vm = reinterpret_cast<AnmVmPrimitiveView *>(vmObject);
    SupervisorPrimitiveView *supervisor = PrimitiveSupervisor();
    if (anm->spritesToDraw != 0) this->FlushVertexBuffer();
    if (anm->currentVertexShader != 3) {
        supervisor->d3dDevice->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
        anm->currentVertexShader = 3;
    }
    this->SetRenderStateForVm(vmObject);
    if (anm->currentTexture != vm->loadedSprite->texture) {
        anm->currentTexture = reinterpret_cast<IDirect3DTexture8 *>(vm->loadedSprite->texture);
        supervisor->d3dDevice->SetTexture(0, anm->currentTexture);
    }
    g_Supervisor.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    supervisor->d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, vertexCount - 2, vertices, sizeof(VertexTex1DiffuseXyzrhw));
    return 0;
}

void AnmManager::SetupVertexBuffer()
{
    AnmManagerPrimitiveView *anm = PrimitiveManager(this);
    void *lockedVertexBuffer;

    anm->untexturedVector[2].pos.x = -128.0f;
    anm->untexturedVector[0].pos.x = -128.0f;
    anm->untexturedVector[3].pos.x = 128.0f;
    anm->untexturedVector[1].pos.x = 128.0f;
    anm->untexturedVector[1].pos.y = -128.0f;
    anm->untexturedVector[0].pos.y = -128.0f;
    anm->untexturedVector[3].pos.y = 128.0f;
    anm->untexturedVector[2].pos.y = 128.0f;
    anm->untexturedVector[3].pos.z = 0.0f;
    anm->untexturedVector[2].pos.z = 0.0f;
    anm->untexturedVector[1].pos.z = 0.0f;
    anm->untexturedVector[0].pos.z = 0.0f;
    anm->untexturedVector[2].rhw = 0.0f;
    anm->untexturedVector[0].rhw = 0.0f;
    anm->untexturedVector[3].rhw = 1.0f;
    anm->untexturedVector[1].rhw = 1.0f;
    anm->untexturedVector[1].diffuse = 0;
    anm->untexturedVector[0].diffuse = 0;
    anm->untexturedVector[3].diffuse = 0x3f800000;
    anm->untexturedVector[2].diffuse = 0x3f800000;

    g_BackgroundQuadVertices[0].pos = anm->untexturedVector[0].pos;
    g_BackgroundQuadVertices[1].pos = anm->untexturedVector[1].pos;
    g_BackgroundQuadVertices[2].pos = anm->untexturedVector[2].pos;
    g_BackgroundQuadVertices[3].pos = anm->untexturedVector[3].pos;
    *reinterpret_cast<unsigned long *>(&g_BackgroundQuadVertices[0].textureUV.x) =
        *reinterpret_cast<unsigned long *>(&anm->untexturedVector[0].rhw);
    *reinterpret_cast<unsigned long *>(&g_BackgroundQuadVertices[0].textureUV.y) =
        anm->untexturedVector[0].diffuse;
    *reinterpret_cast<unsigned long *>(&g_BackgroundQuadVertices[1].textureUV.x) =
        *reinterpret_cast<unsigned long *>(&anm->untexturedVector[1].rhw);
    *reinterpret_cast<unsigned long *>(&g_BackgroundQuadVertices[1].textureUV.y) =
        anm->untexturedVector[1].diffuse;
    *reinterpret_cast<unsigned long *>(&g_BackgroundQuadVertices[2].textureUV.x) =
        *reinterpret_cast<unsigned long *>(&anm->untexturedVector[2].rhw);
    *reinterpret_cast<unsigned long *>(&g_BackgroundQuadVertices[2].textureUV.y) =
        anm->untexturedVector[2].diffuse;
    *reinterpret_cast<unsigned long *>(&g_BackgroundQuadVertices[3].textureUV.x) =
        *reinterpret_cast<unsigned long *>(&anm->untexturedVector[3].rhw);
    *reinterpret_cast<unsigned long *>(&g_BackgroundQuadVertices[3].textureUV.y) =
        anm->untexturedVector[3].diffuse;

    PrimitiveSupervisor()->d3dDevice->CreateVertexBuffer(
        sizeof(anm->untexturedVector), 0, D3DFVF_XYZ | D3DFVF_TEX1,
        D3DPOOL_MANAGED, &anm->quadVertexBuffer);
    anm->quadVertexBuffer->Lock(0, 0, reinterpret_cast<BYTE **>(&lockedVertexBuffer), 0);
    memcpy(lockedVertexBuffer, anm->untexturedVector, sizeof(anm->untexturedVector));
    anm->quadVertexBuffer->Unlock();
    PrimitiveSupervisor()->d3dDevice->SetStreamSource(
        0, PrimitiveManager(g_AnmManager)->quadVertexBuffer,
        sizeof(VertexDiffuseXyzrhw));
}
