#include "PlayerLifecycleView.hpp"

#include <math.h>

// Retail calls the CRT wrappers; /Oi would otherwise fold these to fsin/fcos.
#pragma function(sin, cos)

static double __stdcall CollisionSin(float angle)
{
    return sin(static_cast<double>(angle));
}

static double __stdcall CollisionCos(float angle)
{
    return cos(static_cast<double>(angle));
}

// Retail callers pass outVector in ECX, point in EDX, and angle on the stack.
void __fastcall RotatePlayerCollisionVector(
    PlayerPositionView *outVector,
    PlayerPositionView *point,
    float angle)
{
    float sinOut = static_cast<float>(CollisionSin(angle));
    register double cosOut = CollisionCos(angle);
    volatile PlayerPositionView &nativePoint = *point;

    register double rotatedX = cosOut;
    rotatedX = rotatedX * nativePoint.x;
    rotatedX -= sinOut * nativePoint.y;
    outVector->x = static_cast<float>(rotatedX);

    cosOut = cosOut * nativePoint.y;
    cosOut += sinOut * nativePoint.x;
    outVector->y = static_cast<float>(cosOut);
}
