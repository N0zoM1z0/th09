#include "PlayerLifecycleView.hpp"
#include "PlayerCollisionQueryCapacity.hpp"

#include <stddef.h>

struct PlayerCollisionCircleAppendView
{
    PlayerLifecycleView *player00;
    PlayerCollisionQueryRecordView records[PlayerCollisionQueryCapacity::RecordCount];
    int count1804;

    void AppendCircleRecord(
        const PlayerPositionView *position,
        float radius,
        int unknown);
};

typedef char PlayerCollisionCircleAppendCountAt1804[
    (offsetof(PlayerCollisionCircleAppendView, count1804) ==
     offsetof(PlayerCollisionQueryStateView, count1804) &&
     sizeof(PlayerCollisionCircleAppendView) == sizeof(PlayerCollisionQueryStateView)) ? 1 : -1];

void PlayerCollisionCircleAppendView::AppendCircleRecord(
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
