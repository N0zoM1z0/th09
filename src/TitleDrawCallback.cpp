#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

struct TitleDrawView
{
    unsigned char unknown00000[0x11B98];
    AnmVm *vms11B98;
    AnmVm *auxVm11B9C;
    unsigned char unknown11BA0[0x1B224 - 0x11BA0];
    int vmCount1B224;
    int currentScreen1B228;
    unsigned char unknown1B22C[0x1B238 - 0x1B22C];
    int chainState1B238;

    int DrawReplayMenu();
    int DrawReplaySave();
    int DrawResult();
    int DrawMusicRoom();
};

typedef char TitleDrawVmsAt11B98[
    (offsetof(TitleDrawView, vms11B98) == 0x11B98) ? 1 : -1];
typedef char TitleDrawVmCountAt1B224[
    (offsetof(TitleDrawView, vmCount1B224) == 0x1B224) ? 1 : -1];
typedef char TitleDrawScreenAt1B228[
    (offsetof(TitleDrawView, currentScreen1B228) == 0x1B228) ? 1 : -1];
typedef char TitleDrawChainStateAt1B238[
    (offsetof(TitleDrawView, chainState1B238) == 0x1B238) ? 1 : -1];

int __fastcall TitleDrawCallback(void *object)
{
    TitleDrawView *title = static_cast<TitleDrawView *>(object);

    g_Supervisor.ConfigureGameplayViewport(2);
    if (title->chainState1B238 == 0)
    {
        *reinterpret_cast<void **>(
            reinterpret_cast<unsigned char *>(g_AnmManager) + 0x12880) = 0;
        g_AnmManager->CopySurfaceToBackbuffer(0, 0, 0, 0, 0);

        AnmVm *vm = title->vms11B98;
        for (int i = 0; i < title->vmCount1B224; ++i, ++vm)
        {
            if (g_AnmManager->SpriteHasTexture(vm))
            {
                Float3 savedPosition = vm->pos;
                vm->pos += vm->pos2;

                if (*reinterpret_cast<float *>(
                        reinterpret_cast<unsigned char *>(vm) + 0x08) == 0.0f)
                    g_AnmManager->DrawNoRotation(vm);
                else
                    g_AnmManager->Draw2D(vm);

                vm->pos = savedPosition;
            }
        }

        if (title->auxVm11B9C != 0)
            g_AnmManager->DrawNoRotation(title->auxVm11B9C);

        switch (title->currentScreen1B228)
        {
        case 3:
        case 4:
        case 5:
            break;
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

        g_AnmManager->FlushVertexBuffer();
    }
    return 1;
}
