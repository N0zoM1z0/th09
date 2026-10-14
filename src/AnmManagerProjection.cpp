#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include <stddef.h>
#include <math.h>
#include <windows.h>
#include <d3d8.h>
#include "AnmCameraProjectionViews.hpp"
struct VertexTex1DiffuseXyzrhw { float x,y,z,rhw; unsigned long diffuse; float u,v; };
struct VertexDiffuseXyzrhw { float x,y,z,rhw; unsigned long diffuse; };
typedef char VertexProjectionSourceSizeIs14[(sizeof(VertexDiffuseXyzrhw)==0x14)?1:-1];
extern VertexTex1DiffuseXyzrhw g_AnmRenderQuad[4];
struct AnmManagerProjectionView {
    unsigned char unknown00000[0x12024]; D3DXMATRIX cachedWorldMatrix; unsigned char unknown12064[0x830]; VertexDiffuseXyzrhw untexturedVertices[4];
};
typedef char AnmManagerCachedWorldAt12024[(offsetof(AnmManagerProjectionView,cachedWorldMatrix)==0x12024)?1:-1];
typedef char AnmManagerUntexturedAt12894[(offsetof(AnmManagerProjectionView,untexturedVertices)==0x12894)?1:-1];

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
