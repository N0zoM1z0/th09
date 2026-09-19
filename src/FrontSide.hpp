#pragma once

#include "AnmVmLifecycle.hpp"
#include "Chain.hpp"
#include "PlayerLifecycleView.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

class AnmLoaded;

struct FrontSideAuxView
{
    AnmLoaded *sideAnm00;
    AnmLoaded *optionalAnm04;
    int unknown08;
    AnmVm vms0C[2];
    int unknown554;

    FrontSideAuxView();
};

typedef char FrontSideAuxVmsAt0C[
    (offsetof(FrontSideAuxView, vms0C) == 0x0C) ? 1 : -1];
typedef char FrontSideAuxUnknown554At554[
    (offsetof(FrontSideAuxView, unknown554) == 0x554) ? 1 : -1];
typedef char FrontSideAuxSizeIs558[
    (sizeof(FrontSideAuxView) == 0x558) ? 1 : -1];

// Target-facing reconstruction of the TH09 per-side gameplay HUD owner. The
// semantic name is anchored by the target registry tag "FrontSideInf". Member
// role names are reconstruction aliases and do not claim original identifiers.
struct FrontSide
{
    FrontSide();

    static FrontSide *Create(int sideIndex);
    static void Destroy(FrontSide *frontSide);
    static int AddedCallback(FrontSide *frontSide);
    int Reset();
    static int OnUpdate(FrontSide *frontSide);
    static int OnDraw(FrontSide *frontSide);

    void LoadScript67();
    void ResetTransitionState();
    void ActivateMeterVms();
    void DeactivateMeterVms();
    int UpdateMeterHundreds(int level);
    void ClearState(int value);
    void ResetPatternTiming();
    int SetPatternTiming(int frames);
    int SetTimingDigits(int frames);

    // VC7.1 constructs these target-observed groups independently even though
    // they occupy one contiguous 0xA65C-byte AnmVm region.
    AnmVm statusVms0000[11];       // +0x0000
    AnmVm stockVms1D0C[5];        // +0x1D0C
    AnmVm gaugeVms2A40[7];        // +0x2A40
    AnmVm vm3CBC;                  // +0x3CBC
    AnmVm scoreVms3F60[10];       // +0x3F60
    AnmVm vm59C8;                  // +0x59C8
    AnmVm meterVms5C6C[4];        // +0x5C6C
    AnmVm markerVms66FC[3];       // +0x66FC
    AnmVm stateVms6EE8[2];        // +0x6EE8
    AnmVm vm7430;                  // +0x7430
    AnmVm timingVms76D4[6];       // +0x76D4
    AnmVm rankVms86AC[7];         // +0x86AC
    AnmVm counterVms9928[5];      // +0x9928

    int transitionStateA65C;
    ChainElem *calcChainA660;
    ChainElem *drawChainA664;
    AnmLoaded *frontAnmA668;
    int sideIndexA66C;
    PlayerSideStateView *sideStateA670;
    PlayerSideStateView *otherSideStateA674;
    FrontSideAuxView auxA678;
    ZunTimer transitionTimerABD0;
    ZunTimer updateTimerABDC;
};

typedef char FrontSideStatusVmsAt0000[
    (offsetof(FrontSide, statusVms0000) == 0x0000) ? 1 : -1];
typedef char FrontSideStockVmsAt1D0C[
    (offsetof(FrontSide, stockVms1D0C) == 0x1D0C) ? 1 : -1];
typedef char FrontSideGaugeVmsAt2A40[
    (offsetof(FrontSide, gaugeVms2A40) == 0x2A40) ? 1 : -1];
typedef char FrontSideVm3CBCAt3CBC[
    (offsetof(FrontSide, vm3CBC) == 0x3CBC) ? 1 : -1];
typedef char FrontSideScoreVmsAt3F60[
    (offsetof(FrontSide, scoreVms3F60) == 0x3F60) ? 1 : -1];
typedef char FrontSideVm59C8At59C8[
    (offsetof(FrontSide, vm59C8) == 0x59C8) ? 1 : -1];
typedef char FrontSideMeterVmsAt5C6C[
    (offsetof(FrontSide, meterVms5C6C) == 0x5C6C) ? 1 : -1];
typedef char FrontSideMarkerVmsAt66FC[
    (offsetof(FrontSide, markerVms66FC) == 0x66FC) ? 1 : -1];
typedef char FrontSideStateVmsAt6EE8[
    (offsetof(FrontSide, stateVms6EE8) == 0x6EE8) ? 1 : -1];
typedef char FrontSideVm7430At7430[
    (offsetof(FrontSide, vm7430) == 0x7430) ? 1 : -1];
typedef char FrontSideTimingVmsAt76D4[
    (offsetof(FrontSide, timingVms76D4) == 0x76D4) ? 1 : -1];
typedef char FrontSideRankVmsAt86AC[
    (offsetof(FrontSide, rankVms86AC) == 0x86AC) ? 1 : -1];
typedef char FrontSideCounterVmsAt9928[
    (offsetof(FrontSide, counterVms9928) == 0x9928) ? 1 : -1];
typedef char FrontSideTransitionStateAtA65C[
    (offsetof(FrontSide, transitionStateA65C) == 0xA65C) ? 1 : -1];
typedef char FrontSideFrontAnmAtA668[
    (offsetof(FrontSide, frontAnmA668) == 0xA668) ? 1 : -1];
typedef char FrontSideSideIndexAtA66C[
    (offsetof(FrontSide, sideIndexA66C) == 0xA66C) ? 1 : -1];
typedef char FrontSideAuxAtA678[
    (offsetof(FrontSide, auxA678) == 0xA678) ? 1 : -1];
typedef char FrontSideTransitionTimerAtABD0[
    (offsetof(FrontSide, transitionTimerABD0) == 0xABD0) ? 1 : -1];
typedef char FrontSideUpdateTimerAtABDC[
    (offsetof(FrontSide, updateTimerABDC) == 0xABDC) ? 1 : -1];
typedef char FrontSideSizeIsABE8[
    (sizeof(FrontSide) == 0xABE8) ? 1 : -1];
