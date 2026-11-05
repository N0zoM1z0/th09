// SPDX-License-Identifier: GPL-3.0-only
// Adapted from Cirno-Labs/th09 src/app.cpp, App::OnUpdate,
// commit d7653152becabb8c86f2f43d49015ecc11bd0652.
// Original MIT portions retain their grant in LICENSE; see THIRD_PARTY_NOTICES.md.
#include "Supervisor.hpp"
#include "Chain.hpp"
#include "AnmManager.hpp"
#include "SupervisorNetworkState.hpp"
#include "SupervisorFrameQueueStorageView.hpp"
#include "SupervisorVersionView.hpp"
#include "SoundPlayer.hpp"
#include "Ending.hpp"
#include "GameManagerSetupView.hpp"

#include <d3d8.h>

namespace
{
// Each original 0xF0 Supervisor ctor-array element owns its two trailing
// frame-offset floats. Their three instances are the +0x160/+0x250/+0x340
// words used by the full OnUpdate callback.
typedef char SupervisorUpdateNativeFrameOffsets[
    (sizeof(SupervisorCtorArrayElement) == 0xF0 &&
     offsetof(SupervisorCtorArrayElement, frameOffsetX) == 0xE8 &&
     offsetof(SupervisorCtorArrayElement, frameOffsetY) == 0xEC &&
     offsetof(Supervisor, ctorArray078) == 0x78) ? 1 : -1];
typedef char SupervisorUpdateNativeQueueAt47C[
    (offsetof(Supervisor, entries47C) == 0x47C &&
     offsetof(Supervisor, lastReceived56C) == 0x56C) ? 1 : -1];

}

extern AnmVm g_SupervisorLoadingVms[3];
extern int g_ScreenEffectCounter;

extern void __fastcall ResetSupervisorServiceChannels();
extern void __fastcall SupervisorNetworkMarkInactive(void *state);
extern int __fastcall SupervisorSubthreadIsRunning(Supervisor *supervisor);
extern int __fastcall SupervisorServiceUpdate(Supervisor *supervisor);
extern void __fastcall GameManager_CutChain();
extern int __fastcall RegisterTitleScreen(int mode);

int Supervisor::OnUpdate(Supervisor *app)
{
    app->field478 = 0;
    if (g_SupervisorNetworkState->active == 0)
    {
        ResetSupervisorServiceChannels();
    }
    else if (!SupervisorSubthreadIsRunning(app))
    {
        for (;;)
        {
            switch (SupervisorServiceUpdate(app))
            {
            case 1:
                continue;
            case 2:
                return 3;
            case 3:
                g_SupervisorNetworkState->ResetSession();
                SupervisorNetworkMarkInactive(g_SupervisorNetworkState);
                g_SupervisorNetworkState->connectionState = 0;
                app->curState590 = 1;
                break;
            case 4:
                g_SupervisorNetworkState->ResetSession();
                return 4;
            }
            break;
        }
    }
    if ((app->flagsLow5D4 >> 7 & 1) && !SupervisorSubthreadIsRunning(app))
    {
        return 4;
    }
    g_Supervisor.frameState794 = 0xff;
    g_AnmManager->ResetForSupervisorFrame();
    g_Supervisor.ctorArray078[0].frameOffsetX = 0.0f;
    g_Supervisor.ctorArray078[0].frameOffsetY = 0.0f;
    g_Supervisor.ctorArray078[1].frameOffsetX = 0.0f;
    g_Supervisor.ctorArray078[1].frameOffsetY = 0.0f;
    g_Supervisor.ctorArray078[2].frameOffsetX = 0.0f;
    g_Supervisor.ctorArray078[2].frameOffsetY = 0.0f;
    g_AnmManager->ExecuteScriptArray(g_SupervisorLoadingVms, 3);
    if (g_AnmManager->ServicePreloadedAnims() != 0)
    {
        return 4;
    }
    if (app->startupThreadState6BC != 0)
    {
        if (app->startupThreadState6BC == 2)
        {
            return 4;
        }
        if (app->subthreadActive != 0)
        {
            return 1;
        }
        app->startupThreadState6BC = 0;
    }
    g_SoundPlayer.UpdateFades();
    if (app->wantedState58C != app->curState590)
    {
        app->wantedState2 = app->wantedState58C;
        switch (app->wantedState58C)
        {
        case 0:
        title:
            app->curState590 = 1;
            g_Supervisor.d3dDevice08->ResourceManagerDiscardBytes(0);
            if (RegisterTitleScreen(0) != 0)
            {
                return 4;
            }
            break;
        case 1:
            switch (app->curState590)
            {
            case -1:
                return 4;
            case 2:
                if (GameManagerSetupLayout::RegisterChain() != 0)
                {
                    return 4;
                }
                break;
            case 4:
                return 5;
            case 6:
                app->curState590 = 1;
                g_Supervisor.d3dDevice08->ResourceManagerDiscardBytes(0);
                if (RegisterTitleScreen(1) != 0)
                {
                    return 4;
                }
                break;
            case 9:
                if (Ending::RegisterChain() != 0)
                {
                    return 4;
                }
                break;
            }
            break;
        case 5:
        case 6:
        case 8:
            switch (app->curState590)
            {
            case -1:
                return 4;
            case 1:
                app->curState590 = 0;
                goto title;
            }
            break;
        case 2:
            switch (app->curState590)
            {
            case -1:
                return 4;
            case 1:
                GameManager_CutChain();
                app->curState590 = 0;
                goto title;
            case 6:
                GameManager_CutChain();
                app->curState590 = 1;
                g_Supervisor.d3dDevice08->ResourceManagerDiscardBytes(0);
                if (RegisterTitleScreen(1) != 0)
                {
                    return 4;
                }
                break;
            case 10:
            case 13:
                GameManager_CutChain();
                if (GameManagerSetupLayout::RegisterChain() != 0)
                {
                    return 4;
                }
                app->curState590 = 2;
                break;
            case 11:
                g_Supervisor.value59C = 1;
                GameManager_CutChain();
                if (GameManagerSetupLayout::RegisterChain() != 0)
                {
                    return 4;
                }
                app->curState590 = 2;
                break;
            case 12:
                g_Supervisor.curState590 = 3;
                GameManager_CutChain();
                if (GameManagerSetupLayout::RegisterChain() != 0)
                {
                    return 4;
                }
                app->curState590 = 2;
                break;
            case 3:
                GameManager_CutChain();
                if (GameManagerSetupLayout::RegisterChain() != 0)
                {
                    return 4;
                }
                app->curState590 = 2;
                break;
            case 7:
                GameManager_CutChain();
                app->curState590 = 0;
                app->curState590 = 1;
                g_Supervisor.d3dDevice08->ResourceManagerDiscardBytes(0);
                if (RegisterTitleScreen(1) != 0)
                {
                    return 4;
                }
                break;
            case 9:
                GameManager_CutChain();
                if (Ending::RegisterChain() != 0)
                {
                    return 4;
                }
                break;
            }
            break;
        case 9:
            switch (app->curState590)
            {
            case -1:
                return 4;
            case 1:
                app->curState590 = 0;
                goto title;
            case 6:
                app->curState590 = 1;
                g_Supervisor.d3dDevice08->ResourceManagerDiscardBytes(0);
                if (RegisterTitleScreen(1) != 0)
                {
                    return 4;
                }
                break;
            }
            break;
        }
    }
    app->wantedState58C = app->curState590;
    app->calcCount454++;
    if (app->calcCount454 % 4000 == 3999)
    {
        if (static_cast<SupervisorVersionView *>(&g_Supervisor)->CheckVersion("0150a", g_Supervisor.exeSize7A4, g_Supervisor.exeChecksum7A0) != 0)
        {
            return 4;
        }
    }
    if (g_ScreenEffectCounter != 0)
    {
        g_ScreenEffectCounter--;
    }
    return 1;
}
