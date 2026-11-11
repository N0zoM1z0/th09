#include "AsciiManager.hpp"

// Boss marker slots are native four-entry AnmVm records (0x2A4 stride).
static __forceinline AnmVm &NativeAsciiBossMarker(AsciiManager *owner, int index)
{
    return *reinterpret_cast<AnmVm *>(
        reinterpret_cast<unsigned char *>(&owner->bossMarkerVms17C4[0]) +
        static_cast<unsigned int>(index) * sizeof(owner->bossMarkerVms17C4[0]));
}

void AsciiManager::SetBossMarkerInterrupt(int slot, short interrupt)
{
    NativeAsciiBossMarker(this, slot).SetInterrupt(interrupt);
}

void AsciiManager::SetBossMarkerPosition(int slot, const Float3 *position)
{
    NativeAsciiBossMarker(this, slot).pos = *position;
}
