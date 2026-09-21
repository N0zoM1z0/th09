#include "Chain.hpp"
#include "FrontSupport.hpp"
#include "ZunMemory.hpp"

#include <stddef.h>

struct FrontFactoryConstructView
{
    unsigned char storage[0x1293C];
    FrontFactoryConstructView();
};

struct FrontFactoryView
{
    ChainElem *calcChain00;
    ChainElem *drawChain04;
    unsigned char storage008[0x1293C - 0x08];
};
typedef char FrontFactorySizeIs1293C[
    (sizeof(FrontFactoryView) == 0x1293C) ? 1 : -1];

extern Chain g_Chain;

int FrontCalcCallback(void *front);
int FrontDrawCallback(void *front);
int FrontAddedCallback(void *front);

void *__fastcall CreateGameSubsystem()
{
    FrontFactoryConstructView *constructed = new FrontFactoryConstructView;
    FrontFactoryView *front = reinterpret_cast<FrontFactoryView *>(
        g_ZunMemory.AddToRegistry(
            constructed, sizeof(FrontFactoryView), const_cast<char *>("FrontInf")));

    reinterpret_cast<Front *>(front)->Reset();

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
