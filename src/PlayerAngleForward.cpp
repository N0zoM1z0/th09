#include "PlayerForwardAngleView.hpp"

typedef float f32;

namespace Th09EclRunControl
{
f32 __stdcall VectorAngle(f32 y, f32 x);
}

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
