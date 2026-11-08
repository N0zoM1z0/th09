#include "PlayerShotInitializeView.hpp"
struct PlayerShotSpawn3DescriptorView
{
    short scheduleValue00;
    unsigned char unknown02[0x36];
};

// Preserve the original shot callback parameter's ABI spelling.  Its
// physical +0x43C..+0x448 velocity fields are PlayerShotCtorView's
// native Float3, already used by movement and the exact update callback.
struct PlayerShotSpawn3ShotView;
typedef char PlayerShotSpawn3NativeVelocityAt43C[
    (offsetof(PlayerShotCtorView, velocity43C) == 0x43C) ? 1 : -1];

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
        reinterpret_cast<PlayerLifecycleView *>(player)->InitializeShot(
            reinterpret_cast<PlayerShotCtorView *>(shot),
            reinterpret_cast<PlayerShtDescriptorView *>(entry));
        PlayerShotCtorView *nativeShot =
            reinterpret_cast<PlayerShotCtorView *>(shot);
        nativeShot->velocity43C.x = 0.0f;
        nativeShot->velocity43C.y = -24.0f;
        PlayerLifecycleView *lifecycle =
            reinterpret_cast<PlayerLifecycleView *>(player);
        lifecycle->movementFields1CCC.speedMultiplier1CE0 = 0.5f;
        lifecycle->movementFields1CCC.speedMultiplier1CDC = 0.5f;
        return 1;
    }

    return 0;
}
