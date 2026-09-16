#include "Supervisor.hpp"

#include <stddef.h>

struct SupervisorSubthreadView
{
    unsigned char unknown000[0x6AC];
    int runningSubthreadHandle;
};

typedef char SupervisorSubthreadHandleAt6AC[
    (offsetof(SupervisorSubthreadView, runningSubthreadHandle) == 0x6AC) ? 1 : -1];

int __fastcall SupervisorSubthreadIsRunning(Supervisor *supervisor)
{
    SupervisorSubthreadView *view =
        reinterpret_cast<SupervisorSubthreadView *>(supervisor);
    return view->runningSubthreadHandle != 0;
}
