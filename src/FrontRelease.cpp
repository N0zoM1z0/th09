#include "Chain.hpp"

#include <stddef.h>
#include <stdlib.h>

extern Chain g_Chain;

struct FrontReleaseView
{
    ChainElem *calcChain00;
    ChainElem *drawChain04;

    void ReleaseCore();
};

typedef char FrontReleaseCalcAt00[
    (offsetof(FrontReleaseView, calcChain00) == 0x00) ? 1 : -1];
typedef char FrontReleaseDrawAt04[
    (offsetof(FrontReleaseView, drawChain04) == 0x04) ? 1 : -1];

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
