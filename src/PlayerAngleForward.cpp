#include <stddef.h>

typedef float f32;

namespace Th09EclRunControl
{
f32 __stdcall VectorAngle(f32 y, f32 x);
}

struct PlayerForwardAngleFloat3View
{
    f32 x;
    f32 y;
    f32 z;

    operator f32 *();
};

struct PlayerForwardAngleView
{
    unsigned char unknown000[0x1B88];
    PlayerForwardAngleFloat3View position1B88;

    f32 AngleFromPlayerToPoint(PlayerForwardAngleFloat3View *position);
};

typedef char PlayerForwardAnglePositionAt1B88[
    (offsetof(PlayerForwardAngleView, position1B88) == 0x1B88) ? 1 : -1];

f32 PlayerForwardAngleView::AngleFromPlayerToPoint(PlayerForwardAngleFloat3View *position)
{
    f32 yDelta;
    f32 xDelta;
    f32 *playerPosition;

    playerPosition = this->position1B88.operator f32 *();
    xDelta = position->x - playerPosition[0];
    yDelta = position->y - playerPosition[1];

    if (yDelta == 0.0f && xDelta == 0.0f)
        return 3.1415927f / 2.0f;

    return Th09EclRunControl::VectorAngle(yDelta, xDelta);
}
