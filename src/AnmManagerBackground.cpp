#include "AnmManager.hpp"
#include "AnmManagerFileSlotView.hpp"
#include "AnmManagerBackgroundView.hpp"

#include <stddef.h>

static __inline AnmManagerBackgroundView *BackgroundManager(AnmManager *manager)
{
    return reinterpret_cast<AnmManagerBackgroundView *>(manager);
}

void AnmManager::SetCameraMode(int mode)
{
    BackgroundManager(this)->cameraMode = (unsigned char)mode;
}

void AnmManager::SetMixColorDefault()
{
    BackgroundManager(this)->useMixColor = 0;
    BackgroundManager(this)->mixColor = 0x80808080;
}

void AnmManager::SetMixColor(unsigned long color)
{
    BackgroundManager(this)->useMixColor = 1;
    BackgroundManager(this)->mixColor = color;
}

void AnmManager::Draw2DAndFlush(AnmVm *vm)
{
    this->Draw2D(vm);
    this->FlushVertexBuffer();
}

AnmLoaded *AnmManager::GetAnm(int anmIdx)
{
    AnmManagerReadView *manager = reinterpret_cast<AnmManagerReadView *>(this);
    // The exact resource loader addresses these same real 0x120-byte
    // file records from manager +0x24, not a shadow ANM instance.
    return &reinterpret_cast<AnmManagerReadSlotView *>(
        reinterpret_cast<unsigned int>(&manager->slots[0]) +
        static_cast<unsigned int>(anmIdx) * sizeof(manager->slots[0]))->loaded;
}
