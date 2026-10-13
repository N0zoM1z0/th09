#pragma once

#include "AsciiManager.hpp"
#include <stddef.h>

// Original TH09 Type3 and Type4 trail attacks allocate independent objects
// with the same target-reviewed 0x190-byte state/three-float/history layout.
// Different callbacks interpret the +0x04 vector as motion or a parameter.
// Float3 is nontrivial under VC7.1 and cannot occur in a union; we model a
// single physically shared vector slot without claiming shared semantics or
// original class names.
struct ExAttackHistory32ExtraView
{
    int state00;
    Float3 motion04;
    Float3 history10[32];
};

typedef ExAttackHistory32ExtraView ExAttackType3Extra;
typedef ExAttackHistory32ExtraView ExAttackType3UpdateExtra;
typedef ExAttackHistory32ExtraView ExAttackType4Extra;
typedef ExAttackHistory32ExtraView ExAttackType4UpdateExtra;
typedef ExAttackHistory32ExtraView ExAttackType4SourceExtra;

typedef char ExAttackHistory32ExtraSizeIs190[
    (sizeof(ExAttackHistory32ExtraView) == 0x190) ? 1 : -1];
typedef char ExAttackHistory32MotionAt04[
    (offsetof(ExAttackHistory32ExtraView, motion04) == 0x04) ? 1 : -1];
typedef char ExAttackHistory32HistoryAt10[
    (offsetof(ExAttackHistory32ExtraView, history10) == 0x10) ? 1 : -1];
typedef char ExAttackHistory32HistoryEndIs190[
    (offsetof(ExAttackHistory32ExtraView, history10) +
     sizeof(((ExAttackHistory32ExtraView *)0)->history10) == 0x190) ? 1 : -1];
