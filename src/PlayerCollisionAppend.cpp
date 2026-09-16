#include "PlayerLifecycleView.hpp"

void PlayerCollisionQueryStateView::AppendLaserRecord(
    const PlayerPositionView *position,
    const PlayerPositionView *size,
    const PlayerPositionView *origin,
    float angle,
    int unknown)
{
    if (this->count1804 >= 128)
        return;

    this->records[this->count1804].position00 = *position;
    this->records[this->count1804].origin0C = *origin;
    this->records[this->count1804].halfSize18 = *size;
    this->records[this->count1804].halfSize18.x *= 0.5f;
    this->records[this->count1804].halfSize18.y *= 0.5f;
    this->records[this->count1804].radius24 = 0.0f;
    this->records[this->count1804].angle28 = angle;
    this->records[this->count1804].unknown2C = unknown;
    ++this->count1804;
}
