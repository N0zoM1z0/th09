#include "EnemyManager.hpp"
#include "BulletManager.hpp"
#include "RngRuntimeLeaves.hpp"

struct EnemyEclOpcode2ContextView
{
    unsigned char unknown00[0x3C];
    float accelerationMagnitude3C;
};

struct EnemyEclOpcode2SideView
{
    unsigned char unknown00[0x08];
    EtamaController *etama08;
};

struct EnemyEclOpcode2SoundView
{
    void PlaySoundByIdx(int soundIndex, int pan);
};

struct BulletTransformResetView
{
    void ResetTransformRuntime();
};

extern RngRuntimeView g_ReplayRng;
extern EnemyEclOpcode2SoundView g_SoundPlayer;

void __fastcall EnemyEclOpcode2(EnemyView *enemy, void *)
{
    EnemyEclOpcode2SideView *side =
        reinterpret_cast<EnemyEclOpcode2SideView *>(
            enemy->manager00->sideState320);

    Bullet *bullet = side->etama08->primaryBullets;
    int remaining = 536;
    do
    {
        if (bullet->state != 0 && bullet->state != 6)
        {
            if (bullet->sprites.bulletVm.spriteSize.y <= 16.0f)
            {
                bullet->controller->bulletAnm->SetSprite(
                    &bullet->sprites.bulletVm,
                    bullet->sprites.unknownD44 + 15);
            }

            bullet->speed = 0.0f;
            *reinterpret_cast<int *>(&bullet->unknownD74[0]) = 0;
            *reinterpret_cast<int *>(&bullet->unknownD74[4]) = 0;
            bullet->angle =
                g_ReplayRng.GetRandomF32SignedInRange(3.1415927f);
            *reinterpret_cast<int *>(&bullet->unknownD80[0]) = 0;
            *reinterpret_cast<int *>(&bullet->unknownD80[4]) = 0;
            bullet->velocity.x = 0.0f;
            bullet->velocity.y = 0.0f;
            bullet->velocity.z = 0.0f;

            reinterpret_cast<BulletTransformResetView *>(bullet)
                ->ResetTransformRuntime();

            bullet->InstallWaitTransform(0, 0, 60);
            bullet->InstallVectorAccelerationTransform(
                1, 0, 60,
                (*reinterpret_cast<EnemyEclOpcode2ContextView **>(
                    reinterpret_cast<unsigned char *>(enemy) + 0x2CE0))
                    ->accelerationMagnitude3C,
                -999.0f);
        }

        ++bullet;
    }
    while (--remaining != 0);

    EnemyEclOpcode2SideView *opposing =
        reinterpret_cast<EnemyEclOpcode2SideView *>(
            enemy->manager00->opposingSideState324);

    bullet = opposing->etama08->primaryBullets;
    for (int i = 0; i < 536; ++i, ++bullet)
    {
        if (bullet->state != 0 && bullet->state != 6)
        {
            if (bullet->sprites.bulletVm.spriteSize.y <= 16.0f &&
                static_cast<int>(bullet->sprites.unknownD44) < 343)
            {
                bullet->controller->bulletAnm->SetSprite(
                    &bullet->sprites.bulletVm,
                    bullet->sprites.unknownD44 + 15);
            }

            bullet->speed = 0.0f;
            *reinterpret_cast<int *>(&bullet->unknownD74[0]) = 0;
            *reinterpret_cast<int *>(&bullet->unknownD74[4]) = 0;
            bullet->angle =
                g_ReplayRng.GetRandomF32SignedInRange(3.1415927f);
            *reinterpret_cast<int *>(&bullet->unknownD80[0]) = 0;
            *reinterpret_cast<int *>(&bullet->unknownD80[4]) = 0;
            bullet->velocity.x = 0.0f;
            bullet->velocity.y = 0.0f;
            bullet->velocity.z = 0.0f;

            reinterpret_cast<BulletTransformResetView *>(bullet)
                ->ResetTransformRuntime();

            if ((i & 1) == 0 &&
                bullet->sprites.bulletVm.spriteSize.y == 16.0f)
            {
                bullet->bulletType = 0;
                bullet->InstallSpriteTransform(0, 0, 0, 15);
                bullet->InstallWaitTransform(1, 0, 60);
                bullet->InstallVectorAccelerationTransform(
                    2, 0, 60,
                    (*reinterpret_cast<EnemyEclOpcode2ContextView **>(
                        reinterpret_cast<unsigned char *>(enemy) + 0x2CE0))
                        ->accelerationMagnitude3C,
                    -999.0f);
            }
            else
            {
                bullet->InstallWaitTransform(0, 0, 60);
                bullet->InstallVectorAccelerationTransform(
                    1, 0, 60,
                    (*reinterpret_cast<EnemyEclOpcode2ContextView **>(
                        reinterpret_cast<unsigned char *>(enemy) + 0x2CE0))
                        ->accelerationMagnitude3C,
                    -999.0f);
            }
        }
    }

    g_SoundPlayer.PlaySoundByIdx(52, 0);
}
