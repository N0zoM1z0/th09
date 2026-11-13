#include <windows.h>
#include <stddef.h>

#include "Supervisor.hpp"

// Both lock-lifetime leaves operate on the same five actual sections in
// the full Supervisor process-global, not a separate short prefix view.
void Supervisor::InitializeCriticalSections()
{
    for (int i = 0; i < 5; ++i)
        InitializeCriticalSection(&criticalSections6C0[i]);
}

void Supervisor::DeleteCriticalSections()
{
    for (int i = 0; i < 5; ++i)
        DeleteCriticalSection(&criticalSections6C0[i]);
}
