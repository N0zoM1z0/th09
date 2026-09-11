#include "Supervisor.hpp"
#include "AnmManager.hpp"
#include "AsciiManager.hpp"

#include <windows.h>

struct SupervisorAddedGameManagerView
{
    void ResetForSupervisorAdded();
};

struct SupervisorAddedAnmManagerView
{
    int LoadSurface(int index, const char *filename);
    void *LoadAnm(int index, const char *filename);
    void ReleaseSurface(int index);
    int SetupVertexBuffer();
};

struct SupervisorAddedLayout
{
    unsigned char unknown000[0x5A8];
    int suppressFpsDisplay;
    int disableVsync;
    unsigned char unknown5B0[0x5B8 - 0x5B0];
    float gameSpeed;
    unsigned char unknown5BC[0x5D0 - 0x5BC];
    void *loadingAnm;
    unsigned char unknown5D4[0x6BC - 0x5D4];
    int startupThreadState;

    void SetupLoadingVms(Float3 *position);
    int ThreadStart(LPTHREAD_START_ROUTINE startFunction, void *startParam);
};

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

    reinterpret_cast<SupervisorAddedAnmManagerView *>(g_AnmManager)->LoadSurface(
        8, "title/th09logo.jpg");
    callbackSupervisor->loadingAnm =
        reinterpret_cast<SupervisorAddedAnmManagerView *>(g_AnmManager)->LoadAnm(
            2, "nowloading.anm");
    if (callbackSupervisor->loadingAnm == NULL)
    {
        reinterpret_cast<SupervisorAddedAnmManagerView *>(g_AnmManager)->ReleaseSurface(0);
        return -1;
    }

    globalSupervisor->suppressFpsDisplay = 1;
    if (!globalSupervisor->disableVsync && SupervisorCheckFps() != 0)
    {
        reinterpret_cast<SupervisorAddedAnmManagerView *>(g_AnmManager)->ReleaseSurface(0);
        return -2;
    }

    reinterpret_cast<SupervisorAddedAnmManagerView *>(g_AnmManager)->SetupVertexBuffer();
    SupervisorAddedCreateTextBuffer();

    Float3 position(500.0f, 440.0f, 0.0f);
    globalSupervisor->SetupLoadingVms(&position);
    globalSupervisor->startupThreadState = 1;
    globalSupervisor->ThreadStart(
        (LPTHREAD_START_ROUTINE)Supervisor::StartupThread, supervisor);
    return 0;
}
