#pragma once

#include <stddef.h>

struct PlayerAngleFloat3View
{
    float x;
    float y;
    float z;

    operator float *();
};

struct PlayerAngleView
{
    unsigned char unknown000[0x1B88];
    PlayerAngleFloat3View position1B88;

    float AngleToPoint(PlayerAngleFloat3View *position);
};

typedef char PlayerAnglePositionAt1B88[
    (offsetof(PlayerAngleView, position1B88) == 0x1B88) ? 1 : -1];
