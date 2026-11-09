#include "Chain.hpp"
#include "FrontFactoryConstructView.hpp"
#include "ZunMemory.hpp"

#include <stddef.h>

extern Chain g_Chain;

int FrontCalcCallback(void *front);
int FrontDrawCallback(void *front);
int FrontAddedCallback(void *front);

void *__fastcall CreateGameSubsystem()
{
    FrontFactoryConstructView *constructed = new FrontFactoryConstructView;
    FrontFactoryConstructView *front = static_cast<FrontFactoryConstructView *>(
        g_ZunMemory.AddToRegistry(
            constructed, sizeof(FrontFactoryConstructView),
            const_cast<char *>("FrontInf")));

    front->Reset();

    front->calcChain00 =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(FrontCalcCallback));
    front->calcChain00->arg = front;
    front->calcChain00->addedCallback =
        reinterpret_cast<ChainLifetimeCallback>(FrontAddedCallback);
    if (g_Chain.AddToCalcChain(front->calcChain00, 22) != 0)
        return 0;

    front->drawChain04 =
        g_Chain.CreateElem(reinterpret_cast<ChainCallback>(FrontDrawCallback));
    front->drawChain04->arg = front;
    g_Chain.AddToDrawChain(front->drawChain04, 28);
    return front;
}
