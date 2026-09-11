#include "Supervisor.hpp"
#include "Chain.hpp"
#include "AnmManager.hpp"

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
    unsigned char unknown00C[0x454 - 0x0C];
    int calcCount;
    unsigned char unknown458[0x478 - 0x458];
    int field478;
    unsigned char unknown47C[0x58C - 0x47C];
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
    unsigned char unknown6C0[0x7A0 - 0x6C0];
    int exeChecksum;
    int exeSize;
};

typedef char SupervisorUpdateLayoutSizeCheck[
    (sizeof(SupervisorUpdateLayout) == 0x7A8) ? 1 : -1];

struct SupervisorServiceState
{
    unsigned char unknown000[0xA8];
    int valueA8;
    int valueAC;
    int valueB0;
    int valueB4;
    int valueB8;
    int valueBC;
    int valueC0;

    void Reset432F70();
    void Reset42E9B0();
};

struct AnmManagerSupervisorView
{
    void ResetForSupervisorFrame();
    void UpdateLoadingVms(void *vms, int count);
    int ServiceSupervisorResources();
};

struct SoundPlayerSupervisorView
{
    void UpdateSupervisorFrame();
};

struct SupervisorFrameOffset
{
    float x;
    float y;
    unsigned char unknown008[0xE8];
};

typedef char SupervisorFrameOffsetSizeCheck[
    (sizeof(SupervisorFrameOffset) == 0xF0) ? 1 : -1];

struct SupervisorMethodView
{
    int CheckVersion(char *versionString, int exeSize, int exeChecksum);
};
}

extern SupervisorServiceState *g_SupervisorServiceState;
extern SoundPlayerSupervisorView g_SoundPlayerSupervisorView;
extern unsigned char g_SupervisorLoadingVms[];
extern SupervisorFrameOffset g_SupervisorFrameOffsets[3];
extern int g_SupervisorState794;
extern int g_ScreenEffectCounter;
extern char g_VersionString[];

extern void __fastcall ResetSupervisorServiceChannels();
extern int __fastcall SupervisorSubthreadIsRunning(Supervisor *supervisor);
extern int __fastcall SupervisorServiceUpdate(Supervisor *supervisor);
extern void __fastcall GameManager_CutChain();
extern int __fastcall GameManager_RegisterChain();
extern int __fastcall RegisterStateObject(int mode);
extern int __fastcall RegisterState9Object();

int Supervisor::OnUpdate(Supervisor *supervisor)
{
    SupervisorUpdateLayout *s =
        reinterpret_cast<SupervisorUpdateLayout *>(supervisor);
    SupervisorUpdateLayout *globalSupervisor =
        reinterpret_cast<SupervisorUpdateLayout *>(&g_Supervisor);
    s->field478 = 0;

    if (g_SupervisorServiceState->valueA8 == 0)
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
                g_SupervisorServiceState->Reset432F70();
                g_SupervisorServiceState->Reset42E9B0();
                g_SupervisorServiceState->valueB8 = 0;
                s->curState = SUPERVISOR_STATE_1;
                goto UPDATE_FRAME;

            case 4:
                g_SupervisorServiceState->Reset432F70();
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;

            default:
                goto UPDATE_FRAME;
            }
        }
    }

UPDATE_FRAME:
    if (s->flags5D4 < 0 && !SupervisorSubthreadIsRunning(supervisor))
        return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;

    g_SupervisorState794 = 0xff;
    reinterpret_cast<AnmManagerSupervisorView *>(g_AnmManager)
        ->ResetForSupervisorFrame();
    g_SupervisorFrameOffsets[0].x = 0.0f;
    g_SupervisorFrameOffsets[0].y = 0.0f;
    g_SupervisorFrameOffsets[1].x = 0.0f;
    g_SupervisorFrameOffsets[1].y = 0.0f;
    g_SupervisorFrameOffsets[2].x = 0.0f;
    g_SupervisorFrameOffsets[2].y = 0.0f;
    reinterpret_cast<AnmManagerSupervisorView *>(g_AnmManager)
        ->UpdateLoadingVms(g_SupervisorLoadingVms, 3);
    if (reinterpret_cast<AnmManagerSupervisorView *>(g_AnmManager)
            ->ServiceSupervisorResources() != 0)
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

    g_SoundPlayerSupervisorView.UpdateSupervisorFrame();

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
            if (RegisterStateObject(0) != 0)
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            break;

        case SUPERVISOR_STATE_1:
            switch (previousState)
            {
            case SUPERVISOR_STATE_NEG1:
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            case SUPERVISOR_STATE_2:
                if (GameManager_RegisterChain() != 0)
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
                if (GameManager_RegisterChain() != 0)
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
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
                if (GameManager_RegisterChain() != 0)
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                s->curState = SUPERVISOR_STATE_2;
                break;

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
        reinterpret_cast<SupervisorMethodView *>(globalSupervisor)->CheckVersion(
            g_VersionString, globalSupervisor->exeSize,
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
    if (RegisterStateObject(1) != 0)
        return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
    goto TRANSITION_DONE;

REGISTER_MODE1_B:
    s->curState = SUPERVISOR_STATE_1;
    globalSupervisor->d3dDevice->ResourceManagerDiscardBytes(0);
    if (RegisterStateObject(1) != 0)
        return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
    goto TRANSITION_DONE;

REGISTER_STATE9:
    if (RegisterState9Object() != 0)
        return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
    goto TRANSITION_DONE;
}
