#include "EffectManager.hpp"

#include <stddef.h>

// Preserve the leaf's original return type and decorated ABI. The returned
// record is the real Effect object in EffectManager::effects.
struct EffectFixedSlotRecordView;

struct EffectFixedSlotManagerView
{
    unsigned char unknown00[0x30];
    Effect *effects30;

    EffectFixedSlotRecordView *GetFixedSlotEffect(int slot);
};

typedef char EffectFixedSlotNativeRecordSize[
    (sizeof(Effect) == 0xD8 &&
     offsetof(Effect, position) == 0x0C &&
     offsetof(Effect, active) == 0xC4) ? 1 : -1];
typedef char EffectFixedSlotNativePoolAt30[
    (offsetof(EffectManager, effects) == 0x30) ? 1 : -1];

EffectFixedSlotRecordView *
EffectFixedSlotManagerView::GetFixedSlotEffect(int slot)
{
    return reinterpret_cast<EffectFixedSlotRecordView *>(
        &effects30[256 + slot]);
}
