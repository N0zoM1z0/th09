#pragma once

// Target-observed Player damage projection; original class identity is unproved.
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
