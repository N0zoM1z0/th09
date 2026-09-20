#include "ZunTimer.hpp"

struct PlayerShotUpdate5PlayerView
{
};

struct PlayerShotUpdate5ShotView
{
    unsigned char unknown000[0x43C];
    float velocityX43C;
    float velocityY440;
    unsigned char unknown444[0x454 - 0x444];
    ZunTimer timer454;
    unsigned char unknown460[0x02];
    short state462;
    unsigned char unknown464[0x0C];
    short mode470;
    unsigned char flag472;
};

int __fastcall PlayerShotUpdateCallbackType5(
    PlayerShotUpdate5PlayerView *,
    PlayerShotUpdate5ShotView *shot)
{
    if (shot->mode470 == 1)
    {
        shot->velocityX43C = 0.0f;
        shot->velocityY440 = 0.0f;
        if (shot->timer454 >= 90)
            shot->state462 = 0;
    }

    shot->flag472 = 1;
    return 0;
}
