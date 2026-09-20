#include "BulletManager.hpp"
#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "Supervisor.hpp"

extern void __fastcall fsincos(float *sine, float *cosine, float angle);
extern float __stdcall AddNormalizeAngle(float angle, float delta);

struct BulletDrawEffectManagerView
{
    int DrawBulletLayerEffects();
};

struct BulletDrawSideStateView
{
    unsigned char unknown00[0x0C];
    BulletDrawEffectManagerView *effectManager0C;
};

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

    float *position = bullet->position.operator float *();
    float *vmPosition = vm->pos.operator float *();
    vmPosition[0] = g_GameManager.TransformPopupX(position[0]);
    vmPosition[1] = g_GameManager.TransformPopupY(position[1]);
    vmPosition[2] = 0.05f;
    vm->color1 |= 0x00FFFFFFu;

    if (*reinterpret_cast<short *>(vm->unknown_01FC) != 0)
        vm->SetZRotation(AddNormalizeAngle(1.5707964f + bullet->angle, 0.0f));

    return g_AnmManager->Draw2D(vm);
}

int EtamaController::OnDraw(EtamaController *controller)
{
    g_Supervisor.ConfigureGameplayViewport(controller->sideIndex);

    Laser *laser = controller->lasers;
    for (int i = 0; i < 48; ++i, ++laser)
    {
        if (laser->inUse == 0)
            continue;

        float sine;
        float cosine;
        fsincos(&sine, &cosine, laser->angle);

        float halfLength =
            (laser->endOffset - laser->startOffset) * 0.5f +
            laser->startOffset;

        float *position = laser->position.operator float *();
        float *bodyPosition = laser->bodyVm.pos.operator float *();
        bodyPosition[0] =
            g_GameManager.TransformPopupX(position[0] + cosine * halfLength);
        bodyPosition[1] =
            g_GameManager.TransformPopupY(position[1] + sine * halfLength);
        bodyPosition[2] = 0.07f;
        laser->color = -1;
        g_AnmManager->Draw2D(&laser->bodyVm);

        if (laser->startOffset < 16.0f || laser->speed == 0.0f)
        {
            if (!laser->hideCapDuringStartup || laser->state != 0)
            {
                float *capPosition =
                    laser->startCapVm.pos.operator float *();
                capPosition[0] = g_GameManager.TransformPopupX(
                    position[0] + cosine * laser->startOffset);
                capPosition[1] = g_GameManager.TransformPopupY(
                    position[1] + sine * laser->startOffset);
                capPosition[2] = 0.05f;

                float widthScale = laser->width / 10.0f;
                laser->startCapVm.color1 = laser->bodyVm.color1;
                laser->startCapVm.flagsWord |= 0x40;
                laser->startCapVm.color1 |= 0xFF000000u;
                laser->startCapVm.scale.x =
                    widthScale * ((16.0f - laser->startOffset) / 16.0f);
                laser->startCapVm.scale.y = laser->startCapVm.scale.x;
                if (laser->startCapVm.scale.y <= 0.0f)
                {
                    laser->startCapVm.scale.x = widthScale;
                    laser->startCapVm.scale.y =
                        laser->startCapVm.scale.x;
                }

                g_AnmManager->Draw2D(&laser->startCapVm);
            }
        }
    }

    for (int i = 0; i < 6; ++i)
    {
        Bullet *node = controller->drawBuckets[i];
        while (node != NULL)
        {
            DrawSingleBullet(node);
            node = node->nextInDrawBucket;
        }
    }

    reinterpret_cast<BulletDrawSideStateView *>(controller->sideState)
        ->effectManager0C->DrawBulletLayerEffects();
    return 1;
}

namespace
{

struct EtamaAnmLoadedStateView
{
    unsigned char unknown00[4];
    void *loaded04;
};

struct EtamaLoadedSpriteView
{
    unsigned char unknown00[0x30];
    float height30;
};

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

inline short &BaseSpriteIndex(AnmVm *vm)
{
    return *reinterpret_cast<short *>(
        reinterpret_cast<unsigned char *>(vm) + 0x218);
}

inline EtamaLoadedSpriteView *LoadedSprite(AnmVm *vm)
{
    return reinterpret_cast<EtamaLoadedSpriteView *>(vm->loadedSprite);
}

} // namespace

int EtamaController::AddedCallback(EtamaController *controller)
{
    {
        AnmManager *anmManager = g_AnmManager;
        AnmLoaded *anm = anmManager->GetAnm(8);
        if (reinterpret_cast<EtamaAnmLoadedStateView *>(anm)->loaded04 == NULL)
            anm = anmManager->PreloadAnm(8, const_cast<char *>("etama.anm"));
        controller->bulletAnm = reinterpret_cast<BulletAnmLoadedView *>(anm);
        if (anm == NULL)
            return -1;
    }

    for (unsigned int i = 0; i < 23; ++i)
    {
        BulletTypeSprites *sprites = &controller->bulletTypeSprites[i];
        reinterpret_cast<AnmLoaded *>(controller->bulletAnm)
            ->SetAndExecuteScriptIdx(&sprites->bulletVm, kBulletSpriteScripts[i][0]);
        reinterpret_cast<AnmLoaded *>(controller->bulletAnm)
            ->SetAndExecuteScriptIdx(&sprites->spawnFastVm, kBulletSpriteScripts[i][1]);
        reinterpret_cast<AnmLoaded *>(controller->bulletAnm)
            ->SetAndExecuteScriptIdx(&sprites->spawnNormalVm, kBulletSpriteScripts[i][2]);
        reinterpret_cast<AnmLoaded *>(controller->bulletAnm)
            ->SetAndExecuteScriptIdx(&sprites->spawnSlowVm, kBulletSpriteScripts[i][3]);
        reinterpret_cast<AnmLoaded *>(controller->bulletAnm)
            ->SetAndExecuteScriptIdx(&sprites->despawnVm, kBulletSpriteScripts[i][4]);

        sprites->bulletVm.flagsWord |= 0x2000;
        sprites->spawnFastVm.flagsWord |= 0x2000;
        sprites->spawnNormalVm.flagsWord |= 0x2000;
        sprites->spawnSlowVm.flagsWord |= 0x2000;
        sprites->despawnVm.flagsWord |= 0x2000;

        sprites->unknownD44 = sprites->bulletVm.activeSpriteIndex;
        BaseSpriteIndex(&sprites->bulletVm) = sprites->bulletVm.activeSpriteIndex;
        sprites->spriteHeightPx =
            static_cast<unsigned char>(LoadedSprite(&sprites->bulletVm)->height30);

        if (LoadedSprite(&sprites->bulletVm)->height30 <= 8.0f)
        {
            sprites->collisionSize.x = 4.0f;
            sprites->collisionSize.y = 4.0f;
            sprites->drawBucketIndex = 5;
        }
        else if (LoadedSprite(&sprites->bulletVm)->height30 <= 16.0f)
        {
            switch (kBulletSpriteScripts[i][0])
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
        else if (LoadedSprite(&sprites->bulletVm)->height30 <= 32.0f)
        {
            switch (kBulletSpriteScripts[i][0])
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
