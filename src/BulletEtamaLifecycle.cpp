#include "AnmManager.hpp"

int PreloadEtamaStartup()
{
    return g_AnmManager->PreloadAnm(8, "etama.anm") == 0;
}

void ReleaseEtamaAnm()
{
    g_AnmManager->ReleaseAnm(8);
}
