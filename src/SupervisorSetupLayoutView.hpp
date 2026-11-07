#pragma once

#include "SupervisorLoadingSetupView.hpp"
#include <stddef.h>

// Shared native three-coordinate source range consumed by the exact 214-byte
// loading+capture installer and both independent gameplay/Title call sites.
struct SetupLoadingPosition
{
    float x;
    float y;
    float z;
};
typedef char SetupLoadingPositionNativeExtent[
    (sizeof(SetupLoadingPosition) == sizeof(Float3) &&
     offsetof(SetupLoadingPosition, x) == offsetof(Float3, x) &&
     offsetof(SetupLoadingPosition, y) == offsetof(Float3, y) &&
     offsetof(SetupLoadingPosition, z) == offsetof(Float3, z)) ? 1 : -1];

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
