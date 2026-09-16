#include "AnmManager.hpp"
#include "AsciiManager.hpp"

#include <stddef.h>

struct SupervisorLoadingSetupView
{
    unsigned char unknown000[0x5D0];
    AnmLoaded *loadingAnm;
    unsigned char unknown5D4[0x740 - 0x5D4];
    int loadingVmsHaveBeenSetup;

    void SetupLoadingVms(Float3 *position);
};

typedef char SupervisorLoadingAnmAt5D0[
    (offsetof(SupervisorLoadingSetupView, loadingAnm) == 0x5D0) ? 1 : -1];
typedef char SupervisorLoadingSetupAt740[
    (offsetof(SupervisorLoadingSetupView, loadingVmsHaveBeenSetup) == 0x740) ? 1 : -1];

extern AnmVm g_SupervisorLoadingVms[3];

void SupervisorLoadingSetupView::SetupLoadingVms(Float3 *position)
{
    if (this->loadingVmsHaveBeenSetup != 0)
        return;

    this->loadingAnm->ExecuteAnmIdx(&g_SupervisorLoadingVms[0], 0);
    this->loadingAnm->ExecuteAnmIdx(&g_SupervisorLoadingVms[1], 1);
    this->loadingAnm->ExecuteAnmIdx(&g_SupervisorLoadingVms[2], 2);
    this->loadingVmsHaveBeenSetup = 1;

    g_SupervisorLoadingVms[0].pos = *position;
    g_SupervisorLoadingVms[1].pos = *position;
    g_SupervisorLoadingVms[2].pos = *position;
}
