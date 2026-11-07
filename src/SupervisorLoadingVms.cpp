#include "AnmManager.hpp"
#include "AsciiManager.hpp"

#include <stddef.h>

#include "SupervisorLoadingSetupView.hpp"

extern AnmVm g_SupervisorLoadingVms[3];

void SupervisorLoadingSetupView::SetupLoadingVms(Float3 *position)
{
    if (this->loadingVmsHaveBeenSetup != 0)
        return;

    this->loadingAnm5D0->ExecuteAnmIdx(&g_SupervisorLoadingVms[0], 0);
    this->loadingAnm5D0->ExecuteAnmIdx(&g_SupervisorLoadingVms[1], 1);
    this->loadingAnm5D0->ExecuteAnmIdx(&g_SupervisorLoadingVms[2], 2);
    this->loadingVmsHaveBeenSetup = 1;

    g_SupervisorLoadingVms[0].pos = *position;
    g_SupervisorLoadingVms[1].pos = *position;
    g_SupervisorLoadingVms[2].pos = *position;
}
