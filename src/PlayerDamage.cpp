#include "PlayerDamageView.hpp"
#include "SoundPlayer.hpp"

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

int PlayerDamagePlayerView::CalcDamageToEnemy(
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
    PlayerDamageShotView *shot;

    if (!timer303C8.HasTicked())
        return 0;

    PlayerBuildAabb(&enemyTopLeft, &enemyBottomRight, position, hitbox);

    shot = shotsC11C;
    if (bombHit != NULL)
        *bombHit = 0;
    for (int i = 0; i < 128; ++i, ++shot)
    {
        if (shot->state462 == 0)
            continue;
        if (shot->state462 != 1)
            continue;

        PlayerBuildAabb(
            &shotTopLeft, &shotBottomRight,
            &shot->position2A4, &shot->hitboxSize430);

        if (shotTopLeft.y > enemyBottomRight.y ||
            shotTopLeft.x > enemyBottomRight.x ||
            shotBottomRight.y < enemyTopLeft.y ||
            shotBottomRight.x < enemyTopLeft.x)
            continue;

        if (shot->shotType464 == 2 && shot->timer454 % 2 != 0)
            continue;

        if (shot->collisionCallback47C != NULL &&
            shot->collisionCallback47C(this, shot, position))
            continue;

        damage += shot->damage460;

        if (shot->shotType464 != 2 && shot->shotType464 != 3)
        {
            if (shot->state462 == 1)
            {
                savedRotation =
                    shot->rotationZ08;
                anmFileBC->SetAndExecuteScriptIdx(
                    shot, shot->animationIndex46C + 6);
                shot->rotationZ08 =
                    savedRotation;
                reinterpret_cast<PlayerDamageFloat3 *>(
                    shot->position2A4.operator float *())->z = 0.1f;
            }

            shot->state462 = 2;
            shot->velocity43C.x *= 0.125f;
            shot->velocity43C.y *= 0.125f;

            if (shot->shotType464 == 4)
            {
                PlayerDamageFloat3 *effectPosition =
                    &shotsC11C[0].position2A4;
                sideState0C->effectManager0C->SpawnEffectInFixedSlot(
                    34, effectPosition, 6,
                    static_cast<unsigned int>(-1));
                reinterpret_cast<PlayerDamageRegionCreateView *>(this)
                    ->CreateCircleType0(
                        effectPosition, 0.0f, 1.6f, 6, 70, 0);
                g_SoundPlayer.PlaySoundByIdx(
                    16, sideIndex08 != 0 ? 500 : -500);
            }
        }
    }

    *primaryAccumulator = damage;

    // Keep the slot authoritative across rotation and region-state writes.
    PlayerDamageRegionView **slot = activeRegionsB100;
    while (*slot != NULL)
    {

        if ((*slot)->type38 != 0 &&
            (*slot)->type38 != 2 &&
            (*slot)->type38 != 4)
            goto nextRegion;
        if ((*slot)->delay40 > 0)
            goto nextRegion;
        if ((*slot)->lifetime24 % (*slot)->collisionInterval34 != 0)
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

        damage += (*slot)->value28;
        (*slot)->hitAccumulator2C += (*slot)->value28;
        if ((*slot)->hitCap30 > 0 &&
            (*slot)->hitCap30 <= (*slot)->hitAccumulator2C)
        {
            (*slot)->value28 = 0;
            damage +=
                (*slot)->hitCap30 - (*slot)->hitAccumulator2C;
        }

        if ((*slot)->type38 == 4)
            *secondaryAccumulator += (*slot)->value28;

nextRegion:
        ++slot;
    }

    return damage;
}
