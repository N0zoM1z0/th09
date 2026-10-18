#include "GameManagerMode.hpp"
#include "FrontMessageOwnerView.hpp"

#include <stddef.h>

extern FrontMessageInitSourceView *g_FrontMessageSource;
extern int g_FrontMessageOverride;

void FrontMessageOwnerView::InitializeMessageRuntime()
{
    if (g_GameManager.IsGameMode0() || g_GameManager.IsGameMode1())
    {
        messageRuntimeE944.Setup(
            g_FrontMessageOverride == 0
                ? g_FrontMessageSource->messageIndex06
                : 201,
            0);
    }
    else
    {
        messageRuntimeE944.Setup(
            g_FrontMessageOverride == 0
                ? g_FrontMessageSource->messageIndex06
                : 201,
            0);
    }
    transitionVmCount1095C = 0;
    transitionFrame10960 = 0;
    transitionSide10964 = 0;
    transitionState11EA8 = 0;
}
