#include "EffectManager.hpp"
#include "AnmManager.hpp"
#include "EtamaAnmResourceSlot.hpp"

int EffectManager::AddedCallback(void *context)
{
    AnmLoaded *loadedAnm = g_AnmManager->GetAnm(ANM_FILE_SLOT_ETAMA);
    if (loadedAnm->rawData04 == NULL)
        loadedAnm = g_AnmManager->PreloadAnm(ANM_FILE_SLOT_ETAMA, "etama.anm");
    static_cast<EffectManager *>(context)->effectAnm = loadedAnm;
    return 0;
}
