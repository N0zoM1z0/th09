#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "Chain.hpp"

extern Chain g_Chain;
extern ChainElem g_AsciiManagerCalcChain;
extern ChainElem g_AsciiManagerDrawChainLowPrio;

int AsciiManager::DeletedCallback(AsciiManager *)
{
    g_AnmManager->ReleaseAnm(ANM_FILE_SLOT_ASCII);
    g_AnmManager->ReleaseAnm(ANM_FILE_SLOT_CAPTURE);
    return 0;
}

void AsciiManager::CutChain()
{
    g_Chain.Cut(&g_AsciiManagerCalcChain);
    g_Chain.Cut(&g_AsciiManagerDrawChainLowPrio);
}
