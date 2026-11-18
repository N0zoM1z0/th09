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

    double rotatedX = cosOut;
    rotatedX *= point->x;
    rotatedX -= sinOut * point->y;
    outVector->x = static_cast<float>(rotatedX);

    cosOut *= point->y;
    cosOut += sinOut * point->x;
    outVector->y = static_cast<float>(cosOut);
}
