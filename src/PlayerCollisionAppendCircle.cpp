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

    // The original 0x30-byte collision records are addressed with a live
    // signed DWORD count and native modulo-32 byte strides, not by C++
    // out-of-bounds array subscripting. Preserve each distinct live read.

    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&records[0]) +
        static_cast<unsigned int>(count1804) * sizeof(records[0]))).position00 = *position;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&records[0]) +
        static_cast<unsigned int>(count1804) * sizeof(records[0]))).radius24 = radius;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&records[0]) +
        static_cast<unsigned int>(count1804) * sizeof(records[0]))).unknown2C = unknown;
    // Final native INC DWORD preserves modulo-32 counter state.
    count1804 = static_cast<int>(static_cast<unsigned int>(count1804) + 1u);
}
