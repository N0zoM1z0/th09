#include "GameManagerRuntimeView.hpp"
#include "FrontMessageOwnerView.hpp"
#include "FrontFactoryConstructView.hpp"

#include <stddef.h>


void FrontFactoryConstructView::InitializeMessageRuntime()
{
    if (g_GameManager.IsGameMode0() || g_GameManager.IsGameMode1())
    {
        messageRuntimeE944.Setup(
            g_GameManager.value100 == 0
                ? g_GameManager.frontMessageInitSourceF0->messageIndex06
                : 201,
            0);
    }
    else
    {
        messageRuntimeE944.Setup(
            g_GameManager.value100 == 0
                ? g_GameManager.frontMessageInitSourceF0->messageIndex06
                : 201,
            0);
    }
    transitionVmCount1095C = 0;
    transitionFrame10960 = 0;
    transitionSide10964 = 0;
    transitionState11EA8 = 0;
}
