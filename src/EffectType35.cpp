#include "EffectManager.hpp"
#include "AnmManager.hpp"
#include "PlayerSpawnShotsView.hpp"

struct EffectShotSideView
{
    unsigned char unknown00[4];
    PlayerSpawnShotsView *player04;
};

int __fastcall EffectInitializeCallbackType35(Effect *effect)
{
    reinterpret_cast<EffectShotSideView *>(effect->manager->sideState)->player04
        ->anmFileBC->SetAndExecuteScriptIdx(reinterpret_cast<AnmVm *>(effect->vms), 7);
    effect->vms->flags |= 0x2000;
    effect->vms->pos2.x = 0.0f;
    effect->vms->pos2.y = 0.0f;
    effect->vms->pos2.z = 0.0f;
    return 0;
}

int __fastcall EffectUpdateCallbackType35(Effect *effect)
{
    reinterpret_cast<EffectShotSideView *>(effect->manager->sideState)->player04
        ->SpawnShots(effect->timer, 2);
    return 1;
}
