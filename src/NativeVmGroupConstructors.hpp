#pragma once

#include "AnmVmLifecycle.hpp"
#include "ZunTimer.hpp"
#include <stddef.h>

// Three original TH09 1.50a constructor bodies initialize independent groups
// of AnmVm instances. Their allocation owners and implicit/explicit origins
// remain unresolved; these are source-equivalent views, not original names.
struct NativeVmSevenPlusOneView
{
    unsigned char unknown00[8];
    AnmVm vms08[7];
    AnmVm tail1284;

    NativeVmSevenPlusOneView();
};

struct NativeVmFivePlusOneView
{
    unsigned char unknown00[8];
    AnmVm vms08[5];
    AnmVm tail0D3C;

    NativeVmFivePlusOneView();
};

struct NativeVmThreeView
{
    unsigned char unknown00[8];
    AnmVm vms08[3];

    NativeVmThreeView();
};

typedef char NativeVmGroupOffsetsMatchOriginal[
    (sizeof(AnmVm) == 0x2A4 &&
     offsetof(NativeVmSevenPlusOneView, vms08) == 0x08 &&
     offsetof(NativeVmSevenPlusOneView, tail1284) == 0x1284 &&
     offsetof(NativeVmFivePlusOneView, vms08) == 0x08 &&
     offsetof(NativeVmFivePlusOneView, tail0D3C) == 0x0D3C &&
     offsetof(NativeVmThreeView, vms08) == 0x08) ? 1 : -1];

// Original +0xB240 array elements each default-construct one 12-byte timer.
struct NativeVmTimerRecord38View
{
    unsigned char unknown00[0x1C];
    ZunTimer timer1C;
    unsigned char unknown28[0x10];

    NativeVmTimerRecord38View();
};

// The original 256-element 0x60-byte array invokes the shared three-byte
// empty-constructor code at 0x4343D0. Nominal origin remains unknown.
struct NativeVmEmptyRecord60View
{
    unsigned char storage[0x60];

    NativeVmEmptyRecord60View();
};

// Natural nested construction corresponding to the 227-byte original owner.
// The separate AnmVm members are deliberate: the target emits 9 individual
// constructor calls rather than an EH-array constructor for those members.
struct NativeVmCompositeOwnerView
{
    AnmVm vm0000;
    AnmVm vm02A4;
    AnmVm vm0548;
    AnmVm vm07EC;
    AnmVm vm0A90;
    AnmVm vm0D34;
    AnmVm vm0FD8;
    AnmVm vm127C;
    AnmVm vm1520;
    AnmVm vmArray17C4[4];
    unsigned char unknown2254[0x10];
    NativeVmEmptyRecord60View emptyRecords2264[256];
    unsigned char unknown8264[0x3C];
    NativeVmSevenPlusOneView seven82A0;
    NativeVmFivePlusOneView five97C8;
    NativeVmThreeView threeA7A8;
    AnmVm vmAF9C;
    NativeVmTimerRecord38View timerRecordsB240[200];
    unsigned char unknownDE00[8];
    AnmVm vmDE08;

    NativeVmCompositeOwnerView();
};

typedef char NativeVmCompositeSourceLayout[
    (sizeof(NativeVmTimerRecord38View) == 0x38 &&
     offsetof(NativeVmTimerRecord38View, timer1C) == 0x1C &&
     sizeof(NativeVmEmptyRecord60View) == 0x60 &&
     offsetof(NativeVmCompositeOwnerView, vmArray17C4) == 0x17C4 &&
     offsetof(NativeVmCompositeOwnerView, emptyRecords2264) == 0x2264 &&
     offsetof(NativeVmCompositeOwnerView, seven82A0) == 0x82A0 &&
     offsetof(NativeVmCompositeOwnerView, five97C8) == 0x97C8 &&
     offsetof(NativeVmCompositeOwnerView, threeA7A8) == 0xA7A8 &&
     offsetof(NativeVmCompositeOwnerView, vmAF9C) == 0xAF9C &&
     offsetof(NativeVmCompositeOwnerView, timerRecordsB240) == 0xB240 &&
     offsetof(NativeVmCompositeOwnerView, vmDE08) == 0xDE08 &&
     sizeof(NativeVmCompositeOwnerView) == 0xE0AC) ? 1 : -1];
