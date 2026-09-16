#pragma once

#include "AnmVmLifecycle.hpp"
#include "Chain.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

class AnmLoaded;
class Background;

// Target-facing reconstruction of the TH09 per-side CardAttack owner.  The
// semantic name comes from the target registry tag "CardAttackInf"; it is not
// a claim about an original source identifier.
struct CardAttackSideStateView
{
    Background *background00;
    unsigned char unknown004[0x34];
};

typedef char CardAttackSideStateSizeIs38[
    (sizeof(CardAttackSideStateView) == 0x38) ? 1 : -1];

struct CardAttackFloat2
{
    float x;
    float y;
};

struct CardAttackFloat3
{
    float x;
    float y;
    float z;
};

struct CardAttackVmView
{
    unsigned char unknown000[0x18];
    CardAttackFloat2 scale18;
    unsigned char unknown020[0x1F0 - 0x20];
    unsigned long color1F0;
    unsigned long color1F4;
    unsigned int flags1F8;
    unsigned char unknown1FC[0x208 - 0x1FC];
    CardAttackFloat3 position208;
    unsigned char unknown214[0x288 - 0x214];
    CardAttackFloat3 offset288;
    unsigned char unknown294[0x2A4 - 0x294];
};

typedef char CardAttackVmSizeIs2A4[
    (sizeof(CardAttackVmView) == 0x2A4) ? 1 : -1];
typedef char CardAttackVmColorAt1F0[
    (offsetof(CardAttackVmView, color1F0) == 0x1F0) ? 1 : -1];
typedef char CardAttackVmFlagsAt1F8[
    (offsetof(CardAttackVmView, flags1F8) == 0x1F8) ? 1 : -1];
typedef char CardAttackVmPositionAt208[
    (offsetof(CardAttackVmView, position208) == 0x208) ? 1 : -1];
typedef char CardAttackVmOffsetAt288[
    (offsetof(CardAttackVmView, offset288) == 0x288) ? 1 : -1];

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
    CardAttackSideStateView *sideState04;
    CardAttackSideStateView *otherSideState08;
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
    unsigned char unknown1340[0x28];
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
typedef char CardAttackSizeIs1368[
    (sizeof(CardAttack) == 0x1368) ? 1 : -1];
