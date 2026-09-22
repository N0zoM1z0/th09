#include "GameManagerMode.hpp"

#include <stddef.h>

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

extern FrontMessageInitSourceView *g_FrontMessageSource;
extern int g_FrontMessageOverride;

void FrontMessageOwnerView::InitializeMessageRuntime()
{
    int messageIndex;

    if (g_GameManager.IsGameMode0() || g_GameManager.IsGameMode1())
    {
        if (g_FrontMessageOverride == 0)
            messageIndex = g_FrontMessageSource->messageIndex06;
        else
            messageIndex = 201;
    }
    else
    {
        if (g_FrontMessageOverride == 0)
            messageIndex = g_FrontMessageSource->messageIndex06;
        else
            messageIndex = 201;
    }

    messageRuntimeE944.Setup(messageIndex, 0);
    transitionVmCount1095C = 0;
    transitionFrame10960 = 0;
    transitionSide10964 = 0;
    transitionState11EA8 = 0;
}
