#pragma once

#include <stddef.h>

// The independently reviewed Front/shared message-runtime owner: the same
// 0xE944 offset and thiscall are consumed by gameplay cleanup and added callbacks.
// Names are reconstruction views, not original retail identifiers.
struct FrontMessageSetupView
{
    void Setup(int messageIndex, int mode);
};

struct FrontMessageInitSourceView
{
    unsigned char unknown00[0x06];
    short messageIndex06;
};

struct FrontMessageOwnerView
{
    unsigned char unknown00000[0xE944];
    FrontMessageSetupView messageRuntimeE944;
    unsigned char unknown0E948[0x1095C - 0xE948];
    int transitionVmCount1095C;
    int transitionFrame10960;
    int transitionSide10964;
    unsigned char unknown10968[0x11EA8 - 0x10968];
    int transitionState11EA8;

    void InitializeMessageRuntime();
};

typedef char FrontMessageInitRuntimeAtE944[
    (offsetof(FrontMessageOwnerView,messageRuntimeE944)==0xE944)?1:-1];
typedef char FrontMessageInitVmCountAt1095C[
    (offsetof(FrontMessageOwnerView,transitionVmCount1095C)==0x1095C)?1:-1];
typedef char FrontMessageInitStateAt11EA8[
    (offsetof(FrontMessageOwnerView,transitionState11EA8)==0x11EA8)?1:-1];
typedef char FrontMessageOwnerSizeIs11EAC[
    (sizeof(FrontMessageOwnerView)==0x11EAC)?1:-1];
typedef char FrontMessageInitFrameAt10960[
    (offsetof(FrontMessageOwnerView,transitionFrame10960)==0x10960)?1:-1];
typedef char FrontMessageInitSideAt10964[
    (offsetof(FrontMessageOwnerView,transitionSide10964)==0x10964)?1:-1];
