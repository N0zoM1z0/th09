#include <stddef.h>

typedef float f32;

namespace Th09EclRunControl
{
f32 __stdcall VectorAngle(f32 y, f32 x);
}

struct PlayerAngleFloat3View
{
    f32 x;
    f32 y;
    f32 z;

    operator f32 *();
};

struct PlayerAngleView
{
    unsigned char unknown000[0x1B88];
    PlayerAngleFloat3View position1B88;

    f32 AngleToPoint(PlayerAngleFloat3View *position);
};

typedef char PlayerAnglePositionAt1B88[
    (offsetof(PlayerAngleView, position1B88) == 0x1B88) ? 1 : -1];

f32 PlayerAngleView::AngleToPoint(PlayerAngleFloat3View *position)
{
    f32 yDelta;
    f32 xDelta;
    f32 *playerPosition;

    playerPosition = this->position1B88.operator f32 *();
    xDelta = playerPosition[0] - position->x;
    yDelta = playerPosition[1] - position->y;

    if (yDelta == 0.0f && xDelta == 0.0f)
        return 3.1415927f / 2.0f;

    return Th09EclRunControl::VectorAngle(yDelta, xDelta);
}
