#include "ZunTimer.hpp"
#include "PlayerEnemyTrackingNative.hpp"
#include <math.h>

namespace Th09EclRunControl
{
float __stdcall VectorAngle(float y, float x);
}


int __fastcall PlayerShotUpdateCallbackType1(
    PlayerLifecycleView *player,
    PlayerShotCtorView *shot)
{
    float xDelta;
    float yDelta;
    float magnitude;

    if (shot->state462 == 1 &&
        static_cast<int>(shot->timer454) >= 40 &&
        reinterpret_cast<ZunTimer *>(&shot->timer454)->HasTicked())
    {
        const PlayerEnemyTrackingNativeView &tracking = player->tracking30364;
        if (tracking.trackedX00 > -900.0f)
        {
            // Preserve the independently original-bound operator float*()
            // call, but access the actual PlayerPositionView x/y subobject
            // rather than indexing past a pointer to its first float member.
            float *positionBytes = shot->position2A4.operator float *();
            const PlayerPositionView &position =
                *reinterpret_cast<const PlayerPositionView *>(positionBytes);
            xDelta = tracking.trackedX00 - position.x;
            yDelta = tracking.trackedY04 - position.y;

            magnitude =
                sqrtf(xDelta * xDelta + yDelta * yDelta) /
                (shot->speed44C * 0.25f);
            if (magnitude < 1.0f)
                magnitude = 1.0f;

            xDelta = xDelta / magnitude + shot->velocity43C.x;
            yDelta = yDelta / magnitude + shot->velocity43C.y;
            magnitude = sqrtf(xDelta * xDelta + yDelta * yDelta);

            shot->speed44C = magnitude > 10.0f ? 10.0f : magnitude;
            if (shot->speed44C < 1.0f)
                shot->speed44C = 1.0f;

            shot->velocity43C.x =
                xDelta * shot->speed44C / magnitude;
            shot->velocity43C.y =
                yDelta * shot->speed44C / magnitude;
        }
        else if (shot->speed44C < 10.0f)
        {
            shot->speed44C += 0.33333334f;
            xDelta = shot->velocity43C.x;
            yDelta = shot->velocity43C.y;
            magnitude = sqrtf(xDelta * xDelta + yDelta * yDelta);
            shot->velocity43C.x =
                xDelta * shot->speed44C / magnitude;
            shot->velocity43C.y =
                yDelta * shot->speed44C / magnitude;
        }
    }

    shot->angle450 =
        Th09EclRunControl::VectorAngle(
            shot->velocity43C.y, shot->velocity43C.x);
    return 0;
}
