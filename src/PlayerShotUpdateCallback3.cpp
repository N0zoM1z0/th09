#include <math.h>

float AddNormalizeAngle(float angle, float delta);

namespace Th09EclRunControl
{
float __stdcall VectorAngle(float y, float x);
}

struct PlayerShotUpdate3Vec3
{
    float x;
    float y;
    float z;
};

struct PlayerShotUpdate3TimerView
{
    int previous;
    float subFrame;
    int current;

    int GetCurrent();
};

struct PlayerShotUpdate3PlayerView
{
    unsigned char unknown0000[0x30364];
    PlayerShotUpdate3Vec3 trackedEnemyPosition30364;
};

struct PlayerShotUpdate3ShotView
{
    unsigned char unknown000[0x2A4];
    PlayerShotUpdate3Vec3 position2A4;
    unsigned char unknown2B0[0x43C - 0x2B0];
    float velocityX43C;
    float velocityY440;
    unsigned char unknown444[0x44C - 0x444];
    float speed44C;
    float angle450;
    PlayerShotUpdate3TimerView timer454;
    unsigned char unknown460[2];
    short state462;
};

int __fastcall PlayerShotUpdateCallbackType3(
    PlayerShotUpdate3PlayerView *player,
    PlayerShotUpdate3ShotView *shot)
{
    if (shot->state462 == 1 && shot->timer454.GetCurrent() == 30)
    {
        float angle;

        if (player->trackedEnemyPosition30364.x >= -144.0f)
        {
            float xDelta =
                player->trackedEnemyPosition30364.x -
                shot->position2A4.x;
            float yDelta =
                player->trackedEnemyPosition30364.y -
                shot->position2A4.y;

            if (yDelta == 0.0f && xDelta == 0.0f)
            {
                angle = shot->angle450 + 1.5707964f;
            }
            else
            {
                angle =
                    Th09EclRunControl::VectorAngle(yDelta, xDelta) +
                    shot->angle450;
            }
        }
        else
        {
            angle = -1.5707964f;
        }

        angle = AddNormalizeAngle(angle, 0.0f);
        shot->velocityX43C = cosf(angle) * shot->speed44C;
        shot->velocityY440 = sinf(angle) * shot->speed44C;
        shot->angle450 = angle;
    }

    return 0;
}
