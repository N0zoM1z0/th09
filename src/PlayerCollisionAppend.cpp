#include "PlayerLifecycleView.hpp"
#include "PlayerCollisionQueryCapacity.hpp"

void PlayerCollisionQueryStateView::AppendLaserRecord(
    const PlayerPositionView *position,
    const PlayerPositionView *size,
    const PlayerPositionView *origin,
    float angle,
    int unknown)
{
    if (this->count1804 >= PlayerCollisionQueryCapacity::RecordCount)
        return;

    // The original 0x30-byte collision records are addressed with a live
    // signed DWORD count and native modulo-32 byte strides, not by C++
    // out-of-bounds array subscripting. Preserve each distinct live read.

    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&this->records[0]) +
        static_cast<unsigned int>(this->count1804) * sizeof(records[0]))).position00 = *position;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&this->records[0]) +
        static_cast<unsigned int>(this->count1804) * sizeof(records[0]))).origin0C = *origin;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&this->records[0]) +
        static_cast<unsigned int>(this->count1804) * sizeof(records[0]))).halfSize18 = *size;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&this->records[0]) +
        static_cast<unsigned int>(this->count1804) * sizeof(records[0]))).halfSize18.x *= 0.5f;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&this->records[0]) +
        static_cast<unsigned int>(this->count1804) * sizeof(records[0]))).halfSize18.y *= 0.5f;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&this->records[0]) +
        static_cast<unsigned int>(this->count1804) * sizeof(records[0]))).radius24 = 0.0f;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&this->records[0]) +
        static_cast<unsigned int>(this->count1804) * sizeof(records[0]))).angle28 = angle;
    (*reinterpret_cast<PlayerCollisionQueryRecordView *>(
        reinterpret_cast<unsigned int>(&this->records[0]) +
        static_cast<unsigned int>(this->count1804) * sizeof(records[0]))).unknown2C = unknown;
    // Final native INC DWORD preserves modulo-32 counter state.
    this->count1804 = static_cast<int>(static_cast<unsigned int>(this->count1804) + 1u);
}
