#include "ReplayRuntimeLeaves.hpp"
#include "SupervisorFrameQueueView.hpp"
#include "SupervisorFrameQueueStorageView.hpp"
#include "SupervisorNetworkState.hpp"
#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include <stddef.h>

struct TitleSupervisorView
{
    unsigned char unknown000[0x458];
    int frameCounter458;
    int confirmedFrame45C;
    int frameMode460;
    unsigned int frameStartTime464;
    unsigned int lastFrameTime468;
    unsigned int waitTime46C;
    unsigned int unknown470;
    unsigned int connectionTime474;
    int networkBusy478;
    // These are the exact 0x0C-byte entries consumed by the Supervisor
    // frame-queue APIs, not an unrelated byte or integer allocation.
    SupervisorFrameEntryStorageView frameQueues47C[20];
    SupervisorFrameEntryStorageView lastReceived56C[2];
};

typedef char TitleResetFrameAt458[
    (offsetof(TitleSupervisorView, frameCounter458) == 0x458) ? 1 : -1];
typedef char TitleResetQueuesAt47C[
    (offsetof(TitleSupervisorView, frameQueues47C) == 0x47C) ? 1 : -1];
typedef char TitleResetLastReceivedAt56C[
    (offsetof(TitleSupervisorView, lastReceived56C) == 0x56C) ? 1 : -1];
typedef char TitleResetQueueBytesAreF0[
    (sizeof(((TitleSupervisorView *)0)->frameQueues47C) == 0xF0) ? 1 : -1];
typedef char TitleResetLastReceivedBytesAre18[
    (sizeof(((TitleSupervisorView *)0)->lastReceived56C) == 0x18) ? 1 : -1];

extern TitleSupervisorView g_TitleSupervisor;
extern ReplayRngView g_ReplayRng;
extern void __fastcall SupervisorNetworkMarkActive(void *state);

void __fastcall ResetTitleMode4Supervisor(TitleSupervisorView *supervisor)
{
    supervisor->frameCounter458 = 0;
    supervisor->confirmedFrame45C = 0;
    supervisor->frameMode460 = 0;

    unsigned int now = timeGetTime();
    supervisor->frameStartTime464 = now;
    supervisor->lastFrameTime468 = now;
    supervisor->connectionTime474 = now;
    supervisor->networkBusy478 = 0;

    memset(supervisor->frameQueues47C, 0, sizeof(supervisor->frameQueues47C));
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
