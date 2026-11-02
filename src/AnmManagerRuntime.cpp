#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>
#include <string.h>

typedef unsigned int u32;
typedef short i16;

// Use the single original-offset ANM VM base/tail instead of a second
// incompatible local 0x2A4-byte class. Runtime method signatures and
// compiler-visible loaded-VM ownership remain unchanged.

typedef char AnmRuntimeVmSizeIs2A4[(sizeof(AnmVm) == 0x2A4) ? 1 : -1];
typedef char AnmRuntimeFlagsAt1F8[(offsetof(AnmVm, flagsWord) == 0x1F8) ? 1 : -1];
typedef char AnmRuntimePendingAt1FE[(offsetof(AnmVm, pendingInterrupt) == 0x1FE) ? 1 : -1];
typedef char AnmRuntimeDrawTypeAt200[(offsetof(AnmVm, playerBulletHitAnimationType) == 0x200) ? 1 : -1];
typedef char AnmRuntimeActiveSpriteAt214[(offsetof(AnmVm, activeSpriteIndex) == 0x214) ? 1 : -1];
typedef char AnmRuntimeScriptAt21A[(offsetof(AnmVm, scriptIndex) == 0x21A) ? 1 : -1];
typedef char AnmRuntimeLoadedSpriteAt224[(offsetof(AnmVm, loadedSprite) == 0x224) ? 1 : -1];

struct AnmLoadedRuntimeView
{
    short anmIdx;
    unsigned char unknown002[0x0E];
    AnmRawInstr **scripts;
    unsigned char unknown014[0x04];
    int numberEntriesToBeLoaded;
};
typedef char AnmLoadedRuntimeSizeIs1C[(sizeof(AnmLoadedRuntimeView) == 0x1C) ? 1 : -1];

struct AnmFileRuntimeSlot
{
    void *textures;
    unsigned char unknown004[0x11C];
};
typedef char AnmFileRuntimeSlotSizeIs120[(sizeof(AnmFileRuntimeSlot) == 0x120) ? 1 : -1];

struct AnmManagerRuntimeView
{
    unsigned char unknown000[0x0C];
    int scriptsStartedThisFrame;
    unsigned char unknown010[0x28];
    AnmFileRuntimeSlot anmFiles[1];
};
typedef char AnmManagerRuntimeFilesAt38[(offsetof(AnmManagerRuntimeView, anmFiles) == 0x38) ? 1 : -1];

void AnmVm::SetInterrupt(i16 interrupt)
{
    pendingInterrupt = interrupt;
}

int AnmManager::SpriteHasTexture(AnmVm *vm)
{
    if (vm->loadedSprite == NULL)
    {
        return 0;
    }
    if (vm->loadedSprite->anmIdx < 0)
    {
        return 0;
    }
    return reinterpret_cast<AnmManagerRuntimeView *>(this)->anmFiles[vm->loadedSprite->anmIdx].textures != NULL;
}

void AnmManager::DrawPlayerBullet(AnmVm *vm)
{
    switch (vm->playerBulletHitAnimationType)
    {
    case 0:
        DrawNoRotation(vm);
        break;
    case 1:
        DrawNoRotationNoRound(vm);
        break;
    case 2:
        Draw2D(vm);
        break;
    case 3:
        Draw2DRotatedOrAxisAligned(vm);
        break;
    case 4:
        DrawCameraFacingQuad(vm);
        break;
    case 5:
        DrawProjected3DQuad(vm);
        break;
    }
}

void AnmManager::SetInterruptArray(AnmVm *vm, int count, i16 interrupt)
{
    while (count != 0)
    {
        if (g_AnmManager->SpriteHasTexture(vm))
        {
            vm->SetInterrupt(interrupt);
        }
        vm++;
        count--;
    }
}

void AnmManager::ExecuteScriptArray(AnmVm *vm, int count)
{
    while (count != 0)
    {
        if (vm->scriptIndex >= 0)
        {
            g_AnmManager->ExecuteScript(vm);
        }
        vm++;
        count--;
    }
}

void AnmLoaded::SetAndExecuteScript(AnmVm *vm, AnmRawInstr *beginningOfScript)
{
    AnmLoadedRuntimeView *self = reinterpret_cast<AnmLoadedRuntimeView *>(this);

    if (beginningOfScript == NULL || self->numberEntriesToBeLoaded != 0)
    {
        memset(vm, 0, sizeof(AnmVm));
    }
    else
    {
        vm->Initialize();
        vm->anmFileIndex = self->anmIdx;
        vm->anmFile = this;
        vm->flip = 0;
        vm->beginningOfScript = beginningOfScript;
        vm->currentInstruction = vm->beginningOfScript;
        vm->currentTimeInScript = 0;
        vm->visible = 0;
        g_AnmManager->ExecuteScript(vm);
        reinterpret_cast<AnmManagerRuntimeView *>(g_AnmManager)->scriptsStartedThisFrame++;
    }
}

void AnmLoaded::ExecuteAnmIdxArray(AnmVm *vm, int scriptIndex, int count)
{
    while (count != 0)
    {
        ExecuteAnmIdx(vm, scriptIndex);
        vm->baseSpriteIndex = vm->activeSpriteIndex;
        scriptIndex++;
        vm++;
        count--;
    }
}
