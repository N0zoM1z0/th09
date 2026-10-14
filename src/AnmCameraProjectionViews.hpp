#ifndef TH09_ANM_CAMERA_PROJECTION_VIEWS_HPP
#define TH09_ANM_CAMERA_PROJECTION_VIEWS_HPP

#include "AnmManager.hpp"
#include <stddef.h>
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
#endif // TH09_ANM_CAMERA_PROJECTION_VIEWS_HPP
