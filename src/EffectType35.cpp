#include "EffectManager.hpp"
#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"
#include "PlayerSpawnShotsView.hpp"

// The native animation VM shares the exact 0x2A4-byte Effect VM stride;
// Type35 mutates real +0x1F8 flags and +0x288..+0x290 secondary position.
typedef char EffectType35NativeVmSlots[
    (sizeof(EffectVmView) == sizeof(AnmVm) &&
     offsetof(AnmVmBase, flagsWord) == 0x1F8 &&
     offsetof(AnmVm, pos2) == 0x288) ? 1 : -1];

struct EffectShotSideView
{
    unsigned char unknown00[4];
    PlayerSpawnShotsView *player04;
};

int __fastcall EffectInitializeCallbackType35(Effect *effect)
{
    reinterpret_cast<EffectShotSideView *>(effect->manager->sideState)->player04
        ->anmFileBC->SetAndExecuteScriptIdx(reinterpret_cast<AnmVm *>(effect->vms), 7);
    reinterpret_cast<AnmVm *>(effect->vms)->flagsWord |= 0x2000;
    reinterpret_cast<AnmVm *>(effect->vms)->pos2.x = 0.0f;
    reinterpret_cast<AnmVm *>(effect->vms)->pos2.y = 0.0f;
    reinterpret_cast<AnmVm *>(effect->vms)->pos2.z = 0.0f;
    return 0;
}

int __fastcall EffectUpdateCallbackType35(Effect *effect)
{
    reinterpret_cast<EffectShotSideView *>(effect->manager->sideState)->player04
        ->SpawnShots(effect->timer, 2);
    return 1;
}
