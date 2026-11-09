#include "AsciiManager.hpp"

void AsciiManager::SetBossMarkerInterrupt(int slot, short interrupt)
{
    bossMarkerVms17C4[slot].SetInterrupt(interrupt);
}

void AsciiManager::SetBossMarkerPosition(int slot, const Float3 *position)
{
    bossMarkerVms17C4[slot].pos = *position;
}
