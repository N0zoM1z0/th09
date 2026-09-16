#include "Background.hpp"
#include "Chain.hpp"

#include <stdlib.h>

extern Chain g_Chain;

void __fastcall ReleaseSubsystem0(void *object)
{
    Background *background = static_cast<Background *>(object);
    if (background != NULL)
    {
        g_Chain.Cut(background->calcChain);
        g_Chain.Cut(background->drawHighChain);
        g_Chain.Cut(background->drawLowChain);
        free(background);
    }
}
