// SPDX-License-Identifier: GPL-3.0-only
// OnDraw adapted from Cirno-Labs/th09 d7653152; original MIT portions retain
// their grant in LICENSE. See THIRD_PARTY_NOTICES.md.
#include "BulletManager.hpp"
#include "AnmManager.hpp"
#include "EtamaAnmResourceSlot.hpp"
#include "EffectManager.hpp"
#include "RuntimeAngleMath.hpp"
#include "AsciiGameManagerView.hpp"
#include "Supervisor.hpp"

#include <stddef.h>
#include <d3dx8math.h>

// The native VM position consumed by the renderer begins at Laser+0x208.
typedef char EtamaDrawBodyPositionOffsetIs208[
    (offsetof(Laser, bodyVm) + offsetof(AnmVm, pos) == 0x208) ? 1 : -1];

extern void __fastcall fsincos(float *sine, float *cosine, float angle);

// SDK conversions use the same checked three-float storage and this pointer.
typedef char EtamaSdkVectorLayoutMatchesFloat3[
    (sizeof(D3DXVECTOR3) == sizeof(Float3) && sizeof(Float3) == 12 &&
     offsetof(D3DXVECTOR3, x) == offsetof(Float3, x) &&
     offsetof(D3DXVECTOR3, y) == offsetof(Float3, y) &&
     offsetof(D3DXVECTOR3, z) == offsetof(Float3, z)) ? 1 : -1];

static int DrawSingleBullet(Bullet *bullet)
{
    AnmVm *vm;
    switch (bullet->state)
    {
    case 2:
        vm = &bullet->sprites.spawnFastVm;
        break;
    case 3:
        vm = &bullet->sprites.spawnNormalVm;
        break;
    case 4:
        vm = &bullet->sprites.spawnSlowVm;
        break;
    case 5:
        vm = &bullet->sprites.despawnVm;
        break;
    default:
        vm = &bullet->sprites.bulletVm;
        break;
    }

    // Keep both exact SDK operator float*() calls, but access the genuine
    // three-float vectors rather than indexing past a float subobject.
    D3DXVECTOR3 *position = reinterpret_cast<D3DXVECTOR3 *>(
        reinterpret_cast<D3DXVECTOR3 &>(bullet->position).operator float *());
    D3DXVECTOR3 *vmPosition = reinterpret_cast<D3DXVECTOR3 *>(
        reinterpret_cast<D3DXVECTOR3 &>(vm->pos).operator float *());
    vmPosition->x = g_GameManager.TransformPopupX(position->x);
    vmPosition->y = g_GameManager.TransformPopupY(position->y);
    vmPosition->z = 0.05f;
    vm->color1 |= 0x00FFFFFFu;

    if (*reinterpret_cast<short *>(vm->unknown_01FC) != 0)
        vm->SetZRotation(AddNormalizeAngle(1.5707964f + bullet->angle, 0.0f));

    return g_AnmManager->Draw2D(vm);
}

int EtamaController::OnDraw(EtamaController *mgr)
{
    int i;
    float s;
    float ca;
    Laser *laser;
    float len;
    float scale;
    Bullet *bullet;

    g_Supervisor.ConfigureGameplayViewport(mgr->sideIndex);
    laser = mgr->lasers;
    for (i = 0x30; i != 0; i--, laser++)
    {
        if (!laser->inUse)
        {
            continue;
        }
        fsincos(&s, &ca, laser->angle);
        len = (laser->endOffset - laser->startOffset) / 2.0f + laser->startOffset;
        reinterpret_cast<D3DXVECTOR3 &>(laser->bodyVm.pos)[0] = g_GameManager.TransformPopupX(len * ca + reinterpret_cast<D3DXVECTOR3 &>(laser->position)[0]);
        reinterpret_cast<D3DXVECTOR3 &>(laser->bodyVm.pos)[1] = g_GameManager.TransformPopupY(len * s + reinterpret_cast<D3DXVECTOR3 &>(laser->position)[1]);
        reinterpret_cast<D3DXVECTOR3 &>(laser->bodyVm.pos)[2] = 0.07f;
        laser->color = laser->color & 0xff000000 | 0xffffff;
        g_AnmManager->Draw2D(&laser->bodyVm);
        if ((laser->startOffset < 16.0f || laser->speed == 0.0f) && (laser->hideCapDuringStartup == 0 || laser->state != 0))
        {
            reinterpret_cast<D3DXVECTOR3 &>(laser->startCapVm.pos)[0] = g_GameManager.TransformPopupX(ca * laser->startOffset + reinterpret_cast<D3DXVECTOR3 &>(laser->position)[0]);
            reinterpret_cast<D3DXVECTOR3 &>(laser->startCapVm.pos)[1] = g_GameManager.TransformPopupY(s * laser->startOffset + reinterpret_cast<D3DXVECTOR3 &>(laser->position)[1]);
            reinterpret_cast<D3DXVECTOR3 &>(laser->startCapVm.pos)[2] = 0.05f;
            laser->startCapVm.color1 = laser->bodyVm.color1;
            laser->startCapVm.flag6 = 1;
            laser->startCapVm.color1 = laser->startCapVm.color1 & 0xffffff | 0xff000000;
            scale = laser->width / 10.0f;
            laser->startCapVm.scale.y = laser->startCapVm.scale.x = scale * ((16.0f - laser->startOffset) / 16.0f);
            if (laser->startCapVm.scale.x <= 0.0f)
            {
                laser->startCapVm.scale.y = laser->startCapVm.scale.x = scale;
            }
            g_AnmManager->Draw2D(&laser->startCapVm);
        }
    }
    for (i = 0; i < 6; i++)
    {
        // The six draw heads live in the real controller DWORD array.
        bullet = *reinterpret_cast<Bullet **>(
            reinterpret_cast<unsigned int>(&mgr->drawBuckets[0]) +
            static_cast<unsigned int>(i) * sizeof(mgr->drawBuckets[0]));
        while (bullet != NULL)
        {
            DrawSingleBullet(bullet);
            bullet = bullet->nextInDrawBucket;
        }
    }
    mgr->sideState->effectManager0C->DrawBulletLayerEffects();
    return 1;
}

namespace
{

// This is the native 0x44-byte sprite owned by the ANM loader, not an
// Etama-only 0x34-byte approximation. The exact sprite-assignment leaf and
// Enemy's 3,900-byte OnUpdate independently bind this height at +0x30.
typedef char EtamaNativeSpriteHeightAt30[
    (sizeof(AnmLoadedSprite) == 0x44 &&
     offsetof(AnmLoadedSprite, heightPx) == 0x30) ? 1 : -1];

static const int kBulletSpriteScripts[23][5] = {
    {0, 18, 19, 20, 15}, {1, 21, 22, 23, 16}, {2, 21, 22, 23, 16},
    {3, 21, 22, 23, 16}, {4, 21, 22, 23, 16}, {5, 21, 22, 23, 16},
    {6, 21, 22, 23, 16}, {7, 24, 24, 24, 17}, {8, 24, 24, 24, 17},
    {9, 24, 24, 24, 17}, {25, 27, 27, 27, 26}, {96, 21, 22, 23, 16},
    {97, 21, 22, 23, 16}, {98, 21, 22, 23, 16}, {99, 24, 24, 24, 17},
    {100, 24, 24, 24, 17}, {101, 21, 22, 23, 16}, {102, 21, 22, 23, 16},
    {103, 24, 24, 24, 17}, {104, 24, 24, 24, 17}, {105, 24, 24, 24, 17},
    {106, 21, 22, 23, 16}, {107, 24, 24, 24, 17},
};



} // namespace

int EtamaController::AddedCallback(EtamaController *controller)
{
    {
        AnmManager *anmManager = g_AnmManager;
        AnmLoaded *anm = anmManager->GetAnm(ANM_FILE_SLOT_ETAMA);
        if (anm->rawData04 == NULL)
            anm = anmManager->PreloadAnm(ANM_FILE_SLOT_ETAMA, const_cast<char *>("etama.anm"));
        controller->bulletAnm = anm;
        if (anm == NULL)
            return -1;
    }

    // The 23 genuine 0x14-byte script rows hold five native DWORD values;
    // keep each live script lookup at its original call and switch site.
    for (unsigned int i = 0; i < sizeof(kBulletSpriteScripts) / sizeof(kBulletSpriteScripts[0]); ++i)
    {
        // The active 23 template slots are 0xD48-byte native records;
        // preserve the actual controller allocation and its live index.
        BulletTypeSprites *sprites = reinterpret_cast<BulletTypeSprites *>(
            reinterpret_cast<unsigned int>(&controller->bulletTypeSprites[0]) +
            static_cast<unsigned int>(i) * sizeof(controller->bulletTypeSprites[0]));
        controller->bulletAnm
            ->SetAndExecuteScriptIdx(&sprites->bulletVm, (*reinterpret_cast<const int *>(
            reinterpret_cast<unsigned int>(&kBulletSpriteScripts[0][0]) +
            static_cast<unsigned int>(i) * sizeof(kBulletSpriteScripts[0]) +
            0u * sizeof(kBulletSpriteScripts[0][0]))));
        controller->bulletAnm
            ->SetAndExecuteScriptIdx(&sprites->spawnFastVm, (*reinterpret_cast<const int *>(
            reinterpret_cast<unsigned int>(&kBulletSpriteScripts[0][0]) +
            static_cast<unsigned int>(i) * sizeof(kBulletSpriteScripts[0]) +
            1u * sizeof(kBulletSpriteScripts[0][0]))));
        // Preserve the original element lvalue across the VM script call.
        const int &normalScript = *reinterpret_cast<const int *>(
            reinterpret_cast<unsigned int>(&kBulletSpriteScripts[0][2]) +
            static_cast<unsigned int>(i) * sizeof(kBulletSpriteScripts[0]));
        controller->bulletAnm
            ->SetAndExecuteScriptIdx(&sprites->spawnNormalVm, normalScript);
        controller->bulletAnm
            ->SetAndExecuteScriptIdx(&sprites->spawnSlowVm, (*reinterpret_cast<const int *>(
            reinterpret_cast<unsigned int>(&kBulletSpriteScripts[0][0]) +
            static_cast<unsigned int>(i) * sizeof(kBulletSpriteScripts[0]) +
            3u * sizeof(kBulletSpriteScripts[0][0]))));
        controller->bulletAnm
            ->SetAndExecuteScriptIdx(&sprites->despawnVm, (*reinterpret_cast<const int *>(
            reinterpret_cast<unsigned int>(&kBulletSpriteScripts[0][0]) +
            static_cast<unsigned int>(i) * sizeof(kBulletSpriteScripts[0]) +
            4u * sizeof(kBulletSpriteScripts[0][0]))));

        sprites->bulletVm.flagsWord |= 0x2000;
        sprites->spawnFastVm.flagsWord |= 0x2000;
        sprites->spawnNormalVm.flagsWord |= 0x2000;
        sprites->spawnSlowVm.flagsWord |= 0x2000;
        sprites->despawnVm.flagsWord |= 0x2000;

        sprites->unknownD44 = sprites->bulletVm.activeSpriteIndex;
        sprites->bulletVm.baseSpriteIndex = sprites->bulletVm.activeSpriteIndex;
        sprites->spriteHeightPx =
            static_cast<unsigned char>(sprites->bulletVm.loadedSprite->heightPx);

        if (sprites->bulletVm.loadedSprite->heightPx <= 8.0f)
        {
            sprites->collisionSize.x = 4.0f;
            sprites->collisionSize.y = 4.0f;
            sprites->drawBucketIndex = 5;
        }
        else if (sprites->bulletVm.loadedSprite->heightPx <= 16.0f)
        {
            switch ((*reinterpret_cast<const int *>(
            reinterpret_cast<unsigned int>(&kBulletSpriteScripts[0][0]) +
            static_cast<unsigned int>(i) * sizeof(kBulletSpriteScripts[0]) +
            0u * sizeof(kBulletSpriteScripts[0][0]))))
            {
            case 2: case 4: case 6: case 96: case 97: case 98:
            case 101: case 102:
                sprites->collisionSize.x = 4.0f;
                sprites->collisionSize.y = 4.0f;
                sprites->drawBucketIndex = 4;
                break;
            case 5:
                sprites->collisionSize.x = 4.0f;
                sprites->collisionSize.y = 4.0f;
                sprites->drawBucketIndex = 3;
            case 106:
                sprites->collisionSize.x = 4.0f;
                sprites->collisionSize.y = 4.0f;
                sprites->drawBucketIndex = 4;
                break;
            default:
                sprites->collisionSize.x = 6.0f;
                sprites->collisionSize.y = 6.0f;
                sprites->drawBucketIndex = 3;
                break;
            }
        }
        else if (sprites->bulletVm.loadedSprite->heightPx <= 32.0f)
        {
            switch ((*reinterpret_cast<const int *>(
            reinterpret_cast<unsigned int>(&kBulletSpriteScripts[0][0]) +
            static_cast<unsigned int>(i) * sizeof(kBulletSpriteScripts[0]) +
            0u * sizeof(kBulletSpriteScripts[0][0]))))
            {
            case 8: case 103: case 104: case 105: case 107:
                sprites->collisionSize.x = 5.0f;
                sprites->collisionSize.y = 5.0f;
                sprites->drawBucketIndex = 2;
                break;
            case 9: case 99: case 100:
                sprites->collisionSize.x = 8.0f;
                sprites->collisionSize.y = 8.0f;
                sprites->drawBucketIndex = 1;
                break;
            default:
                sprites->collisionSize.x = 10.0f;
                sprites->collisionSize.y = 10.0f;
                sprites->drawBucketIndex = 1;
                break;
            }
        }
        else
        {
            sprites->drawBucketIndex = 0;
            sprites->collisionSize.x = 24.0f;
            sprites->collisionSize.y = 24.0f;
        }
    }

    return 0;
}
