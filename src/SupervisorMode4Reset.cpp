#include "ReplayRuntimeLeaves.hpp"
#include "SupervisorFrameQueueView.hpp"
#include "Supervisor.hpp"
#include "SupervisorFrameQueueStorageView.hpp"
#include "SupervisorNetworkState.hpp"
#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include <stddef.h>

// Keep the existing target-bound function argument and global symbol name,
// but share the real Supervisor's 0x7B0 storage.  No alternate 0x584-byte
// network/queue object is allocated in TH09.
struct TitleSupervisorView : Supervisor {};
typedef char TitleResetNativeSupervisorState[
    (sizeof(TitleSupervisorView) == sizeof(Supervisor) &&
     offsetof(TitleSupervisorView, networkTiming458) == 0x458 &&
     offsetof(TitleSupervisorView, networkBusy478) == 0x478 &&
     offsetof(TitleSupervisorView, entries47C) == 0x47C &&
     sizeof(((TitleSupervisorView *)0)->entries47C) == 0xF0 &&
     offsetof(TitleSupervisorView, lastReceived56C) == 0x56C &&
     sizeof(((TitleSupervisorView *)0)->lastReceived56C) == 0x18) ? 1 : -1];

extern TitleSupervisorView g_TitleSupervisor;
extern ReplayRngView g_ReplayRng;
extern void __fastcall SupervisorNetworkMarkActive(void *state);

void __fastcall ResetTitleMode4Supervisor(TitleSupervisorView *supervisor)
{
    supervisor->networkTiming458.frameCounter = 0;
    supervisor->networkTiming458.confirmedFrame = 0;
    supervisor->networkTiming458.frameMode = 0;

    unsigned int now = timeGetTime();
    supervisor->networkTiming458.frameStartTime = now;
    supervisor->networkTiming458.lastFrameTime = now;
    supervisor->networkTiming458.connectionTime = now;
    supervisor->networkBusy478 = 0;

    memset(supervisor->entries47C, 0, sizeof(supervisor->entries47C));
    memset(supervisor->lastReceived56C, 0, sizeof(supervisor->lastReceived56C));

    SupervisorFrameQueueView *frameQueues =
        reinterpret_cast<SupervisorFrameQueueView *>(&g_TitleSupervisor);

    frameQueues->InsertReceivedFrame(
        0, -9999, 0, (short)g_ReplayRng.GetSeed());
    frameQueues->InsertReceivedFrame(
        1, -9999, 0, (short)g_ReplayRng.GetSeed());

    SupervisorNetworkMarkActive(g_SupervisorNetworkState);
    g_SupervisorNetworkState->connectionState = 0;
}
