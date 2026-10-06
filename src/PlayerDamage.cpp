#include <stddef.h>

struct PlayerDamageFloat3
{
    float x;
    float y;
    float z;
    operator float *();
};

typedef char PlayerDamageFloat3SizeIs0C[
    (sizeof(PlayerDamageFloat3) == 0x0C) ? 1 : -1];
typedef char PlayerDamageFloat3XAt00[
    (offsetof(PlayerDamageFloat3, x) == 0x00) ? 1 : -1];
typedef char PlayerDamageFloat3ZAt08[
    (offsetof(PlayerDamageFloat3, z) == 0x08) ? 1 : -1];

struct PlayerDamageTimer
{
    int previous;
    float subFrame;
    int current;
    int HasTicked();
    int operator%(int value);
};

struct PlayerDamagePlayerView;
struct PlayerDamageShotView;

typedef int (__fastcall *PlayerDamageShotCollisionCallback)(
    PlayerDamagePlayerView *player,
    PlayerDamageShotView *shot,
    PlayerDamageFloat3 *targetPosition);

struct PlayerDamageShotView
{
    unsigned char unknown000[0x08];
    float rotationZ08;
    unsigned char unknown00C[0x2A4 - 0x0C];
    PlayerDamageFloat3 position2A4;
    unsigned char unknown2B0[0x430 - 0x2B0];
    PlayerDamageFloat3 hitboxSize430;
    PlayerDamageFloat3 velocity43C;
    unsigned char unknown448[0x454 - 0x448];
    PlayerDamageTimer timer454;
    short damage460;
    short state462;
    short shotType464;
    unsigned char unknown466[0x46C - 0x466];
    short animationIndex46C;
    unsigned char unknown46E[0x47C - 0x46E];
    PlayerDamageShotCollisionCallback collisionCallback47C;
    unsigned char unknown480[0x484 - 0x480];
};

typedef char PlayerDamageShotSizeIs484[
    (sizeof(PlayerDamageShotView) == 0x484) ? 1 : -1];
typedef char PlayerDamageShotHitboxAt430[
    (offsetof(PlayerDamageShotView, hitboxSize430) == 0x430) ? 1 : -1];
typedef char PlayerDamageShotDamageAt460[
    (offsetof(PlayerDamageShotView, damage460) == 0x460) ? 1 : -1];
typedef char PlayerDamageShotCallbackAt47C[
    (offsetof(PlayerDamageShotView, collisionCallback47C) == 0x47C) ? 1 : -1];

struct PlayerDamageRegionView
{
    float centerX00;
    float centerY04;
    float radius08;
    float radiusGrowth0C;
    float halfWidth10;
    float halfHeight14;
    float growthX18;
    float growthY1C;
    float angle20;
    int lifetime24;
    int value28;
    int hitAccumulator2C;
    int hitCap30;
    int collisionInterval34;
    int type38;
    unsigned char active3C;
    unsigned char unknown3D[3];
    int delay40;

    float BottomY() const
    {
        return centerY04 + halfHeight14;
    }
};

typedef char PlayerDamageRegionSizeIs44[
    (sizeof(PlayerDamageRegionView) == 0x44) ? 1 : -1];

struct PlayerDamageAnmLoadedView
{
    void SetAndExecuteScriptIdx(void *vm, int scriptIndex);
};

struct PlayerDamageEffectManagerView
{
    void *SpawnEffectInFixedSlot(
        int effectId, PlayerDamageFloat3 *position,
        int slotIndex, unsigned int color);
};

struct PlayerDamageSideView
{
    unsigned char unknown00[0x0C];
    PlayerDamageEffectManagerView *effectManager0C;
};

struct PlayerDamageSoundView
{
    void PlaySoundByIdx(int soundId, int pan);
};

extern PlayerDamageSoundView g_SoundPlayer;

void __fastcall PlayerDamageRotate(
    PlayerDamageFloat3 *out,
    PlayerDamageFloat3 *point,
    float angle);

struct PlayerDamageRegionCreateView
{
    PlayerDamageRegionView *CreateCircleType0(
        const PlayerDamageFloat3 *center,
        float radius, float radiusGrowth,
        int value, int lifetime, int delay);
};

struct PlayerDamagePlayerView
{
    unsigned char unknown00000[0x08];
    int sideIndex08;
    PlayerDamageSideView *sideState0C;
    unsigned char unknown010[0xBC - 0x10];
    PlayerDamageAnmLoadedView *anmFileBC;
    unsigned char unknown0C0[0xB100 - 0x0C0];
    PlayerDamageRegionView *activeRegionsB100[514];
    unsigned char unknownB908[0xC11C - 0xB908];
    PlayerDamageShotView shotsC11C[128];
    unsigned char unknown3031C[0x303C8 - 0x3031C];
    PlayerDamageTimer timer303C8;

    int CalcDamageToEnemy(
        PlayerDamageFloat3 *position,
        PlayerDamageFloat3 *hitbox,
        int *primaryAccumulator,
        int *bombHit,
        int *secondaryAccumulator);
};

typedef char PlayerDamageSideAt0C[
    (offsetof(PlayerDamagePlayerView, sideState0C) == 0x0C) ? 1 : -1];
typedef char PlayerDamageActiveAtB100[
    (offsetof(PlayerDamagePlayerView, activeRegionsB100) == 0xB100) ? 1 : -1];
typedef char PlayerDamageShotsAtC11C[
    (offsetof(PlayerDamagePlayerView, shotsC11C) == 0xC11C) ? 1 : -1];
typedef char PlayerDamageTimerAt303C8[
    (offsetof(PlayerDamagePlayerView, timer303C8) == 0x303C8) ? 1 : -1];

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
