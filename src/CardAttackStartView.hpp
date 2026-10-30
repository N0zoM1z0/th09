#pragma once

#include "CardAttack.hpp"

struct CardAttackStartSideView;

struct CardAttackStartView
{
    int sideIndex00;
    CardAttackStartSideView *sideState04;
    CardAttackStartSideView *otherSideState08;
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
    unsigned char unknown1340[8];
    int primaryValue1348;
    int primaryRow134C;
    int primaryClass1350;
    int secondaryValue1354;
    int secondaryRow1358;
    int secondaryClass135C;
    void *extra1360;
    void *extra1364;

    void StartAttack(
        int callbackRow, int attackClass, int value, const char *text);
};

typedef char CardAttackStartPrimaryAt1328[
    (offsetof(CardAttackStartView, primaryUpdate1328) == 0x1328) ? 1 : -1];
typedef char CardAttackStartTailAt1348[
    (offsetof(CardAttackStartView, primaryValue1348) == 0x1348) ? 1 : -1];
typedef char CardAttackStartSizeIs1368[
    (sizeof(CardAttackStartView) == 0x1368) ? 1 : -1];
