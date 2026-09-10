#include "Supervisor.hpp"

#include <windows.h>

namespace
{
struct SupervisorLockLayout
{
    // Only the lock-related offsets below are established for TH09 so far.
    unsigned char unknown_0000[0x6C0];
    CRITICAL_SECTION criticalSections[5];
    unsigned char lockCounts[5];
};
}

void Supervisor::EnterCriticalSectionWrapper(int id)
{
    SupervisorLockLayout *layout = reinterpret_cast<SupervisorLockLayout *>(this);
    EnterCriticalSection(&layout->criticalSections[id]);
    layout->lockCounts[id]++;
}

void Supervisor::LeaveCriticalSectionWrapper(int id)
{
    SupervisorLockLayout *layout = reinterpret_cast<SupervisorLockLayout *>(this);
    LeaveCriticalSection(&layout->criticalSections[id]);
    layout->lockCounts[id]--;
}
