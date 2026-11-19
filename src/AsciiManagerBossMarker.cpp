#include "AsciiManager.hpp"

// Four native boss marker states belong to the real AsciiManager array.
void AsciiManager::SetBossMarkerState(int slot, int state)
{
    bossMarkerStates2254[slot] = state;
}
