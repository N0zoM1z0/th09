#include "ScreenEffect.hpp"
#include "Supervisor.hpp"
#include "TitleSupervisorRuntimeView.hpp"
#include "AnmVmLifecycle.hpp"
#include "AnmCaptureRequest.hpp"
#include "AnmManager.hpp"
#include "SupervisorSetupLayoutView.hpp"

// Both independently exact loading callbacks own the same native 0x2A4-byte
// ANM VM array as the exact Supervisor loading-VM and capture installers.
typedef char SupervisorLoadingTransitionNativeVms[
    (sizeof(AnmVm) == 0x2A4 &&
     offsetof(AnmVm, pendingInterrupt) == 0x1FE) ? 1 : -1];
extern AnmVm g_SupervisorLoadingVms[3];
extern ScreenEffect *g_SupervisorScreenEffect;

void TitleSupervisorView::HideLoadingVms()
{
    if (loadingVmsHaveBeenSetup == 1)
    {
        g_SupervisorLoadingVms[0].SetInterrupt(1);
        g_SupervisorLoadingVms[1].SetInterrupt(1);
        g_SupervisorLoadingVms[2].SetInterrupt(1);
        loadingVmsHaveBeenSetup = 0;
    }

    if (g_SupervisorScreenEffect != 0)
    {
        g_SupervisorScreenEffect->BeginFadeRelease();
        g_SupervisorScreenEffect = 0;
    }
}

void SupervisorSetupLayout::SetupLoadingVmsAndInitCapture(
    SetupLoadingPosition *position)
{
    if (loadingVmsHaveBeenSetup == 0)
    {
        loadingAnm5D0->ExecuteAnmIdx(&g_SupervisorLoadingVms[0], 0);
        loadingAnm5D0->ExecuteAnmIdx(&g_SupervisorLoadingVms[1], 1);
        loadingAnm5D0->ExecuteAnmIdx(&g_SupervisorLoadingVms[2], 2);
        loadingVmsHaveBeenSetup = 1;

        g_SupervisorLoadingVms[0].pos =
            *reinterpret_cast<Float3 *>(position);
        g_SupervisorLoadingVms[1].pos =
            *reinterpret_cast<Float3 *>(position);
        g_SupervisorLoadingVms[2].pos =
            *reinterpret_cast<Float3 *>(position);
    }

    reinterpret_cast<AnmCaptureRequestView *>(g_AnmManager)
        ->QueueCaptureRequest(8, 0, 0, 640, 480, 0, 0, 640, 480);
}

void __fastcall FinishLoading(Supervisor *supervisor)
{
    if (supervisor->loadingVmsHaveBeenSetup == 1)
    {
        g_SupervisorLoadingVms[0].SetInterrupt(2);
        g_SupervisorLoadingVms[1].SetInterrupt(2);
        g_SupervisorLoadingVms[2].SetInterrupt(2);
        supervisor->loadingVmsHaveBeenSetup = 2;
    }

    if (g_SupervisorScreenEffect != 0)
    {
        g_SupervisorScreenEffect->BeginFadeRelease();
        g_SupervisorScreenEffect = 0;
    }
}
