#include "Supervisor.hpp"
#include "SupervisorLoadingSetupView.hpp"
#include "AnmManager.hpp"
#include "AsciiManager.hpp"

#include <windows.h>

struct SupervisorAddedGameManagerView
{
    void ResetForSupervisorAdded();
};

// The 238-byte startup callback shares both its real 0x7B0-byte Supervisor
// allocation and exact VM/thread methods with the independent leaf owners.
struct SupervisorAddedLayout : SupervisorLoadingSetupView {};

typedef char SupervisorAddedSuppressFpsAt5A8[
    (offsetof(SupervisorAddedLayout, suppressFpsDisplay) == 0x5A8) ? 1 : -1];
typedef char SupervisorAddedDisableVsyncAt5AC[
    (offsetof(SupervisorAddedLayout, disableVsync) == 0x5AC) ? 1 : -1];
typedef char SupervisorAddedGameSpeedAt5B8[
    (offsetof(SupervisorAddedLayout, gameSpeed) == 0x5B8) ? 1 : -1];
typedef char SupervisorAddedLoadingAnmAt5D0[
    (offsetof(SupervisorAddedLayout, loadingAnm) == 0x5D0) ? 1 : -1];
typedef char SupervisorAddedStartupStateAt6BC[
    (offsetof(SupervisorAddedLayout, startupThreadState) == 0x6BC) ? 1 : -1];

extern SupervisorAddedGameManagerView g_GameManager;
extern int SupervisorAddedLoadDat();
extern int SupervisorCheckFps();
extern int SupervisorAddedCreateTextBuffer();

int Supervisor::AddedCallback(Supervisor *supervisor)
{
    SupervisorAddedLayout *globalSupervisor =
        reinterpret_cast<SupervisorAddedLayout *>(&g_Supervisor);
    SupervisorAddedLayout *callbackSupervisor =
        reinterpret_cast<SupervisorAddedLayout *>(supervisor);

    globalSupervisor->gameSpeed = 1.0f;
    g_GameManager.ResetForSupervisorAdded();

    if (SupervisorAddedLoadDat() != 0)
        return -1;

    g_AnmManager->LoadSurface(
        8, "title/th09logo.jpg");
    callbackSupervisor->loadingAnm =
        g_AnmManager->LoadAnm(
            2, "nowloading.anm");
    if (callbackSupervisor->loadingAnm == NULL)
    {
        g_AnmManager->ReleaseSurface(0);
        return -1;
    }

    globalSupervisor->suppressFpsDisplay = 1;
    if (!globalSupervisor->disableVsync && SupervisorCheckFps() != 0)
    {
        g_AnmManager->ReleaseSurface(0);
        return -2;
    }

    g_AnmManager->SetupVertexBuffer();
    SupervisorAddedCreateTextBuffer();

    Float3 position(500.0f, 440.0f, 0.0f);
    globalSupervisor->SetupLoadingVms(&position);
    globalSupervisor->startupThreadState = 1;
    globalSupervisor->ThreadStart(
        (LPTHREAD_START_ROUTINE)Supervisor::StartupThread, supervisor);
    return 0;
}
