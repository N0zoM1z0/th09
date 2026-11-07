#pragma once

#include "Supervisor.hpp"

#include <stddef.h>

// Score.dat's timekeeping methods keep their independently original-bound
// ScoreSupervisorView ABI, but borrow the one true 0x7B0-byte Supervisor
// allocation rather than a second 0x798-byte prefix layout.
struct ScoreSupervisorView : Supervisor {
    void UpdatePlayTime();
    void UpdateGameTime();
};

typedef char ScoreSupervisorNativeStorage[
    (sizeof(ScoreSupervisorView) == sizeof(Supervisor) &&
     offsetof(ScoreSupervisorView, totalPlayTime) == 0x798 &&
     offsetof(ScoreSupervisorView, systemTime) == 0x79C &&
     offsetof(ScoreSupervisorView, exeChecksum7A0) == 0x7A0 &&
     offsetof(ScoreSupervisorView, exeSize7A4) == 0x7A4) ? 1 : -1];
