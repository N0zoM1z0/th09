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

struct SupervisorUpdateLayout
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice;
    unsigned char unknown00C[0x160 - 0x0C];
    float frameOffsetX160;
    float frameOffsetY164;
    unsigned char unknown168[0x250 - 0x168];
    float frameOffsetX250;
    float frameOffsetY254;
    unsigned char unknown258[0x340 - 0x258];
    float frameOffsetX340;
    float frameOffsetY344;
    unsigned char unknown348[0x454 - 0x348];
    int calcCount;
    unsigned char unknown458[0x478 - 0x458];
    int field478;
    SupervisorFrameEntryStorageView entries47C[20];
    SupervisorFrameEntryStorageView lastReceived56C[2];
    unsigned char unknown584[0x58C - 0x584];
    int wantedState;
    int curState;
    int wantedState2;
    int value598;
    int value59C;
    unsigned char unknown5A0[0x5D4 - 0x5A0];
    signed char flags5D4;
    unsigned char unknown5D5[0x6AC - 0x5D5];
    int runningSubthreadHandle;
    unsigned char unknown6B0[8];
    int subthreadActive;
    int startupThreadState;
    unsigned char unknown6C0[0x794 - 0x6C0];
    int frameState794;
    unsigned char unknown798[8];
    int exeChecksum;
    int exeSize;
};

typedef char SupervisorUpdateLayoutSizeCheck[
    (sizeof(SupervisorUpdateLayout) == 0x7A8) ? 1 : -1];
typedef char SupervisorUpdateQueuesAt47C[
    (offsetof(SupervisorUpdateLayout, entries47C) == 0x47C) ? 1 : -1];
typedef char SupervisorUpdateLastReceivedAt56C[
    (offsetof(SupervisorUpdateLayout, lastReceived56C) == 0x56C) ? 1 : -1];
typedef char SupervisorUpdateAfterQueuesAt584[
    (offsetof(SupervisorUpdateLayout, unknown584) == 0x584) ? 1 : -1];
typedef char SupervisorUpdateFrameOffsetsAt160[
    (offsetof(SupervisorUpdateLayout, frameOffsetX160) == 0x160 &&
     offsetof(SupervisorUpdateLayout, frameOffsetY164) == 0x164 &&
     offsetof(SupervisorUpdateLayout, frameOffsetX250) == 0x250 &&
     offsetof(SupervisorUpdateLayout, frameOffsetY254) == 0x254 &&
     offsetof(SupervisorUpdateLayout, frameOffsetX340) == 0x340 &&
     offsetof(SupervisorUpdateLayout, frameOffsetY344) == 0x344) ? 1 : -1];
typedef char SupervisorUpdateFrameStateAt794[
    (offsetof(SupervisorUpdateLayout, frameState794) == 0x794) ? 1 : -1];

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
    SupervisorUpdateLayout *s =
        reinterpret_cast<SupervisorUpdateLayout *>(supervisor);
    SupervisorUpdateLayout *globalSupervisor =
        reinterpret_cast<SupervisorUpdateLayout *>(&g_Supervisor);
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
                s->curState = SUPERVISOR_STATE_1;
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
    if (s->flags5D4 < 0 && !SupervisorSubthreadIsRunning(supervisor))
        return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;

    globalSupervisor->frameState794 = 0xff;
    g_AnmManager->ResetForSupervisorFrame();
    globalSupervisor->frameOffsetX160 = 0.0f;
    globalSupervisor->frameOffsetY164 = 0.0f;
    globalSupervisor->frameOffsetX250 = 0.0f;
    globalSupervisor->frameOffsetY254 = 0.0f;
    globalSupervisor->frameOffsetX340 = 0.0f;
    globalSupervisor->frameOffsetY344 = 0.0f;
    g_AnmManager->ExecuteScriptArray(g_SupervisorLoadingVms, 3);
    if (g_AnmManager->ServicePreloadedAnims() != 0)
    {
        return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
    }

    if (s->startupThreadState != 0)
    {
        if (s->startupThreadState == 2)
            return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
        if (s->subthreadActive != 0)
            return CHAIN_CALLBACK_RESULT_CONTINUE;
        s->startupThreadState = 0;
    }

    g_SoundPlayer.UpdateFades();

    if (s->wantedState != s->curState)
    {
        int previousState = s->curState;
        s->wantedState2 = s->wantedState;

        switch (s->wantedState)
        {
        case SUPERVISOR_STATE_0:
        REGISTER_MODE0:
            s->curState = SUPERVISOR_STATE_1;
            globalSupervisor->d3dDevice->ResourceManagerDiscardBytes(0);
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
                s->curState = SUPERVISOR_STATE_0;
                goto REGISTER_MODE0;

            case SUPERVISOR_STATE_3:
            REREGISTER_GAME_MANAGER:
                GameManager_CutChain();
                if (GameManagerSetupLayout::RegisterChain() != 0)
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            SET_GAME_MANAGER_STATE2:
                s->curState = SUPERVISOR_STATE_2;
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
                globalSupervisor->curState = SUPERVISOR_STATE_3;
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
                s->curState = SUPERVISOR_STATE_0;
                goto REGISTER_MODE0;
            }
            break;

        case SUPERVISOR_STATE_9:
            switch (previousState)
            {
            case SUPERVISOR_STATE_NEG1:
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            case SUPERVISOR_STATE_1:
                s->curState = SUPERVISOR_STATE_0;
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
    s->wantedState = s->curState;
    s->calcCount++;
    if (s->calcCount % 4000 == 3999 &&
        reinterpret_cast<SupervisorVersionView *>(globalSupervisor)->CheckVersion(
            "0150a", globalSupervisor->exeSize,
            globalSupervisor->exeChecksum) != 0)
    {
        return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
    }

    if (g_ScreenEffectCounter != 0)
        g_ScreenEffectCounter--;
    return CHAIN_CALLBACK_RESULT_CONTINUE;

REGISTER_MODE1_A:
    s->curState = SUPERVISOR_STATE_1;
    globalSupervisor->d3dDevice->ResourceManagerDiscardBytes(0);
    registrationContext = 1;
    goto REGISTER_TITLE_MODE;

REGISTER_MODE1_B:
    s->curState = SUPERVISOR_STATE_1;
    globalSupervisor->d3dDevice->ResourceManagerDiscardBytes(0);
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
