#pragma once

#include <stddef.h>

// Forward direction at0x0041BB00;the opposite helper at0x0041BB70
// has a separate declaration. Names describe observed source-facing roles.
struct PlayerForwardAngleFloat3View
{
    float x;
    float y;
    float z;

    operator float *();
};

struct PlayerForwardAngleView
{
    unsigned char unknown000[0x1B88];
    PlayerForwardAngleFloat3View position1B88;

    float AngleFromPlayerToPoint(PlayerForwardAngleFloat3View *position);
};

typedef char PlayerForwardAnglePositionAt1B88[
    (offsetof(PlayerForwardAngleView, position1B88) == 0x1B88) ? 1 : -1];
