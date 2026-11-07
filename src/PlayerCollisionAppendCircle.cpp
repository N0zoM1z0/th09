#include "PlayerLifecycleView.hpp"
#include "PlayerCollisionQueryCapacity.hpp"

#include <stddef.h>

void PlayerCollisionQueryStateView::AppendCircleRecord(
    const PlayerPositionView *position,
    float radius,
    int unknown)
{
    if (count1804 >= PlayerCollisionQueryCapacity::RecordCount)
        return;

    records[count1804].position00 = *position;
    records[count1804].radius24 = radius;
    records[count1804].unknown2C = unknown;
    ++count1804;
}
