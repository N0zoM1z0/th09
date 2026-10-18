#include "AsciiBossMarkerVmView.hpp"

void AsciiBossMarkerVmView::SetBossMarkerInterrupt(
    int slot, short interrupt)
{
    bossMarkerVms17C4[slot].SetInterrupt(interrupt);
}

void AsciiBossMarkerVmView::SetBossMarkerPosition(
    int slot, const Float3 *position)
{
    bossMarkerVms17C4[slot].pos208 = *position;
}
