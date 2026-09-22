#include "Background.hpp"
#include "AnmManager.hpp"
#include "AnmMatrixRuntime.hpp"
#include "Supervisor.hpp"

#include <windows.h>
#include <d3d8.h>
#include <math.h>
#include <stddef.h>

typedef unsigned char u8;
typedef unsigned int u32;


struct Float2
{
    float x;
    float y;
};

struct Float3
{
    Float3() {}
    Float3(float x, float y, float z);
    Float3 operator+(const Float3 &other) const;
    Float3 operator-(const Float3 &other) const;
    Float3 *operator/=(float scalar);
    operator float *();

    float x;
    float y;
    float z;
};

typedef char BackgroundRenderFloat2SizeIs08[
    (sizeof(Float2) == 0x08) ? 1 : -1];
typedef char BackgroundRenderFloat3SizeIs0C[
    (sizeof(Float3) == 0x0C) ? 1 : -1];

struct D3DXVECTOR3 : public D3DVECTOR {};
struct D3DXMATRIX : public D3DMATRIX {};

D3DXVECTOR3 *WINAPI D3DXVec3Normalize(
    D3DXVECTOR3 *out, const D3DXVECTOR3 *vector);
D3DXVECTOR3 *WINAPI D3DXVec3Project(
    D3DXVECTOR3 *out,
    const D3DXVECTOR3 *vector,
    const D3DVIEWPORT8 *viewport,
    const D3DXMATRIX *projection,
    const D3DXMATRIX *view,
    const D3DXMATRIX *world);
float __fastcall D3DXVec3Length(const D3DXVECTOR3 *value);
float __fastcall D3DXVec3LengthSq(const D3DXVECTOR3 *value);
float __fastcall D3DXVec3Dot(
    const D3DXVECTOR3 *lhs, const D3DXVECTOR3 *rhs);

struct AnmProjectionContextView
{
    Float3 cameraPosition00;
    Float3 cameraLookAt0C;
    Float3 cameraUp18;
    Float3 cameraForward24;
    Float3 cameraRight30;
    Float3 cameraPositionOffset3C;
    float fieldOfView48;
    D3DXMATRIX viewMatrix4C;
    D3DXMATRIX projectionMatrix8C;
    D3DVIEWPORT8 viewportCC;
};
typedef char BackgroundRenderContextViewAt4C[
    (offsetof(AnmProjectionContextView, viewMatrix4C) == 0x4C) ? 1 : -1];
typedef char BackgroundRenderContextProjectionAt8C[
    (offsetof(AnmProjectionContextView, projectionMatrix8C) == 0x8C) ? 1 : -1];
typedef char BackgroundRenderContextViewportAtCC[
    (offsetof(AnmProjectionContextView, viewportCC) == 0xCC) ? 1 : -1];

extern AnmProjectionContextView *g_AnmProjectionContext;

struct BackgroundRenderSupervisorCameraView
{
    Float3 cameraPosition00;
    Float3 cameraLookAt0C;
    Float3 cameraUp18;
    Float3 cameraForward24;
    Float3 cameraRight30;
    Float3 cameraPositionOffset3C;
    float fieldOfView48;
    unsigned char unknown04C[0xF0 - 0x4C];
};
typedef char BackgroundRenderSupervisorCameraSizeIsF0[
    (sizeof(BackgroundRenderSupervisorCameraView) == 0xF0) ? 1 : -1];

struct BackgroundRenderSupervisorView
{
    unsigned char unknown000[0x78];
    BackgroundRenderSupervisorCameraView configurations78[3];
};
typedef char BackgroundRenderSupervisorConfigurationsAt78[
    (offsetof(BackgroundRenderSupervisorView, configurations78) == 0x78) ? 1 : -1];



struct BackgroundRenderAnmVmView
{
    u8 unknown000[0x18];
    Float2 scale18;
    u8 unknown020[0x08];
    Float2 spriteSize28;
    Float2 uvScrollPos30;
    u8 unknown038[0x1F0 - 0x38];
    u32 color1F0;
    u32 color2F4;
    u8 flags1F8[4];
    u8 renderMode1FC;
    u8 unknown1FD;
    short pendingInterrupt1FE;
    u8 unknown200[4];
    AnmLoaded *anmFile204;
    Float3 pos208;
    short activeSpriteIndex214;
    u8 unknown216[4];
    short scriptIndex21A;
    u8 unknown21C[4];
    void *currentInstruction220;
    AnmLoadedSprite *loadedSprite224;
    u8 unknown228[0x60];
    Float3 pos2_288;
    u8 unknown294[0x10];
};
typedef char BackgroundRenderAnmVmSizeIs2A4[
    (sizeof(BackgroundRenderAnmVmView) == 0x2A4) ? 1 : -1];
typedef char BackgroundRenderVmColorAt1F0[
    (offsetof(BackgroundRenderAnmVmView, color1F0) == 0x1F0) ? 1 : -1];
typedef char BackgroundRenderVmPosAt208[
    (offsetof(BackgroundRenderAnmVmView, pos208) == 0x208) ? 1 : -1];
typedef char BackgroundRenderVmSpriteAt224[
    (offsetof(BackgroundRenderAnmVmView, loadedSprite224) == 0x224) ? 1 : -1];
typedef char BackgroundRenderVmPos2At288[
    (offsetof(BackgroundRenderAnmVmView, pos2_288) == 0x288) ? 1 : -1];

static __forceinline BackgroundRenderAnmVmView *BackgroundVmView(AnmVm *vm)
{
    return reinterpret_cast<BackgroundRenderAnmVmView *>(vm);
}

struct BackgroundRenderLoadedSpriteView
{
    u8 unknown00[0x20];
    Float2 uvStart20;
    Float2 uvEnd28;
    float heightPx30;
    float widthPx34;
};
typedef char BackgroundRenderSpriteWidthAt34[
    (offsetof(BackgroundRenderLoadedSpriteView, widthPx34) == 0x34) ? 1 : -1];

union BackgroundRenderColor
{
    u32 value;
    struct
    {
        u8 b;
        u8 g;
        u8 r;
        u8 a;
    };
};

struct RawStageQuadType1
{
    short type;
    short byteSize;
    short anmScript;
    short vmIndex;
    BackgroundPoint position1;
    BackgroundPoint position2;
    float width;
};
typedef char BackgroundRenderQuad1SizeIs24[
    (sizeof(RawStageQuadType1) == 0x24) ? 1 : -1];

struct BackgroundStageVertex
{
    BackgroundStageVertex();
    Float3 pos;
    float w;
    BackgroundRenderColor diffuse;
    Float2 textureUV;
};
typedef char BackgroundStageVertexSizeIs1C[
    (sizeof(BackgroundStageVertex) == 0x1C) ? 1 : -1];

int Background::RenderObjects(int mode)
{
    RawStageQuadType1 *quadType1;
    BackgroundRenderAnmVmView *quadVm;
    RawStageObjectInstance *instance;
    int fogState;
    RawStageObject *object;
    float objectDistance;
    RawStageQuadBasic *quad;
    float radius;
    float quadWidth;
    BackgroundRenderColor originalColor;

    instance = this->stageObjectInstances;

    Float3 quadPosition;
    Float3 cameraVector;
    Float3 projectedSecond;
    Float3 projectSource(0.0f, 0.0f, 0.0f);
    AnmMatrixRuntime worldMatrix;

    fogState = 255;

    g_Supervisor.ApplyCameraMode1();
    g_AnmManager->SetCameraMode(1);
    worldMatrix.SetIdentity();

    cameraVector.x = g_AnmProjectionContext->viewMatrix4C._11;
    cameraVector.y = g_AnmProjectionContext->viewMatrix4C._12;
    cameraVector.z = g_AnmProjectionContext->viewMatrix4C._13;
    D3DXVec3Normalize(
        reinterpret_cast<D3DXVECTOR3 *>(&cameraVector),
        reinterpret_cast<D3DXVECTOR3 *>(&cameraVector));

    for (; instance->objectId >= 0; ++instance)
    {
        object = this->stageObjects[instance->objectId];
        if (object->zLevel == mode)
        {
            quad = &object->firstQuad;

            quadPosition.x =
                object->position.x + instance->position.x -
                this->stagePosition.x + object->size.x * 0.5f;
            quadPosition.y =
                object->position.y + instance->position.y -
                this->stagePosition.y + object->size.y * 0.5f;
            quadPosition.z =
                object->position.z + instance->position.z -
                this->stagePosition.z + object->size.z * 0.5f;

            quadPosition =
                quadPosition -
                (reinterpret_cast<BackgroundRenderSupervisorView *>(&g_Supervisor)->configurations78[0].cameraPosition00 +
                 reinterpret_cast<BackgroundRenderSupervisorView *>(&g_Supervisor)->configurations78[0].cameraPositionOffset3C);

            if (D3DXVec3LengthSq(
                    reinterpret_cast<D3DXVECTOR3 *>(&quadPosition)) >
                this->cullingDistanceSq)
            {
                continue;
            }

            objectDistance = D3DXVec3Dot(
                reinterpret_cast<D3DXVECTOR3 *>(&quadPosition),
                reinterpret_cast<D3DXVECTOR3 *>(
                    &reinterpret_cast<BackgroundRenderSupervisorView *>(&g_Supervisor)->configurations78[0].cameraForward24));
            radius =
                D3DXVec3Length(
                    reinterpret_cast<D3DXVECTOR3 *>(
                        &object->size)) *
                    0.5f +
                1280.0f;
            if (objectDistance > radius || objectDistance < 80.0f)
                continue;

            object->flags |= 2;

            for (; quad->type >= 0;
                 quad = reinterpret_cast<RawStageQuadBasic *>(
                     reinterpret_cast<u8 *>(quad) + quad->byteSize))
            {
                quadVm = reinterpret_cast<BackgroundRenderAnmVmView *>(
                    &this->stageObjectVms[quad->vmIndex]);

                switch (quad->type)
                {
                case 0:
                {
                    quadVm->pos208.x =
                        quadVm->pos2_288.x + quad->position.x +
                        instance->position.x - this->stagePosition.x;
                    quadVm->pos208.y =
                        quadVm->pos2_288.y + quad->position.y +
                        instance->position.y - this->stagePosition.y;
                    quadVm->pos208.z =
                        quadVm->pos2_288.z + quad->position.z +
                        instance->position.z - this->stagePosition.z;

                    if (quad->width != 0.0f)
                        quadVm->scale18.x = quad->width / reinterpret_cast<BackgroundRenderLoadedSpriteView *>(quadVm->loadedSprite224)->widthPx34;
                    if (quad->height != 0.0f)
                        quadVm->scale18.y = quad->height / reinterpret_cast<BackgroundRenderLoadedSpriteView *>(quadVm->loadedSprite224)->heightPx30;

                    if ((quadVm->renderMode1FC & 0x0F) == 2)
                    {
                        float *vmPosition = quadVm->pos208.operator float *();
                        worldMatrix.m[3][0] = vmPosition[0];
                        worldMatrix.m[3][1] = vmPosition[1];
                        worldMatrix.m[3][2] = vmPosition[2];

                        D3DXVec3Project(
                            reinterpret_cast<D3DXVECTOR3 *>(&quadPosition),
                            reinterpret_cast<D3DXVECTOR3 *>(&projectSource),
                            &g_AnmProjectionContext->viewportCC,
                            &g_AnmProjectionContext->projectionMatrix8C,
                            &g_AnmProjectionContext->viewMatrix4C,
                            reinterpret_cast<D3DXMATRIX *>(&worldMatrix));

                        if (quad->width != 0.0f)
                            quadWidth = quad->width;
                        else
                            quadWidth = reinterpret_cast<BackgroundRenderLoadedSpriteView *>(quadVm->loadedSprite224)->widthPx34;

                        worldMatrix.m[3][0] =
                            cameraVector.x * quadWidth * quadVm->scale18.x +
                            worldMatrix.m[3][0];
                        worldMatrix.m[3][1] =
                            cameraVector.y * quadWidth * quadVm->scale18.x +
                            worldMatrix.m[3][1];
                        worldMatrix.m[3][2] =
                            cameraVector.z * quadWidth * quadVm->scale18.x +
                            worldMatrix.m[3][2];

                        D3DXVec3Project(
                            reinterpret_cast<D3DXVECTOR3 *>(&projectedSecond),
                            reinterpret_cast<D3DXVECTOR3 *>(&projectSource),
                            &g_AnmProjectionContext->viewportCC,
                            &g_AnmProjectionContext->projectionMatrix8C,
                            &g_AnmProjectionContext->viewMatrix4C,
                            reinterpret_cast<D3DXMATRIX *>(&worldMatrix));

                        projectedSecond = projectedSecond - quadPosition;
                        quadVm->scale18.x =
                            D3DXVec3Length(
                                reinterpret_cast<D3DXVECTOR3 *>(
                                    &projectedSecond)) /
                            quadWidth;
                        quadVm->scale18.y = quadVm->scale18.x;
                        if (quadWidth < 0.0f)
                            quadVm->scale18.y = -quadVm->scale18.y;

                        projectedSecond =
                            quadVm->pos208 -
                            (g_AnmProjectionContext->cameraPosition00 +
                             g_AnmProjectionContext->cameraPositionOffset3C);
                        quadWidth = D3DXVec3Length(
                            reinterpret_cast<D3DXVECTOR3 *>(&projectedSecond));

                        originalColor.value = quadVm->color1F0;
                        if (this->skyFog.nearPlane < quadWidth)
                        {
                            quadWidth =
                                (this->skyFog.nearPlane - quadWidth) /
                                (this->skyFog.nearPlane -
                                 this->skyFog.farPlane);
                            if (quadWidth >= 1.0f)
                                continue;

                            BackgroundRenderColor &color =
                                *reinterpret_cast<BackgroundRenderColor *>(
                                    &quadVm->color1F0);
                            color.b = color.b -
                                static_cast<u8>(
                                    (color.b - (*reinterpret_cast<BackgroundRenderColor *>(&this->skyFog.color)).b) * quadWidth);
                            color.g = color.g -
                                static_cast<u8>(
                                    (color.g - (*reinterpret_cast<BackgroundRenderColor *>(&this->skyFog.color)).g) * quadWidth);
                            color.r = color.r -
                                static_cast<u8>(
                                    (color.r - (*reinterpret_cast<BackgroundRenderColor *>(&this->skyFog.color)).r) * quadWidth);
                            color.a = static_cast<u8>(
                                color.a * (1.0f - quadWidth));
                        }

                        quadVm->pos208 = quadPosition;
                        if (quadVm->pos208.z < 0.0f ||
                            quadVm->pos208.z > 1.0f)
                        {
                            goto restoreColor;
                        }

                        if (fogState != 0)
                        {
                            if (!g_Supervisor.IsFogDisabled())
                                g_Supervisor.DisableFog();
                            fogState = 0;
                        }

                        g_AnmManager->DrawNoRotationNoRound(reinterpret_cast<AnmVm *>(quadVm));
                        g_AnmManager->ClearTexture();

                        if ((quadVm->renderMode1FC & 0xF0) == 0x10 &&
                            this->skyFog.nearPlane > quadWidth &&
                            this->collectSpecialEffectPoints != 0)
                        {
                            this->specialEffectPoints[
                                this->specialEffectPointCount] =
                                    *reinterpret_cast<BackgroundPoint *>(
                                        &quadPosition);
                            this->specialEffectPoints[
                                this->specialEffectPointCount].z = 0.0f;
                            ++this->specialEffectPointCount;
                        }

                    restoreColor:
                        quadVm->color1F0 = originalColor.value;
                    }
                    else
                    {
                        if (!g_Supervisor.IsFogDisabled() && fogState != 1)
                        {
                            if (!g_Supervisor.IsFogDisabled())
                                g_Supervisor.EnableFog();
                            fogState = 1;
                        }
                        g_AnmManager->Draw3D(reinterpret_cast<AnmVm *>(quadVm));
                    }
                    break;
                }

                case 1:
                {
                    quadType1 =
                        reinterpret_cast<RawStageQuadType1 *>(quad);
                    Float3 type1World;
                    Float3 type1ProjectedSecond;
                    BackgroundStageVertex vertices[4];
                    float halfWidthFirst;
                    float halfWidthSecond;
                    float type1Width;

                    type1World.x =
                        quadType1->position1.x + instance->position.x -
                        this->stagePosition.x;
                    type1World.y =
                        quadType1->position1.y + instance->position.y -
                        this->stagePosition.y;
                    type1World.z =
                        quadType1->position1.z + instance->position.z -
                        this->stagePosition.z;

                    worldMatrix.m[3][0] = type1World.x;
                    worldMatrix.m[3][1] = type1World.y;
                    worldMatrix.m[3][2] = type1World.z;

                    D3DXVec3Project(
                        reinterpret_cast<D3DXVECTOR3 *>(&quadPosition),
                        reinterpret_cast<D3DXVECTOR3 *>(&projectSource),
                        &g_AnmProjectionContext->viewportCC,
                        &g_AnmProjectionContext->projectionMatrix8C,
                        &g_AnmProjectionContext->viewMatrix4C,
                        reinterpret_cast<D3DXMATRIX *>(&worldMatrix));

                    if (quadType1->width != 0.0f)
                        type1Width = quadType1->width;
                    else
                        type1Width =
                            reinterpret_cast<BackgroundRenderLoadedSpriteView *>(quadVm->loadedSprite224)->widthPx34;

                    worldMatrix.m[3][0] =
                        cameraVector.x * type1Width +
                        worldMatrix.m[3][0];
                    worldMatrix.m[3][1] =
                        cameraVector.y * type1Width +
                        worldMatrix.m[3][1];
                    worldMatrix.m[3][2] =
                        cameraVector.z * type1Width +
                        worldMatrix.m[3][2];

                    D3DXVec3Project(
                        reinterpret_cast<D3DXVECTOR3 *>(&projectedSecond),
                        reinterpret_cast<D3DXVECTOR3 *>(&projectSource),
                        &g_AnmProjectionContext->viewportCC,
                        &g_AnmProjectionContext->projectionMatrix8C,
                        &g_AnmProjectionContext->viewMatrix4C,
                        reinterpret_cast<D3DXMATRIX *>(&worldMatrix));

                    projectedSecond = projectedSecond - quadPosition;
                    halfWidthFirst =
                        D3DXVec3Length(
                            reinterpret_cast<D3DXVECTOR3 *>(&projectedSecond)) *
                        0.5f;

                    projectedSecond =
                        type1World -
                        (g_AnmProjectionContext->cameraPosition00 +
                         g_AnmProjectionContext->cameraPositionOffset3C);
                    type1Width = D3DXVec3Length(
                        reinterpret_cast<D3DXVECTOR3 *>(&projectedSecond));

                    if (this->skyFog.nearPlane < type1Width)
                    {
                        float fogRatio =
                            (this->skyFog.nearPlane - type1Width) /
                            (this->skyFog.nearPlane -
                             this->skyFog.farPlane);
                        if (fogRatio < 1.0f)
                        {
                            vertices[1].diffuse.b =
                                (*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).b - static_cast<u8>(
                                    ((*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).b - (*reinterpret_cast<BackgroundRenderColor *>(&this->skyFog.color)).b) * fogRatio);
                            vertices[0].diffuse.b =
                                vertices[1].diffuse.b;
                            vertices[1].diffuse.g =
                                (*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).g - static_cast<u8>(
                                    ((*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).g - (*reinterpret_cast<BackgroundRenderColor *>(&this->skyFog.color)).g) * fogRatio);
                            vertices[0].diffuse.g =
                                vertices[1].diffuse.g;
                            vertices[1].diffuse.r =
                                (*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).r - static_cast<u8>(
                                    ((*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).r - (*reinterpret_cast<BackgroundRenderColor *>(&this->skyFog.color)).r) * fogRatio);
                            vertices[0].diffuse.r =
                                vertices[1].diffuse.r;
                            vertices[1].diffuse.a = static_cast<u8>(
                                (*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).a * (1.0f - fogRatio));
                            vertices[0].diffuse.a =
                                vertices[1].diffuse.a;
                        }
                        else
                        {
                            vertices[1].diffuse.a = 0;
                            vertices[0].diffuse.a = 0;
                        }
                    }
                    else
                    {
                        vertices[1].diffuse.value = (*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).value;
                        vertices[0].diffuse.value = (*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).value;
                    }

                    type1World.x =
                        quadType1->position2.x + instance->position.x -
                        this->stagePosition.x;
                    type1World.y =
                        quadType1->position2.y + instance->position.y -
                        this->stagePosition.y;
                    type1World.z =
                        quadType1->position2.z + instance->position.z -
                        this->stagePosition.z;

                    worldMatrix.m[3][0] = type1World.x;
                    worldMatrix.m[3][1] = type1World.y;
                    worldMatrix.m[3][2] = type1World.z;

                    D3DXVec3Project(
                        reinterpret_cast<D3DXVECTOR3 *>(&type1ProjectedSecond),
                        reinterpret_cast<D3DXVECTOR3 *>(&projectSource),
                        &g_AnmProjectionContext->viewportCC,
                        &g_AnmProjectionContext->projectionMatrix8C,
                        &g_AnmProjectionContext->viewMatrix4C,
                        reinterpret_cast<D3DXMATRIX *>(&worldMatrix));

                    if (quadType1->width != 0.0f)
                        type1Width = quadType1->width;
                    else
                        type1Width =
                            reinterpret_cast<BackgroundRenderLoadedSpriteView *>(quadVm->loadedSprite224)->widthPx34;

                    worldMatrix.m[3][0] =
                        cameraVector.x * type1Width +
                        worldMatrix.m[3][0];
                    worldMatrix.m[3][1] =
                        cameraVector.y * type1Width +
                        worldMatrix.m[3][1];
                    worldMatrix.m[3][2] =
                        cameraVector.z * type1Width +
                        worldMatrix.m[3][2];

                    D3DXVec3Project(
                        reinterpret_cast<D3DXVECTOR3 *>(&projectedSecond),
                        reinterpret_cast<D3DXVECTOR3 *>(&projectSource),
                        &g_AnmProjectionContext->viewportCC,
                        &g_AnmProjectionContext->projectionMatrix8C,
                        &g_AnmProjectionContext->viewMatrix4C,
                        reinterpret_cast<D3DXMATRIX *>(&worldMatrix));

                    projectedSecond =
                        projectedSecond - type1ProjectedSecond;
                    halfWidthSecond =
                        D3DXVec3Length(
                            reinterpret_cast<D3DXVECTOR3 *>(
                                &projectedSecond)) *
                        0.5f;

                    projectedSecond =
                        type1World -
                        (g_AnmProjectionContext->cameraPosition00 +
                         g_AnmProjectionContext->cameraPositionOffset3C);
                    type1Width = D3DXVec3Length(
                        reinterpret_cast<D3DXVECTOR3 *>(
                            &projectedSecond));

                    if (this->skyFog.nearPlane < type1Width)
                    {
                        float fogRatio =
                            (this->skyFog.nearPlane - type1Width) /
                            (this->skyFog.nearPlane -
                             this->skyFog.farPlane);
                        if (fogRatio < 1.0f)
                        {
                            vertices[3].diffuse.b =
                                (*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).b - static_cast<u8>(
                                    ((*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).b - (*reinterpret_cast<BackgroundRenderColor *>(&this->skyFog.color)).b) * fogRatio);
                            vertices[2].diffuse.b =
                                vertices[3].diffuse.b;
                            vertices[3].diffuse.g =
                                (*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).g - static_cast<u8>(
                                    ((*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).g - (*reinterpret_cast<BackgroundRenderColor *>(&this->skyFog.color)).g) * fogRatio);
                            vertices[2].diffuse.g =
                                vertices[3].diffuse.g;
                            vertices[3].diffuse.r =
                                (*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).r - static_cast<u8>(
                                    ((*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).r - (*reinterpret_cast<BackgroundRenderColor *>(&this->skyFog.color)).r) * fogRatio);
                            vertices[2].diffuse.r =
                                vertices[3].diffuse.r;
                            vertices[3].diffuse.a = static_cast<u8>(
                                (*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).a * (1.0f - fogRatio));
                            vertices[2].diffuse.a =
                                vertices[3].diffuse.a;
                        }
                        else
                        {
                            vertices[3].diffuse.a = 0;
                            vertices[2].diffuse.a = 0;
                        }
                    }
                    else
                    {
                        vertices[3].diffuse.value = (*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).value;
                        vertices[2].diffuse.value = (*reinterpret_cast<BackgroundRenderColor *>(&quadVm->color1F0)).value;
                    }

                    projectSource = type1ProjectedSecond - quadPosition;
                    type1Width = sqrtf(
                        projectSource.x * projectSource.x +
                        projectSource.y * projectSource.y);
                    if (type1Width < 0.00001f)
                        continue;

                    projectSource /= type1Width;

                    if (quadPosition.z < 0.0f ||
                        quadPosition.z > 1.0f ||
                        type1ProjectedSecond.z < 0.0f ||
                        type1ProjectedSecond.z > 1.0f)
                        continue;

                    vertices[0].pos.x =
                        projectSource.y * halfWidthFirst +
                        quadPosition.x;
                    vertices[0].pos.y =
                        quadPosition.y -
                        projectSource.x * halfWidthFirst;
                    vertices[0].pos.z = quadPosition.z;
                    vertices[1].pos.x =
                        quadPosition.x -
                        projectSource.y * halfWidthFirst;
                    vertices[1].pos.y =
                        projectSource.x * halfWidthFirst +
                        quadPosition.y;
                    vertices[1].pos.z = quadPosition.z;
                    vertices[2].pos.x =
                        projectSource.y * halfWidthSecond +
                        type1ProjectedSecond.x;
                    vertices[2].pos.y =
                        type1ProjectedSecond.y -
                        projectSource.x * halfWidthSecond;
                    vertices[2].pos.z = type1ProjectedSecond.z;
                    vertices[3].pos.x =
                        type1ProjectedSecond.x -
                        projectSource.y * halfWidthSecond;
                    vertices[3].pos.y =
                        projectSource.x * halfWidthSecond +
                        type1ProjectedSecond.y;
                    vertices[3].pos.z = type1ProjectedSecond.z;

                    vertices[2].textureUV.x =
                        reinterpret_cast<BackgroundRenderLoadedSpriteView *>(quadVm->loadedSprite224)->uvStart20.x + quadVm->uvScrollPos30.x;
                    vertices[0].textureUV.x =
                        vertices[2].textureUV.x;
                    vertices[3].textureUV.x =
                        reinterpret_cast<BackgroundRenderLoadedSpriteView *>(quadVm->loadedSprite224)->uvEnd28.x + quadVm->uvScrollPos30.x;
                    vertices[1].textureUV.x =
                        vertices[3].textureUV.x;
                    vertices[1].textureUV.y =
                        reinterpret_cast<BackgroundRenderLoadedSpriteView *>(quadVm->loadedSprite224)->uvStart20.y + quadVm->uvScrollPos30.y;
                    vertices[0].textureUV.y =
                        vertices[1].textureUV.y;
                    vertices[3].textureUV.y =
                        reinterpret_cast<BackgroundRenderLoadedSpriteView *>(quadVm->loadedSprite224)->uvEnd28.y + quadVm->uvScrollPos30.y;
                    vertices[2].textureUV.y =
                        vertices[3].textureUV.y;
                    vertices[0].w =
                        vertices[1].w =
                        vertices[2].w =
                        vertices[3].w = 1.0f;

                    if (fogState != 0)
                    {
                        if (!g_Supervisor.IsFogDisabled())
                            g_Supervisor.DisableFog();
                        fogState = 0;
                    }

                    g_AnmManager->QueueSpriteQuad(
                        reinterpret_cast<AnmVm *>(quadVm),
                        reinterpret_cast<VertexTex1DiffuseXyzrhw *>(
                            vertices));
                    break;
                }

                default:
                    break;
                }

            }
        }
    }

    return 0;
}
