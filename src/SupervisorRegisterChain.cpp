#include "Supervisor.hpp"
#include "Chain.hpp"

#include <stddef.h>

namespace
{
struct SupervisorRegisterLayout
{
    unsigned char unknown000[0x454];
    int calcCount;
    unsigned char unknown458[0x58C - 0x458];
    int wantedState;
    int curState;
};

typedef char SupervisorRegisterCalcCountAt454[
    (offsetof(SupervisorRegisterLayout, calcCount) == 0x454) ? 1 : -1];
typedef char SupervisorRegisterWantedAt58C[
    (offsetof(SupervisorRegisterLayout, wantedState) == 0x58C) ? 1 : -1];
typedef char SupervisorRegisterCurrentAt590[
    (offsetof(SupervisorRegisterLayout, curState) == 0x590) ? 1 : -1];
}

extern Chain g_Chain;
extern int __fastcall SupervisorDeletedCallback(Supervisor *supervisor);
extern int __fastcall SupervisorDrawFps(Supervisor *supervisor);
extern int __fastcall SupervisorDrawMain(Supervisor *supervisor);
extern int __fastcall SupervisorDrawLoading(Supervisor *unused);

int SupervisorRegisterChain()
{
    SupervisorRegisterLayout *supervisor =
        reinterpret_cast<SupervisorRegisterLayout *>(&g_Supervisor);

    supervisor->wantedState = 0;
    supervisor->curState = -1;
    supervisor->calcCount = 0;

    ChainElem *elem = g_Chain.CreateElem((ChainCallback)Supervisor::OnUpdate);
    elem->arg = &g_Supervisor;
    elem->addedCallback = (ChainLifetimeCallback)Supervisor::AddedCallback;
    elem->deletedCallback = (ChainLifetimeCallback)SupervisorDeletedCallback;

    int result = g_Chain.AddToCalcChain(elem, 0);
    if (result != 0)
        return result;

    elem = g_Chain.CreateElem((ChainCallback)SupervisorDrawFps);
    elem->arg = &g_Supervisor;
    g_Chain.AddToDrawChain(elem, 31);

    elem = g_Chain.CreateElem((ChainCallback)SupervisorDrawMain);
    elem->arg = &g_Supervisor;
    g_Chain.AddToDrawChain(elem, 0);

    elem = g_Chain.CreateElem((ChainCallback)SupervisorDrawLoading);
    elem->arg = &g_Supervisor;
    g_Chain.AddToDrawChain(elem, 2);

    return 0;
}
