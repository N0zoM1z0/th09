#pragma once

#include "FrontMessageTableView.hpp"
#include "AnmVmLifecycle.hpp"
#include "ZunTimer.hpp"

// Native constructed groups and the original-bound draw/load/release fields
// share one 0x1D70-byte runtime. The historical original class name is unknown.
struct FrontMessageRuntimeView
{
    FrontMessageTableView *primary00;
    FrontMessageInstructionView *current04;
    int currentIndex08;
    FrontMessageTableView *secondary0C;
    unsigned int state10;
    AnmVm messageVms14[8];
    AnmVm messageVms1534[2];
    AnmVm messageVm1A7C;
    unsigned long textColors1D20[4];
    unsigned long shadowColors1D30[4];
    ZunTimer timer1D40;
    unsigned char unknown1D4C[0x04];
    int textSelectorCount1D50;
    int glyphSize1D54;
    ZunTimer timer1D58;
    unsigned char colorIndex1D64;
    unsigned char flag1D65;
    unsigned char flag1D66;
    unsigned char flag1D67;
    unsigned char unknown1D68[0x04];
    unsigned char fontFlag1D6C;
    unsigned char boxVisible1D6D;
    unsigned char unknown1D6E[0x02];

    FrontMessageRuntimeView();
    int Draw();
    void ReleaseForSideCount(int startIndex);
    int LoadForSideCount(char *primaryPath, char *secondaryPath, int startIndex);
};

typedef char FrontMessageRuntimeViewSizeIs1D70[
    (sizeof(FrontMessageRuntimeView) == 0x1D70) ? 1 : -1];
typedef char FrontMessageRuntimeCurrentAt04[
    (offsetof(FrontMessageRuntimeView, current04) == 0x04) ? 1 : -1];
typedef char FrontMessageRuntimeIndexAt08[
    (offsetof(FrontMessageRuntimeView, currentIndex08) == 0x08) ? 1 : -1];
typedef char FrontMessageRuntimeSecondaryAt0C[
    (offsetof(FrontMessageRuntimeView, secondary0C) == 0x0C) ? 1 : -1];

typedef char FrontMessageRuntimeNativeConstructedMembers[
    (offsetof(FrontMessageRuntimeView, messageVms14) == 0x14 &&
     offsetof(FrontMessageRuntimeView, messageVms1534) == 0x1534 &&
     offsetof(FrontMessageRuntimeView, messageVm1A7C) == 0x1A7C &&
     offsetof(FrontMessageRuntimeView, timer1D40) == 0x1D40 &&
     offsetof(FrontMessageRuntimeView, textColors1D20) == 0x1D20 &&
     offsetof(FrontMessageRuntimeView, shadowColors1D30) == 0x1D30 &&
     offsetof(FrontMessageRuntimeView, timer1D58) == 0x1D58 &&
     offsetof(FrontMessageRuntimeView, textSelectorCount1D50) == 0x1D50 &&
     offsetof(FrontMessageRuntimeView, glyphSize1D54) == 0x1D54 &&
     offsetof(FrontMessageRuntimeView, colorIndex1D64) == 0x1D64 &&
     offsetof(FrontMessageRuntimeView, boxVisible1D6D) == 0x1D6D) ? 1 : -1];
