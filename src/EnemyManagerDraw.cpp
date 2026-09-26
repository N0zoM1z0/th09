#include "EnemyManager.hpp"
#include "AsciiManager.hpp"

#include <stddef.h>

// Target-facing rendering views for the TH09 EnemyManager draw seam.  These
// names describe observed roles; they do not claim original identifiers.

struct EnemyDrawSpriteView
{
    int anmIdx00;
    void *texture04;
    unsigned char unknown008[0x18];
    float uvStartX20;
    float uvStartY24;
    float uvEndX28;
    float uvEndY2C;
    float heightPx30;
    float widthPx34;
    unsigned char unknown038[0x0C];
};
typedef char EnemyDrawSpriteSizeIs44[(sizeof(EnemyDrawSpriteView) == 0x44) ? 1 : -1];

struct EnemyDrawVmView
{
    unsigned char unknown000[0x18];
    Float2 scale18;
    unsigned char unknown020[0x08];
    Float2 spriteSize28;
    Float2 uvScrollPos30;
    unsigned char unknown038[0x1F0 - 0x38];
    unsigned long color1F0;
    unsigned long color2F4;
    unsigned int flags1F8;
    short type1FC;
    short pendingInterrupt1FE;
    unsigned char unknown200[0x08];
    Float3 pos208;
    short activeSprite214;
    unsigned char unknown216[0x04];
    short scriptIndex21A;
    unsigned char unknown21C[0x08];
    EnemyDrawSpriteView *loadedSprite224;
    unsigned char unknown228[0x60];
    Float3 pos2_288;
    unsigned char unknown294[0x10];

    void SetZRotation(float angle);
};
typedef char EnemyDrawVmSizeIs2A4[(sizeof(EnemyDrawVmView) == 0x2A4) ? 1 : -1];
typedef char EnemyDrawVmTypeAt1FC[(offsetof(EnemyDrawVmView, type1FC) == 0x1FC) ? 1 : -1];
typedef char EnemyDrawVmPosAt208[(offsetof(EnemyDrawVmView, pos208) == 0x208) ? 1 : -1];
typedef char EnemyDrawVmSpriteAt224[(offsetof(EnemyDrawVmView, loadedSprite224) == 0x224) ? 1 : -1];
typedef char EnemyDrawVmPos2At288[(offsetof(EnemyDrawVmView, pos2_288) == 0x288) ? 1 : -1];

struct EnemyDrawVertex
{
    Float3 pos;
    float rhw;
    unsigned long diffuse;
    Float2 textureUV;
};
typedef char EnemyDrawVertexSizeIs1C[(sizeof(EnemyDrawVertex) == 0x1C) ? 1 : -1];

struct EnemyDrawTrailSampleView
{
    Float3 position00;
    Float3 velocity0C;
    float angle18;
};
typedef char EnemyDrawTrailSampleSizeIs1C[
    (sizeof(EnemyDrawTrailSampleView) == 0x1C) ? 1 : -1];

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
    unsigned char unknown3384[0x33E8 - 0x3384];
    EnemyDrawTrailSampleView trailSamples33E8[96];
    EnemyDrawVertex trailVertices3E68[192];
    unsigned char unknown5368[0x53A0 - 0x5368];
    unsigned char trailFlags53A0;
    unsigned char unknown53A1;
    short trailHistoryLength53A2;
    short trailCollisionLength53A4;
    short trailSampleStride53A6;
    unsigned char unknown53A8[0x5430 - 0x53A8];
};
typedef char EnemyDrawViewSizeIs5430[(sizeof(EnemyDrawView) == 0x5430) ? 1 : -1];
typedef char EnemyDrawSecondaryAt2AC[(offsetof(EnemyDrawView, secondaryVms2AC) == 0x2AC) ? 1 : -1];
typedef char EnemyDrawWorldAt2DD4[(offsetof(EnemyDrawView, worldPosition2DD4) == 0x2DD4) ? 1 : -1];
typedef char EnemyDrawFlagsAt337C[(offsetof(EnemyDrawView, flags337C) == 0x337C) ? 1 : -1];
typedef char EnemyDrawTrailAt33E8[(offsetof(EnemyDrawView, trailSamples33E8) == 0x33E8) ? 1 : -1];
typedef char EnemyDrawVerticesAt3E68[(offsetof(EnemyDrawView, trailVertices3E68) == 0x3E68) ? 1 : -1];
typedef char EnemyDrawTrailFlagsAt53A0[(offsetof(EnemyDrawView, trailFlags53A0) == 0x53A0) ? 1 : -1];

struct EnemyDrawAnmManagerView
{
    int Draw2D(EnemyDrawVmView *vm);
    int DrawVertices(EnemyDrawVmView *vm, EnemyDrawVertex *vertices, int vertexCount);
};

struct EnemyDrawGameManagerView
{
    float TransformPopupX(float value);
    float TransformPopupY(float value);
};

struct EnemyDrawSupervisorView
{
    int PrepareEnemyDraw(int sideIndex);
};

extern EnemyDrawAnmManagerView *g_AnmManager;
extern EnemyDrawGameManagerView g_GameManager;
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
                if (vm->scriptIndex21A >= 0)
                {
                    if (vm->type1FC != 0)
                        vm->SetZRotation(enemy->movementAngle2DE0);

                    if ((enemy->flags3380 & ENEMY_DRAW_SECONDARY_FIXED_OFFSET) == 0)
                        vm->pos208 = enemy->worldPosition2DD4 + vm->pos2_288;
                    else
                        vm->pos208 = enemy->worldPosition2DD4 + enemy->vm008.pos2_288;

                    vm->pos208.z = 0.3f;
                    vm->pos208.x = g_GameManager.TransformPopupX(vm->pos208.x);
                    vm->pos208.y = g_GameManager.TransformPopupY(vm->pos208.y);
                    g_AnmManager->Draw2D(vm);
                }
            }

            if ((enemy->flags337C & ENEMY_DRAW_ROTATE_WITH_MOVEMENT) != 0)
                enemy->vm008.SetZRotation(enemy->movementAngle2DE0);

            enemy->vm008.pos208 = enemy->worldPosition2DD4 + enemy->vm008.pos2_288;
            enemy->vm008.pos208.x = g_GameManager.TransformPopupX(enemy->vm008.pos208.x);
            enemy->vm008.pos208.y = g_GameManager.TransformPopupY(enemy->vm008.pos208.y);
            enemy->vm008.pos208.z = 0.25f;

            if (enemy->trailFlags53A0 != 0)
            {
                float savedScaleX = enemy->vm008.scale18.x;
                float savedScaleY = enemy->vm008.scale18.y;
                unsigned long savedColor = enemy->vm008.color1F0;

                if ((enemy->trailFlags53A0 & ENEMY_TRAIL_RENDER_AS_STRIP) == 0)
                {
                    for (int k = enemy->trailHistoryLength53A2 - 1; k > 0;
                         k -= enemy->trailSampleStride53A6)
                    {
                        if (enemy->trailSamples33E8[k].position00.x < -990.0f)
                            continue;

                        if ((enemy->flags337C & ENEMY_DRAW_ROTATE_WITH_MOVEMENT) != 0)
                            enemy->vm008.SetZRotation(
                                enemy->trailSamples33E8[k].angle18);

                        if ((enemy->trailFlags53A0 & ENEMY_TRAIL_TAPER) != 0)
                            enemy->vm008.scale18.x =
                                savedScaleX -
                                static_cast<float>(k) * savedScaleX /
                                    static_cast<float>(enemy->trailHistoryLength53A2);

                        if ((enemy->trailFlags53A0 & ENEMY_TRAIL_FADE) != 0)
                            reinterpret_cast<unsigned char *>(&enemy->vm008.color1F0)[3] =
                                reinterpret_cast<unsigned char *>(&savedColor)[3] -
                                reinterpret_cast<unsigned char *>(&savedColor)[3] * k /
                                    enemy->trailHistoryLength53A2;

                        enemy->vm008.pos208 =
                            enemy->trailSamples33E8[k].position00 + enemy->vm008.pos2_288;
                        enemy->vm008.pos208.z = 0.3f;
                        enemy->vm008.pos208.x =
                            g_GameManager.TransformPopupX(enemy->vm008.pos208.x);
                        enemy->vm008.pos208.y =
                            g_GameManager.TransformPopupY(enemy->vm008.pos208.y);
                        g_AnmManager->Draw2D(&enemy->vm008);
                    }
                }
                else
                {
                    int vertexCount = 0;
                    for (int k = 0; k < enemy->trailHistoryLength53A2;
                         k += enemy->trailSampleStride53A6)
                    {
                        if (enemy->trailSamples33E8[k].position00.x < -990.0f)
                            break;
                        vertexCount += 2;
                    }

                    if (vertexCount > 2)
                    {
                        EnemyDrawSpriteView *sprite = enemy->vm008.loadedSprite224;
                        float uvSpan = sprite->uvEndX28 - sprite->uvStartX20;
                        float uvStep = uvSpan / ((vertexCount + 1) / 2 - 1);
                        float uv = sprite->uvEndX28 + enemy->vm008.uvScrollPos30.x;
                        EnemyDrawVertex *vertices = enemy->trailVertices3E68;
                        float previousAngle;

                        for (int k = 0; k < enemy->trailHistoryLength53A2;
                             k += enemy->trailSampleStride53A6, uv -= uvStep)
                        {
                            if (enemy->trailSamples33E8[k].position00.x < -990.0f)
                                break;

                            float angle;
                            if (k == 0)
                            {
                                angle = enemy->trailSamples33E8[0].angle18;
                            }
                            else
                            {
                                angle = EnemyDrawInterpolateWrappedAngle(
                                    enemy->trailSamples33E8[k - 1].angle18,
                                    enemy->trailSamples33E8[k].angle18, 0.5f);
                            }

                            unsigned char taperFlag =
                                enemy->trailFlags53A0 & ENEMY_TRAIL_TAPER;
                            if (taperFlag != 0 &&
                                k > 0 &&
                                k + enemy->trailSampleStride53A6 <
                                    enemy->trailHistoryLength53A2)
                            {
                                float nextAngle = EnemyDrawInterpolateWrappedAngle(
                                    enemy->trailSamples33E8[
                                        k + enemy->trailSampleStride53A6 - 1].angle18,
                                    enemy->trailSamples33E8[
                                        enemy->trailSampleStride53A6].angle18,
                                    0.5f);
                                if (static_cast<float>(EnemyDrawAbs(previousAngle - angle)) <
                                        0.00001f &&
                                    static_cast<float>(EnemyDrawAbs(angle - nextAngle)) <
                                        0.00001f)
                                {
                                    vertexCount -= 2;
                                    continue;
                                }
                            }

                            previousAngle = angle;
                            float sinAngle = EnemyDrawSin(angle);
                            float cosAngle = EnemyDrawCos(angle);
                            float halfCenter = 0.0f;
                            float halfWidth =
                                savedScaleY *
                                enemy->vm008.loadedSprite224->heightPx30 * 0.5f;
                            if (taperFlag != 0)
                            {
                                float taper = 1.0f -
                                    static_cast<float>(k) /
                                        static_cast<float>(enemy->trailHistoryLength53A2);
                                halfCenter *= taper;
                                halfWidth *= taper;
                            }

                            vertices[1].diffuse = enemy->vm008.color1F0;
                            vertices[0].diffuse = vertices[1].diffuse;
                            if ((enemy->trailFlags53A0 & ENEMY_TRAIL_FADE) != 0)
                            {
                                reinterpret_cast<unsigned char *>(&vertices[1].diffuse)[3] =
                                    reinterpret_cast<unsigned char *>(&savedColor)[3] -
                                    reinterpret_cast<unsigned char *>(&savedColor)[3] * k /
                                        enemy->trailHistoryLength53A2;
                                reinterpret_cast<unsigned char *>(&vertices[0].diffuse)[3] =
                                    reinterpret_cast<unsigned char *>(&vertices[1].diffuse)[3];
                            }

                            vertices[0].pos = enemy->trailSamples33E8[k].position00;
                            vertices[0].pos.x +=
                                cosAngle * halfCenter - sinAngle * halfWidth + 32.0f;
                            vertices[0].pos.y +=
                                sinAngle * halfCenter + cosAngle * halfWidth + 16.0f;
                            vertices[0].textureUV.x = uv;
                            vertices[0].textureUV.y =
                                enemy->vm008.loadedSprite224->uvStartY24 +
                                enemy->vm008.uvScrollPos30.y;
                            ++vertices;

                            vertices[0].pos = enemy->trailSamples33E8[k].position00;
                            vertices[0].pos.x +=
                                cosAngle * halfCenter + sinAngle * halfWidth + 32.0f;
                            vertices[0].pos.y +=
                                sinAngle * halfCenter - cosAngle * halfWidth + 16.0f;
                            vertices[0].textureUV.x = uv;
                            vertices[0].textureUV.y =
                                enemy->vm008.loadedSprite224->uvEndY2C +
                                enemy->vm008.uvScrollPos30.y;
                            ++vertices;
                        }

                        if (vertexCount > 2)
                            g_AnmManager->DrawVertices(
                                &enemy->vm008,
                                enemy->trailVertices3E68,
                                vertexCount);
                    }
                }

                enemy->vm008.scale18.x = savedScaleX;
                enemy->vm008.scale18.y = savedScaleY;
                enemy->vm008.color1F0 = savedColor;
            }

            if ((enemy->trailFlags53A0 & ENEMY_TRAIL_HIDE_HEAD) == 0 &&
                (enemy->flags337C & ENEMY_DRAW_HIDE_PRIMARY) == 0)
            {
                g_AnmManager->Draw2D(&enemy->vm008);
            }

            for (int k = 1; k < 2; ++k, ++vm)
            {
                if (vm->scriptIndex21A >= 0)
                {
                    if (vm->type1FC != 0)
                        vm->SetZRotation(-enemy->movementAngle2DE0);

                    if ((enemy->flags3380 & ENEMY_DRAW_SECONDARY_FIXED_OFFSET) == 0)
                        vm->pos208 = enemy->worldPosition2DD4 + vm->pos2_288;
                    else
                        vm->pos208 = enemy->worldPosition2DD4 + enemy->vm008.pos2_288;

                    vm->pos208.z = 0.3f;
                    vm->pos208.x = g_GameManager.TransformPopupX(vm->pos208.x);
                    vm->pos208.y = g_GameManager.TransformPopupY(vm->pos208.y);
                    g_AnmManager->Draw2D(vm);
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
