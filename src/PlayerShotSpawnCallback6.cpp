#include "PlayerShotInitializeView.hpp"
struct PlayerShotSpawn6Vec3
{
    float x;
    float y;
    float z;
};

struct PlayerShotSpawn6EffectView
{
    unsigned char unknown00[0x0C];
    PlayerShotSpawn6Vec3 position0C;
};

struct PlayerShotSpawn6EffectManagerView
{
    unsigned char unknown00[0x30];
    void *effects30;
    PlayerShotSpawn6EffectView *GetFixedSlotEffect(int slot);
};

struct PlayerShotSpawn6SideView
{
    unsigned char unknown00[0x0C];
    PlayerShotSpawn6EffectManagerView *effectManager0C;
};

struct PlayerShotSpawn6DescriptorView;
struct PlayerShotSpawn6ShotView;
struct PlayerShotSpawn6PlayerView
{
    unsigned char unknown0000[0x0C];
    PlayerShotSpawn6SideView *sideState0C;

};

int __fastcall PlayerShotSpawnCallbackType6(
    PlayerShotSpawn6PlayerView *player,
    PlayerShotSpawn6ShotView *shot,
    int value,
    PlayerShotSpawn6DescriptorView *entry)
{
    PlayerShtDescriptorView *nativeEntry =
        reinterpret_cast<PlayerShtDescriptorView *>(entry);
    if (value == nativeEntry->fireInterval)
    {
        PlayerShotCtorView *nativeShot =
            reinterpret_cast<PlayerShotCtorView *>(shot);
        reinterpret_cast<PlayerLifecycleView *>(player)->InitializeShot(
            nativeShot, nativeEntry);
        reinterpret_cast<PlayerShotSpawn6Vec3 &>(nativeShot->position2A4) =
            player->sideState0C->effectManager0C->GetFixedSlotEffect(5)
                ->position0C;
        nativeShot->angle450 = -1.5707964f;
        nativeShot->velocity43C.x = 0.0f;
        nativeShot->velocity43C.y = -nativeEntry->speed18;
        return 1;
    }
    return 0;
}
