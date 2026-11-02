#include "ZunTimer.hpp"
#include "PlayerLifecycleView.hpp"

struct PlayerShotUpdate4Vec3
{
    float x;
    float y;
    float z;
};

struct PlayerShotUpdate4RegionPointView
{
    float x;
    float y;
    float z;
};

// Preserve the canonical callback's parameter type in its mangled ABI.
// The actual shot record used by this callback is PlayerShotCtorView.
struct PlayerShotUpdate4ShotView;
typedef char PlayerShotUpdate4NativeShotOffsets[
    (offsetof(PlayerShotCtorView, position2A4) == 0x2A4 &&
     offsetof(PlayerShotCtorView, velocity43C) == 0x43C &&
     offsetof(PlayerShotCtorView, timer454) == 0x454 &&
     offsetof(PlayerShotCtorView, state462) == 0x462) ? 1 : -1];

// The recorded C++ symbol for the rectangle-making helpers is kept for
// genuine call identity; the callback's Player position/speed storage
// itself comes from the native PlayerLifecycleView.
struct PlayerShotUpdate4PlayerView
{
    void *CreateRectType1(
        const PlayerShotUpdate4RegionPointView *center,
        float width, float height, int lifetime, int delay);
    void *CreateRectType0(
        const PlayerShotUpdate4RegionPointView *center,
        float width, float height, int value, int lifetime, int delay);
};

int __fastcall PlayerShotUpdateCallbackType4(
    PlayerShotUpdate4PlayerView *player,
    PlayerShotUpdate4ShotView *shot)
{
    PlayerLifecycleView *lifecycle =
        reinterpret_cast<PlayerLifecycleView *>(player);
    PlayerShotCtorView *nativeShot =
        reinterpret_cast<PlayerShotCtorView *>(shot);
    PlayerShotUpdate4Vec3 *position =
        reinterpret_cast<PlayerShotUpdate4Vec3 *>(&nativeShot->position2A4);
    *position = *reinterpret_cast<const PlayerShotUpdate4Vec3 *>(
        &lifecycle->position1B88);
    // Retain the target's FLD position.y / FADD velocity.y order on VC7.1.
    // The temporary also keeps this 249-byte callback's exact x87 schedule
    // while the shot storage belongs to the native PlayerShotCtorView.
    float velocityY = nativeShot->velocity43C.y;
    nativeShot->position2A4.y += velocityY;
    nativeShot->velocity43C.y -= 1.0f;

    ZunTimer *timer = reinterpret_cast<ZunTimer *>(&nativeShot->timer454);
    if (*timer >= 15)
    {
        lifecycle->movementFields1CCC.speedMultiplier1CE0 = 1.0f;
        lifecycle->movementFields1CCC.speedMultiplier1CDC = 1.0f;
    }

    if (nativeShot->state462 == 1)
    {
        float width = (float)*timer + 80.0f;
        PlayerShotUpdate4RegionPointView center =
            *reinterpret_cast<PlayerShotUpdate4RegionPointView *>(position);
        center.y -= 16.0f;
        center.x -= (width - 96.0f) * 0.5f;

        player->CreateRectType0(
            &center, width, 40.0f, 15, 0, 0);
        player->CreateRectType1(
            &center, width, 30.0f, 0, 0);
    }

    return 0;
}
