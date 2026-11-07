#pragma once

#include "PlayerLifecycleView.hpp"
#include <stddef.h>

// Every Player collision append path consumes the same 0x1808-byte query
// allocation: a Player owner, 128 native 0x30-byte records, then the count.
// The bound belongs to the source array, not a second per-append constant.
struct PlayerCollisionQueryCapacity
{
    enum
    {
        RecordCount = sizeof(((PlayerCollisionQueryStateView *)0)->records) /
                      sizeof(((PlayerCollisionQueryStateView *)0)->records[0])
    };
};

typedef char PlayerCollisionQueryCapacityLayout[
    (PlayerCollisionQueryCapacity::RecordCount == 128 &&
     sizeof(PlayerCollisionQueryRecordView) == 0x30 &&
     offsetof(PlayerCollisionQueryStateView, records) == 0x04 &&
     offsetof(PlayerCollisionQueryStateView, count1804) ==
         offsetof(PlayerCollisionQueryStateView, records) +
         sizeof(((PlayerCollisionQueryStateView *)0)->records) &&
     sizeof(PlayerCollisionQueryStateView) == 0x1808) ? 1 : -1];
