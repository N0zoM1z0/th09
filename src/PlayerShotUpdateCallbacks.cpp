#include "PlayerLifecycleView.hpp"

struct PlayerShotUpdate5PlayerView
{
};

struct PlayerShotUpdate5ShotView;
int __fastcall PlayerShotUpdateCallbackType5(
    PlayerShotUpdate5PlayerView *,
    PlayerShotUpdate5ShotView *shot)
{
    PlayerShotCtorView *nativeShot =
        reinterpret_cast<PlayerShotCtorView *>(shot);
    if (nativeShot->mode470 == 1)
    {
        nativeShot->velocity43C.x = 0.0f;
        nativeShot->velocity43C.y = 0.0f;
        if (nativeShot->timer454 >= 90)
            nativeShot->state462 = 0;
    }

    nativeShot->flag472 = 1;
    return 0;
}
