#include "ScreenEffect.hpp"
#include "Supervisor.hpp"

struct SupervisorLoadingTransitionVmView
{
    unsigned char bytes[0x2A4];

    void SetInterrupt(short interrupt);
};

struct TitleSupervisorView
{
    unsigned char unknown000[0x740];
    int loadingVmsHaveBeenSetup740;

    void HideLoadingVms();
};

struct SupervisorLoadingFinishView
{
    unsigned char unknown000[0x740];
    int loadingVmsHaveBeenSetup740;
};

extern SupervisorLoadingTransitionVmView g_SupervisorLoadingVms[3];
extern ScreenEffect *g_SupervisorScreenEffect;

void TitleSupervisorView::HideLoadingVms()
{
    if (loadingVmsHaveBeenSetup740 == 1)
    {
        g_SupervisorLoadingVms[0].SetInterrupt(1);
        g_SupervisorLoadingVms[1].SetInterrupt(1);
        g_SupervisorLoadingVms[2].SetInterrupt(1);
        loadingVmsHaveBeenSetup740 = 0;
    }

    if (g_SupervisorScreenEffect != 0)
    {
        g_SupervisorScreenEffect->BeginFadeRelease();
        g_SupervisorScreenEffect = 0;
    }
}

void __fastcall FinishLoading(Supervisor *supervisor)
{
    SupervisorLoadingFinishView *view =
        reinterpret_cast<SupervisorLoadingFinishView *>(supervisor);

    if (view->loadingVmsHaveBeenSetup740 == 1)
    {
        g_SupervisorLoadingVms[0].SetInterrupt(2);
        g_SupervisorLoadingVms[1].SetInterrupt(2);
        g_SupervisorLoadingVms[2].SetInterrupt(2);
        view->loadingVmsHaveBeenSetup740 = 2;
    }

    if (g_SupervisorScreenEffect != 0)
    {
        g_SupervisorScreenEffect->BeginFadeRelease();
        g_SupervisorScreenEffect = 0;
    }
}
