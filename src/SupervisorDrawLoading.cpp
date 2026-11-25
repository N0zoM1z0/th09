#include "Supervisor.hpp"
#include "AnmManager.hpp"
#include "AnmVmLifecycle.hpp"

#include <stddef.h>

extern AnmVm g_SupervisorLoadingVms[3];

// These three original 0x2A4-byte startup animation VMs are an actual
// global array. Each callback rereads the live corresponding element.

int __fastcall SupervisorDrawLoading(Supervisor *unused)
{
    int i = 0;
    int count = 3;
    do
    {
        g_SupervisorLoadingVms[i].pos += g_SupervisorLoadingVms[i].pos2;
        g_AnmManager->Draw2D(&g_SupervisorLoadingVms[i]);
        g_SupervisorLoadingVms[i].pos -= g_SupervisorLoadingVms[i].pos2;
        i++;
    } while (--count != 0);

    return 1;
}
