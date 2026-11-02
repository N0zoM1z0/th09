#include "Supervisor.hpp"

#include <stddef.h>

// The exact 14-byte subthread predicate belongs to the same Supervisor
// +0x6AC thread-handle storage as the complete gameplay setup owner.
typedef char SupervisorSubthreadHandleAt6AC[
    (offsetof(Supervisor, runningSubthreadHandle) == 0x6AC) ? 1 : -1];

int __fastcall SupervisorSubthreadIsRunning(Supervisor *supervisor)
{
    return supervisor->runningSubthreadHandle != 0;
}
