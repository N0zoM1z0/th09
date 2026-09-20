#include "AsciiGameManagerView.hpp"
#include "AnmManager.hpp"

struct PlayerShotDraw1PlayerView;

struct PlayerShotDraw1Vec3
{
    float x;
    float y;
    float z;
};

struct PlayerShotDraw1ShotView
{
    unsigned char unknown000[0x1F3];
    unsigned char alpha1F3;
    unsigned char unknown1F4[0x208 - 0x1F4];
    PlayerShotDraw1Vec3 vmPosition208;
    unsigned char unknown214[0x2B0 - 0x214];
    PlayerShotDraw1Vec3 history2B0[32];
    unsigned char unknown430[0x46A - 0x430];
    short trailSegmentCount46A;
};

int __fastcall PlayerShotDrawCallbackType1(
    PlayerShotDraw1PlayerView *,
    PlayerShotDraw1ShotView *shot)
{
    unsigned int alpha = shot->alpha1F3;
    int index = 0;

    if (2 * shot->trailSegmentCount46A > 0)
    {
        int alphaAccumulator = 0;
        PlayerShotDraw1Vec3 *point = shot->history2B0;

        do
        {
            if (*reinterpret_cast<int *>(&point->x) ==
                static_cast<int>(0xC479C000u))
            {
                break;
            }

            shot->vmPosition208.x =
                g_GameManager.TransformPopupX(point->x);
            shot->vmPosition208.y =
                g_GameManager.TransformPopupY(point->y);
            shot->vmPosition208.z = 0.0f;

            shot->alpha1F3 = static_cast<unsigned char>(
                alpha -
                alphaAccumulator /
                    (2 * shot->trailSegmentCount46A));

            g_AnmManager->Draw2D(
                reinterpret_cast<AnmVm *>(shot));

            alphaAccumulator += alpha;
            ++index;
            ++point;
        }
        while (index < 2 * shot->trailSegmentCount46A);
    }

    shot->alpha1F3 = static_cast<unsigned char>(alpha);
    return 0;
}
