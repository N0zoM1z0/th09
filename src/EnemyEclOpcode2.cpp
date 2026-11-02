#include "EnemyEclExInstructions.hpp"
#include "SoundPlayer.hpp"
#include "BulletManager.hpp"
#include "EnemyManager.hpp"
#include "AnmManager.hpp"
#include "RngRuntimeLeaves.hpp"

typedef char EnemyEclOpcode2NativeContextAndSideAt3C[
    (offsetof(EnemyView, activeContext2CE0) == 0x2CE0 &&
     offsetof(Th09EclContextView, floatVariables3C) == 0x3C &&
     offsetof(EnemyManagerView, sideState320) == 0x320 &&
     offsetof(EnemyManagerView, opposingSideState324) == 0x324 &&
     offsetof(EnemySideStateView, etama08) == 0x08) ? 1 : -1];

struct BulletTransformResetView
{
    void ResetTransformRuntime();
};

extern RngRuntimeView g_Rng;

void __fastcall EnemyEclExInstruction2(
    EnemyView *enemy, EnemyEclExInstructionView *)
{
    EnemySideStateView *side = enemy->manager00->sideState320;

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
                g_Rng.GetRandomF32SignedInRange(3.1415927f);
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
                enemy->activeContext2CE0->floatVariables3C[0],
                -999.0f);
        }

        ++bullet;
    }
    while (--remaining != 0);

    EnemySideStateView *opposing = enemy->manager00->opposingSideState324;

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
                g_Rng.GetRandomF32SignedInRange(3.1415927f);
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
                    enemy->activeContext2CE0->floatVariables3C[0],
                    -999.0f);
            }
            else
            {
                bullet->InstallWaitTransform(0, 0, 60);
                bullet->InstallVectorAccelerationTransform(
                    1, 0, 60,
                    enemy->activeContext2CE0->floatVariables3C[0],
                    -999.0f);
            }
        }
    }

    g_SoundPlayer.PlaySoundByIdx(52, 0);
}
