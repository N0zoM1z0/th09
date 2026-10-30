#pragma once

#include "AsciiManager.hpp"
#include "FrontMessageTableView.hpp"

// Full bounded random-message runtime projection shared with its actual caller.
// This reconstruction name does not establish the original retail class name.
struct FrontMessageRandomView
{
    FrontMessageTableView *primary00;
    void *current04;
    int currentIndex08;
    FrontMessageTableView *secondary0C;
    int side10;
    unsigned char unknown014[0x1534 - 0x14];
    AnmVm textVmA1534;
    AnmVm textVmB17D8;
    unsigned char unknown1A7C[0x1D20 - 0x1A7C];
    unsigned long textColors1D20[4];
    unsigned long shadowColors1D30[4];
    unsigned char unknown1D40[0x10];
    int textSelectorCount1D50;
    int glyphSize1D54;
    unsigned char unknown1D58[0x0C];
    unsigned char colorIndex1D64;
    unsigned char flag1D65;
    unsigned char flag1D66;
    unsigned char flag1D67;
    unsigned char unknown1D68[4];
    unsigned char fontFlag1D6C;
    unsigned char fontFlag1D6D;
    unsigned char unknown1D6E[2];

    void SetupRandomForSide(int sideIndex);
};

typedef char FrontMessageRandomSizeIs1D70[
    (sizeof(FrontMessageRandomView) == 0x1D70) ? 1 : -1];
