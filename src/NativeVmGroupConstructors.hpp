#pragma once

#include "AnmVmLifecycle.hpp"
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
