#include "ZunTimer.hpp"
#include "PlayerLifecycleView.hpp"

// Preserve the canonical callback's parameter type in its mangled ABI.
// The actual shot record used by this callback is PlayerShotCtorView.
struct PlayerShotUpdate4ShotView;
typedef char PlayerShotUpdate4NativeShotOffsets[
    (offsetof(PlayerShotCtorView, position2A4) == 0x2A4 &&
     offsetof(PlayerShotCtorView, velocity43C) == 0x43C &&
     offsetof(PlayerShotCtorView, timer454) == 0x454 &&
     offsetof(PlayerShotCtorView, state462) == 0x462) ? 1 : -1];

// Preserve only the original callback parameter type; the Player and shot
// data are the constructed native allocations, including position/center.
struct PlayerShotUpdate4PlayerView;

int __fastcall PlayerShotUpdateCallbackType4(
    PlayerShotUpdate4PlayerView *player,
    PlayerShotUpdate4ShotView *shot)
{
    PlayerLifecycleView *lifecycle =
        reinterpret_cast<PlayerLifecycleView *>(player);
    PlayerShotCtorView *nativeShot =
        reinterpret_cast<PlayerShotCtorView *>(shot);
    PlayerPositionView *position = &nativeShot->position2A4;
    *position = lifecycle->position1B88;
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
        PlayerPositionView center = *position;
        center.y -= 16.0f;
        center.x -= (width - 96.0f) * 0.5f;

        lifecycle->CreateRectType0(
            reinterpret_cast<const PlayerRegionPointView *>(&center), width, 40.0f, 15, 0, 0);
        lifecycle->CreateRectType1(
            reinterpret_cast<const PlayerRegionPointView *>(&center), width, 30.0f, 0, 0);
    }

    return 0;
}
