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
    if (shot->state462 == 1 &&
        shot->timer454.GetCurrent() >= 40 &&
        reinterpret_cast<ZunTimer *>(&shot->timer454)->HasTicked())
    {
        if (player->trackedEnemyPosition30364.x > -900.0f)
        {
            float *position = shot->position2A4.operator float *();
            float xDelta =
                player->trackedEnemyPosition30364.x - position[0];
            float yDelta =
                player->trackedEnemyPosition30364.y - position[1];

            float ratio =
                sqrtf(xDelta * xDelta + yDelta * yDelta) /
                (shot->speed44C * 0.25f);
            if (ratio < 1.0f)
                ratio = 1.0f;

            float factor = 1.0f / ratio;
            float x = xDelta * factor + shot->velocityX43C;
            float y = yDelta * factor + shot->velocityY440;
            float magnitude = sqrtf(x * x + y * y);

            if (magnitude <= 10.0f)
                shot->speed44C = magnitude;
            else
                shot->speed44C = 10.0f;

            if (shot->speed44C < 1.0f)
                shot->speed44C = 1.0f;

            float inverse = 1.0f / magnitude;
            shot->velocityX43C =
                inverse * shot->speed44C * x;
            shot->velocityY440 =
                inverse * shot->speed44C * y;
        }
        else if (shot->speed44C < 10.0f)
        {
            float y = shot->velocityY440;
            float x = shot->velocityX43C;
            float speed = shot->speed44C + 0.33333334f;
            shot->speed44C = speed;
            float inverse = 1.0f / sqrtf(y * y + x * x);
            shot->velocityX43C = speed * inverse * x;
            shot->velocityY440 = speed * inverse * y;
        }
    }

    shot->angle450 =
        Th09EclRunControl::VectorAngle(
            shot->velocityY440, shot->velocityX43C);
    return 0;
}
