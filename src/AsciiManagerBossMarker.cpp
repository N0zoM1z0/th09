#include "AsciiManager.hpp"

// The four marker-state DWORDs live in the genuine AsciiManager array.
static __forceinline int &NativeAsciiBossState(AsciiManager *owner, int index)
{
    return *reinterpret_cast<int *>(
        reinterpret_cast<unsigned int>(&owner->bossMarkerStates2254[0]) +
        static_cast<unsigned int>(index) * sizeof(owner->bossMarkerStates2254[0]));
}

void AsciiManager::SetBossMarkerState(int slot, int state)
{
    NativeAsciiBossState(this, slot) = state;
}
