#pragma once

#include "AsciiManager.hpp"
#include "AnmVmLifecycle.hpp"
#include <stddef.h>

// Reviewed Front prefix for the indicator owner at 0x00416080. This is a
// projection of the shared runtime object, not its complete retail class.
// These are the same two native VMs initialized by FrontAddedCallback and
// drawn by FrontDrawCallback, at +0x123F4 and +0x12698 of the Front allocation.
typedef char FrontEnemyIndicatorNativeVmFields[
    (sizeof(AnmVm) == 0x2A4 &&
     offsetof(AnmVmBase, color1Script) + offsetof(AnmVmScriptColor, a) == 0x1F3 &&
     offsetof(AnmVmBase, pos) == 0x208) ? 1 : -1];

struct FrontEnemyIndicatorView
{
    unsigned char unknown00000[0x123F4];
    AnmVm enemyIndicatorVms123F4[2];

    void SetSideEnemyIndicatorPosition(
        int sideIndex, const Float3 *enemyPosition);
};
typedef char FrontEnemyIndicatorVmsAt123F4[
    (offsetof(FrontEnemyIndicatorView, enemyIndicatorVms123F4) == 0x123F4) ? 1 : -1];
