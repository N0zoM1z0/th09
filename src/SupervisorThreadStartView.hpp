#pragma once

#include "SupervisorStartEffectView.hpp"
#include <windows.h>
#include <stddef.h>

// These two 69/108-byte exact original methods are the same concrete
// Supervisor allocation. Preserve their original source-facing symbols
// while consuming its real Win32 handle, ID and thread lifecycle words.
struct SupervisorThreadStartView : SupervisorStartEffectView
{
    void ThreadClose();
    int ThreadStart(LPTHREAD_START_ROUTINE startFunction, void *startParam);
};

typedef char SupervisorThreadStartNativeStorage[
    (sizeof(SupervisorThreadStartView) == sizeof(Supervisor) &&
     offsetof(SupervisorThreadStartView, runningSubthreadHandlePtr) == 0x6AC &&
     offsetof(SupervisorThreadStartView, runningSubthreadId6B0) == 0x6B0 &&
     offsetof(SupervisorThreadStartView, subthreadCloseRequestActive) == 0x6B4 &&
     offsetof(SupervisorThreadStartView, subthreadActive) == 0x6B8) ? 1 : -1];
