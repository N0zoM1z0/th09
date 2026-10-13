#pragma once

#include "ExAttackController.hpp"
#include "ExAttackHistory32ExtraView.hpp"
#include <stddef.h>

// TH09 Type3 exact initializer and nonexact 780-byte update allocate/use
// the same 0x190-byte Extra object: state +0, motion +4, 32 history vectors
// starting at +0x10. Original authored class names remain unverified.
struct ExAttackType3VmView;

// Init also consumes the two tail arguments at +0x38/+0x3C; update uses
// only the common +0x00..+0x37 prefix. +0x1C is one pointer-sized slot,
// viewed as void* during initialization and a VM-array pointer on update.
struct ExAttackType3RecordView
{
    int unknown00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    union
    {
        void *dynamicData1C;
        ExAttackType3VmView *vms1C;
    };
    Float3 position20;
    int vmCount2C;
    int unknown30;
    ExAttackType3Extra *extra34;
    int parameter38;
    float angle3C;
};
typedef ExAttackType3RecordView ExAttackType3UpdateRecord;
typedef char ExAttackType3RecordTimerAt10[
    (offsetof(ExAttackType3RecordView, timer10) == 0x10) ? 1 : -1];
typedef char ExAttackType3RecordVmAt1C[
    (offsetof(ExAttackType3RecordView, vms1C) == 0x1C) ? 1 : -1];
typedef char ExAttackType3RecordPositionAt20[
    (offsetof(ExAttackType3RecordView, position20) == 0x20) ? 1 : -1];
typedef char ExAttackType3RecordExtraAt34[
    (offsetof(ExAttackType3RecordView, extra34) == 0x34) ? 1 : -1];
typedef char ExAttackType3RecordAngleAt3C[
    (offsetof(ExAttackType3RecordView, angle3C) == 0x3C) ? 1 : -1];
typedef char ExAttackType3RecordSizeIs40[
    (sizeof(ExAttackType3RecordView) == 0x40) ? 1 : -1];
