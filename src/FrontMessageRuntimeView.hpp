#pragma once

#include "FrontMessageTableView.hpp"
#include "AnmVmLifecycle.hpp"
#include "ZunTimer.hpp"

// Load/release use this same bounded prefix of the embedded 0x1D70 runtime.
// Update/draw expose richer overlays; this is not full original class recovery.
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
    unsigned char unknown1D20[0x20];
    ZunTimer timer1D40;
    unsigned char unknown1D4C[0x0C];
    ZunTimer timer1D58;
    unsigned char unknown1D64[0x0C];

    FrontMessageRuntimeView();
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
     offsetof(FrontMessageRuntimeView, timer1D58) == 0x1D58) ? 1 : -1];
