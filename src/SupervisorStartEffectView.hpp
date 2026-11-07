#pragma once

#include "ScoreSupervisorView.hpp"

#include <stddef.h>

// The original-bound 47-byte effect-start method acts on the one physical
// Supervisor, independently of its source-facing method spelling. Keep the
// exact method ABI and reuse ScoreSupervisorView's real native storage.
struct SupervisorStartEffectView : ScoreSupervisorView
{
    void StartEffect(int index);
};

typedef char SupervisorStartEffectNativeStorage[
    (sizeof(SupervisorStartEffectView) == sizeof(Supervisor) &&
     offsetof(SupervisorStartEffectView, flags5D4) == 0x5D4 &&
     offsetof(SupervisorStartEffectView, screenTransitionCountdown) == 0x5A4) ? 1 : -1];
