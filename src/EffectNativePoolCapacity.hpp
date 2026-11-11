#pragma once
#include "EffectManager.hpp"
#include <stddef.h>

// The allocation owns a dynamic primary/secondary pool and one live
// sentinel record, returned on full-pool failure by both native spawners.
struct EffectNativePoolCapacity
{
    enum { SentinelRecords = 1 };
    // The original i386 pool counts and sentinel address arithmetic wrap
    // modulo 2^32; convert back to signed only at the existing interfaces.
    // Preserve the volatile first read in the per-frame update bound.
    static int ActiveCount(const EffectManager &manager)
    {
        return static_cast<int>(
            static_cast<unsigned int>(manager.primaryCount) +
            static_cast<unsigned int>(manager.secondaryCount));
    }
    static int ActiveCountWithNativeOrder(const EffectManager &manager)
    {
        // The original update reloads the live primary count as volatile.
        return static_cast<int>(
            static_cast<unsigned int>(
                static_cast<const volatile int &>(manager.primaryCount)) +
            static_cast<unsigned int>(manager.secondaryCount));
    }
    static int AllocatedCount(const EffectManager &manager)
    {
        return static_cast<int>(
            static_cast<unsigned int>(manager.primaryCount) +
            static_cast<unsigned int>(manager.secondaryCount) +
            static_cast<unsigned int>(SentinelRecords));
    }
    static int AllocatedFromCounts(int primaryCount, int secondaryCount)
    {
        return static_cast<int>(
            static_cast<unsigned int>(primaryCount) +
            static_cast<unsigned int>(secondaryCount) +
            static_cast<unsigned int>(SentinelRecords));
    }
    static Effect *FailureSentinel(EffectManager &manager)
    {
        // The original sentinel address is a native 32-bit byte address:
        // base + (primary + secondary) * sizeof(Effect), modulo 2^32.
        return reinterpret_cast<Effect *>(
            reinterpret_cast<unsigned int>(manager.effects) +
            static_cast<unsigned int>(ActiveCount(manager)) *
            static_cast<unsigned int>(sizeof(Effect)));
    }
};
typedef char EffectNativePoolCapacityLayout[
    (sizeof(Effect) == 0xD8 &&
     sizeof(EffectManager) == 0x2DC &&
     offsetof(EffectManager, effects) == 0x30 &&
     offsetof(EffectManager, primaryCount) == 0x34 &&
     offsetof(EffectManager, secondaryCount) == 0x38 &&
     EffectNativePoolCapacity::SentinelRecords == 1) ? 1 : -1];
