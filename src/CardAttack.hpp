#pragma once

#include "AnmVmLifecycle.hpp"
#include "Chain.hpp"
#include "GameManagerSideState.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

class AnmLoaded;
class Background;
typedef AsciiGameManagerSideView CardAttackStartSideView;

// Target-facing reconstruction of the TH09 per-side CardAttack owner.  The
// semantic name comes from the target registry tag "CardAttackInf"; it is not
// a claim about an original source identifier.
// The native 0x38-byte GameManager side record is the same object held by
// CardAttack. The background pointer remains anchored at physical offset 0.
typedef AsciiGameManagerSideView CardAttackSideStateView;
typedef char CardAttackSideStateSizeIs38[
    (sizeof(CardAttackSideStateView) == 0x38 &&
     offsetof(CardAttackSideStateView, background00) == 0x00) ? 1 : -1];

// CardAttack's main and secondary draw owners use the actual AnmVm arrays,
// whose scalar, color, flags and coordinate offsets are independently exact.
typedef char CardAttackVmNativeStorage[
    (sizeof(AnmVm) == 0x2A4 &&
     offsetof(AnmVm, scale) == 0x18 &&
     offsetof(AnmVm, color1) == 0x1F0 &&
     offsetof(AnmVm, flagsWord) == 0x1F8 &&
     offsetof(AnmVm, pos) == 0x208 &&
     offsetof(AnmVm, pos2) == 0x288) ? 1 : -1];

struct CardAttack;
typedef int (*CardAttackUpdateCallback)(CardAttack *cardAttack);
typedef void (*CardAttackDrawCallback)(CardAttack *cardAttack);
typedef void (*CardAttackEndCallback)(CardAttack *cardAttack);

struct CardAttack
{
    CardAttack();

    // Source-facing roles below are reconstructed from direct target use.
    // Callback return types/original identifiers remain unclaimed.
    void RefreshResource();

    static int OnUpdate(CardAttack *cardAttack);
    static int OnDraw(CardAttack *cardAttack);
    static int AddedCallback(CardAttack *cardAttack);
    static void Destroy(CardAttack *cardAttack);
    static CardAttack *Create(int sideIndex);

    int sideIndex00;
    // The exact StartAttack uses a richer view of these same two side pointers.
    union { CardAttackSideStateView *sideState04; CardAttackStartSideView *startSideState04; };
    union { CardAttackSideStateView *otherSideState08; CardAttackStartSideView *startOtherSideState08; };
    ChainElem *calcChain0C;
    ChainElem *drawChain10;
    AnmLoaded *presentationAnm14;
    char text18[0x80];
    ZunTimer timer98;
    int primaryPresentationCounterA4;
    int secondaryPresentationCounterA8;
    AnmVm mainVms0AC[2];
    AnmVm secondaryVms5F4[5];
    CardAttackUpdateCallback primaryUpdate1328;
    CardAttackDrawCallback primaryDraw132C;
    CardAttackEndCallback primaryEnd1330;
    CardAttackUpdateCallback secondaryUpdate1334;
    CardAttackDrawCallback secondaryDraw1338;
    CardAttackEndCallback secondaryEnd133C;
    // This final 0x28-byte interval is consumed by the exact 923-byte
    // StartAttack owner and the independent begin/end callback leaves.
    unsigned char unknown1340[8];
    int primaryValue1348;
    int primaryRow134C;
    int primaryClass1350;
    int secondaryValue1354;
    int secondaryRow1358;
    int secondaryClass135C;
    void *extra1360;
    void *extra1364;
};

typedef char CardAttackTimerAt98[
    (offsetof(CardAttack, timer98) == 0x98) ? 1 : -1];
typedef char CardAttackMainVmsAtAC[
    (offsetof(CardAttack, mainVms0AC) == 0x0AC) ? 1 : -1];
typedef char CardAttackSecondaryVmsAt5F4[
    (offsetof(CardAttack, secondaryVms5F4) == 0x5F4) ? 1 : -1];
typedef char CardAttackCallbacksAt1328[
    (offsetof(CardAttack, primaryUpdate1328) == 0x1328) ? 1 : -1];
typedef char CardAttackSecondaryEndAt133C[
    (offsetof(CardAttack, secondaryEnd133C) == 0x133C) ? 1 : -1];
typedef char CardAttackNativeTailFields[
    (offsetof(CardAttack, primaryValue1348) == 0x1348 &&
     offsetof(CardAttack, primaryRow134C) == 0x134C &&
     offsetof(CardAttack, primaryClass1350) == 0x1350 &&
     offsetof(CardAttack, secondaryValue1354) == 0x1354 &&
     offsetof(CardAttack, secondaryRow1358) == 0x1358 &&
     offsetof(CardAttack, secondaryClass135C) == 0x135C &&
     offsetof(CardAttack, extra1360) == 0x1360 &&
     offsetof(CardAttack, extra1364) == 0x1364) ? 1 : -1];
typedef char CardAttackSizeIs1368[
    (sizeof(CardAttack) == 0x1368) ? 1 : -1];
