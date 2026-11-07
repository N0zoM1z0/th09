#pragma once
#include "ReplayManagerRuntimeView.hpp"
#include "ReplayInputState.hpp"
#include "GameConfiguration.hpp"
#include <stddef.h>

// The two gameplay-side configuration gates are separate from the third
// shared replay/controller input state; all three have true native storage.
struct ReplayNativeInputGateCapacity
{
    enum
    {
        SideCount = sizeof(((GameConfiguration *)0)->sideFocusMode) /
                    sizeof(((GameConfiguration *)0)->sideFocusMode[0]),
        StateCount = sizeof(g_ReplayInputStates) /
                     sizeof(g_ReplayInputStates[0]),
        SharedInputStateIndex = StateCount - 1
    };
};
typedef char ReplayNativeInputGateOriginalShape[
    (ReplayNativeInputGateCapacity::SideCount == 2 &&
     ReplayNativeInputGateCapacity::StateCount == 3 &&
     ReplayNativeInputGateCapacity::SharedInputStateIndex == 2 &&
     ReplayRecordingCapacity::InputStreamCount == ReplayNativeInputGateCapacity::StateCount &&
     sizeof(ReplayInputState) == 0x8E &&
     offsetof(GameConfiguration, sideFocusMode) == 0xB4) ? 1 : -1];
