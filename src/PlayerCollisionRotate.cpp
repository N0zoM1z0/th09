#include "PlayerLifecycleView.hpp"

#include <math.h>

void RotatePlayerCollisionVector(
    PlayerPositionView *outVector,
    PlayerPositionView *point,
    float angle)
{
    float sinOut = sinf(angle);
    float cosOut = cosf(angle);

    outVector->x = cosOut * point->x - sinOut * point->y;
    outVector->y = cosOut * point->y + sinOut * point->x;
}
