#include "PlayerLifecycleView.hpp"

void PlayerCollisionQueryStateView::AppendBoxRecord(
    const PlayerPositionView *position,
    const PlayerPositionView *size,
    int unknown)
{
    if (count1804 >= 128)
        return;

    records[count1804].position00 = *position;
    records[count1804].halfSize18 = *size;
    records[count1804].halfSize18.x *= 0.5f;
    records[count1804].halfSize18.y *= 0.5f;
    records[count1804].radius24 = 0.0f;
    records[count1804].angle28 = 0.0f;
    records[count1804].unknown2C = unknown;
    ++count1804;
}
