#include "ScreenEffect.hpp"
#include "Supervisor.hpp"
#include "TitleSupervisorRuntimeView.hpp"

struct SupervisorLoadingTransitionVmView
{
    unsigned char bytes[0x2A4];

    void SetInterrupt(short interrupt);
};

extern SupervisorLoadingTransitionVmView g_SupervisorLoadingVms[3];
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
