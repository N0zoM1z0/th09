// Partial target-facing reconstruction of the TH09 gameplay setup thread.
// The local layout views name only fields exercised by this bounded packet;
// they do not claim the complete original GameManager or Supervisor layouts.
// Provisional helper names describe observed call-site roles only.

#include "AsciiManager.hpp"
#include "GameConfiguration.hpp"
#include "Supervisor.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <string.h>

struct SetupSideState
{
    void *subsystem0;
    void *subsystem1;
    void *subsystem2;
    void *subsystem3;
    void *subsystem4;
    void *subsystem5;
    void *subsystem6;
    void *state;
    unsigned char unknown_20[8];
    int selector;
    unsigned char unknown_2C[8];
    unsigned int flags;
};

typedef char SetupSideStateSizeCheck[(sizeof(SetupSideState) == 0x38) ? 1 : -1];

struct SetupStateBuffer
{
    float value00;
    unsigned int elapsed04;
    unsigned char unknown_08[8];
    int phase10;
    unsigned char unknown_14[0x8C];
    int valueA0;
    int valueA4;
};

struct GameManagerSetupLayout
{
    SetupSideState sides[2];
    unsigned char unknown_70[0x0C];
    void *sharedSubsystem;
    unsigned char unknown_80[0x28];
    void *gameSubsystem;
    void *secondarySubsystem;
    int valueB0;
    int limitB4;
    int valueB8;
    unsigned char unknown_BC[4];
    int valueC0;
    int valueC4;
    int valueC8;
    int valueCC;
    int valueD0;
    void *stageObject;
    unsigned char clearD8[0x10];
    unsigned char unknown_E8[4];
    float speedEC;
    unsigned char unknown_F0[0x0C];
    int counterFC;
    int value100;
    int value104;
    int value108;
    int value10C;
    unsigned char unknown_110[8];
    int gameMode;
    int setupMode;
    int value120;
    int gameplaySetupState;
    unsigned char unknown_128[0x0C];
    unsigned int flags;
    unsigned char unknown_138[8];
    int value140;
    unsigned char unknown_144;
    unsigned char modeByte145;
    unsigned short seed146;
    unsigned char unknown_148[0x1FE];
    unsigned short seedCopy346;
    int value348;
    unsigned char unknown_34C[0x28];
    int value374;
    unsigned char unknown_378[0x14];
    int value38C;

    void UpdateProgress();
    void AdvanceTimedState();
    static void CleanupGameplayState();
};

struct SetupEventManager
{
    void Emit(int eventId, int argument);
};

struct SupervisorSetupLayout
{
    unsigned char unknown_000[0x388];
    GameConfiguration config;
    unsigned char unknown_454[0x13C];
    int state590;
    int value594;
    int value598;
    int value59C;
    unsigned char unknown_5A0[4];
    int screenTransitionCountdown;
    unsigned char unknown_5A8[0x2C];
    unsigned int flags;
    unsigned char unknown_5D8[0xD4];
    int runningSubthreadHandle;
    unsigned char unknown_6B0[4];
    int subthreadCloseRequestActive;
    int subthreadActive;
    unsigned char unknown_6BC[0xE0];
    unsigned int systemTime;
};

extern GameManagerSetupLayout g_GameManager;
extern GameConfiguration *g_GameConfiguration;
extern unsigned char g_SetupBuffer;
extern int g_SetupStatus;
extern unsigned short g_SetupSeedSource;
extern SetupEventManager g_SetupEventManager;
extern AsciiManager g_AsciiManager;
extern void __fastcall ReleaseGameSubsystem(void *object);
extern void __fastcall ReleaseSecondarySubsystem(void *object);
extern void __fastcall ReleaseSubsystem0(void *object);
extern void __fastcall ReleaseSubsystem1(void *object);
extern void __fastcall ReleaseSubsystem2(void *object);
extern void __fastcall ReleaseSubsystem3(void *object);
extern void __fastcall ReleaseSubsystem4(void *object);
extern void __fastcall ReleaseSubsystem5(void *object);
extern void __fastcall ReleaseSubsystem6(void *object);

extern void __fastcall ResetGameManager(GameManagerSetupLayout *manager);
extern void __fastcall ResetSetupBuffer(void *buffer);
extern void __fastcall ReleaseStageObject(void *object);
extern void *__fastcall CreateStageObject(int mode, void *context);
extern void __fastcall SetStageObjectMode(void *object, int mode);
extern void __fastcall BeginLoading(Supervisor *supervisor);
extern int __fastcall FinishLoading(Supervisor *supervisor);
extern void *__fastcall CreateGameSubsystem();
extern void __fastcall InitializeGameSubsystems();
extern void *__fastcall RegisterSubsystem0(int side);
extern void *__fastcall RegisterSubsystem1(int unused, int side, int selector);
extern void *__fastcall RegisterSubsystem2(int side);
extern void *__fastcall RegisterSubsystem3(int side, int size, int kind);
extern void *__fastcall RegisterSubsystem4(int side);
extern void *__fastcall RegisterSubsystem5(int side);
extern void *__fastcall RegisterSubsystem6(int side);
extern void *__fastcall RegisterSharedSubsystem(int side, int size, int kind);
extern void *__fastcall RegisterSecondarySubsystem();
extern void __fastcall FinalizeSubsystems(void *unused);


void GameManagerSetupLayout::AdvanceTimedState()
{
    SetupStateBuffer *state =
        reinterpret_cast<SetupStateBuffer *>(g_GameManager.sides[0].state);

    if (state->value00 < 7.0f)
    {
        state->value00 += 1.0f;
        g_SetupEventManager.Emit(0x1C, 0);
        state = reinterpret_cast<SetupStateBuffer *>(g_GameManager.sides[0].state);
    }

    state->phase10++;

    SetupStateBuffer *thresholdState =
        reinterpret_cast<SetupStateBuffer *>(g_GameManager.sides[0].state);
    if (g_GameManager.gameMode == 1)
    {
        if (thresholdState->phase10 >= 8)
            thresholdState->phase10 = 99999;
    }
    else if (thresholdState->phase10 >= 5)
    {
        thresholdState->phase10 = 99999;
    }
}


void GameManagerSetupLayout::CleanupGameplayState()
{
    ResetGameManager(&g_GameManager);
    g_AsciiManager.Reset();
    ReleaseGameSubsystem(g_GameManager.gameSubsystem);
    ReleaseSecondarySubsystem(g_GameManager.secondarySubsystem);

    for (int sideIndex = 0; sideIndex < 2; sideIndex++)
    {
        SetupSideState *side = &g_GameManager.sides[sideIndex];
        side->flags &= ~1u;
        ReleaseSubsystem0(side->subsystem0);
        ReleaseSubsystem1(side->subsystem1);
        ReleaseSubsystem2(side->subsystem2);
        ReleaseSubsystem3(side->subsystem3);
        ReleaseSubsystem4(side->subsystem4);
        ReleaseSubsystem5(side->subsystem5);
        ReleaseSubsystem6(side->subsystem6);
    }

    ReleaseSubsystem3(g_GameManager.sharedSubsystem);

    if (g_GameManager.gameMode == 2)
    {
        g_GameManager.limitB4 -= 8;
        if (g_GameManager.limitB4 < g_GameManager.value100)
            g_GameManager.limitB4 = g_GameManager.value100;
    }
    else
    {
        int stageValue = g_GameManager.value100;
        int newLimit;

        switch (g_GameManager.setupMode)
        {
        case 4:
            newLimit = 18;
            g_GameManager.valueC8 = 22;
            g_GameManager.valueC4 = 600 * (stageValue + 1);
            break;
        case 3:
            newLimit = g_GameManager.counterFC + 6;
            g_GameManager.valueC8 = g_GameManager.counterFC / 2 + 17;
            g_GameManager.valueC4 = 600 * stageValue + 900;
            break;
        case 2:
            newLimit = g_GameManager.counterFC + 3;
            g_GameManager.valueC8 = g_GameManager.counterFC / 2 + 15;
            g_GameManager.valueC4 = 600 * stageValue + 900;
            break;
        case 1:
            newLimit = g_GameManager.counterFC + 1;
            g_GameManager.valueC8 = g_GameManager.counterFC + 10;
            g_GameManager.valueC4 = 600 * (stageValue + 2);
            break;
        case 0:
            newLimit = g_GameManager.counterFC / 3 + 1;
            g_GameManager.valueC8 = g_GameManager.counterFC / 2 + 6;
            g_GameManager.valueC4 = 600 * (stageValue + 3);
            break;
        default:
            newLimit = g_GameManager.limitB4;
            break;
        }

        g_GameManager.limitB4 = newLimit - 2 * stageValue;
        if (g_GameManager.limitB4 < 1)
            g_GameManager.limitB4 = 1;
    }

    g_GameManager.valueB0 = 0;
    g_GameManager.value348 = 0;
    g_GameManager.valueCC -= 1000;
    if (g_GameManager.valueCC < 0)
        g_GameManager.valueCC = 0;
}

void __fastcall GameplaySetupThread(void *unused)
{
    GameManagerSetupLayout *manager = &g_GameManager;
    SupervisorSetupLayout *supervisor = reinterpret_cast<SupervisorSetupLayout *>(&g_Supervisor);
    unsigned int previousMode;
    int sideIndex;

    (void)unused;

    ResetGameManager(manager);
    ResetSetupBuffer(&g_SetupBuffer);

    manager->flags |= 4;
    manager->flags &= ~0x1800u;
    manager->value374 = -1;
    previousMode = manager->gameMode;
    manager->valueD0 = 0;
    manager->value348 = 0;
    manager->value38C = 1;
    manager->value120 = 0xff;
    manager->valueB8 = 2;
    manager->valueCC = 0;
    manager->valueC0 = 0;
    manager->value140 = 0;
    manager->value100 = 0;
    manager->value104 = (previousMode == 2) + 1;
    manager->flags &= ~0x4000u;
    manager->value108 = 0;
    manager->value10C = 0;

    if ((manager->flags & 8) == 0)
    {
        if (supervisor->flags & 0x200)
        {
            memset(manager->clearD8, 0, sizeof(manager->clearD8));
            goto mode_ready;
        }
    }
    else
    {
        memset(manager->clearD8, 0, sizeof(manager->clearD8));
        if (supervisor->flags & 0x200)
            goto mode_ready;
        previousMode = manager->gameMode;
    }

    if (previousMode == 0 || previousMode == 1)
        manager->counterFC++;

mode_ready:
    if (g_GameConfiguration != NULL)
    {
        free(g_GameConfiguration);
        g_GameConfiguration = NULL;
    }

    GameConfiguration *allocation =
        static_cast<GameConfiguration *>(operator new(sizeof(GameConfiguration)));
    GameConfiguration *newConfiguration = allocation;
    if (allocation != NULL)
        allocation->Initialize();
    else
        newConfiguration = NULL;
    g_GameConfiguration = newConfiguration;
    *g_GameConfiguration = supervisor->config;
    manager->speedEC = 1.0f;

    if (supervisor->flags & 0x200)
    {
        manager->flags &= ~0x2000u;

        for (sideIndex = 0; sideIndex < 2; sideIndex++)
        {
            if (manager->sides[sideIndex].state != NULL)
            {
                free(manager->sides[sideIndex].state);
                manager->sides[sideIndex].state = NULL;
            }
            manager->sides[sideIndex].state = operator new(0xA0);
            memset(manager->sides[sideIndex].state, 0, 0xA0);
        }

        if (manager->gameMode == 0)
            reinterpret_cast<SetupStateBuffer *>(manager->sides[0].state)->value00 =
                static_cast<float>(g_GameConfiguration->valueAC) + 2.0f;
        else
            reinterpret_cast<SetupStateBuffer *>(manager->sides[0].state)->value00 = 0.0f;

        ReleaseStageObject(manager->stageObject);
        manager->stageObject = NULL;

        if ((manager->flags & 8) == 0)
        {
            manager->seedCopy346 = g_SetupSeedSource;
            g_GameManager.UpdateProgress();
            manager->stageObject = CreateStageObject(0, NULL);
        }
        else
        {
            manager->stageObject = CreateStageObject(1, &manager->modeByte145);
            g_GameManager.UpdateProgress();
        }
    }
    else
    {
        if (manager->gameMode == 0)
        {
            SetupStateBuffer *state =
                reinterpret_cast<SetupStateBuffer *>(manager->sides[0].state);
            if (state->elapsed04 >= 2000000u * state->phase10 + 1000000u)
                g_GameManager.AdvanceTimedState();
        }
        if (manager->gameMode == 1)
        {
            SetupStateBuffer *state =
                reinterpret_cast<SetupStateBuffer *>(manager->sides[0].state);
            if (state->elapsed04 >= 500000u * (state->phase10 + 1))
                g_GameManager.AdvanceTimedState();
        }

        if ((manager->flags & 8) == 0)
        {
            manager->seedCopy346 = g_SetupSeedSource;
            g_GameManager.UpdateProgress();
            SetStageObjectMode(manager->stageObject, 0);
        }
        else
        {
            SetStageObjectMode(manager->stageObject, 1);
            g_GameManager.UpdateProgress();
        }

        if (manager->gameMode == 0 || manager->gameMode == 1)
            memset(manager->sides[1].state, 0, 0xA0);
    }

    BeginLoading(&g_Supervisor);
    manager->valueB0 = 0;
    manager->gameSubsystem = CreateGameSubsystem();
    if (manager->gameSubsystem == NULL)
        goto setup_failed;

    InitializeGameSubsystems();

    for (sideIndex = 0; sideIndex < 2; sideIndex++)
    {
        SetupSideState *side = &manager->sides[sideIndex];

        if (side->selector != 0)
            (&g_GameConfiguration->valueB4)[sideIndex] = 0;
        side->flags &= ~1u;

        side->subsystem0 = RegisterSubsystem0(sideIndex);
        if (side->subsystem0 == NULL)
            goto setup_failed;
        side->subsystem1 = RegisterSubsystem1(0, sideIndex, side->selector);
        if (side->subsystem1 == NULL)
            goto setup_failed;
        side->subsystem2 = RegisterSubsystem2(sideIndex);
        if (side->subsystem2 == NULL)
            goto setup_failed;
        side->subsystem3 = RegisterSubsystem3(sideIndex, 0x100, 7);
        if (side->subsystem3 == NULL)
            goto setup_failed;
        side->subsystem4 = RegisterSubsystem4(sideIndex);
        if (side->subsystem4 == NULL)
            goto setup_failed;
        side->subsystem5 = RegisterSubsystem5(sideIndex);
        if (side->subsystem5 == NULL)
            goto setup_failed;
        side->subsystem6 = RegisterSubsystem6(sideIndex);
        if (side->subsystem6 == NULL)
            goto setup_failed;
    }

    manager->sharedSubsystem = RegisterSharedSubsystem(2, 0x320, 1);
    if (manager->sharedSubsystem == NULL)
        goto setup_failed;

    manager->secondarySubsystem = RegisterSecondarySubsystem();
    if (manager->secondarySubsystem == NULL)
        goto setup_failed;

    switch (manager->setupMode)
    {
    case 4:
        manager->limitB4 = 18;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[0].subsystem1)->valueA0 = 10;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[0].subsystem1)->valueA4 = 10;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[1].subsystem1)->valueA0 = 10;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[1].subsystem1)->valueA4 = 10;
        manager->valueC4 = 600;
        manager->valueC8 = 22;
        break;

    case 3:
        manager->limitB4 = 8;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[0].subsystem1)->valueA0 = 8;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[0].subsystem1)->valueA4 = 8;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[1].subsystem1)->valueA0 = 8;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[1].subsystem1)->valueA4 = 8;
        if (manager->gameMode == 2)
        {
            manager->valueC4 = 900;
            manager->valueC8 = 22;
        }
        else
        {
            manager->limitB4 = manager->counterFC + 6;
            manager->valueC4 = 720;
            manager->valueC8 = manager->counterFC / 2 + 17;
        }
        break;

    case 2:
        manager->limitB4 = 6;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[0].subsystem1)->valueA0 = 4;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[0].subsystem1)->valueA4 = 4;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[1].subsystem1)->valueA0 = 4;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[1].subsystem1)->valueA4 = 4;
        if (manager->gameMode == 2)
        {
            manager->valueC4 = 720;
            manager->valueC8 = 22;
        }
        else
        {
            manager->limitB4 = manager->counterFC + 3;
            manager->valueC4 = 900;
            manager->valueC8 = manager->counterFC / 2 + 15;
        }
        break;

    case 1:
        manager->limitB4 = 1;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[0].subsystem1)->valueA0 = 1;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[0].subsystem1)->valueA4 = 1;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[1].subsystem1)->valueA0 = 1;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[1].subsystem1)->valueA4 = 1;
        if (manager->gameMode == 2)
        {
            manager->valueC4 = 900;
            manager->valueC8 = 20;
        }
        else
        {
            manager->limitB4 = manager->counterFC + 1;
            manager->valueC4 = 1200;
            manager->valueC8 = manager->counterFC + 10;
        }
        break;

    case 0:
        manager->limitB4 = 1;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[0].subsystem1)->valueA0 = 1;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[0].subsystem1)->valueA4 = 1;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[1].subsystem1)->valueA0 = 1;
        reinterpret_cast<SetupStateBuffer *>(manager->sides[1].subsystem1)->valueA4 = 1;
        manager->valueC4 = 1800;
        if (manager->gameMode == 2)
        {
            manager->valueC8 = 16;
        }
        else
        {
            manager->limitB4 = manager->counterFC / 3 + 1;
            manager->valueC8 = manager->counterFC / 2 + 6;
        }
        break;
    }

    FinalizeSubsystems(NULL);
    FinishLoading(&g_Supervisor);
    while ((manager->flags & 0x60u) != 0)
        Sleep(17);

    manager->gameplaySetupState = 0;
    supervisor->runningSubthreadHandle = 0;
    supervisor->subthreadCloseRequestActive = 0;
    supervisor->subthreadActive = 0;
    supervisor->screenTransitionCountdown = 60;
    supervisor->value59C = 0;
    g_SetupStatus = 2;
    supervisor->systemTime = timeGetTime();
    return;

setup_failed:
    manager->gameplaySetupState = 2;
    FinishLoading(&g_Supervisor);
    supervisor->runningSubthreadHandle = 0;
    supervisor->subthreadCloseRequestActive = 0;
    supervisor->subthreadActive = 0;
    supervisor->value59C = 0;
    g_SetupStatus = 2;
}
