#include <math.h>
#include "PlayerLifecycleView.hpp"

float AddNormalizeAngle(float angle, float delta);

struct PlayerShotUpdate67PlayerView
{
};

struct PlayerShotUpdate67ShotView;
int __fastcall PlayerShotUpdateCallbackType6(
    PlayerShotUpdate67PlayerView *,
    PlayerShotUpdate67ShotView *shot)
{
    PlayerShotCtorView *nativeShot =
        reinterpret_cast<PlayerShotCtorView *>(shot);
    float angle =
        AddNormalizeAngle(nativeShot->angle450, 0.07853981852531433f);
    nativeShot->angle450 = angle;

    float speed = nativeShot->speed44C * 0.96f;
    nativeShot->speed44C = speed;
    nativeShot->velocity43C.x = cosf(angle) * speed;
    nativeShot->velocity43C.y = sinf(angle) * speed;
    return 0;
}

int __fastcall PlayerShotUpdateCallbackType7(
    PlayerShotUpdate67PlayerView *,
    PlayerShotUpdate67ShotView *shot)
{
    PlayerShotCtorView *nativeShot =
        reinterpret_cast<PlayerShotCtorView *>(shot);
    float angle =
        AddNormalizeAngle(nativeShot->angle450, -0.07853981852531433f);
    nativeShot->angle450 = angle;

    float speed = nativeShot->speed44C * 0.96f;
    nativeShot->speed44C = speed;
    nativeShot->velocity43C.x = cosf(angle) * speed;
    nativeShot->velocity43C.y = sinf(angle) * speed;
    return 0;
}
