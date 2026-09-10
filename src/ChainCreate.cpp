#include "Chain.hpp"
#include "ZunMemory.hpp"

ChainElem *Chain::CreateElem(ChainCallback callback)
{
    ChainElem *elem = (ChainElem *)g_ZunMemory.AddToRegistry(
        new ChainElem(), sizeof(ChainElem), "funcChainInf");
    elem->SetCallback(callback);
    elem->isHeapAllocated = true;
    return elem;
}
