#include "AnmManager.hpp"

#include <stddef.h>

int PreloadFrontStartup()
{
    return g_AnmManager->PreloadAnm(10, "front.anm") == NULL;
}

void SupervisorDeletedReleaseSubsystemC()
{
    g_AnmManager->ReleaseAnm(10);
}
