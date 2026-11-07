#pragma once

#include "BulletManager.hpp"
#include <stddef.h>

// Source-facing physical limits of the single native EtamaController.
// The final record in *each* bullet array carries the pool-end sentinel;
// the other records are allocatable. The OnUpdate linear scan walks across
// the first sentinel and excludes only the final secondary sentinel.
// Use C++ integral enum constants so signed loop comparisons do not silently
// turn into unsigned size_t comparisons in the pinned VC7.1 game code.
struct EtamaNativePoolLimits
{
    enum
    {
        PrimaryRecordCount = sizeof(((EtamaController *)0)->primaryBullets) /
                             sizeof(((EtamaController *)0)->primaryBullets[0]),
        SecondaryRecordCount = sizeof(((EtamaController *)0)->secondaryBullets) /
                               sizeof(((EtamaController *)0)->secondaryBullets[0]),
        PrimaryUsableCount = PrimaryRecordCount - 1,
        SecondaryUsableCount = SecondaryRecordCount - 1,
        LinearScanCount = PrimaryRecordCount + SecondaryUsableCount,
        LaserRecordCount = sizeof(((EtamaController *)0)->lasers) /
                           sizeof(((EtamaController *)0)->lasers[0])
    };
};

typedef char EtamaNativePoolContiguousSentinelLayout[
    (EtamaNativePoolLimits::PrimaryRecordCount == 176 &&
     EtamaNativePoolLimits::SecondaryRecordCount == 361 &&
     EtamaNativePoolLimits::PrimaryUsableCount == 175 &&
     EtamaNativePoolLimits::SecondaryUsableCount == 360 &&
     EtamaNativePoolLimits::LinearScanCount == 536 &&
     EtamaNativePoolLimits::LaserRecordCount == 48 &&
     offsetof(EtamaController, secondaryBullets) ==
         offsetof(EtamaController, primaryBullets) +
         sizeof(((EtamaController *)0)->primaryBullets) &&
     offsetof(EtamaController, lasers) ==
         offsetof(EtamaController, secondaryBullets) +
         sizeof(((EtamaController *)0)->secondaryBullets)) ? 1 : -1];
