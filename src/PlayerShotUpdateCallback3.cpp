#include "PlayerEnemyTrackingNative.hpp"
#include <math.h>

float AddNormalizeAngle(float angle, float delta);

namespace Th09EclRunControl
{
float __stdcall VectorAngle(float y, float x);
}


int __fastcall PlayerShotUpdateCallbackType3(
    PlayerLifecycleView *player,
    PlayerShotCtorView *shot)
{
    if (shot->state462 == 1 && static_cast<int>(shot->timer454) == 30)
    {
        float angle;

        const PlayerEnemyTrackingNativeView &tracking = player->tracking30364;
        if (tracking.trackedX00 >= -144.0f)
        {
            float xDelta =
                tracking.trackedX00 -
                shot->position2A4.x;
            float yDelta =
                tracking.trackedY04 -
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
        shot->velocity43C.x = cosf(angle) * shot->speed44C;
        shot->velocity43C.y = sinf(angle) * shot->speed44C;
        shot->angle450 = angle;
    }

    return 0;
}
