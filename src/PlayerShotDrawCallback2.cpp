#include "AnmManager.hpp"
#include "PlayerLifecycleView.hpp"

struct PlayerShotDraw2PlayerView;

struct PlayerShotDraw2ShotView
{
    unsigned char unknown000[0x18];
    float scaleX18;
    float scaleY1C;
    unsigned char unknown020[0x1F0 - 0x20];
    PlayerColorBgraView color1F0;
    unsigned char unknown1F4[0x2B0 - 0x1F4];
    PlayerPositionView history2B0[4];
};

int __fastcall PlayerShotDrawCallbackType2(
    PlayerShotDraw2PlayerView *,
    PlayerShotDraw2ShotView *shot)
{
    unsigned int savedRed = shot->color1F0.red;
    unsigned int savedGreen = shot->color1F0.green;
    unsigned int savedBlue = shot->color1F0.blue;
    unsigned int savedAlpha = shot->color1F0.alpha;

    int index = 0;
    int alphaAccumulator = 0;
    int greenAccumulator = 0;
    int redAccumulator = 0;
    PlayerPositionView *point = shot->history2B0;

    do
    {
        if (*reinterpret_cast<int *>(&point->x) ==
            static_cast<int>(0xC479C000u))
        {
            break;
        }

        float progress = static_cast<float>(index) * 0.33333334f;
        shot->color1F0.red = static_cast<unsigned char>(
            redAccumulator / 4 + 63);
        shot->scaleX18 = 3.0f - progress;
        shot->color1F0.green = static_cast<unsigned char>(
            greenAccumulator / 4 + 63);
        shot->scaleY1C = 4.0f - progress;
        shot->color1F0.alpha = static_cast<unsigned char>(
            alphaAccumulator / 4 + 63);

        g_AnmManager->Draw2D(
            reinterpret_cast<AnmVm *>(shot));

        ++index;
        ++point;
        redAccumulator += savedRed;
        greenAccumulator += savedGreen;
        alphaAccumulator += savedAlpha;
    }
    while (index < 4);

    shot->color1F0.red = static_cast<unsigned char>(savedRed);
    shot->color1F0.green = static_cast<unsigned char>(savedGreen);
    shot->color1F0.blue = savedBlue;
    shot->color1F0.alpha = static_cast<unsigned char>(savedAlpha);
    return 0;
}
