#pragma once

#include "AsciiManager.hpp"

#include <stddef.h>

// The separately exact TH09 boss-marker VM member functions consume this
// genuine AsciiManager-rooted prefix. The rest of the original class remains
// deliberately unknown; this is a bounded call-facing projection.
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
