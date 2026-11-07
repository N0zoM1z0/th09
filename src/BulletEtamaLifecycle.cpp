#include "AnmManager.hpp"
#include "EtamaAnmResourceSlot.hpp"

int PreloadEtamaStartup()
{
    return g_AnmManager->PreloadAnm(ANM_FILE_SLOT_ETAMA, "etama.anm") == 0;
}

void ReleaseEtamaAnm()
{
    g_AnmManager->ReleaseAnm(ANM_FILE_SLOT_ETAMA);
}
