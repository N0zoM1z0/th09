#include <math.h>

float AddNormalizeAngle(float angle, float delta);

struct PlayerShotUpdate67PlayerView
{
};

struct PlayerShotUpdate67ShotView
{
    unsigned char unknown000[0x43C];
    float velocityX43C;
    float velocityY440;
    unsigned char unknown444[0x44C - 0x444];
    float speed44C;
    float angle450;
};

int __fastcall PlayerShotUpdateCallbackType6(
    PlayerShotUpdate67PlayerView *,
    PlayerShotUpdate67ShotView *shot)
{
    float angle =
        AddNormalizeAngle(shot->angle450, 0.07853981852531433f);
    shot->angle450 = angle;

    float speed = shot->speed44C * 0.96f;
    shot->speed44C = speed;
    shot->velocityX43C = cosf(angle) * speed;
    shot->velocityY440 = sinf(angle) * speed;
    return 0;
}

int __fastcall PlayerShotUpdateCallbackType7(
    PlayerShotUpdate67PlayerView *,
    PlayerShotUpdate67ShotView *shot)
{
    float angle =
        AddNormalizeAngle(shot->angle450, -0.07853981852531433f);
    shot->angle450 = angle;

    float speed = shot->speed44C * 0.96f;
    shot->speed44C = speed;
    shot->velocityX43C = cosf(angle) * speed;
    shot->velocityY440 = sinf(angle) * speed;
    return 0;
}
