#include "ZunTimer.hpp"
#include <math.h>

namespace Th09EclRunControl
{
float __stdcall VectorAngle(float y, float x);
}

struct PlayerShotUpdate1Vec3
{
    float x;
    float y;
    float z;

    operator float *();
};

struct PlayerShotUpdate1TimerView
{
    int previous;
    float subFrame;
    int current;

    int GetCurrent();
};

struct PlayerShotUpdate1PlayerView
{
    unsigned char unknown0000[0x30364];
    PlayerShotUpdate1Vec3 trackedEnemyPosition30364;
};

struct PlayerShotUpdate1ShotView
{
    unsigned char unknown000[0x2A4];
    PlayerShotUpdate1Vec3 position2A4;
    unsigned char unknown2B0[0x43C - 0x2B0];
    float velocityX43C;
    float velocityY440;
    unsigned char unknown444[0x44C - 0x444];
    float speed44C;
    float angle450;
    PlayerShotUpdate1TimerView timer454;
    unsigned char unknown460[2];
    short state462;
};

int __fastcall PlayerShotUpdateCallbackType1(
    PlayerShotUpdate1PlayerView *player,
    PlayerShotUpdate1ShotView *shot)
{
    float xDelta;
    float yDelta;
    float magnitude;

    if (shot->state462 == 1 &&
        shot->timer454.GetCurrent() >= 40 &&
        reinterpret_cast<ZunTimer *>(&shot->timer454)->HasTicked())
    {
        if (player->trackedEnemyPosition30364.x > -900.0f)
        {
            float *position = shot->position2A4.operator float *();
            xDelta = player->trackedEnemyPosition30364.x - position[0];
            yDelta = player->trackedEnemyPosition30364.y - position[1];

            magnitude =
                sqrtf(xDelta * xDelta + yDelta * yDelta) /
                (shot->speed44C * 0.25f);
            if (magnitude < 1.0f)
                magnitude = 1.0f;

            xDelta = xDelta / magnitude + shot->velocityX43C;
            yDelta = yDelta / magnitude + shot->velocityY440;
            magnitude = sqrtf(xDelta * xDelta + yDelta * yDelta);

            shot->speed44C = magnitude > 10.0f ? 10.0f : magnitude;
            if (shot->speed44C < 1.0f)
                shot->speed44C = 1.0f;

            shot->velocityX43C =
                xDelta * shot->speed44C / magnitude;
            shot->velocityY440 =
                yDelta * shot->speed44C / magnitude;
        }
        else if (shot->speed44C < 10.0f)
        {
            shot->speed44C += 0.33333334f;
            xDelta = shot->velocityX43C;
            yDelta = shot->velocityY440;
            magnitude = sqrtf(xDelta * xDelta + yDelta * yDelta);
            shot->velocityX43C =
                xDelta * shot->speed44C / magnitude;
            shot->velocityY440 =
                yDelta * shot->speed44C / magnitude;
        }
    }

    shot->angle450 =
        Th09EclRunControl::VectorAngle(
            shot->velocityY440, shot->velocityX43C);
    return 0;
}
