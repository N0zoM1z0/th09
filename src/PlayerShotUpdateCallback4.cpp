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

struct PlayerShotUpdate4ShotView
{
    unsigned char unknown000[0x2A4];
    PlayerShotUpdate4Vec3 position2A4;
    unsigned char unknown2B0[0x43C - 0x2B0];
    float velocityX43C;
    float velocityY440;
    unsigned char unknown444[0x454 - 0x444];
    ZunTimer timer454;
    unsigned char unknown460[2];
    short state462;
};

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
    PlayerShotUpdate4Vec3 *position = &shot->position2A4;
    *position = *reinterpret_cast<const PlayerShotUpdate4Vec3 *>(
        &lifecycle->position1B88);
    shot->position2A4.y += shot->velocityY440;
    shot->velocityY440 -= 1.0f;

    ZunTimer *timer = &shot->timer454;
    if (*timer >= 15)
    {
        lifecycle->movementFields1CCC.speedMultiplier1CE0 = 1.0f;
        lifecycle->movementFields1CCC.speedMultiplier1CDC = 1.0f;
    }

    if (shot->state462 == 1)
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
