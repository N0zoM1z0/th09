#include "PlayerLifecycleView.hpp"

#include <stddef.h>

struct PlayerCollisionCircleAppendView
{
    PlayerLifecycleView *player00;
    PlayerCollisionQueryRecordView records[128];
    int count1804;

    void AppendCircleRecord(
        const PlayerPositionView *position,
        float radius,
        int unknown);
};

typedef char PlayerCollisionCircleAppendCountAt1804[
    (offsetof(PlayerCollisionCircleAppendView, count1804) == 0x1804) ? 1 : -1];

void PlayerCollisionCircleAppendView::AppendCircleRecord(
    const PlayerPositionView *position,
    float radius,
    int unknown)
{
    if (count1804 >= 128)
        return;

    records[count1804].position00 = *position;
    records[count1804].radius24 = radius;
    records[count1804].unknown2C = unknown;
    ++count1804;
}
