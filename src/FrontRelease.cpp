#include "Chain.hpp"
#include "FrontFactoryConstructView.hpp"
#include "StageResourcePolicy.hpp"

#include <stddef.h>
#include <new>
#include <stdlib.h>

extern Chain g_Chain;
extern int g_GameSide1Value20;
extern int g_StageResourceReleasePolicy;

int ShouldReleaseStageResources()
{
    return g_StageResourceReleasePolicy;
}

// The original +0xE944 message subobject and chain heads are members of
// the constructed 0x1293C-byte Front object, not a separate release view.
typedef char NativeFrontReleaseChainAndMessageOffsets[
    (offsetof(FrontFactoryConstructView, calcChain00) == 0x00 &&
     offsetof(FrontFactoryConstructView, drawChain04) == 0x04 &&
     offsetof(FrontFactoryConstructView, messageRuntimeE944) == 0xE944) ? 1 : -1];

int FrontFactoryConstructView::ReleaseCore()
{
    if (ShouldReleaseStageResources())
        messageRuntimeE944.ReleaseForSideCount(10 * g_GameSide1Value20);

    return 0;
}

void __fastcall ReleaseGameSubsystem(void *object)
{
    FrontFactoryConstructView *front = static_cast<FrontFactoryConstructView *>(object);
    if (front != NULL)
    {
        front->ReleaseCore();
        g_Chain.Cut(front->drawChain04);
        g_Chain.Cut(front->calcChain00);
        operator delete(front);
    }
}
