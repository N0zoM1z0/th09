#include "EffectManager.hpp"
#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"
#include "PlayerLifecycleView.hpp"

// The native animation VM shares the exact 0x2A4-byte Effect VM stride;
// Type35 mutates real +0x1F8 flags and +0x288..+0x290 secondary position.
typedef char EffectType35NativeVmSlots[
    (sizeof(AnmVm) == 0x2A4 &&
     offsetof(AnmVmBase, flagsWord) == 0x1F8 &&
     offsetof(AnmVm, pos2) == 0x288) ? 1 : -1];

// EffectManager::Create publishes the genuine 0x38-byte GameManager side
// allocation for playable sides. Type35 borrows its real Player pointer,
// instead of defining a second four-byte-prefix/Player side class.
typedef char EffectType35NativeSidePlayer[
    (sizeof(PlayerSideStateView) == 0x38 &&
     offsetof(PlayerSideStateView, player04) == 0x04 &&
     offsetof(PlayerLifecycleView, header24) +
         offsetof(PlayerHeaderStateView, anmFile98) == 0xBC) ? 1 : -1];

int __fastcall EffectInitializeCallbackType35(Effect *effect)
{
    reinterpret_cast<PlayerSideStateView *>(effect->manager->sideState)->player04
        ->header24.anmFile98->SetAndExecuteScriptIdx(reinterpret_cast<AnmVm *>(effect->vms), 7);
    reinterpret_cast<AnmVm *>(effect->vms)->flagsWord |= 0x2000;
    reinterpret_cast<AnmVm *>(effect->vms)->pos2.x = 0.0f;
    reinterpret_cast<AnmVm *>(effect->vms)->pos2.y = 0.0f;
    reinterpret_cast<AnmVm *>(effect->vms)->pos2.z = 0.0f;
    return 0;
}

int __fastcall EffectUpdateCallbackType35(Effect *effect)
{
    reinterpret_cast<PlayerSideStateView *>(effect->manager->sideState)->player04
        ->SpawnShots(effect->timer, 2);
    return 1;
}
