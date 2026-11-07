#include "Supervisor.hpp"
#include <stddef.h>

// The two side heads at Supervisor +0x480/+0x4F8 are the frame fields
// of entries[0] and entries[10] within the original twenty 0x0C records.
// No separate 0x78-stride view is needed for this exact short predicate.
typedef char SupervisorNativeSyncQueues[
    (offsetof(Supervisor, entries47C) == 0x47C &&
     sizeof(((Supervisor *)0)->entries47C[0]) == 0x0C) ? 1 : -1];

int Supervisor::AreFrameQueuesSynchronized(int side)
{
    Supervisor *view = static_cast<Supervisor *>(this);
    if (view->entries47C[side * 10].frame04 != 0 &&
        view->entries47C[10].frame04 == view->entries47C[0].frame04)
        return 1;
    return 0;
}
