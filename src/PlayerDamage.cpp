#include "PlayerDamageView.hpp"
#include "PlayerLifecycleView.hpp"
#include "SoundPlayer.hpp"
#include "EffectManager.hpp"
#include "AnmManager.hpp"

static void PlayerBuildAabb(
    PlayerDamageFloat3 *topLeft,
    PlayerDamageFloat3 *bottomRight,
    const PlayerDamageFloat3 *center,
    const PlayerDamageFloat3 *size)
{
    topLeft->x = center->x - size->x * 0.5f;
    topLeft->y = center->y - size->y * 0.5f;
    bottomRight->x = center->x + size->x * 0.5f;
    bottomRight->y = center->y + size->y * 0.5f;
}

int PlayerLifecycleView::CalcDamageToEnemy(
    PlayerDamageFloat3 *position,
    PlayerDamageFloat3 *hitbox,
    int *primaryAccumulator,
    int *bombHit,
    int *secondaryAccumulator)
{
    PlayerDamageFloat3 enemyTopLeft;
    PlayerDamageFloat3 enemyBottomRight;
    PlayerDamageFloat3 shotTopLeft;
    PlayerDamageFloat3 shotBottomRight;
    float savedRotation;
    int damage = 0;
    PlayerShotCtorView *shot;

    if (!timer303C8.HasTicked())
        return 0;

    PlayerBuildAabb(&enemyTopLeft, &enemyBottomRight, position, hitbox);

    shot = shots;
    if (bombHit != NULL)
        *bombHit = 0;
    // The player's genuine shot pool contains 128 native 0x484-byte
    // records, consumed with an i386 DWORD cursor and signed index.
    for (int i = 0; i < 128;
         ++*reinterpret_cast<unsigned int *>(&i),
         shot = reinterpret_cast<PlayerShotCtorView *>(
             reinterpret_cast<unsigned int>(shot) + sizeof(*shot)))
    {
        if (shot->state462 == 0)
            continue;
        if (shot->state462 != 1)
            continue;

        PlayerBuildAabb(
            &shotTopLeft, &shotBottomRight,
            reinterpret_cast<const PlayerDamageFloat3 *>(&shot->position2A4),
            reinterpret_cast<const PlayerDamageFloat3 *>(&shot->hitbox430));

        if (shotTopLeft.y > enemyBottomRight.y ||
            shotTopLeft.x > enemyBottomRight.x ||
            shotBottomRight.y < enemyTopLeft.y ||
            shotBottomRight.x < enemyTopLeft.x)
            continue;

        if (shot->shotType464 == 2 && shot->timer454 % 2 != 0)
            continue;

        if (shot->collisionCallback47C != NULL &&
            reinterpret_cast<PlayerDamageShotCollisionCallback>(shot->collisionCallback47C)(
                this, shot, position))
            continue;

        damage = static_cast<int>(
            static_cast<unsigned int>(damage) +
            static_cast<unsigned int>(shot->damage460));

        if (shot->shotType464 != 2 && shot->shotType464 != 3)
        {
            if (shot->state462 == 1)
            {
                savedRotation =
                    shot->vm.rotation.z;
                header24.anmFile98->SetAndExecuteScriptIdx(
                    &shot->vm, static_cast<int>(
                        static_cast<unsigned int>(shot->animationIndex46C) + 6u));
                shot->vm.rotation.z =
                    savedRotation;
                reinterpret_cast<PlayerDamageFloat3 *>(
                    shot->position2A4.operator float *())->z = 0.1f;
            }

            shot->state462 = 2;
            shot->velocity43C.x *= 0.125f;
            shot->velocity43C.y *= 0.125f;

            if (shot->shotType464 == 4)
            {
                PlayerPositionView *effectPosition = &shots[0].position2A4;
                sideState->effectManager0C->SpawnEffectInFixedSlot(
                    34, reinterpret_cast<const EffectFloat3 *>(effectPosition), 6,
                    static_cast<unsigned int>(-1));
                reinterpret_cast<PlayerLifecycleView *>(this)
                    ->CreateCircleType0(
                        reinterpret_cast<const PlayerRegionPointView *>(effectPosition), 0.0f, 1.6f, 6, 70, 0);
                g_SoundPlayer.PlaySoundByIdx(
                    16, sideIndex != 0 ? 500 : -500);
            }
        }
    }

    *primaryAccumulator = damage;

    // Keep the slot authoritative across rotation and region-state writes.
    PlayerCollisionRegionCreateRegionView **slot = activeB100;
    while (*slot != NULL)
    {

        if ((*slot)->type38 != 0 &&
            (*slot)->type38 != 2 &&
            (*slot)->type38 != 4)
            goto nextRegion;
        if ((*slot)->delay40 > 0)
            goto nextRegion;
        if ((*slot)->lifetime24 % (*slot)->interval34 != 0)
            goto nextRegion;

        if ((*slot)->radius08 == 0.0f)
        {
            if ((*slot)->angle20 == 0.0f)
            {
                if ((*slot)->centerX00 - (*slot)->halfWidth10 >
                        enemyBottomRight.x ||
                    (*slot)->centerX00 + (*slot)->halfWidth10 <
                        enemyTopLeft.x ||
                    (*slot)->centerY04 - (*slot)->halfHeight14 >
                        enemyBottomRight.y ||
                    (*slot)->BottomY() <
                        enemyTopLeft.y)
                    goto nextRegion;
            }
            else
            {
                // The completed shot bounds provide the two XY rotation workspaces.
                shotTopLeft.x = position->x - (*slot)->centerX00;
                shotTopLeft.y = position->y - (*slot)->centerY04;
                PlayerDamageRotate(&shotBottomRight, &shotTopLeft, -(*slot)->angle20);

                float halfWidth = hitbox->x * 0.5f;
                if (-(*slot)->halfWidth10 > halfWidth + shotBottomRight.x ||
                    (*slot)->halfWidth10 < shotBottomRight.x - halfWidth)
                    goto nextRegion;

                float halfHeight = hitbox->y * 0.5f;
                if (-(*slot)->halfHeight14 > halfHeight + shotBottomRight.y ||
                    (*slot)->halfHeight14 < shotBottomRight.y - halfHeight)
                    goto nextRegion;
            }
        }
        else
        {
            float radius = (*slot)->radius08;
            float yDelta = (*slot)->centerY04 - position->y;
            float xDelta = (*slot)->centerX00 - position->x;
            if (radius * radius <
                xDelta * xDelta + yDelta * yDelta)
                goto nextRegion;
        }

        damage = static_cast<int>(
            static_cast<unsigned int>(damage) +
            static_cast<unsigned int>((*slot)->value28));
        *reinterpret_cast<unsigned int *>(&(*slot)->hitCount2C) +=
            static_cast<unsigned int>((*slot)->value28);
        if ((*slot)->hitCap30 > 0 &&
            (*slot)->hitCap30 <= (*slot)->hitCount2C)
        {
            (*slot)->value28 = 0;
            damage = static_cast<int>(
                static_cast<unsigned int>(damage) +
                (static_cast<unsigned int>((*slot)->hitCap30) -
                 static_cast<unsigned int>((*slot)->hitCount2C)));
        }

        if ((*slot)->type38 == 4)
            *reinterpret_cast<unsigned int *>(secondaryAccumulator) +=
                static_cast<unsigned int>((*slot)->value28);

nextRegion:
        slot = reinterpret_cast<PlayerCollisionRegionCreateRegionView **>(
            reinterpret_cast<unsigned int>(slot) + sizeof(*slot));
    }

    return damage;
}
