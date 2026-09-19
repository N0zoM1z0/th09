#include "AsciiManager.hpp"

struct AsciiBossMarkerStateView
{
    unsigned char unknown0000[0x2254];
    int states2254[4];
};

void AsciiManager::SetBossMarkerState(int slot, int state)
{
    reinterpret_cast<AsciiBossMarkerStateView *>(this)->states2254[slot] = state;
}
