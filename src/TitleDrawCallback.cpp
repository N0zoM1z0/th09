#define TH09_ANM_MANAGER_LIFECYCLE_LAYOUT 1
#include "AnmManager.hpp"
#include "TitleScreenReplayView.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

// The complete 0x1B388-byte TitleScreenView owns the VM prefix and all
// four draw methods; no second owner allocation or local shadow class.
typedef char TitleDrawFullOwnerCoordinates[
    (sizeof(TitleScreenView) == 0x1B388 &&
     offsetof(TitleScreenView, vms) == 0x11B98 &&
     offsetof(TitleScreenView, currentHelpTextVm) == 0x11B9C &&
     offsetof(TitleScreenView, vmCount) == 0x1B224 &&
     offsetof(TitleScreenView, currentScreen) == 0x1B228 &&
     offsetof(TitleScreenView, chainState) == 0x1B238 &&
     offsetof(AnmManager, currentTexture12880) == 0x12880) ? 1 : -1];
// The draw branch checks native VM rotation.z, not an unrelated float at
// a manually computed +0x08 byte offset.
typedef char TitleDrawNativeVmRotationZAt08[
    (offsetof(AnmVmBase, rotation) + offsetof(Float3, z) == 0x08) ? 1 : -1];

int __fastcall TitleDrawCallback(void *object)
{
    TitleScreenView *title = static_cast<TitleScreenView *>(object);

    g_Supervisor.ConfigureGameplayViewport(2);
    if (title->chainState == 0)
    {
        g_AnmManager->currentTexture12880 = NULL;
        g_AnmManager->CopySurfaceToBackbuffer(0, 0, 0, 0, 0);

        AnmVm *vm = title->vms;
        for (int i = 0; i < title->vmCount; ++i, ++vm)
        {
            if (g_AnmManager->SpriteHasTexture(vm))
            {
                Float3 savedPosition = vm->pos;
                vm->pos += vm->pos2;

                // Equivalent source comparison; the target schedules Draw2D
                // first and DrawNoRotation on the other branch.
                if (vm->rotation.z != 0.0f)
                    g_AnmManager->Draw2D(vm);
                else
                    g_AnmManager->DrawNoRotation(vm);

                vm->pos = savedPosition;
            }
        }

        if (title->currentHelpTextVm != 0)
            g_AnmManager->DrawNoRotation(title->currentHelpTextVm);

        switch (title->currentScreen)
        {
        // Distinct no-draw screen branches share the existing final flush.
        case 3:
            goto flush;
        case 4:
            break;
        case 5:
            goto flush;
        case 6:
            goto flush;
        case 11:
            title->DrawReplayMenu();
            break;
        case 12:
            title->DrawMusicRoom();
            break;
        case 13:
        case 14:
            title->DrawResult();
            break;
        case 15:
            title->DrawReplaySave();
            break;
        }
    flush:
        g_AnmManager->FlushVertexBuffer();
    }
    return 1;
}
