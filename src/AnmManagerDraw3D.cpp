#include "AnmManager.hpp"
#include "VertexDiffuseXyzrhw.hpp"
#include "AnmManagerQuadCacheView.hpp"
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

typedef char VertexDiffuseXyzrhwSizeIs14[(sizeof(VertexDiffuseXyzrhw) == 0x14) ? 1 : -1];

// The 811-byte Draw3D owner consumes the same physical Supervisor D3D8
// device cell at +0x08 as 2D ANM primitives and gameplay presentation.
typedef char Draw3DNativeSupervisorD3DDevice[
    (offsetof(Supervisor, d3dDevice08) == 0x08) ? 1 : -1];

// Matrices live at the native AnmVmBase::matrix1/matrix2/matrix3 slots.
// The D3DX API consumes the same 4x4-float object representation.
typedef char Draw3DNativeSdkMatrixGeometry[
    (sizeof(AnmVmScriptMatrix) == 0x40 &&
     sizeof(D3DXMATRIX) == sizeof(AnmVmScriptMatrix) &&
     offsetof(AnmVmBase, matrix1) == 0x130 &&
     offsetof(AnmVmBase, matrix2) == 0x170 &&
     offsetof(AnmVmBase, matrix3) == 0x1B0) ? 1 : -1];

// The 811-byte Draw3D uses the same native cache tail as ANM primitives.
typedef AnmManagerQuadCacheView AnmManagerDraw3DView;

typedef char Draw3DNativeVmFieldOffsets[
    (offsetof(AnmVmBase, rotation) == 0x000 &&
     offsetof(AnmVmBase, scale) == 0x018 &&
     offsetof(AnmVmBase, spriteSize) == 0x028 &&
     offsetof(AnmVmBase, uvScrollPos) == 0x030 &&
     offsetof(AnmVmBase, matrix1) == 0x130 &&
     offsetof(AnmVmBase, matrix2) == 0x170 &&
     offsetof(AnmVmBase, matrix3) == 0x1B0 &&
     offsetof(AnmVmBase, flagsWord) == 0x1F8 &&
     offsetof(AnmVmBase, pos) == 0x208 &&
     offsetof(AnmVmBase, loadedSprite) == 0x224) ? 1 : -1];

typedef char Draw3DShakeAt1C[(offsetof(AnmManagerRenderCacheView, screenShakeOffset) == 0x1C) ? 1 : -1];
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
    AnmManagerDraw3DView *anm = reinterpret_cast<AnmManagerDraw3DView *>(this);
    Supervisor *supervisor = &g_Supervisor;

    if (!vm->IsVisible())
        return -1;
    if ((vm->flagsWord & 2) == 0)
        return -1;
    if (vm->color1Script.a == 0)
        return -1;

    if (anm->spritesToDraw != 0)
        this->FlushVertexBuffer();

    if ((vm->flagsWord & 0x10000) == 0 && (vm->flagsWord & 0x0C) != 0) {
        reinterpret_cast<D3DXMATRIX &>(vm->matrix2) = reinterpret_cast<D3DXMATRIX &>(vm->matrix1);
        reinterpret_cast<D3DXMATRIX &>(vm->matrix2)._11 *= vm->scale.x;
        reinterpret_cast<D3DXMATRIX &>(vm->matrix2)._22 *= vm->scale.y;
        vm->flagsWord &= ~8u;

        if (0.0 != vm->rotation.x) {
            D3DXMatrixRotationX(&rotationMatrix, vm->rotation.x);
            D3DXMatrixMultiply(&reinterpret_cast<D3DXMATRIX &>(vm->matrix2), &reinterpret_cast<D3DXMATRIX &>(vm->matrix2), &rotationMatrix);
        }
        if (0.0 != vm->rotation.y) {
            D3DXMatrixRotationY(&rotationMatrix, vm->rotation.y);
            D3DXMatrixMultiply(&reinterpret_cast<D3DXMATRIX &>(vm->matrix2), &reinterpret_cast<D3DXMATRIX &>(vm->matrix2), &rotationMatrix);
        }
        if (0.0 != vm->rotation.z) {
            D3DXMatrixRotationZ(&rotationMatrix, vm->rotation.z);
            D3DXMatrixMultiply(&reinterpret_cast<D3DXMATRIX &>(vm->matrix2), &reinterpret_cast<D3DXMATRIX &>(vm->matrix2), &rotationMatrix);
        }
        vm->flagsWord &= ~4u;
    }

    worldTransformMatrix = reinterpret_cast<D3DXMATRIX &>(vm->matrix2);
    unsigned int anchorBits = vm->flagsWord >> 11;
    if ((anchorBits & 1) == 0)
        worldTransformMatrix._41 = vm->pos.x;
    else
        worldTransformMatrix._41 = AnmDraw3DAbs(vm->spriteSize.x * vm->scale.x / 2.0f) + vm->pos.x;
    if ((anchorBits & 2) == 0)
        worldTransformMatrix._42 = vm->pos.y;
    else
        worldTransformMatrix._42 = AnmDraw3DAbs(vm->spriteSize.y * vm->scale.y / 2.0f) + vm->pos.y;
    worldTransformMatrix._41 += this->frameShakeX1C;
    worldTransformMatrix._42 += this->frameShakeY20;

    this->SetRenderStateForVm3D(vm);
    worldTransformMatrix._43 = vm->pos.z;
    supervisor->d3dDevice08->SetTransform(D3DTS_WORLD, &worldTransformMatrix);

    if (anm->currentSprite != vm->loadedSprite ||
        vm->uvScrollPos.x != 0.0f || vm->uvScrollPos.x != 0.0f) {
        anm->currentSprite = vm->loadedSprite;
        textureMatrix = reinterpret_cast<D3DXMATRIX &>(vm->matrix3);
        textureMatrix._31 = vm->loadedSprite->uvStartX + vm->uvScrollPos.x;
        textureMatrix._32 = vm->loadedSprite->uvStartY + vm->uvScrollPos.y;
        supervisor->d3dDevice08->SetTransform(D3DTS_TEXTURE0, &textureMatrix);

    }

    if (anm->currentTexture != vm->loadedSprite->texture) {
        anm->currentTexture = reinterpret_cast<IDirect3DTexture8 *>(vm->loadedSprite->texture);
        supervisor->d3dDevice08->SetTexture(
            0, reinterpret_cast<IDirect3DBaseTexture8 *>(anm->currentTexture));
    }

    if (anm->currentVertexShader != 2) {
        supervisor->d3dDevice08->SetVertexShader(D3DFVF_XYZ | D3DFVF_TEX1);
        supervisor->d3dDevice08->SetStreamSource(
            0, anm->quadVertexBuffer, sizeof(VertexDiffuseXyzrhw));
        supervisor->d3dDevice08->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
        supervisor->d3dDevice08->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
        anm->currentVertexShader = 2;
    }

    supervisor->d3dDevice08->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    return 0;
}
