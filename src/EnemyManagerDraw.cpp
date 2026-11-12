#include "EnemyNativeTrailRenderCapacity.hpp"
// The real wrapped-angle helper is visible here because TH09 retains ECX
// across its call while constructing the trail strip. Its body remains single.
#include "EnemyDrawMath.inl"

#include "EnemyManager.hpp"
#include "AnmManager.hpp"
#include "Supervisor.hpp"
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

// The native array constructs 194 vertices; drawing consumes only the
// first 192 (two for each of 96 samples), all with 0x1C-byte records.
typedef EnemyTrailVertexView EnemyDrawVertex;
typedef char EnemyDrawVertexSizeIs1C[(sizeof(EnemyTrailVertexView) == 0x1C) ? 1 : -1];
typedef char EnemyDrawNativeVertexLayoutAt3E68[
    (offsetof(EnemyView, trailVertices3E68) == 0x3E68 &&
     sizeof(((EnemyView *)0)->trailVertices3E68) == 194 * 0x1C &&
     offsetof(EnemyTrailVertexView, position00) == 0x00 &&
     offsetof(EnemyTrailVertexView, rhw0C) == 0x0C &&
     offsetof(EnemyTrailVertexView, diffuse10) == 0x10 &&
     offsetof(EnemyTrailVertexView, u14) == 0x14 &&
     offsetof(EnemyTrailVertexView, v18) == 0x18) ? 1 : -1];

typedef char EnemyDrawTrailSampleSizeIs1C[
    (sizeof(EnemyTrailSampleView) == 0x1C) ? 1 : -1];

// Source alias only: draw groups contain genuine 0x5430-byte EnemyView
// objects already owned by the exact ECL, Enemy calc and post-ECL callbacks.
typedef EnemyView EnemyDrawView;
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

// Both native draw-chain callbacks call the independently exact 71-byte
// Supervisor::ConfigureScreenViewport, not a standalone Enemy-only method.
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

// Native 0x1C-byte Enemy trail records use a signed index scaled as a
// Win32 unsigned DWORD from the real EnemyView allocation, without a guard.
static inline const EnemyTrailSampleView &EnemyDrawTrailSampleNative(
    const EnemyDrawView *enemy, int index)
{
    return *reinterpret_cast<const EnemyTrailSampleView *>(
        reinterpret_cast<unsigned int>(&enemy->trailSamples33E8[0]) +
        static_cast<unsigned int>(index) * sizeof(enemy->trailSamples33E8[0]));
}

// The fade alpha is byte three of the *real* native 32-bit color DWORD,
// both in the live VM and in its independently saved/vertex color words.
// Form that original Win32 byte address without a fake unsigned-char array.
#define TH09_ENEMY_NATIVE_ALPHA(word) \
    (*reinterpret_cast<unsigned char *>( \
        reinterpret_cast<unsigned int>(&(word)) + 3u))

// The draw groups and secondary ANM VMs are genuine EnemyManager-owned
// arrays; both trail vertex records and ANM VMs use their physical
// i386 DWORD record widths, rather than unrelated C++ subarray bounds.
static int EnemyManagerDrawImpl(
    EnemyManagerView *enemyManager, int drawGroup, int chainPriority)
{
    for (int group = drawGroup; group < chainPriority;
         ++*reinterpret_cast<unsigned int *>(&group))
    {
        EnemyDrawView *enemy = *reinterpret_cast<EnemyDrawView **>(
            reinterpret_cast<unsigned int>(&enemyManager->drawGroupHeads2AC410[0]) +
            static_cast<unsigned int>(group) *
                sizeof(enemyManager->drawGroupHeads2AC410[0]));
        while (enemy != NULL)
        {
            EnemyDrawVmView *vm = &enemy->secondaryVms2AC[0];
            for (int k = 0; k < 1; ++k,
                 vm = reinterpret_cast<EnemyDrawVmView *>(
                     reinterpret_cast<unsigned int>(vm) + sizeof(*vm)))
            {
                if (vm->scriptIndex >= 0)
                {
                    if (vm->type != 0)
                        vm->SetZRotation(enemy->movementAngle2DE0);

                    if ((enemy->secondaryFlags3380 & ENEMY_DRAW_SECONDARY_FIXED_OFFSET) == 0)
                        vm->pos = reinterpret_cast<const Float3 &>(enemy->worldPosition2DD4) + vm->pos2;
                    else
                        vm->pos = reinterpret_cast<const Float3 &>(enemy->worldPosition2DD4) + enemy->primaryAnmVm0008.pos2;

                    vm->pos.z = 0.3f;
                    vm->pos.x = g_GameManager.TransformPopupX(vm->pos.x);
                    vm->pos.y = g_GameManager.TransformPopupY(vm->pos.y);
                    g_AnmManager->Draw2D(reinterpret_cast<AnmVm *>(vm));
                }
            }

            if ((enemy->flags337C & ENEMY_DRAW_ROTATE_WITH_MOVEMENT) != 0)
                enemy->primaryAnmVm0008.SetZRotation(enemy->movementAngle2DE0);

            enemy->primaryAnmVm0008.pos = reinterpret_cast<const Float3 &>(enemy->worldPosition2DD4) + enemy->primaryAnmVm0008.pos2;
            enemy->primaryAnmVm0008.pos.x = g_GameManager.TransformPopupX(enemy->primaryAnmVm0008.pos.x);
            enemy->primaryAnmVm0008.pos.y = g_GameManager.TransformPopupY(enemy->primaryAnmVm0008.pos.y);
            enemy->primaryAnmVm0008.pos.z = 0.25f;

            if (enemy->trailFlags53A0 != 0)
            {
                int k;
                // The two scale components are one snapshot of the actual Float2 field.
                Float2 savedScale = enemy->primaryAnmVm0008.scale;
                unsigned long savedColor = enemy->primaryAnmVm0008.color1;

                if ((enemy->trailFlags53A0 & ENEMY_TRAIL_RENDER_AS_STRIP) == 0)
                {
                    for (k = enemy->trailHistoryLength53A2 - 1; k > 0;
                         k = static_cast<int>(
                             static_cast<unsigned int>(k) -
                             static_cast<unsigned int>(enemy->trailSampleStride53A6)))
                    {
                        if (EnemyDrawTrailSampleNative(enemy, k).position00.x < -990.0f)
                            continue;

                        if ((enemy->flags337C & ENEMY_DRAW_ROTATE_WITH_MOVEMENT) != 0)
                            enemy->primaryAnmVm0008.SetZRotation(
                                EnemyDrawTrailSampleNative(enemy, k).angle18);

                        if ((enemy->trailFlags53A0 & ENEMY_TRAIL_TAPER) != 0)
                            enemy->primaryAnmVm0008.scale.x =
                                savedScale.x -
                                static_cast<float>(k) * savedScale.x /
                                    static_cast<float>(enemy->trailHistoryLength53A2);

                        if ((enemy->trailFlags53A0 & ENEMY_TRAIL_FADE) != 0)
                            TH09_ENEMY_NATIVE_ALPHA(enemy->primaryAnmVm0008.color1) =
                                TH09_ENEMY_NATIVE_ALPHA(savedColor) -
                                TH09_ENEMY_NATIVE_ALPHA(savedColor) * k /
                                    enemy->trailHistoryLength53A2;

                        enemy->primaryAnmVm0008.pos =
                            EnemyDrawTrailSampleNative(enemy, k).position00 + enemy->primaryAnmVm0008.pos2;
                        enemy->primaryAnmVm0008.pos.z = 0.3f;
                        enemy->primaryAnmVm0008.pos.x =
                            g_GameManager.TransformPopupX(enemy->primaryAnmVm0008.pos.x);
                        enemy->primaryAnmVm0008.pos.y =
                            g_GameManager.TransformPopupY(enemy->primaryAnmVm0008.pos.y);
                        g_AnmManager->Draw2D(reinterpret_cast<AnmVm *>(&enemy->primaryAnmVm0008));
                    }
                }
                else
                {
                    int vertexCount = 0;
                    for (k = 0; k < enemy->trailHistoryLength53A2;
                         k = static_cast<int>(
                             static_cast<unsigned int>(k) +
                             static_cast<unsigned int>(enemy->trailSampleStride53A6)))
                    {
                        if (EnemyDrawTrailSampleNative(enemy, k).position00.x < -990.0f)
                            break;
                        vertexCount = static_cast<int>(
                            static_cast<unsigned int>(vertexCount) +
                            static_cast<unsigned int>(
                                EnemyNativeTrailRenderCapacity::VerticesPerSample));
                    }

                    if (vertexCount > EnemyNativeTrailRenderCapacity::VerticesPerSample)
                    {
                        AnmLoadedSprite *sprite = enemy->primaryAnmVm0008.loadedSprite;
                        float uvSpan = sprite->uvEndX - sprite->uvStartX;
                        float uvStep = uvSpan / (static_cast<int>(static_cast<unsigned int>(vertexCount) + 1u) /
                            EnemyNativeTrailRenderCapacity::VerticesPerSample - 1);
                        float uv = sprite->uvEndX + enemy->primaryAnmVm0008.uvScrollPos.x;
                        EnemyDrawVertex *vertices = enemy->trailVertices3E68;
                        float previousAngle;

                        for (k = 0; k < enemy->trailHistoryLength53A2;
                             k = static_cast<int>(
                                 static_cast<unsigned int>(k) +
                                 static_cast<unsigned int>(enemy->trailSampleStride53A6)),
                             uv -= uvStep)
                        {
                            if (EnemyDrawTrailSampleNative(enemy, k).position00.x < -990.0f)
                                break;

                            float angle;
                            if (k == 0)
                            {
                                angle = enemy->trailSamples33E8[0].angle18;
                            }
                            else
                            {
                                angle = EnemyDrawInterpolateWrappedAngle(
                                    EnemyDrawTrailSampleNative(
                                        enemy, static_cast<int>(
                                            static_cast<unsigned int>(k) - 1u)).angle18,
                                    EnemyDrawTrailSampleNative(enemy, k).angle18, 0.5f);
                            }

                            unsigned char taperFlag =
                                enemy->trailFlags53A0 & ENEMY_TRAIL_TAPER;
                            if (taperFlag != 0 && k > 0)
                            {
                                // Both indices survive the first absolute-value call in TH09.
                                int sampleStride = enemy->trailSampleStride53A6;
                                int nextSample = static_cast<int>(
                                    static_cast<unsigned int>(k) +
                                    static_cast<unsigned int>(sampleStride));
                                if (nextSample < enemy->trailHistoryLength53A2)
                                {
                                    // Preserve strict short-circuit sampling, including NaNs.
                                    if (static_cast<float>(EnemyDrawAbs(previousAngle - angle)) <
                                            0.00001f)
                                    {
                                        // Preserve the Float3/angle helper's float return,
                                        // then the distinct long-double difference lifetime.
                                        const float interpolated =
                                            EnemyDrawInterpolateWrappedAngle(
                                                EnemyDrawTrailSampleNative(
                                                    enemy, static_cast<int>(
                                                        static_cast<unsigned int>(nextSample) - 1u)).angle18,
                                                EnemyDrawTrailSampleNative(enemy, sampleStride).angle18,
                                                0.5f);
                                        long double delta = angle - interpolated;
                                        if (static_cast<float>(EnemyDrawAbs(
                                                static_cast<float>(delta))) < 0.00001f)
                                        {
                                            vertexCount = static_cast<int>(
                                                static_cast<unsigned int>(vertexCount) -
                                                static_cast<unsigned int>(
                                                    EnemyNativeTrailRenderCapacity::VerticesPerSample));
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
                                enemy->primaryAnmVm0008.loadedSprite->heightPx * 0.5f;
                            if (taperFlag != 0)
                            {
                                float taper = 1.0f -
                                    static_cast<float>(k) /
                                        static_cast<float>(enemy->trailHistoryLength53A2);
                                halfCenter *= taper;
                                halfWidth *= taper;
                            }

                            vertices[1].diffuse10 = enemy->primaryAnmVm0008.color1;
                            vertices[0].diffuse10 = vertices[1].diffuse10;
                            if ((enemy->trailFlags53A0 & ENEMY_TRAIL_FADE) != 0)
                            {
                                TH09_ENEMY_NATIVE_ALPHA(vertices[1].diffuse10) =
                                    TH09_ENEMY_NATIVE_ALPHA(savedColor) -
                                    TH09_ENEMY_NATIVE_ALPHA(savedColor) * k /
                                        enemy->trailHistoryLength53A2;
                                TH09_ENEMY_NATIVE_ALPHA(vertices[0].diffuse10) =
                                    TH09_ENEMY_NATIVE_ALPHA(vertices[1].diffuse10);
                            }

                            vertices[0].position00 = EnemyDrawTrailSampleNative(enemy, k).position00;
                            vertices[0].position00.x +=
                                cosAngle * halfCenter - sinAngle * halfWidth + 32.0f;
                            vertices[0].position00.y +=
                                sinAngle * halfCenter + cosAngle * halfWidth + 16.0f;
                            vertices[0].u14 = uv;
                            vertices[0].v18 =
                                enemy->primaryAnmVm0008.loadedSprite->uvStartY +
                                enemy->primaryAnmVm0008.uvScrollPos.y;
                            vertices = reinterpret_cast<EnemyDrawVertex *>(
                                reinterpret_cast<unsigned int>(vertices) +
                                sizeof(*vertices));

                            vertices[0].position00 = EnemyDrawTrailSampleNative(enemy, k).position00;
                            vertices[0].position00.x +=
                                cosAngle * halfCenter + sinAngle * halfWidth + 32.0f;
                            vertices[0].position00.y +=
                                sinAngle * halfCenter - cosAngle * halfWidth + 16.0f;
                            vertices[0].u14 = uv;
                            vertices[0].v18 =
                                enemy->primaryAnmVm0008.loadedSprite->uvEndY +
                                enemy->primaryAnmVm0008.uvScrollPos.y;
                            vertices = reinterpret_cast<EnemyDrawVertex *>(
                                reinterpret_cast<unsigned int>(vertices) +
                                sizeof(*vertices));
                        }

                        if (vertexCount > EnemyNativeTrailRenderCapacity::VerticesPerSample)
                            g_AnmManager->DrawVertices(
                                reinterpret_cast<AnmVm *>(&enemy->primaryAnmVm0008),
                                reinterpret_cast<VertexTex1DiffuseXyzrhw *>(
                                    enemy->trailVertices3E68),
                                vertexCount);
                    }
                }

                enemy->primaryAnmVm0008.scale.x = savedScale.x;
                enemy->primaryAnmVm0008.scale.y = savedScale.y;
                enemy->primaryAnmVm0008.color1 = savedColor;
            }

            if ((enemy->trailFlags53A0 & ENEMY_TRAIL_HIDE_HEAD) == 0 &&
                (enemy->flags337C & ENEMY_DRAW_HIDE_PRIMARY) == 0)
            {
                g_AnmManager->Draw2D(reinterpret_cast<AnmVm *>(&enemy->primaryAnmVm0008));
            }

            for (int k = 1; k < EnemyNativeTrailRenderCapacity::SecondaryVmCount;
                 ++k,
                 vm = reinterpret_cast<EnemyDrawVmView *>(
                     reinterpret_cast<unsigned int>(vm) + sizeof(*vm)))
            {
                if (vm->scriptIndex >= 0)
                {
                    if (vm->type != 0)
                        vm->SetZRotation(-enemy->movementAngle2DE0);

                    if ((enemy->secondaryFlags3380 & ENEMY_DRAW_SECONDARY_FIXED_OFFSET) == 0)
                        vm->pos = reinterpret_cast<const Float3 &>(enemy->worldPosition2DD4) + vm->pos2;
                    else
                        vm->pos = reinterpret_cast<const Float3 &>(enemy->worldPosition2DD4) + enemy->primaryAnmVm0008.pos2;

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
    g_Supervisor.ConfigureScreenViewport(enemyManager->sideIndex31C);
    return EnemyManagerDrawImpl(enemyManager, 0, EnemyNativeTrailRenderCapacity::DrawGroupCount / 2);
}

int __fastcall EnemyManagerView::OnDrawLowPrio(EnemyManagerView *enemyManager)
{
    g_Supervisor.ConfigureScreenViewport(enemyManager->sideIndex31C);
    return EnemyManagerDrawImpl(enemyManager, EnemyNativeTrailRenderCapacity::DrawGroupCount / 2, EnemyNativeTrailRenderCapacity::DrawGroupCount);
}

#undef TH09_ENEMY_NATIVE_ALPHA
