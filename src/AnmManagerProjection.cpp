#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include <stddef.h>
#include <math.h>
#include <windows.h>
#include <d3d8.h>
struct D3DXVECTOR3 : public D3DVECTOR {};
struct D3DXMATRIX : public D3DMATRIX {};
D3DXMATRIX *D3DXMatrixIdentity(D3DXMATRIX *matrix);
D3DXMATRIX *WINAPI D3DXMatrixMultiply(D3DXMATRIX *out, const D3DXMATRIX *left, const D3DXMATRIX *right);
D3DXMATRIX *WINAPI D3DXMatrixRotationX(D3DXMATRIX *out, float angle);
D3DXMATRIX *WINAPI D3DXMatrixRotationY(D3DXMATRIX *out, float angle);
D3DXMATRIX *WINAPI D3DXMatrixRotationZ(D3DXMATRIX *out, float angle);
float D3DXVec3Length(const D3DXVECTOR3 *vector);
// Neutral view of target 0x0040F1C0; source owner and original name remain unresolved.
long double __stdcall AnmProjectionAbs(float value);
D3DXVECTOR3 *WINAPI D3DXVec3Project(D3DXVECTOR3 *out, const D3DXVECTOR3 *vector, const D3DVIEWPORT8 *viewport, const D3DXMATRIX *projection, const D3DXMATRIX *view, const D3DXMATRIX *world);
struct VertexTex1DiffuseXyzrhw { float x,y,z,rhw; unsigned long diffuse; float u,v; };
struct VertexDiffuseXyzrhw { float x,y,z,rhw; unsigned long diffuse; };
typedef char VertexProjectionSourceSizeIs14[(sizeof(VertexDiffuseXyzrhw)==0x14)?1:-1];
extern VertexTex1DiffuseXyzrhw g_AnmRenderQuad[4];
struct AnmProjectionContextView {
    unsigned char unknown000[0x30]; Float3 cameraRight; unsigned char unknown03C[0x10];
    D3DXMATRIX viewMatrix; D3DXMATRIX projectionMatrix; D3DVIEWPORT8 viewport;
};
extern AnmProjectionContextView *g_AnmProjectionContext;
struct AnmVmProjectionView {
    Float3 rotation; unsigned char unknown00C[0x0C]; Float2 scale; unsigned char unknown020[0x08]; Float2 spriteSize;
    unsigned char unknown030[0x100]; D3DXMATRIX matrix1; D3DXMATRIX matrix2; unsigned char unknown1B0[0x48];
    unsigned int flagsWord; unsigned char unknown1FC[0x0C]; Float3 pos;
};
typedef char AnmVmProjectionMatrix1At130[(offsetof(AnmVmProjectionView,matrix1)==0x130)?1:-1];
typedef char AnmVmProjectionMatrix2At170[(offsetof(AnmVmProjectionView,matrix2)==0x170)?1:-1];
typedef char AnmVmProjectionFlagsAt1F8[(offsetof(AnmVmProjectionView,flagsWord)==0x1F8)?1:-1];
typedef char AnmVmProjectionPosAt208[(offsetof(AnmVmProjectionView,pos)==0x208)?1:-1];
struct AnmManagerProjectionView {
    unsigned char unknown00000[0x12024]; D3DXMATRIX cachedWorldMatrix; unsigned char unknown12064[0x830]; VertexDiffuseXyzrhw untexturedVertices[4];
};
typedef char AnmManagerCachedWorldAt12024[(offsetof(AnmManagerProjectionView,cachedWorldMatrix)==0x12024)?1:-1];
typedef char AnmManagerUntexturedAt12894[(offsetof(AnmManagerProjectionView,untexturedVertices)==0x12894)?1:-1];

int AnmManager::ProjectCameraFacingQuad(AnmVm *vm)
{
    float rotation,sine,cosine,xOffset,yOffset,halfHeight,halfWidth;
    D3DXMATRIX worldMatrix;
    D3DXVECTOR3 projectedPosition;
    D3DXVECTOR3 projectedReference;
    Float3 origin(0.0f, 0.0f, 0.0f);
    float *position;
    AnmVmProjectionView *projectionVm=reinterpret_cast<AnmVmProjectionView *>(vm);
    rotation=projectionVm->rotation.z;
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm {
        fld rotation
        fsincos
        fstp cosine
        fstp sine
    }
#else
#error TH09 ProjectCameraFacingQuad requires target-observed x87 FSINCOS.
#endif
    D3DXMatrixIdentity(&worldMatrix);
    position=projectionVm->pos.operator float *();
    worldMatrix._41=position[0]; worldMatrix._42=position[1]; worldMatrix._43=position[2];
    D3DXVec3Project(reinterpret_cast<D3DXVECTOR3 *>(&projectedPosition),reinterpret_cast<D3DXVECTOR3 *>(&origin),&g_AnmProjectionContext->viewport,&g_AnmProjectionContext->projectionMatrix,&g_AnmProjectionContext->viewMatrix,&worldMatrix);
    if (projectedPosition.z<0.0f || projectedPosition.z>1.0f) return -1;
    D3DXVec3Project(reinterpret_cast<D3DXVECTOR3 *>(&projectedReference),reinterpret_cast<D3DXVECTOR3 *>(&g_AnmProjectionContext->cameraRight),&g_AnmProjectionContext->viewport,&g_AnmProjectionContext->projectionMatrix,&g_AnmProjectionContext->viewMatrix,&worldMatrix);
    Float3 delta = *reinterpret_cast<Float3 *>(&projectedReference) -
                   *reinterpret_cast<Float3 *>(&projectedPosition);
    xOffset=D3DXVec3Length(reinterpret_cast<D3DXVECTOR3 *>(&delta))*0.5f;
    halfWidth=xOffset*vm->spriteSize.x*vm->scale.x;
    halfHeight=xOffset*vm->spriteSize.y*vm->scale.y;
    xOffset=projectedPosition.x; yOffset=projectedPosition.y;
    this->TranslateRotation(&g_AnmRenderQuad[0],-halfWidth,-halfHeight,sine,cosine,xOffset,yOffset);
    this->TranslateRotation(&g_AnmRenderQuad[1],halfWidth,-halfHeight,sine,cosine,xOffset,yOffset);
    this->TranslateRotation(&g_AnmRenderQuad[2],-halfWidth,halfHeight,sine,cosine,xOffset,yOffset);
    this->TranslateRotation(&g_AnmRenderQuad[3],halfWidth,halfHeight,sine,cosine,xOffset,yOffset);
    g_AnmRenderQuad[0].z=g_AnmRenderQuad[1].z=g_AnmRenderQuad[2].z=g_AnmRenderQuad[3].z=projectedPosition.z;
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
    return 0;
}

void AnmManager::Project3DQuad(AnmVm *vm)
{
    D3DXMATRIX worldTransformMatrix,rotationMatrix;
    AnmVmProjectionView *projectionVm=reinterpret_cast<AnmVmProjectionView *>(vm);
    AnmManagerProjectionView *anm=reinterpret_cast<AnmManagerProjectionView *>(this);
    if ((projectionVm->flagsWord&0x10000)==0 && (projectionVm->flagsWord&0x0C)!=0) {
        projectionVm->matrix2=projectionVm->matrix1;
        projectionVm->matrix2._11*=vm->scale.x; projectionVm->matrix2._22*=vm->scale.y;
        projectionVm->flagsWord&=~8u;
        if (projectionVm->rotation.x!=0.0) { D3DXMatrixRotationX(&rotationMatrix,projectionVm->rotation.x); D3DXMatrixMultiply(&projectionVm->matrix2,&projectionVm->matrix2,&rotationMatrix); }
        if (projectionVm->rotation.y!=0.0) { D3DXMatrixRotationY(&rotationMatrix,projectionVm->rotation.y); D3DXMatrixMultiply(&projectionVm->matrix2,&projectionVm->matrix2,&rotationMatrix); }
        if (projectionVm->rotation.z!=0.0) { D3DXMatrixRotationZ(&rotationMatrix,projectionVm->rotation.z); D3DXMatrixMultiply(&projectionVm->matrix2,&projectionVm->matrix2,&rotationMatrix); }
        projectionVm->flagsWord&=~4u;
    }
    worldTransformMatrix=projectionVm->matrix2;
    unsigned int anchor = projectionVm->flagsWord >> 11;
    if ((anchor&1)==0) worldTransformMatrix._41=vm->pos.x; else worldTransformMatrix._41=AnmProjectionAbs(vm->spriteSize.x*vm->scale.x/2.0f)+vm->pos.x;
    if ((anchor&2)==0) worldTransformMatrix._42=vm->pos.y; else worldTransformMatrix._42=AnmProjectionAbs(vm->spriteSize.y*vm->scale.y/2.0f)+vm->pos.y;
    worldTransformMatrix._43=vm->pos.z;
    D3DXVec3Project(reinterpret_cast<D3DXVECTOR3 *>(&g_AnmRenderQuad[0]),reinterpret_cast<D3DXVECTOR3 *>(&anm->untexturedVertices[0]),&g_AnmProjectionContext->viewport,&g_AnmProjectionContext->projectionMatrix,&g_AnmProjectionContext->viewMatrix,&worldTransformMatrix);
    D3DXVec3Project(reinterpret_cast<D3DXVECTOR3 *>(&g_AnmRenderQuad[1]),reinterpret_cast<D3DXVECTOR3 *>(&anm->untexturedVertices[1]),&g_AnmProjectionContext->viewport,&g_AnmProjectionContext->projectionMatrix,&g_AnmProjectionContext->viewMatrix,&worldTransformMatrix);
    D3DXVec3Project(reinterpret_cast<D3DXVECTOR3 *>(&g_AnmRenderQuad[2]),reinterpret_cast<D3DXVECTOR3 *>(&anm->untexturedVertices[2]),&g_AnmProjectionContext->viewport,&g_AnmProjectionContext->projectionMatrix,&g_AnmProjectionContext->viewMatrix,&worldTransformMatrix);
    D3DXVec3Project(reinterpret_cast<D3DXVECTOR3 *>(&g_AnmRenderQuad[3]),reinterpret_cast<D3DXVECTOR3 *>(&anm->untexturedVertices[3]),&g_AnmProjectionContext->viewport,&g_AnmProjectionContext->projectionMatrix,&g_AnmProjectionContext->viewMatrix,&worldTransformMatrix);
    anm->cachedWorldMatrix=worldTransformMatrix;
}
