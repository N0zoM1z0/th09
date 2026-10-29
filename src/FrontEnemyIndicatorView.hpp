#pragma once

#include "AsciiManager.hpp"
#include <stddef.h>

// Reviewed Front prefix for the indicator owner at 0x00416080. This is a
// projection of the shared runtime object, not its complete retail class.
struct FrontEnemyIndicatorColorView
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char alpha;
};

struct FrontEnemyIndicatorVmView
{
    unsigned char unknown000[0x1F0];
    FrontEnemyIndicatorColorView color1F0;
    unsigned char unknown1F4[0x208 - 0x1F4];
    Float3 position208;
    unsigned char unknown214[0x2A4 - 0x214];
};
typedef char FrontEnemyIndicatorVmSize2A4[
    (sizeof(FrontEnemyIndicatorVmView) == 0x2A4) ? 1 : -1];
typedef char FrontEnemyIndicatorVmColorAt1F0[
    (offsetof(FrontEnemyIndicatorVmView, color1F0) == 0x1F0) ? 1 : -1];
typedef char FrontEnemyIndicatorVmPositionAt208[
    (offsetof(FrontEnemyIndicatorVmView, position208) == 0x208) ? 1 : -1];

struct FrontEnemyIndicatorView
{
    unsigned char unknown00000[0x123F4];
    FrontEnemyIndicatorVmView enemyIndicatorVms123F4[2];

    void SetSideEnemyIndicatorPosition(
        int sideIndex, const Float3 *enemyPosition);
};
typedef char FrontEnemyIndicatorVmsAt123F4[
    (offsetof(FrontEnemyIndicatorView, enemyIndicatorVms123F4) == 0x123F4) ? 1 : -1];
