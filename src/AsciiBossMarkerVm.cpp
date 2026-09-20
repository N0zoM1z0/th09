#include "AsciiManager.hpp"

#include <stddef.h>

struct AsciiBossMarkerAnmVmView
{
    unsigned char unknown0000[0x208];
    Float3 pos208;
    unsigned char unknown214[0x2A4 - 0x214];

    void SetInterrupt(short interrupt);
};

typedef char AsciiBossMarkerAnmVmSizeIs2A4[
    (sizeof(AsciiBossMarkerAnmVmView) == 0x2A4) ? 1 : -1];
typedef char AsciiBossMarkerAnmVmPosAt208[
    (offsetof(AsciiBossMarkerAnmVmView, pos208) == 0x208) ? 1 : -1];

struct AsciiBossMarkerVmView
{
    unsigned char unknown0000[0x17C4];
    AsciiBossMarkerAnmVmView bossMarkerVms17C4[4];

    void SetBossMarkerInterrupt(int slot, short interrupt);
    void SetBossMarkerPosition(int slot, const Float3 *position);
};

typedef char AsciiBossMarkerVmsAt17C4[
    (offsetof(AsciiBossMarkerVmView, bossMarkerVms17C4) == 0x17C4) ? 1 : -1];

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
