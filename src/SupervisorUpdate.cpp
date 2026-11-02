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
enum SupervisorUpdateState
{
    SUPERVISOR_STATE_NEG1 = -1,
    SUPERVISOR_STATE_0 = 0,
    SUPERVISOR_STATE_1 = 1,
    SUPERVISOR_STATE_2 = 2,
    SUPERVISOR_STATE_3 = 3,
    SUPERVISOR_STATE_4 = 4,
    SUPERVISOR_STATE_5 = 5,
    SUPERVISOR_STATE_6 = 6,
    SUPERVISOR_STATE_7 = 7,
    SUPERVISOR_STATE_8 = 8,
    SUPERVISOR_STATE_9 = 9,
    SUPERVISOR_STATE_10 = 10,
    SUPERVISOR_STATE_11 = 11,
    SUPERVISOR_STATE_12 = 12,
    SUPERVISOR_STATE_13 = 13,
};

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

int Supervisor::OnUpdate(Supervisor *supervisor)
{
    Supervisor *s = supervisor;
    Supervisor *globalSupervisor = &g_Supervisor;
    int registrationContext;

    s->field478 = 0;

    if (g_SupervisorNetworkState->active == 0)
    {
        ResetSupervisorServiceChannels();
        goto UPDATE_FRAME;
    }

    if (!SupervisorSubthreadIsRunning(supervisor))
    {
        for (;;)
        {
            switch (SupervisorServiceUpdate(supervisor))
            {
            case 1:
                continue;

            case 2:
                return CHAIN_CALLBACK_RESULT_BREAK;

            case 3:
                g_SupervisorNetworkState->ResetSession();
                SupervisorNetworkMarkInactive(g_SupervisorNetworkState);
                g_SupervisorNetworkState->connectionState = 0;
                s->curState590 = SUPERVISOR_STATE_1;
                goto UPDATE_FRAME;

            case 4:
                g_SupervisorNetworkState->ResetSession();
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;

            default:
                goto UPDATE_FRAME;
            }
        }
    }

UPDATE_FRAME:
    if (s->flagsLow5D4 < 0 && !SupervisorSubthreadIsRunning(supervisor))
        return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;

    globalSupervisor->frameState794 = 0xff;
    g_AnmManager->ResetForSupervisorFrame();
    globalSupervisor->ctorArray078[0].frameOffsetX = 0.0f;
    globalSupervisor->ctorArray078[0].frameOffsetY = 0.0f;
    globalSupervisor->ctorArray078[1].frameOffsetX = 0.0f;
    globalSupervisor->ctorArray078[1].frameOffsetY = 0.0f;
    globalSupervisor->ctorArray078[2].frameOffsetX = 0.0f;
    globalSupervisor->ctorArray078[2].frameOffsetY = 0.0f;
    g_AnmManager->ExecuteScriptArray(g_SupervisorLoadingVms, 3);
    if (g_AnmManager->ServicePreloadedAnims() != 0)
    {
        return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
    }

    if (s->startupThreadState6BC != 0)
    {
        if (s->startupThreadState6BC == 2)
            return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
        if (s->subthreadActive != 0)
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        s->startupThreadState6BC = 0;
    }

    g_SoundPlayer.UpdateFades();

    if (s->wantedState58C != s->curState590)
    {
        int previousState = s->curState590;
        s->wantedState2 = s->wantedState58C;

        switch (s->wantedState58C)
        {
        case SUPERVISOR_STATE_0:
        REGISTER_MODE0:
            s->curState590 = SUPERVISOR_STATE_1;
            globalSupervisor->d3dDevice08->ResourceManagerDiscardBytes(0);
            registrationContext = 0;
            goto REGISTER_TITLE_MODE;

        case SUPERVISOR_STATE_1:
            switch (previousState)
            {
            case SUPERVISOR_STATE_NEG1:
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            case SUPERVISOR_STATE_2:
                if (GameManagerSetupLayout::RegisterChain() != 0)
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                break;
            case SUPERVISOR_STATE_4:
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR;
            case SUPERVISOR_STATE_6:
                goto REGISTER_MODE1_A;
            case SUPERVISOR_STATE_9:
                goto REGISTER_STATE9;
            default:
                break;
            }
            break;

        case SUPERVISOR_STATE_2:
            switch (previousState)
            {
            case SUPERVISOR_STATE_NEG1:
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;

            case SUPERVISOR_STATE_1:
                GameManager_CutChain();
                s->curState590 = SUPERVISOR_STATE_0;
                goto REGISTER_MODE0;

            case SUPERVISOR_STATE_3:
            REREGISTER_GAME_MANAGER:
                GameManager_CutChain();
                if (GameManagerSetupLayout::RegisterChain() != 0)
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            SET_GAME_MANAGER_STATE2:
                s->curState590 = SUPERVISOR_STATE_2;
                break;

            case SUPERVISOR_STATE_6:
                GameManager_CutChain();
                goto REGISTER_MODE1_B;

            case SUPERVISOR_STATE_7:
                GameManager_CutChain();
                goto REGISTER_MODE1_A;

            case SUPERVISOR_STATE_9:
                GameManager_CutChain();
                goto REGISTER_STATE9;

            case SUPERVISOR_STATE_10:
            case SUPERVISOR_STATE_13:
                GameManager_CutChain();
                if (GameManagerSetupLayout::RegisterChain() != 0)
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                goto SET_GAME_MANAGER_STATE2;

            case SUPERVISOR_STATE_11:
                globalSupervisor->value59C = 1;
                goto REREGISTER_GAME_MANAGER;

            case SUPERVISOR_STATE_12:
                globalSupervisor->curState590 = SUPERVISOR_STATE_3;
                goto REREGISTER_GAME_MANAGER;

            default:
                break;
            }
            break;

        case SUPERVISOR_STATE_5:
        case SUPERVISOR_STATE_6:
        case SUPERVISOR_STATE_8:
            if (previousState == SUPERVISOR_STATE_NEG1)
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            if (previousState == SUPERVISOR_STATE_1)
            {
                s->curState590 = SUPERVISOR_STATE_0;
                goto REGISTER_MODE0;
            }
            break;

        case SUPERVISOR_STATE_9:
            switch (previousState)
            {
            case SUPERVISOR_STATE_NEG1:
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            case SUPERVISOR_STATE_1:
                s->curState590 = SUPERVISOR_STATE_0;
                goto REGISTER_MODE0;
            case SUPERVISOR_STATE_6:
                goto REGISTER_MODE1_B;
            default:
                break;
            }
            break;

        default:
            break;
        }
    }

TRANSITION_DONE:
    s->wantedState58C = s->curState590;
    s->calcCount454++;
    if (s->calcCount454 % 4000 == 3999 &&
        reinterpret_cast<SupervisorVersionView *>(globalSupervisor)->CheckVersion(
            "0150a", globalSupervisor->exeSize7A4,
            globalSupervisor->exeChecksum7A0) != 0)
    {
        return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
    }

    if (g_ScreenEffectCounter != 0)
        g_ScreenEffectCounter--;
    return CHAIN_CALLBACK_RESULT_CONTINUE;

REGISTER_MODE1_A:
    s->curState590 = SUPERVISOR_STATE_1;
    globalSupervisor->d3dDevice08->ResourceManagerDiscardBytes(0);
    registrationContext = 1;
    goto REGISTER_TITLE_MODE;

REGISTER_MODE1_B:
    s->curState590 = SUPERVISOR_STATE_1;
    globalSupervisor->d3dDevice08->ResourceManagerDiscardBytes(0);
    registrationContext = 1;
REGISTER_TITLE_MODE:
    if (RegisterTitleScreen(registrationContext) != 0)
        return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
    goto TRANSITION_DONE;

REGISTER_STATE9:
    if (Ending::RegisterChain() != 0)
        return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
    goto TRANSITION_DONE;
}
