#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"

#include <stddef.h>
#include <math.h>
#include <windows.h>
#include <d3d8.h>

struct D3DXMATRIX : public D3DMATRIX {};
D3DXMATRIX *WINAPI D3DXMatrixMultiply(D3DXMATRIX *out, const D3DXMATRIX *left, const D3DXMATRIX *right);
D3DXMATRIX *WINAPI D3DXMatrixRotationX(D3DXMATRIX *out, float angle);
D3DXMATRIX *WINAPI D3DXMatrixRotationY(D3DXMATRIX *out, float angle);
D3DXMATRIX *WINAPI D3DXMatrixRotationZ(D3DXMATRIX *out, float angle);
// Neutral view of the shared 0x0040F1C0 float absolute-value helper; owner/origin remain review.
long double __stdcall AnmDraw3DAbs(float value);

struct VertexDiffuseXyzrhw
{
    float x, y, z, rhw;
    unsigned long diffuse;
};

typedef char VertexDiffuseXyzrhwSizeIs14[(sizeof(VertexDiffuseXyzrhw) == 0x14) ? 1 : -1];

struct SupervisorDraw3DView
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice;
};

struct AnmVmDraw3DView
{
    Float3 rotation;
    unsigned char unknown00C[0x0C];
    Float2 scale;
    unsigned char unknown020[0x08];
    Float2 spriteSize;
    Float2 uvScrollPos;
    unsigned char unknown038[0xF8];
    D3DXMATRIX matrix1;
    D3DXMATRIX matrix2;
    D3DXMATRIX matrix3;
    unsigned long color1;
    unsigned long color2;
    unsigned int flagsWord;
    unsigned char unknown1FC[0x0C];
    Float3 pos;
    unsigned char unknown214[0x10];
    AnmLoadedSprite *loadedSprite;
};

typedef char Draw3DRotationAt000[(offsetof(AnmVmDraw3DView, rotation) == 0x000) ? 1 : -1];
typedef char Draw3DScaleAt018[(offsetof(AnmVmDraw3DView, scale) == 0x018) ? 1 : -1];
typedef char Draw3DSpriteSizeAt028[(offsetof(AnmVmDraw3DView, spriteSize) == 0x028) ? 1 : -1];
typedef char Draw3DUvAt030[(offsetof(AnmVmDraw3DView, uvScrollPos) == 0x030) ? 1 : -1];
typedef char Draw3DMatrix1At130[(offsetof(AnmVmDraw3DView, matrix1) == 0x130) ? 1 : -1];
typedef char Draw3DMatrix2At170[(offsetof(AnmVmDraw3DView, matrix2) == 0x170) ? 1 : -1];
typedef char Draw3DMatrix3At1B0[(offsetof(AnmVmDraw3DView, matrix3) == 0x1B0) ? 1 : -1];
typedef char Draw3DFlagsAt1F8[(offsetof(AnmVmDraw3DView, flagsWord) == 0x1F8) ? 1 : -1];
typedef char Draw3DPosAt208[(offsetof(AnmVmDraw3DView, pos) == 0x208) ? 1 : -1];
typedef char Draw3DSpriteAt224[(offsetof(AnmVmDraw3DView, loadedSprite) == 0x224) ? 1 : -1];

struct AnmManagerDraw3DView
{
    unsigned char unknown00000[0x1C];
    Float2 screenShakeOffset;
    unsigned char unknown00024[0x12880 - 0x24];
    void *currentTexture;
    unsigned char currentBlendMode;
    unsigned char unknown12885;
    unsigned char currentVertexShader;
    unsigned char disableZWrite;
    unsigned char unknown12888[4];
    AnmLoadedSprite *currentSprite;
    IDirect3DVertexBuffer8 *quadVertexBuffer;
    unsigned char unknown12894[0x128E4 - 0x12894];
    int spritesToDraw;
};

typedef char Draw3DShakeAt1C[(offsetof(AnmManagerDraw3DView, screenShakeOffset) == 0x1C) ? 1 : -1];
typedef char Draw3DTextureAt12880[(offsetof(AnmManagerDraw3DView, currentTexture) == 0x12880) ? 1 : -1];
typedef char Draw3DShaderAt12886[(offsetof(AnmManagerDraw3DView, currentVertexShader) == 0x12886) ? 1 : -1];
typedef char Draw3DSpriteCacheAt1288C[(offsetof(AnmManagerDraw3DView, currentSprite) == 0x1288C) ? 1 : -1];
typedef char Draw3DQuadVbAt12890[(offsetof(AnmManagerDraw3DView, quadVertexBuffer) == 0x12890) ? 1 : -1];
typedef char Draw3DSpritesToDrawAt128E4[(offsetof(AnmManagerDraw3DView, spritesToDraw) == 0x128E4) ? 1 : -1];

int AnmManager::Draw3D(AnmVm *vm)
{
    D3DMATRIX textureMatrix;
    D3DXMATRIX rotationMatrix;
    D3DXMATRIX worldTransformMatrix;
    AnmVmDraw3DView *drawVm = reinterpret_cast<AnmVmDraw3DView *>(vm);
    AnmManagerDraw3DView *anm = reinterpret_cast<AnmManagerDraw3DView *>(this);
    SupervisorDraw3DView *supervisor = reinterpret_cast<SupervisorDraw3DView *>(&g_Supervisor);

    if (!vm->IsVisible())
        return -1;
    if ((drawVm->flagsWord & 2) == 0)
        return -1;
    if (*(reinterpret_cast<unsigned char *>(vm) + 0x1F3) == 0)
        return -1;

    if (anm->spritesToDraw != 0)
        this->FlushVertexBuffer();

    if ((drawVm->flagsWord & 0x10000) == 0 && (drawVm->flagsWord & 0x0C) != 0) {
        drawVm->matrix2 = drawVm->matrix1;
        drawVm->matrix2._11 *= drawVm->scale.x;
        drawVm->matrix2._22 *= drawVm->scale.y;
        drawVm->flagsWord &= ~8u;

        if (0.0f != drawVm->rotation.x) {
            D3DXMatrixRotationX(&rotationMatrix, drawVm->rotation.x);
            D3DXMatrixMultiply(&drawVm->matrix2, &drawVm->matrix2, &rotationMatrix);
        }
        if (0.0f != drawVm->rotation.y) {
            D3DXMatrixRotationY(&rotationMatrix, drawVm->rotation.y);
            D3DXMatrixMultiply(&drawVm->matrix2, &drawVm->matrix2, &rotationMatrix);
        }
        if (0.0f != drawVm->rotation.z) {
            D3DXMatrixRotationZ(&rotationMatrix, drawVm->rotation.z);
            D3DXMatrixMultiply(&drawVm->matrix2, &drawVm->matrix2, &rotationMatrix);
        }
        drawVm->flagsWord &= ~4u;
    }

    unsigned int anchorBits = drawVm->flagsWord >> 11;
    worldTransformMatrix = drawVm->matrix2;
    if ((anchorBits & 1) == 0)
        worldTransformMatrix._41 = drawVm->pos.x;
    else
        worldTransformMatrix._41 = AnmDraw3DAbs(drawVm->spriteSize.x * drawVm->scale.x / 2.0f) + drawVm->pos.x;
    if ((anchorBits & 2) == 0)
        worldTransformMatrix._42 = drawVm->pos.y;
    else
        worldTransformMatrix._42 = AnmDraw3DAbs(drawVm->spriteSize.y * drawVm->scale.y / 2.0f) + drawVm->pos.y;
    worldTransformMatrix._41 += anm->screenShakeOffset.x;
    worldTransformMatrix._42 += anm->screenShakeOffset.y;

    this->SetRenderStateForVm3D(vm);
    worldTransformMatrix._43 = drawVm->pos.z;
    supervisor->d3dDevice->SetTransform(D3DTS_WORLD, &worldTransformMatrix);

    if (anm->currentSprite != drawVm->loadedSprite ||
        drawVm->uvScrollPos.x != 0.0f || drawVm->uvScrollPos.x != 0.0f) {
        anm->currentSprite = drawVm->loadedSprite;
        textureMatrix = drawVm->matrix3;
        textureMatrix._31 = drawVm->loadedSprite->uvStartX + drawVm->uvScrollPos.x;
        textureMatrix._32 = drawVm->loadedSprite->uvStartY + drawVm->uvScrollPos.y;
        supervisor->d3dDevice->SetTransform(D3DTS_TEXTURE0, &textureMatrix);

        if (anm->currentTexture != drawVm->loadedSprite->texture) {
            anm->currentTexture = drawVm->loadedSprite->texture;
            supervisor->d3dDevice->SetTexture(
                0, reinterpret_cast<IDirect3DBaseTexture8 *>(anm->currentTexture));
        }
    }

    if (anm->currentVertexShader != 2) {
        supervisor->d3dDevice->SetVertexShader(D3DFVF_XYZ | D3DFVF_TEX1);
        supervisor->d3dDevice->SetStreamSource(
            0, anm->quadVertexBuffer, sizeof(VertexDiffuseXyzrhw));
        supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
        supervisor->d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
        anm->currentVertexShader = 2;
    }

    supervisor->d3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    return 0;
}
