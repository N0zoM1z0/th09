#pragma once

#include "FrontMessageTableView.hpp"

// Load/release use this same bounded prefix of the embedded 0x1D70 runtime.
// Update/draw expose richer overlays; this is not full original class recovery.
struct FrontMessageRuntimeView
{
    FrontMessageTableView *primary00;
    FrontMessageInstructionView *current04;
    int currentIndex08;
    FrontMessageTableView *secondary0C;
    unsigned char unknown10[0x1D70 - 0x10];

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
