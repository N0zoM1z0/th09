#include <stddef.h>

struct PlayerDamageFloat3
{
    float x;
    float y;
    float z;
    operator float *();
};

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
    int savedRotation;
    int damage = 0;
    PlayerDamageRegionView *region;
    PlayerDamageShotView *shot;

    if (!timer303C8.HasTicked())
        return 0;

    PlayerBuildAabb(&enemyTopLeft, &enemyBottomRight, position, hitbox);

    if (bombHit != NULL)
        *bombHit = 0;

    shot = shotsC11C;
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
                    *reinterpret_cast<int *>(&shot->rotationZ08);
                anmFileBC->SetAndExecuteScriptIdx(
                    shot, shot->animationIndex46C + 6);
                *reinterpret_cast<int *>(&shot->rotationZ08) =
                    savedRotation;
                shot->position2A4.operator float *()[2] = 0.1f;
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

    int totalDamage = damage;
    PlayerDamageRegionView **slot = activeRegionsB100;
    while (*slot != NULL)
    {
        region = *slot;

        if (region->type38 != 0 &&
            region->type38 != 2 &&
            region->type38 != 4)
            goto nextRegion;
        if (region->delay40 > 0)
            goto nextRegion;
        if (region->lifetime24 % region->collisionInterval34 != 0)
            goto nextRegion;

        if (region->radius08 != 0.0f)
        {
            float xDelta = region->centerX00 - position->x;
            float yDelta = region->centerY04 - position->y;
            if (region->radius08 * region->radius08 <
                xDelta * xDelta + yDelta * yDelta)
                goto nextRegion;
        }
        else if (region->angle20 == 0.0f)
        {
            if (region->centerX00 - region->halfWidth10 >
                    enemyBottomRight.x ||
                region->centerX00 + region->halfWidth10 <
                    enemyTopLeft.x ||
                region->centerY04 - region->halfHeight14 >
                    enemyBottomRight.y ||
                region->centerY04 + region->halfHeight14 <
                    enemyTopLeft.y)
                goto nextRegion;
        }
        else
        {
            PlayerDamageFloat3 delta;
            PlayerDamageFloat3 rotated;
            delta.x = position->x - region->centerX00;
            delta.y = position->y - region->centerY04;
            PlayerDamageRotate(&rotated, &delta, -region->angle20);

            float halfWidth = hitbox->x * 0.5f;
            if (halfWidth + rotated.x < -region->halfWidth10 ||
                region->halfWidth10 < rotated.x - halfWidth)
                goto nextRegion;

            float halfHeight = hitbox->y * 0.5f;
            if (halfHeight + rotated.y < -region->halfHeight14 ||
                region->halfHeight14 < rotated.y - halfHeight)
                goto nextRegion;
        }

        totalDamage += region->value28;
        region->hitAccumulator2C += region->value28;
        if (region->hitCap30 > 0 &&
            region->hitCap30 <= region->hitAccumulator2C)
        {
            region->value28 = 0;
            totalDamage +=
                region->hitCap30 - region->hitAccumulator2C;
        }

        if (region->type38 == 4)
            *secondaryAccumulator += region->value28;

nextRegion:
        ++slot;
    }

    return totalDamage;
}
