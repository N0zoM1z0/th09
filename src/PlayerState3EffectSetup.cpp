#include "EffectManager.hpp"
#include "ZunTimer.hpp"

struct PlayerRegionPointView
{
    float x;
    float y;
};
struct PlayerCollisionRegionCreateRegionView;
struct PlayerCollisionRegionCreateView
{
    PlayerCollisionRegionCreateRegionView *CreateCircleType1(
        const PlayerRegionPointView *center, float radius, float radiusGrowth,
        int lifetime, int delay);
    PlayerCollisionRegionCreateRegionView *CreateCircleType4(
        const PlayerRegionPointView *center, float radius, float radiusGrowth,
        int value, int lifetime, int delay);
};
struct PlayerState4OpsView
{
    int updateState00;
    void SetUpdateState(int state);
};
struct PlayerStateEffectSideView
{
    unsigned char unknown00[0x0C];
    EffectManager *effectManager0C;
};
struct PlayerStateEffectPosition
{
    float x;
    float y;
    float z;
};
struct PlayerStateEffectView
{
    unsigned char unknown00000[0x0C];
    PlayerStateEffectSideView *sideState0C;
    unsigned char unknown00010[0x1B88 - 0x10];
    PlayerStateEffectPosition position1B88;
    unsigned char unknown01B94[0x303C8 - 0x1B94];
    ZunTimer timer303C8;

    Effect *ConfigureState3Effect13();
    Effect *ConfigureState3Effect14();
};

Effect *PlayerStateEffectView::ConfigureState3Effect13()
{
    reinterpret_cast<PlayerCollisionRegionCreateView *>(this)->CreateCircleType1(
        reinterpret_cast<PlayerRegionPointView *>(&position1B88),
        0.0f, 4.0f, 0x30, 0);
    reinterpret_cast<PlayerCollisionRegionCreateView *>(this)->CreateCircleType4(
        reinterpret_cast<PlayerRegionPointView *>(&position1B88),
        0.0f, 4.0f, 2, 0x30, 0);
    reinterpret_cast<PlayerState4OpsView *>(this)->SetUpdateState(3);
    timer303C8 = 0x30;
    return sideState0C->effectManager0C->SpawnEffect(
        13, reinterpret_cast<EffectFloat3 *>(&position1B88), 1, 0xFFFFFFFF);
}

Effect *PlayerStateEffectView::ConfigureState3Effect14()
{
    reinterpret_cast<PlayerCollisionRegionCreateView *>(this)->CreateCircleType1(
        reinterpret_cast<PlayerRegionPointView *>(&position1B88),
        0.0f, 4.0f, 0x40, 0);
    reinterpret_cast<PlayerCollisionRegionCreateView *>(this)->CreateCircleType4(
        reinterpret_cast<PlayerRegionPointView *>(&position1B88),
        0.0f, 4.0f, 2, 0x40, 0);
    reinterpret_cast<PlayerState4OpsView *>(this)->SetUpdateState(3);
    timer303C8 = 0x40;
    return sideState0C->effectManager0C->SpawnEffect(
        14, reinterpret_cast<EffectFloat3 *>(&position1B88), 1, 0xFFFFFFFF);
}
