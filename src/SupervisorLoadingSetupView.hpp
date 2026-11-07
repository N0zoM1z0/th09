#pragma once

#include "SupervisorThreadStartView.hpp"
#include "Float3.hpp"
#include <stddef.h>

// The exact 173-byte loading-VM method acts on the shared physical Supervisor
// allocation at +0x5D0/+0x740, not on a second private byte-prefix object.
// Keep its independently proven method symbol and source-facing receiver.
struct SupervisorLoadingSetupView : SupervisorThreadStartView
{
    void SetupLoadingVms(Float3 *position);
};

typedef char SupervisorLoadingSetupNativeStorage[
    (sizeof(SupervisorLoadingSetupView) == sizeof(Supervisor) &&
     offsetof(SupervisorLoadingSetupView, loadingAnm5D0) == 0x5D0 &&
     offsetof(SupervisorLoadingSetupView, loadingVmsHaveBeenSetup) == 0x740) ? 1 : -1];
