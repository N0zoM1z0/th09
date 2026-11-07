#pragma once
#include "EffectManager.hpp"
#include <stddef.h>

// The allocation owns a dynamic primary/secondary pool and one live
// sentinel record, returned on full-pool failure by both native spawners.
struct EffectNativePoolCapacity
{
    enum { SentinelRecords = 1 };
    static int ActiveCount(const EffectManager &manager)
    {
        return manager.primaryCount + manager.secondaryCount;
    }
    static int ActiveCountWithNativeOrder(const EffectManager &manager)
    {
        // The original update reloads the live primary count as volatile.
        return static_cast<const volatile int &>(manager.primaryCount) +
               manager.secondaryCount;
    }
    static int AllocatedCount(const EffectManager &manager)
    {
        return manager.primaryCount + manager.secondaryCount + SentinelRecords;
    }
    static int AllocatedFromCounts(int primaryCount, int secondaryCount)
    {
        return primaryCount + secondaryCount + SentinelRecords;
    }
    static Effect *FailureSentinel(EffectManager &manager)
    {
        return manager.effects + ActiveCount(manager);
    }
};
typedef char EffectNativePoolCapacityLayout[
    (sizeof(Effect) == 0xD8 &&
     sizeof(EffectManager) == 0x2DC &&
     offsetof(EffectManager, effects) == 0x30 &&
     offsetof(EffectManager, primaryCount) == 0x34 &&
     offsetof(EffectManager, secondaryCount) == 0x38 &&
     EffectNativePoolCapacity::SentinelRecords == 1) ? 1 : -1];
