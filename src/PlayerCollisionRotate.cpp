#include "PlayerLifecycleView.hpp"

#include <math.h>

// Retail callers pass outVector in ECX, point in EDX, and angle on the stack.
void __fastcall RotatePlayerCollisionVector(
    PlayerPositionView *outVector,
    PlayerPositionView *point,
    float angle)
{
    float sinOut = sinf(angle);
    float cosOut = cosf(angle);

    outVector->x = cosOut * point->x - sinOut * point->y;
    outVector->y = cosOut * point->y + sinOut * point->x;
}
