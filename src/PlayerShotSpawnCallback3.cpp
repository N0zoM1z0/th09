#include "PlayerShotInitializeView.hpp"
struct PlayerShotSpawn3DescriptorView
{
    short scheduleValue00;
    unsigned char unknown02[0x36];
};

struct PlayerShotSpawn3ShotView
{
    unsigned char unknown000[0x43C];
    float velocityX43C;
    float velocityY440;
    float velocityZ444;
};

// Keep the recorded function-pointer ABI symbol; its receiver is the real
// PlayerLifecycleView, not an independent 0x1CE4-byte player model.
struct PlayerShotSpawn3PlayerView;

int __fastcall PlayerShotSpawnCallbackType3(
    PlayerShotSpawn3PlayerView *player,
    PlayerShotSpawn3ShotView *shot,
    int value,
    PlayerShotSpawn3DescriptorView *entry)
{
    if (value == entry->scheduleValue00)
    {
        reinterpret_cast<PlayerShotInitPlayerView *>(player)->InitializeShot(
            reinterpret_cast<PlayerShotInitShotView *>(shot),
            reinterpret_cast<PlayerShotInitDescriptorView *>(entry));
        shot->velocityX43C = 0.0f;
        shot->velocityY440 = -24.0f;
        PlayerLifecycleView *lifecycle =
            reinterpret_cast<PlayerLifecycleView *>(player);
        lifecycle->movementFields1CCC.speedMultiplier1CE0 = 0.5f;
        lifecycle->movementFields1CCC.speedMultiplier1CDC = 0.5f;
        return 1;
    }

    return 0;
}
