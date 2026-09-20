#include "Chain.hpp"
#include "StageResourcePolicy.hpp"

#include <stddef.h>
#include <stdlib.h>

extern Chain g_Chain;
extern int g_GameSide1Value20;
extern int g_StageResourceReleasePolicy;

int ShouldReleaseStageResources()
{
    return g_StageResourceReleasePolicy;
}

struct FrontMessageRuntimeView
{
    void ReleaseForSideCount(int count);
};

struct FrontReleaseView
{
    ChainElem *calcChain00;
    ChainElem *drawChain04;
    unsigned char unknown008[0xE93C];
    FrontMessageRuntimeView messageRuntimeE944;

    int ReleaseCore();
};

typedef char FrontReleaseCalcAt00[
    (offsetof(FrontReleaseView, calcChain00) == 0x00) ? 1 : -1];
typedef char FrontReleaseDrawAt04[
    (offsetof(FrontReleaseView, drawChain04) == 0x04) ? 1 : -1];

typedef char FrontReleaseMessageRuntimeAtE944[
    (offsetof(FrontReleaseView, messageRuntimeE944) == 0xE944) ? 1 : -1];

int FrontReleaseView::ReleaseCore()
{
    if (ShouldReleaseStageResources())
        messageRuntimeE944.ReleaseForSideCount(10 * g_GameSide1Value20);

    return 0;
}

void __fastcall ReleaseGameSubsystem(void *object)
{
    FrontReleaseView *front = static_cast<FrontReleaseView *>(object);
    if (front != NULL)
    {
        front->ReleaseCore();
        g_Chain.Cut(front->drawChain04);
        g_Chain.Cut(front->calcChain00);
        free(front);
    }
}
