#pragma once

#include "CardAttack.hpp"

struct CardAttackStartSideView;

// Source-facing method class retains its proven ABI spelling, but inherits
// the real CardAttack allocation instead of redefining 0x1368 bytes of it.
struct CardAttackStartView : CardAttack
{
    void StartAttack(
        int callbackRow, int attackClass, int value, const char *text);
};

typedef char CardAttackStartViewSameNativeStorage[
    (sizeof(CardAttackStartView) == sizeof(CardAttack) &&
     offsetof(CardAttackStartView, startSideState04) == 0x04 &&
     offsetof(CardAttackStartView, startOtherSideState08) == 0x08) ? 1 : -1];

typedef char CardAttackStartPrimaryAt1328[
    (offsetof(CardAttackStartView, primaryUpdate1328) == 0x1328) ? 1 : -1];
typedef char CardAttackStartTailAt1348[
    (offsetof(CardAttackStartView, primaryValue1348) == 0x1348) ? 1 : -1];
typedef char CardAttackStartSizeIs1368[
    (sizeof(CardAttackStartView) == 0x1368) ? 1 : -1];
