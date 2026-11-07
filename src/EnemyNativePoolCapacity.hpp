#pragma once

#include "EnemyManager.hpp"
#include <stddef.h>

// The manager allocates one sentinel Enemy after its 128 scan/spawn slots.
// Consumers must not traverse the sentinel as an active record. The last
// physical record is still part of the manager's actual native allocation.
struct EnemyNativePoolCapacity
{
    enum
    {
        PhysicalSlots = sizeof(((EnemyManagerView *)0)->enemies5758) /
                        sizeof(((EnemyManagerView *)0)->enemies5758[0]),
        ActiveSlots = PhysicalSlots - 1,
        FailureSentinelIndex = ActiveSlots
    };
};

// These are source/layout checks, not claims about the original type name.
typedef char EnemyNativePoolCapacityLayout[
    (EnemyNativePoolCapacity::PhysicalSlots == 129 &&
     EnemyNativePoolCapacity::ActiveSlots == 128 &&
     sizeof(EnemyView) == 0x5430 &&
     offsetof(EnemyManagerView, enemies5758) == 0x5758 &&
     offsetof(EnemyManagerView, enemies5758) +
         EnemyNativePoolCapacity::FailureSentinelIndex * sizeof(EnemyView) ==
         0x2A6F58 &&
     offsetof(EnemyManagerView, remoteSlots2AC388) ==
         offsetof(EnemyManagerView, enemies5758) +
         sizeof(((EnemyManagerView *)0)->enemies5758)) ? 1 : -1];
