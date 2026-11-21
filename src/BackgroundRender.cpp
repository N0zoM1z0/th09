#include "Background.hpp"
#include "AnmManager.hpp"
#include "AnmMatrixRuntime.hpp"
#include "Supervisor.hpp"
#include "Float3.hpp"

#include <windows.h>
#include <d3d8.h>
#include <d3dx8math.h>
#include <math.h>
#include <stddef.h>

typedef unsigned char u8;
typedef unsigned int u32;


typedef char BackgroundRenderFloat2SizeIs08[
    (sizeof(Float2) == 0x08) ? 1 : -1];
typedef char BackgroundRenderFloat3SizeIs0C[
    (sizeof(Float3) == 0x0C) ? 1 : -1];


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

// Use the constructed Supervisor's real 0xF0-byte viewport array.
typedef char BackgroundRenderNativeSupervisorCameraAt78[
    (offsetof(Supervisor, ctorArray078) == 0x78 &&
     offsetof(SupervisorCtorArrayElement, cameraPosition00) == 0x00 &&
     offsetof(SupervisorCtorArrayElement, cameraForward24) == 0x24 &&
     offsetof(SupervisorCtorArrayElement, cameraPositionOffset3C) == 0x3C)
         ? 1 : -1];

// Borrow the true 0x2A4-byte ANM VM already constructed by ANM.
typedef char BackgroundRenderNativeVmFields[
    (sizeof(AnmVm) == 0x2A4 &&
     offsetof(AnmVmBase, scale) == 0x18 &&
     offsetof(AnmVmBase, spriteSize) == 0x28 &&
     offsetof(AnmVmBase, uvScrollPos) == 0x30 &&
     offsetof(AnmVmBase, color1) == 0x1F0 &&
     offsetof(AnmVmBase, unknown_01FC) == 0x1FC &&
     offsetof(AnmVmBase, pos) == 0x208 &&
     offsetof(AnmVmBase, loadedSprite) == 0x224 &&
     offsetof(AnmVm, pos2) == 0x288) ? 1 : -1];

// The loader and renderer consume the real 0x44-byte sprite.
typedef char BackgroundRenderNativeSpritePixelAndUvOffsets[
    (sizeof(AnmLoadedSprite) == 0x44 &&
     offsetof(AnmLoadedSprite, uvStartX) == 0x20 &&
     offsetof(AnmLoadedSprite, uvStartY) == 0x24 &&
     offsetof(AnmLoadedSprite, uvEndX) == 0x28 &&
     offsetof(AnmLoadedSprite, uvEndY) == 0x2C &&
     offsetof(AnmLoadedSprite, heightPx) == 0x30 &&
     offsetof(AnmLoadedSprite, widthPx) == 0x34) ? 1 : -1];

// Stage fog and ANM VM colors are the original packed 32-bit words.
// Their BGRA byte lanes are accessed through the same packed word storage.
// Keep each original VM/fog channel read and write in its native order.


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

// The four on-wire color components are the exact bytes of one DWORD.
typedef char BackgroundRenderNativeColorWord[
    (sizeof(BackgroundRenderColor) == sizeof(u32) &&
     offsetof(BackgroundRenderColor, b) == 0 &&
     offsetof(BackgroundRenderColor, g) == 1 &&
     offsetof(BackgroundRenderColor, r) == 2 &&
     offsetof(BackgroundRenderColor, a) == 3) ? 1 : -1];

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
    AnmVm *quadVm;
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

    // Stage instances form a real file-loaded array terminated by a
    // negative object ID. Advance the native record pointer directly.
    for (; instance->objectId >= 0;
         ++instance)
    {
        // The nonnegative original object ID selects the native object table.
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
                (g_Supervisor.ctorArray078[0].cameraPosition00 +
                 g_Supervisor.ctorArray078[0].cameraPositionOffset3C);

            if (D3DXVec3LengthSq(
                    reinterpret_cast<D3DXVECTOR3 *>(&quadPosition)) >
                this->cullingDistanceSq)
            {
                continue;
            }

            objectDistance = D3DXVec3Dot(
                reinterpret_cast<D3DXVECTOR3 *>(&quadPosition),
                reinterpret_cast<D3DXVECTOR3 *>(
                    &g_Supervisor.ctorArray078[0].cameraForward24));
            radius =
                D3DXVec3Length(
                    reinterpret_cast<D3DXVECTOR3 *>(
                        &object->size)) *
                    0.5f +
                1280.0f;
            if (objectDistance > radius || objectDistance < 80.0f)
                continue;

            object->flags |= 2;

            // Stage quads are variable-length wire records. Advance their
            // true byte pointer by the signed 16-bit encoded size.
            for (; quad->type >= 0;
                 quad = reinterpret_cast<RawStageQuadBasic *>(reinterpret_cast<unsigned char *>(quad) + quad->byteSize))
            {
                // Select the original 0x2A4-byte ANM VM by its wire ID.
                quadVm = &this->stageObjectVms[quad->vmIndex];

                switch (quad->type)
                {
                case 0:
                {
                    quadVm->pos.x =
                        quadVm->pos2.x + quad->position.x +
                        instance->position.x - this->stagePosition.x;
                    quadVm->pos.y =
                        quadVm->pos2.y + quad->position.y +
                        instance->position.y - this->stagePosition.y;
                    quadVm->pos.z =
                        quadVm->pos2.z + quad->position.z +
                        instance->position.z - this->stagePosition.z;

                    if (quad->width != 0.0f)
                        quadVm->scale.x = quad->width / quadVm->loadedSprite->widthPx;
                    if (quad->height != 0.0f)
                        quadVm->scale.y = quad->height / quadVm->loadedSprite->heightPx;

                    if ((quadVm->unknown_01FC[0] & 0x0F) == 2)
                    {
                        // The source still executes the native Float3
                        // conversion but reads actual position members,
                        // not an out-of-subobject float[3] projection.
                        float *vmPositionWords = quadVm->pos.operator float *();
                        const Float3 &vmPosition =
                            *reinterpret_cast<const Float3 *>(vmPositionWords);
                        worldMatrix.m[3][0] = vmPosition.x;
                        worldMatrix.m[3][1] = vmPosition.y;
                        worldMatrix.m[3][2] = vmPosition.z;

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
                            quadWidth = quadVm->loadedSprite->widthPx;

                        worldMatrix.m[3][0] =
                            cameraVector.x * quadWidth * quadVm->scale.x +
                            worldMatrix.m[3][0];
                        worldMatrix.m[3][1] =
                            cameraVector.y * quadWidth * quadVm->scale.x +
                            worldMatrix.m[3][1];
                        worldMatrix.m[3][2] =
                            cameraVector.z * quadWidth * quadVm->scale.x +
                            worldMatrix.m[3][2];

                        D3DXVec3Project(
                            reinterpret_cast<D3DXVECTOR3 *>(&projectedSecond),
                            reinterpret_cast<D3DXVECTOR3 *>(&projectSource),
                            &g_AnmProjectionContext->viewportCC,
                            &g_AnmProjectionContext->projectionMatrix8C,
                            &g_AnmProjectionContext->viewMatrix4C,
                            reinterpret_cast<D3DXMATRIX *>(&worldMatrix));

                        projectedSecond = projectedSecond - quadPosition;
                        quadVm->scale.x =
                            D3DXVec3Length(
                                reinterpret_cast<D3DXVECTOR3 *>(
                                    &projectedSecond)) /
                            quadWidth;
                        quadVm->scale.y = quadVm->scale.x;
                        if (quadWidth < 0.0f)
                            quadVm->scale.y = -quadVm->scale.y;

                        projectedSecond =
                            quadVm->pos -
                            (g_AnmProjectionContext->cameraPosition00 +
                             g_AnmProjectionContext->cameraPositionOffset3C);
                        quadWidth = D3DXVec3Length(
                            reinterpret_cast<D3DXVECTOR3 *>(&projectedSecond));

                        originalColor.value = quadVm->color1;
                        if (this->skyFog.nearPlane < quadWidth)
                        {
                            quadWidth =
                                (this->skyFog.nearPlane - quadWidth) /
                                (this->skyFog.nearPlane -
                                 this->skyFog.farPlane);
                            if (quadWidth >= 1.0f)
                                continue;

                            // Mutate the actual VM color word's four byte lanes
                            // without treating its DWORD as a separate union.
                            BackgroundRenderColor &color = reinterpret_cast<BackgroundRenderColor &>(quadVm->color1);
                            color.b = color.b -
                                static_cast<u8>(
                                    (color.b - reinterpret_cast<const BackgroundRenderColor &>(this->skyFog.color).b) * quadWidth);
                            color.g = color.g -
                                static_cast<u8>(
                                    (color.g - reinterpret_cast<const BackgroundRenderColor &>(this->skyFog.color).g) * quadWidth);
                            color.r = color.r -
                                static_cast<u8>(
                                    (color.r - reinterpret_cast<const BackgroundRenderColor &>(this->skyFog.color).r) * quadWidth);
                            color.a = static_cast<u8>(
                                color.a * (1.0f - quadWidth));
                        }

                        quadVm->pos = quadPosition;
                        if (quadVm->pos.z < 0.0f ||
                            quadVm->pos.z > 1.0f)
                        {
                            goto restoreColor;
                        }

                        if (fogState != 0)
                        {
                            if (!g_Supervisor.IsFogDisabled())
                                g_Supervisor.DisableFog();
                            fogState = 0;
                        }

                        g_AnmManager->DrawNoRotationNoRound(quadVm);
                        g_AnmManager->ClearTexture();

                        if ((quadVm->unknown_01FC[0] & 0xF0) == 0x10 &&
                            this->skyFog.nearPlane > quadWidth &&
                            this->collectSpecialEffectPoints != 0)
                        {
                            this->specialEffectPoints[this->specialEffectPointCount].point =
                                    *reinterpret_cast<BackgroundPoint *>(
                                        &quadPosition);
                            this->specialEffectPoints[this->specialEffectPointCount].point.z = 0.0f;
                            ++this->specialEffectPointCount;
                        }

                    restoreColor:
                        quadVm->color1 = originalColor.value;
                    }
                    else
                    {
                        if (!g_Supervisor.IsFogDisabled() && fogState != 1)
                        {
                            if (!g_Supervisor.IsFogDisabled())
                                g_Supervisor.EnableFog();
                            fogState = 1;
                        }
                        g_AnmManager->Draw3D(quadVm);
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
                            quadVm->loadedSprite->widthPx;

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
                                reinterpret_cast<const BackgroundRenderColor &>(quadVm->color1).b - static_cast<u8>(
                                    (reinterpret_cast<const BackgroundRenderColor &>(quadVm->color1).b - reinterpret_cast<const BackgroundRenderColor &>(this->skyFog.color).b) * fogRatio);
                            vertices[0].diffuse.b =
                                vertices[1].diffuse.b;
                            vertices[1].diffuse.g =
                                reinterpret_cast<const BackgroundRenderColor &>(quadVm->color1).g - static_cast<u8>(
                                    (reinterpret_cast<const BackgroundRenderColor &>(quadVm->color1).g - reinterpret_cast<const BackgroundRenderColor &>(this->skyFog.color).g) * fogRatio);
                            vertices[0].diffuse.g =
                                vertices[1].diffuse.g;
                            vertices[1].diffuse.r =
                                reinterpret_cast<const BackgroundRenderColor &>(quadVm->color1).r - static_cast<u8>(
                                    (reinterpret_cast<const BackgroundRenderColor &>(quadVm->color1).r - reinterpret_cast<const BackgroundRenderColor &>(this->skyFog.color).r) * fogRatio);
                            vertices[0].diffuse.r =
                                vertices[1].diffuse.r;
                            vertices[1].diffuse.a = static_cast<u8>(
                                reinterpret_cast<const BackgroundRenderColor &>(quadVm->color1).a * (1.0f - fogRatio));
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
                        vertices[1].diffuse.value = quadVm->color1;
                        vertices[0].diffuse.value = quadVm->color1;
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
                            quadVm->loadedSprite->widthPx;

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
                                reinterpret_cast<const BackgroundRenderColor &>(quadVm->color1).b - static_cast<u8>(
                                    (reinterpret_cast<const BackgroundRenderColor &>(quadVm->color1).b - reinterpret_cast<const BackgroundRenderColor &>(this->skyFog.color).b) * fogRatio);
                            vertices[2].diffuse.b =
                                vertices[3].diffuse.b;
                            vertices[3].diffuse.g =
                                reinterpret_cast<const BackgroundRenderColor &>(quadVm->color1).g - static_cast<u8>(
                                    (reinterpret_cast<const BackgroundRenderColor &>(quadVm->color1).g - reinterpret_cast<const BackgroundRenderColor &>(this->skyFog.color).g) * fogRatio);
                            vertices[2].diffuse.g =
                                vertices[3].diffuse.g;
                            vertices[3].diffuse.r =
                                reinterpret_cast<const BackgroundRenderColor &>(quadVm->color1).r - static_cast<u8>(
                                    (reinterpret_cast<const BackgroundRenderColor &>(quadVm->color1).r - reinterpret_cast<const BackgroundRenderColor &>(this->skyFog.color).r) * fogRatio);
                            vertices[2].diffuse.r =
                                vertices[3].diffuse.r;
                            vertices[3].diffuse.a = static_cast<u8>(
                                reinterpret_cast<const BackgroundRenderColor &>(quadVm->color1).a * (1.0f - fogRatio));
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
                        vertices[3].diffuse.value = quadVm->color1;
                        vertices[2].diffuse.value = quadVm->color1;
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
                        quadVm->loadedSprite->uvStartX + quadVm->uvScrollPos.x;
                    vertices[0].textureUV.x =
                        vertices[2].textureUV.x;
                    vertices[3].textureUV.x =
                        quadVm->loadedSprite->uvEndX + quadVm->uvScrollPos.x;
                    vertices[1].textureUV.x =
                        vertices[3].textureUV.x;
                    vertices[1].textureUV.y =
                        quadVm->loadedSprite->uvStartY + quadVm->uvScrollPos.y;
                    vertices[0].textureUV.y =
                        vertices[1].textureUV.y;
                    vertices[3].textureUV.y =
                        quadVm->loadedSprite->uvEndY + quadVm->uvScrollPos.y;
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
                        quadVm,
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
