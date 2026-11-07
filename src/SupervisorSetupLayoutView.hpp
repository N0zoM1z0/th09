#pragma once

#include "SupervisorLoadingSetupView.hpp"
#include <stddef.h>

struct SetupLoadingPosition;

// Both the 214-byte capture installer and gameplay setup callbacks operate
// on one Supervisor allocation, and share the exact 173-byte loading-VM API.
// Only source-facing method identities are asserted; there is no extra data.
struct SupervisorSetupLayout : SupervisorLoadingSetupView
{
    void SetupLoadingVmsAndInitCapture(SetupLoadingPosition *position);
};

typedef char SupervisorSetupLayoutNativeLoadingView[
    (sizeof(SupervisorSetupLayout) == sizeof(Supervisor) &&
     offsetof(SupervisorSetupLayout, loadingAnm5D0) == 0x5D0 &&
     offsetof(SupervisorSetupLayout, loadingVmsHaveBeenSetup) == 0x740) ? 1 : -1];
