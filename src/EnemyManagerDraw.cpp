// The real wrapped-angle helper is visible here because TH09 retains ECX
// across its call while constructing the trail strip. Its body remains single.
#include "EnemyDrawMath.inl"

#include "EnemyManager.hpp"
#include "AnmManager.hpp"
#include "AsciiManager.hpp"

#include <stddef.h>

// Target-facing rendering views for the TH09 EnemyManager draw seam.  These
// names describe observed roles; they do not claim original identifiers.

// TH09's exact ANM loader and SetSprite leaf own these original UV
// coordinates and physical sprite dimensions, not a second draw-only object.
typedef char EnemyDrawNativeLoadedSpriteAt0x44[
    (sizeof(AnmLoadedSprite) == 0x44 &&
     offsetof(AnmLoadedSprite, uvStartX) == 0x20 &&
     offsetof(AnmLoadedSprite, uvStartY) == 0x24 &&
     offsetof(AnmLoadedSprite, uvEndX) == 0x28 &&
     offsetof(AnmLoadedSprite, uvEndY) == 0x2C &&
     offsetof(AnmLoadedSprite, heightPx) == 0x30 &&
     offsetof(AnmLoadedSprite, widthPx) == 0x34) ? 1 : -1];

// The draw VMs are the very same 0x2A4-byte ANM records used by the
// script executor, Player, Enemy calc and exact ANM rasterizer. Keep the
// lexical type alias only where it helps describe the surrounding Enemy
// projection; it owns no separate byte layout or method declaration.
typedef AnmVm EnemyDrawVmView;
typedef char EnemyDrawVmSizeIs2A4[(sizeof(EnemyDrawVmView) == 0x2A4) ? 1 : -1];
typedef char EnemyDrawVmNativeBaseState[
    (offsetof(AnmVmBase, scale) == 0x18 &&
     offsetof(AnmVmBase, uvScrollPos) == 0x30 &&
     offsetof(AnmVmBase, color1) == 0x1F0 &&
     offsetof(AnmVmBase, flagsWord) == 0x1F8) ? 1 : -1];
typedef char EnemyDrawVmTypeAt1FC[(offsetof(AnmVmBase, type) == 0x1FC) ? 1 : -1];
typedef char EnemyDrawVmPosAt208[(offsetof(AnmVmBase, pos) == 0x208) ? 1 : -1];
typedef char EnemyDrawVmSpriteAt224[(offsetof(AnmVmBase, loadedSprite) == 0x224) ? 1 : -1];
typedef char EnemyDrawVmPos2At288[(offsetof(AnmVm, pos2) == 0x288) ? 1 : -1];

struct EnemyDrawVertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
    Float2 textureUV;
};
typedef char EnemyDrawVertexSizeIs1C[(sizeof(EnemyDrawVertex) == 0x1C) ? 1 : -1];
// Both caller ABI layouts use the same physical 192-record vertex buffer.
// Preserve the DrawVertices parameter class spelling while sourcing storage
// exclusively from the native EnemyView rather than a second 0x1500 array.
typedef char EnemyDrawNativeVertexLayoutAt3E68[
    (offsetof(EnemyView, trailVertices3E68) == 0x3E68 &&
     sizeof(EnemyTrailVertexView) == sizeof(EnemyDrawVertex) &&
     offsetof(EnemyTrailVertexView, position00) == offsetof(EnemyDrawVertex, pos) &&
     offsetof(EnemyTrailVertexView, rhw0C) == offsetof(EnemyDrawVertex, rhw) &&
     offsetof(EnemyTrailVertexView, diffuse10) == offsetof(EnemyDrawVertex, diffuse) &&
     offsetof(EnemyTrailVertexView, u14) == offsetof(EnemyDrawVertex, textureUV) &&
     offsetof(EnemyTrailVertexView, v18) ==
         offsetof(EnemyDrawVertex, textureUV) + sizeof(float)) ? 1 : -1];

typedef char EnemyDrawTrailSampleSizeIs1C[
    (sizeof(EnemyTrailSampleView) == 0x1C) ? 1 : -1];

struct EnemyDrawView
{
    EnemyManagerView *manager00;
    EnemyDrawView *nextInDrawGroup04;
    EnemyDrawVmView vm008;
    EnemyDrawVmView secondaryVms2AC[2];
    unsigned char unknown7F4[0x2DD4 - 0x7F4];
    Float3 worldPosition2DD4;
    float movementAngle2DE0;
    unsigned char unknown2DE4[0x337C - 0x2DE4];
    unsigned int flags337C;
    unsigned int flags3380;
    // ECL and Enemy calc own trail history, strip vertices and timers on
    // the native EnemyView; this draw-only legacy VM overlay stays opaque.
    unsigned char unknown3384[0x5430 - 0x3384];
};
typedef char EnemyDrawViewSizeIs5430[(sizeof(EnemyDrawView) == 0x5430) ? 1 : -1];
typedef char EnemyDrawSecondaryAt2AC[(offsetof(EnemyDrawView, secondaryVms2AC) == 0x2AC) ? 1 : -1];
typedef char EnemyDrawWorldAt2DD4[(offsetof(EnemyDrawView, worldPosition2DD4) == 0x2DD4) ? 1 : -1];
typedef char EnemyDrawFlagsAt337C[(offsetof(EnemyDrawView, flags337C) == 0x337C) ? 1 : -1];
typedef char EnemyDrawTrailAt33E8[(offsetof(EnemyView, trailSamples33E8) == 0x33E8) ? 1 : -1];
typedef char EnemyDrawVerticesAt3E68[(offsetof(EnemyView, trailVertices3E68) == 0x3E68) ? 1 : -1];
typedef char EnemyDrawTrailFlagsAt53A0[(offsetof(EnemyView, trailFlags53A0) == 0x53A0) ? 1 : -1];

// All ANM rendering methods in this 1,758-byte owner use the exact
// native AnmManager receiver and its true Draw2D/DrawVertices ABI.
// EnemyDrawVmView is now a name-only alias of AnmVm; native callee calls
// retain explicit pointer views at the original sites without changing code.
// Enemy rendering uses the same native GameManager instance and exact
// popup coordinate helpers as the bullet and Effect renderers.
typedef char EnemyDrawNativeGameManagerCoordinates[
    (offsetof(AsciiGameManagerView, coordinateOffsetX) == 0x350 &&
     offsetof(AsciiGameManagerView, coordinateOffsetY) == 0x354) ? 1 : -1];

struct EnemyDrawSupervisorView
{
    int PrepareEnemyDraw(int sideIndex);
};

extern EnemyDrawSupervisorView g_Supervisor;

// Independent target helpers.  Source ownership/names remain review.
extern long double __stdcall EnemyDrawAbs(float value);
extern float __stdcall EnemyDrawCos(float angle);
extern float __stdcall EnemyDrawSin(float angle);
extern float __stdcall EnemyDrawInterpolateWrappedAngle(
    float angle1, float angle2, float factor);

enum EnemyDrawFlags
{
    ENEMY_DRAW_HIDE_PRIMARY = 0x00000020,
    ENEMY_DRAW_ROTATE_WITH_MOVEMENT = 0x00400000,
    ENEMY_DRAW_SECONDARY_FIXED_OFFSET = 0x00000020
};

enum EnemyTrailRenderFlags
{
    ENEMY_TRAIL_TAPER = 0x02,
    ENEMY_TRAIL_FADE = 0x04,
    ENEMY_TRAIL_RENDER_AS_STRIP = 0x08,
    ENEMY_TRAIL_HIDE_HEAD = 0x10
};

static int EnemyManagerDrawImpl(
    EnemyManagerView *enemyManager, int drawGroup, int chainPriority)
{
    for (int group = drawGroup; group < chainPriority; ++group)
    {
        EnemyDrawView *enemy = reinterpret_cast<EnemyDrawView *>(
            enemyManager->drawGroupHeads2AC410[group]);
        while (enemy != NULL)
        {
            EnemyDrawVmView *vm = &enemy->secondaryVms2AC[0];
            for (int k = 0; k < 1; ++k, ++vm)
            {
                if (vm->scriptIndex >= 0)
                {
                    if (vm->type != 0)
                        vm->SetZRotation(enemy->movementAngle2DE0);

                    if ((enemy->flags3380 & ENEMY_DRAW_SECONDARY_FIXED_OFFSET) == 0)
                        vm->pos = enemy->worldPosition2DD4 + vm->pos2;
                    else
                        vm->pos = enemy->worldPosition2DD4 + enemy->vm008.pos2;

                    vm->pos.z = 0.3f;
                    vm->pos.x = g_GameManager.TransformPopupX(vm->pos.x);
                    vm->pos.y = g_GameManager.TransformPopupY(vm->pos.y);
                    g_AnmManager->Draw2D(reinterpret_cast<AnmVm *>(vm));
                }
            }

            if ((enemy->flags337C & ENEMY_DRAW_ROTATE_WITH_MOVEMENT) != 0)
                enemy->vm008.SetZRotation(enemy->movementAngle2DE0);

            enemy->vm008.pos = enemy->worldPosition2DD4 + enemy->vm008.pos2;
            enemy->vm008.pos.x = g_GameManager.TransformPopupX(enemy->vm008.pos.x);
            enemy->vm008.pos.y = g_GameManager.TransformPopupY(enemy->vm008.pos.y);
            enemy->vm008.pos.z = 0.25f;

            if (reinterpret_cast<EnemyView *>(enemy)->trailFlags53A0 != 0)
            {
                int k;
                // The two scale components are one snapshot of the actual Float2 field.
                Float2 savedScale = enemy->vm008.scale;
                unsigned long savedColor = enemy->vm008.color1;

                if ((reinterpret_cast<EnemyView *>(enemy)->trailFlags53A0 & ENEMY_TRAIL_RENDER_AS_STRIP) == 0)
                {
                    for (k = reinterpret_cast<EnemyView *>(enemy)->trailHistoryLength53A2 - 1; k > 0;
                         k -= reinterpret_cast<EnemyView *>(enemy)->trailSampleStride53A6)
                    {
                        if (reinterpret_cast<EnemyView *>(enemy)->trailSamples33E8[k].position00.x < -990.0f)
                            continue;

                        if ((enemy->flags337C & ENEMY_DRAW_ROTATE_WITH_MOVEMENT) != 0)
                            enemy->vm008.SetZRotation(
                                reinterpret_cast<EnemyView *>(enemy)->trailSamples33E8[k].angle18);

                        if ((reinterpret_cast<EnemyView *>(enemy)->trailFlags53A0 & ENEMY_TRAIL_TAPER) != 0)
                            enemy->vm008.scale.x =
                                savedScale.x -
                                static_cast<float>(k) * savedScale.x /
                                    static_cast<float>(reinterpret_cast<EnemyView *>(enemy)->trailHistoryLength53A2);

                        if ((reinterpret_cast<EnemyView *>(enemy)->trailFlags53A0 & ENEMY_TRAIL_FADE) != 0)
                            reinterpret_cast<unsigned char *>(&enemy->vm008.color1)[3] =
                                reinterpret_cast<unsigned char *>(&savedColor)[3] -
                                reinterpret_cast<unsigned char *>(&savedColor)[3] * k /
                                    reinterpret_cast<EnemyView *>(enemy)->trailHistoryLength53A2;

                        enemy->vm008.pos =
                            reinterpret_cast<EnemyView *>(enemy)->trailSamples33E8[k].position00 + enemy->vm008.pos2;
                        enemy->vm008.pos.z = 0.3f;
                        enemy->vm008.pos.x =
                            g_GameManager.TransformPopupX(enemy->vm008.pos.x);
                        enemy->vm008.pos.y =
                            g_GameManager.TransformPopupY(enemy->vm008.pos.y);
                        g_AnmManager->Draw2D(reinterpret_cast<AnmVm *>(&enemy->vm008));
                    }
                }
                else
                {
                    int vertexCount = 0;
                    for (k = 0; k < reinterpret_cast<EnemyView *>(enemy)->trailHistoryLength53A2;
                         k += reinterpret_cast<EnemyView *>(enemy)->trailSampleStride53A6)
                    {
                        if (reinterpret_cast<EnemyView *>(enemy)->trailSamples33E8[k].position00.x < -990.0f)
                            break;
                        vertexCount += 2;
                    }

                    if (vertexCount > 2)
                    {
                        AnmLoadedSprite *sprite = enemy->vm008.loadedSprite;
                        float uvSpan = sprite->uvEndX - sprite->uvStartX;
                        float uvStep = uvSpan / ((vertexCount + 1) / 2 - 1);
                        float uv = sprite->uvEndX + enemy->vm008.uvScrollPos.x;
                        EnemyDrawVertex *vertices = reinterpret_cast<EnemyDrawVertex *>(
                            reinterpret_cast<EnemyView *>(enemy)->trailVertices3E68);
                        float previousAngle;

                        for (k = 0; k < reinterpret_cast<EnemyView *>(enemy)->trailHistoryLength53A2;
                             k += reinterpret_cast<EnemyView *>(enemy)->trailSampleStride53A6, uv -= uvStep)
                        {
                            if (reinterpret_cast<EnemyView *>(enemy)->trailSamples33E8[k].position00.x < -990.0f)
                                break;

                            float angle;
                            if (k == 0)
                            {
                                angle = reinterpret_cast<EnemyView *>(enemy)->trailSamples33E8[0].angle18;
                            }
                            else
                            {
                                angle = EnemyDrawInterpolateWrappedAngle(
                                    reinterpret_cast<EnemyView *>(enemy)->trailSamples33E8[k - 1].angle18,
                                    reinterpret_cast<EnemyView *>(enemy)->trailSamples33E8[k].angle18, 0.5f);
                            }

                            unsigned char taperFlag =
                                reinterpret_cast<EnemyView *>(enemy)->trailFlags53A0 & ENEMY_TRAIL_TAPER;
                            if (taperFlag != 0 && k > 0)
                            {
                                // Both indices survive the first absolute-value call in TH09.
                                int sampleStride = reinterpret_cast<EnemyView *>(enemy)->trailSampleStride53A6;
                                int nextSample = k + sampleStride;
                                if (nextSample < reinterpret_cast<EnemyView *>(enemy)->trailHistoryLength53A2)
                                {
                                    // Preserve strict short-circuit sampling, including NaNs.
                                    if (static_cast<float>(EnemyDrawAbs(previousAngle - angle)) <
                                            0.00001f)
                                    {
                                        // Preserve the Float3/angle helper's float return,
                                        // then the distinct long-double difference lifetime.
                                        const float interpolated =
                                            EnemyDrawInterpolateWrappedAngle(
                                                reinterpret_cast<EnemyView *>(enemy)->trailSamples33E8[nextSample - 1].angle18,
                                                reinterpret_cast<EnemyView *>(enemy)->trailSamples33E8[sampleStride].angle18,
                                                0.5f);
                                        long double delta = angle - interpolated;
                                        if (static_cast<float>(EnemyDrawAbs(
                                                static_cast<float>(delta))) < 0.00001f)
                                        {
                                            vertexCount -= 2;
                                            continue;
                                        }
                                    }
                                }
                            }

                            previousAngle = angle;
                            float sinAngle = EnemyDrawSin(angle);
                            float cosAngle = EnemyDrawCos(angle);
                            float halfCenter = 0.0f;
                            float halfWidth =
                                savedScale.y *
                                enemy->vm008.loadedSprite->heightPx * 0.5f;
                            if (taperFlag != 0)
                            {
                                float taper = 1.0f -
                                    static_cast<float>(k) /
                                        static_cast<float>(reinterpret_cast<EnemyView *>(enemy)->trailHistoryLength53A2);
                                halfCenter *= taper;
                                halfWidth *= taper;
                            }

                            vertices[1].diffuse = enemy->vm008.color1;
                            vertices[0].diffuse = vertices[1].diffuse;
                            if ((reinterpret_cast<EnemyView *>(enemy)->trailFlags53A0 & ENEMY_TRAIL_FADE) != 0)
                            {
                                reinterpret_cast<unsigned char *>(&vertices[1].diffuse)[3] =
                                    reinterpret_cast<unsigned char *>(&savedColor)[3] -
                                    reinterpret_cast<unsigned char *>(&savedColor)[3] * k /
                                        reinterpret_cast<EnemyView *>(enemy)->trailHistoryLength53A2;
                                reinterpret_cast<unsigned char *>(&vertices[0].diffuse)[3] =
                                    reinterpret_cast<unsigned char *>(&vertices[1].diffuse)[3];
                            }

                            vertices[0].pos = reinterpret_cast<EnemyView *>(enemy)->trailSamples33E8[k].position00;
                            vertices[0].pos.x +=
                                cosAngle * halfCenter - sinAngle * halfWidth + 32.0f;
                            vertices[0].pos.y +=
                                sinAngle * halfCenter + cosAngle * halfWidth + 16.0f;
                            vertices[0].textureUV.x = uv;
                            vertices[0].textureUV.y =
                                enemy->vm008.loadedSprite->uvStartY +
                                enemy->vm008.uvScrollPos.y;
                            ++vertices;

                            vertices[0].pos = reinterpret_cast<EnemyView *>(enemy)->trailSamples33E8[k].position00;
                            vertices[0].pos.x +=
                                cosAngle * halfCenter + sinAngle * halfWidth + 32.0f;
                            vertices[0].pos.y +=
                                sinAngle * halfCenter - cosAngle * halfWidth + 16.0f;
                            vertices[0].textureUV.x = uv;
                            vertices[0].textureUV.y =
                                enemy->vm008.loadedSprite->uvEndY +
                                enemy->vm008.uvScrollPos.y;
                            ++vertices;
                        }

                        if (vertexCount > 2)
                            g_AnmManager->DrawVertices(
                                reinterpret_cast<AnmVm *>(&enemy->vm008),
                                reinterpret_cast<VertexTex1DiffuseXyzrhw *>(
                                    reinterpret_cast<EnemyView *>(enemy)->trailVertices3E68),
                                vertexCount);
                    }
                }

                enemy->vm008.scale.x = savedScale.x;
                enemy->vm008.scale.y = savedScale.y;
                enemy->vm008.color1 = savedColor;
            }

            if ((reinterpret_cast<EnemyView *>(enemy)->trailFlags53A0 & ENEMY_TRAIL_HIDE_HEAD) == 0 &&
                (enemy->flags337C & ENEMY_DRAW_HIDE_PRIMARY) == 0)
            {
                g_AnmManager->Draw2D(reinterpret_cast<AnmVm *>(&enemy->vm008));
            }

            for (int k = 1; k < 2; ++k, ++vm)
            {
                if (vm->scriptIndex >= 0)
                {
                    if (vm->type != 0)
                        vm->SetZRotation(-enemy->movementAngle2DE0);

                    if ((enemy->flags3380 & ENEMY_DRAW_SECONDARY_FIXED_OFFSET) == 0)
                        vm->pos = enemy->worldPosition2DD4 + vm->pos2;
                    else
                        vm->pos = enemy->worldPosition2DD4 + enemy->vm008.pos2;

                    vm->pos.z = 0.3f;
                    vm->pos.x = g_GameManager.TransformPopupX(vm->pos.x);
                    vm->pos.y = g_GameManager.TransformPopupY(vm->pos.y);
                    g_AnmManager->Draw2D(reinterpret_cast<AnmVm *>(vm));
                }
            }

            enemy = enemy->nextInDrawGroup04;
        }
    }

    return 1;
}

int __fastcall EnemyManagerView::OnDrawHighPrio(EnemyManagerView *enemyManager)
{
    g_Supervisor.PrepareEnemyDraw(enemyManager->sideIndex31C);
    return EnemyManagerDrawImpl(enemyManager, 0, 2);
}

int __fastcall EnemyManagerView::OnDrawLowPrio(EnemyManagerView *enemyManager)
{
    g_Supervisor.PrepareEnemyDraw(enemyManager->sideIndex31C);
    return EnemyManagerDrawImpl(enemyManager, 2, 4);
}
