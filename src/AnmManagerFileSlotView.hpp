#pragma once

#include "AnmManager.hpp"
#include <stddef.h>

// One target-bound view of the ANM manager file-slot region, shared by
// resource loading and lookup. The original nominal class is unknown.
// ReadAnmEntries target layout: manager prefix +0x24, then 0x120-byte file slots.
struct AnmManagerReadSlotView
{
    AnmLoaded loaded;
    int releasePending;
    char path[0x100];
};

typedef char AnmManagerReadSlotSizeIs120[
    (sizeof(AnmManagerReadSlotView) == 0x120) ? 1 : -1];
typedef char AnmManagerReadReleaseAt1C[
    (offsetof(AnmManagerReadSlotView, releasePending) == 0x1C) ? 1 : -1];
typedef char AnmManagerReadPathAt20[
    (offsetof(AnmManagerReadSlotView, path) == 0x20) ? 1 : -1];

struct AnmManagerReadView
{
    unsigned char unknown000[0x24];
    AnmManagerReadSlotView slots[256];
};

typedef char AnmManagerReadSlotsAt24[
    (offsetof(AnmManagerReadView, slots) == 0x24) ? 1 : -1];

typedef char AnmManagerReadLoadedAt24[
    (offsetof(AnmManagerReadView, slots) +
     offsetof(AnmManagerReadSlotView, loaded) == 0x24) ? 1 : -1];
