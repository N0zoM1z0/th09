#include "Supervisor.hpp"
#include "AnmManager.hpp"

#include <stddef.h>

namespace
{
struct SupervisorDrawFpsLayout
{
    unsigned char unknown000[0x478];
    int drawFpsVm;
};

typedef char SupervisorDrawFpsFieldAt478[
    (offsetof(SupervisorDrawFpsLayout, drawFpsVm) == 0x478) ? 1 : -1];
}

extern void __fastcall SupervisorCalculateFps(int shouldDraw);
extern AnmVm *g_SupervisorFpsVm;

int __fastcall SupervisorDrawFps(Supervisor *supervisor)
{
    SupervisorCalculateFps(1);

    SupervisorDrawFpsLayout *layout =
        reinterpret_cast<SupervisorDrawFpsLayout *>(supervisor);
    if (layout->drawFpsVm != 0)
    {
        g_Supervisor.ConfigureGameplayViewport(2);
        g_AnmManager->DrawNoRotation(g_SupervisorFpsVm);
    }

    return 1;
}
