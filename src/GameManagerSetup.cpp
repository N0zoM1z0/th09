// Partial target-facing reconstruction of the TH09 gameplay setup thread.
// The local layout views name only fields exercised by this bounded packet;
// they do not claim the complete original GameManager or Supervisor layouts.
// Provisional helper names describe observed call-site roles only.

#include "AnmCaptureRequest.hpp"
#include "AsciiManager.hpp"
#include "AnmManager.hpp"
#include "Chain.hpp"
#include "GameConfiguration.hpp"
#include "ReplayInputState.hpp"
#include "Supervisor.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct SetupStateBuffer;

struct SetupSideState
{
    void *subsystem0;
    void *subsystem1;
    void *subsystem2;
    void *subsystem3;
    void *subsystem4;
    void *subsystem5;
    void *subsystem6;
    SetupStateBuffer *state;
    int value20;
    int value24;
    int selector;
    unsigned char unknown_2C[8];
    unsigned int flags;
};

typedef char SetupSideStateSizeCheck[(sizeof(SetupSideState) == 0x38) ? 1 : -1];

struct SetupStateBuffer
{
    float value00;
    unsigned int value04;
    unsigned int value08;
    unsigned int value0C;
    int phase10;
    unsigned char value14;
    unsigned char unknown_15[0x8B];
    int valueA0;
    int valueA4;
};

struct SetupLoadingPosition
{
    float x;
    float y;
    float z;
};

typedef char SetupLoadingPositionSizeCheck[
    (sizeof(SetupLoadingPosition) == 0x0C) ? 1 : -1];

struct SpawnedSideObjectView
{
    unsigned char unknown0000[0x335C];
    int selector335C;
    unsigned char unknown3360[0x20];
    unsigned int flags3380;
};

struct SideSpawnerSetupView
{
    SpawnedSideObjectView *Spawn(
        short type, SetupLoadingPosition *position, int sprite, signed char variant,
        int lifetime, int mirror, int unknown);
};

struct SideAuxSetupView
{
    void Trigger(int index);
};

struct GameSubsystemSetupView
{
    int IsActive();
};

struct SetupRngView
{
    int Next();
};

struct AnmManagerOnUpdateView
{
    void ReleaseSurface(int index);
};

struct SetupScoreRecordView
{
    unsigned int magic;
    unsigned short chapterSize;
    unsigned short chapterSizeCopy;
    unsigned char version;
    unsigned char runtimeMarker;
    unsigned char unknown0A[2];
    int value0C;
    float value10;
    unsigned char sideValue14;
    unsigned char setupMode15;
    unsigned char unknown16;
    unsigned char zero17;
    unsigned char unknown18[0x13];
    unsigned char stateValue2B;
};

typedef char SetupScoreRecordViewSizeCheck[
    (sizeof(SetupScoreRecordView) == 0x2C) ? 1 : -1];

struct GameManagerSetupLayout
{
    SetupSideState sides[2];
    unsigned char unknown_70[0x0C];
    void *sharedSubsystem;
    unsigned char unknown_80[0x28];
    void *gameSubsystem;
    void *secondarySubsystem;
    unsigned int valueB0;
    int limitB4;
    int valueB8;
    unsigned char unknown_BC[4];
    unsigned int valueC0;
    unsigned int valueC4;
    int valueC8;
    int valueCC;
    int valueD0;
    void *stageObject;
    unsigned char clearD8[0x10];
    void *heapE8;
    float speedEC;
    unsigned char unknown_F0[4];
    int valueF4;
    int valueF8;
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
    int gameplaySetupWaitFrames;
    int playtimeFrames;
    int replayPauseRecorded;
    unsigned int flags;
    unsigned char unknown_138[4];
    unsigned char inGameMenu;
    unsigned char menuBlock13D;
    unsigned char menuBlock13E;
    unsigned char unknown_13F;
    int demoFrameCount;
    unsigned char unknown_144;
    unsigned char modeByte145;
    unsigned short seed146;
    unsigned char unknown_148[0x1FE];
    unsigned short seedCopy346;
    int value348;
    unsigned char unknown_34C[0x28];
    int value374;
    unsigned char unknown_378[0x14];
    int stageStartupMode;

    void UpdateProgress();
    void AdvanceTimedState();
    static int OnUpdate(GameManagerSetupLayout *gameManager);
    static int OnDraw(GameManagerSetupLayout *gameManager);
    static int AddedCallback(GameManagerSetupLayout *gameManager);
    static int DeletedCallback(GameManagerSetupLayout *gameManager);
    static int RegisterChain();
    static void CleanupGameplayState();
};

struct SoundPlayerSetupView
{
    void QueueCommand(int opcode, int argument, char *path);
    void PlaySoundByIdx(int soundIndex, int pan);
    int ProcessQueues();
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
    unsigned char unknown_5A8[0x28];
    AnmLoaded *loadingAnm5D0;
    unsigned int flags;
    unsigned char unknown_5D8[0xD4];
    int runningSubthreadHandle;
    unsigned char unknown_6B0[4];
    int subthreadCloseRequestActive;
    int subthreadActive;
    unsigned char unknown_6BC[0x84];
    int loadingVmsHaveBeenSetup;
    unsigned char unknown_744[0x54];
    unsigned int totalPlayTime;
    unsigned int systemTime;

    void SetupLoadingVmsAndInitCapture(SetupLoadingPosition *position);
    void StartEffect(int index);
    int ThreadStart(LPTHREAD_START_ROUTINE startFunction, void *startParam);
    int StartStageAudio(int mode, int index);
    int FadeOutMusic(float seconds);
    void UpdatePlayTime();
    void UpdateGameTime();
};


typedef char SupervisorSetupLayoutLoadingAnmAt5D0[
    (offsetof(SupervisorSetupLayout, loadingAnm5D0) == 0x5D0) ? 1 : -1];
typedef char SupervisorSetupLayoutLoadingFlagAt740[
    (offsetof(SupervisorSetupLayout, loadingVmsHaveBeenSetup) == 0x740) ? 1 : -1];

extern GameManagerSetupLayout g_GameManager;
extern Chain g_Chain;
extern ChainElem g_GameManagerCalcChain;
extern ChainElem g_GameManagerDrawChain;
extern GameConfiguration *g_GameConfiguration;
extern char g_ReplayPlayTimeText[];
extern int g_SetupStatus;
extern unsigned short g_SetupSeedSource;
extern SoundPlayerSetupView g_SoundPlayer;
extern AsciiManager g_AsciiManager;
extern AnmVm g_SupervisorLoadingVms[3];
extern SetupScoreRecordView g_DeletedScoreRecord;
extern unsigned char g_DeletedScratch[0x2BC0];
extern int g_GameManagerChainState;
extern SetupRngView g_SetupRng;
extern int g_LastSpawnLane;
extern void *__fastcall RegisterScreenEffect(
    int type, int duration, int x, int y, int unknown, int priority, int argument);
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
extern void __fastcall FormatCurrentDateString(char *buffer);
extern void __fastcall ReleaseStageObject(void *object);
extern void *__fastcall CreateStageObject(int mode, void *context);
extern void __fastcall SetStageObjectMode(void *object, int mode);
extern void __fastcall BeginLoading(Supervisor *supervisor);
extern void __fastcall FinishLoading(Supervisor *supervisor);
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
extern void __fastcall SupervisorCalculateFps(int shouldDraw);
extern void __fastcall GameplaySetupThread(void *unused);
int GameManagerSetupLayout::OnUpdate(GameManagerSetupLayout *gameManager)
{
    SupervisorSetupLayout *supervisor =
        reinterpret_cast<SupervisorSetupLayout *>(&g_Supervisor);

    g_GameManager.playtimeFrames++;

    if (g_GameManager.gameMode != 2 ||
        g_GameManager.sides[0].selector != 0 ||
        g_GameManager.sides[1].selector != 0)
    {
        unsigned short input00 = g_ReplayInputStates[2].currentInput;
        unsigned short input02 = g_ReplayInputStates[2].word02;
        unsigned short input04 = g_ReplayInputStates[2].repeatOutput;
        unsigned short input06 = g_ReplayInputStates[2].word06;
        unsigned short input08 = g_ReplayInputStates[2].word08;

        g_ReplayInputStates[0].currentInput = input00;
        g_ReplayInputStates[0].word02 = input02;
        g_ReplayInputStates[0].repeatOutput = input04;
        g_ReplayInputStates[0].word06 = input06;
        g_ReplayInputStates[0].word08 = input08;
        g_ReplayInputStates[1].currentInput = input00;
        g_ReplayInputStates[1].word02 = input02;
        g_ReplayInputStates[1].repeatOutput = input04;
        g_ReplayInputStates[1].word06 = input06;
        g_ReplayInputStates[1].word08 = input08;
    }

    if (g_GameManager.gameplaySetupState != 0)
    {
        if (g_GameManager.gameplaySetupState == 2)
            return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
        gameManager->gameplaySetupWaitFrames++;
        return CHAIN_CALLBACK_RESULT_BREAK;
    }

    if (g_GameManager.stageStartupMode != 0)
    {
        reinterpret_cast<AnmManagerOnUpdateView *>(g_AnmManager)->ReleaseSurface(8);
        supervisor->loadingVmsHaveBeenSetup = 0;
        if (g_GameManager.stageStartupMode == 1)
            supervisor->StartStageAudio(0, 0);
        g_GameManager.stageStartupMode = 0;
    }

    if (gameManager->menuBlock13D == 0 &&
        gameManager->menuBlock13E == 0 &&
        gameManager->inGameMenu == 0 &&
        (gameManager->flags & 2u) == 0 &&
        (g_ReplayInputStates[2].word06 & 8u) != 0)
    {
        gameManager->inGameMenu = 1;
        gameManager->replayPauseRecorded = 1;
        g_SoundPlayer.QueueCommand(6, 0, "Pause");
        g_SoundPlayer.PlaySoundByIdx(0x22, 0);
        supervisor->UpdateGameTime();
    }

    if ((gameManager->flags & 2u) != 0)
    {
        if (g_ReplayInputStates[2].word06 != 0)
            supervisor->state590 = 1;

        gameManager->demoFrameCount++;
        if (gameManager->demoFrameCount == 6000)
        {
            RegisterScreenEffect(4, 120, 0, 0, 0, 0x23, 2);
            supervisor->FadeOutMusic(3.0f);
        }
        if (gameManager->demoFrameCount == 6120)
        {
            supervisor->state590 = 1;
            return CHAIN_CALLBACK_RESULT_BREAK;
        }
    }

    if (gameManager->inGameMenu == 1 ||
        gameManager->inGameMenu == 2 ||
        gameManager->menuBlock13D != 0 ||
        gameManager->menuBlock13E != 0)
    {
        return CHAIN_CALLBACK_RESULT_BREAK;
    }

    if (!reinterpret_cast<GameSubsystemSetupView *>(g_GameManager.gameSubsystem)->IsActive())
    {
        g_GameManager.valueD0++;

        if (++g_GameManager.valueB0 % g_GameManager.valueC4 ==
            g_GameManager.valueC4 - 1 &&
            ++g_GameManager.limitB4 > g_GameManager.valueC8)
        {
            g_GameManager.limitB4 = g_GameManager.valueC8;
        }

        g_GameManager.valueC0++;
        if (g_GameManager.valueB0 % 3600u == 3599u)
        {
            if (g_GameManager.sides[0].state->valueA0 < 16)
                g_GameManager.sides[0].state->valueA0++;
            if (g_GameManager.sides[0].state->valueA4 < 16)
                g_GameManager.sides[0].state->valueA4++;
            if (g_GameManager.sides[1].state->valueA0 < 16)
                g_GameManager.sides[1].state->valueA0++;
            if (g_GameManager.sides[1].state->valueA4 < 16)
                g_GameManager.sides[1].state->valueA4++;
        }

        if (g_GameManager.valueB8 < 6 &&
            g_GameManager.valueC0 % 720u == 719u)
        {
            g_GameManager.valueB8++;
        }

        if ((g_GameManager.flags & 0x1800u) == 0 &&
            (g_GameManager.sides[0].flags & 1u) == 0)
        {
            unsigned int elapsed = gameManager->valueB0;
            int increase;
            if (elapsed < 1800u)
                increase = 3;
            else if (elapsed < 3600u)
                increase = 4;
            else
                increase = elapsed < 5400u ? 6 : 7;
            g_GameManager.valueCC += increase;
        }

        if (g_GameManager.valueCC > 10000)
        {
            int lane = static_cast<unsigned short>(g_SetupRng.Next()) % 4;
            if (lane == g_LastSpawnLane)
            {
                lane++;
                if (lane >= 4)
                    lane = 0;
            }

            g_GameManager.valueCC -= 10000;
            SetupLoadingPosition position;
            position.x = 0.0f;
            position.z = 0.0f;
            g_LastSpawnLane = lane;
            position.y = -32.0f;

            SpawnedSideObjectView *object =
                reinterpret_cast<SideSpawnerSetupView *>(g_GameManager.sides[0].subsystem4)
                    ->Spawn(19, &position, 830, -2, 10000, 0, 0);
            object->selector335C = lane;
            object->flags3380 |= 0x2000u;

            object =
                reinterpret_cast<SideSpawnerSetupView *>(g_GameManager.sides[1].subsystem4)
                    ->Spawn(19, &position, 830, -2, 10000, 0, 0);
            object->flags3380 |= 0x2000u;
            object->selector335C = lane;

            g_SoundPlayer.PlaySoundByIdx(0x33, 0);
            reinterpret_cast<SideAuxSetupView *>(g_GameManager.sides[0].subsystem6)->Trigger(1);
            reinterpret_cast<SideAuxSetupView *>(g_GameManager.sides[1].subsystem6)->Trigger(1);
        }
    }

    for (int sideIndex = 0; sideIndex < 2; sideIndex++)
    {
        if (g_GameManager.sides[sideIndex].state->value08 >= 1000000000u)
            g_GameManager.sides[sideIndex].state->value08 = 999999999u;

        if (g_GameManager.sides[sideIndex].state->value04 !=
            g_GameManager.sides[sideIndex].state->value08)
        {
            if (g_GameManager.sides[sideIndex].state->value08 <
                g_GameManager.sides[sideIndex].state->value04)
            {
                g_GameManager.sides[sideIndex].state->value08 =
                    g_GameManager.sides[sideIndex].state->value04;
            }

            unsigned int step =
                (g_GameManager.sides[sideIndex].state->value08 -
                 g_GameManager.sides[sideIndex].state->value04) /
                15u;
            if (step >= 578910u)
                step = 578910u;
            else if (step == 0)
                step = 1;

            if (g_GameManager.sides[sideIndex].state->value0C < step)
                g_GameManager.sides[sideIndex].state->value0C = step;
            if (g_GameManager.sides[sideIndex].state->value04 +
                    g_GameManager.sides[sideIndex].state->value0C >
                g_GameManager.sides[sideIndex].state->value08)
            {
                g_GameManager.sides[sideIndex].state->value0C =
                    g_GameManager.sides[sideIndex].state->value08 -
                    g_GameManager.sides[sideIndex].state->value04;
            }
            g_GameManager.sides[sideIndex].state->value04 +=
                g_GameManager.sides[sideIndex].state->value0C;
            if (g_GameManager.sides[sideIndex].state->value04 >=
                g_GameManager.sides[sideIndex].state->value08)
            {
                g_GameManager.sides[sideIndex].state->value0C = 0;
                g_GameManager.sides[sideIndex].state->value04 =
                    g_GameManager.sides[sideIndex].state->value08;
            }
        }
    }

    if (g_GameManager.gameMode == 0)
    {
        if (g_GameManager.sides[0].state->value04 >=
            static_cast<unsigned int>(
                2000000 * g_GameManager.sides[0].state->phase10 + 1000000))
        {
            g_GameManager.AdvanceTimedState();
        }
    }
    if (g_GameManager.gameMode == 1 &&
        g_GameManager.sides[0].state->value04 >=
            static_cast<unsigned int>(
                500000 * (g_GameManager.sides[0].state->phase10 + 1)))
    {
        g_GameManager.AdvanceTimedState();
    }

    gameManager->value348++;
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}


int GameManagerSetupLayout::OnDraw(GameManagerSetupLayout *gameManager)
{
    SupervisorSetupLayout *supervisor =
        reinterpret_cast<SupervisorSetupLayout *>(&g_Supervisor);

    if (gameManager->inGameMenu != 0)
        gameManager->inGameMenu = 2;

    if (supervisor->state590 != 2)
        return CHAIN_CALLBACK_RESULT_BREAK;

    if ((gameManager->flags & 0x60u) == 0x20u)
        return CHAIN_CALLBACK_RESULT_BREAK;

    if (gameManager->gameplaySetupState != 0)
        return CHAIN_CALLBACK_RESULT_BREAK;

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

int GameManagerSetupLayout::RegisterChain()
{
    GameManagerSetupLayout *manager = &g_GameManager;

    g_GameManager.value348 = 0;
    g_GameManagerCalcChain.callback =
        (ChainCallback)GameManagerSetupLayout::OnUpdate;
    g_GameManagerCalcChain.addedCallback =
        (ChainLifetimeCallback)GameManagerSetupLayout::AddedCallback;
    g_GameManagerCalcChain.deletedCallback =
        (ChainLifetimeCallback)GameManagerSetupLayout::DeletedCallback;
    g_GameManagerCalcChain.arg = manager;
    if (g_Chain.AddToCalcChain(&g_GameManagerCalcChain, 2))
        return -1;

    g_GameManagerDrawChain.callback =
        (ChainCallback)GameManagerSetupLayout::OnDraw;
    g_GameManagerDrawChain.addedCallback = NULL;
    g_GameManagerDrawChain.deletedCallback = NULL;
    g_GameManagerDrawChain.arg = manager;
    g_Chain.AddToDrawChain(&g_GameManagerDrawChain, 5);
    return 0;
}


int GameManagerSetupLayout::AddedCallback(GameManagerSetupLayout *gameManager)
{
    SupervisorSetupLayout *supervisor =
        reinterpret_cast<SupervisorSetupLayout *>(&g_Supervisor);

    if (supervisor->state590 != 3 &&
        supervisor->state590 != 12 &&
        supervisor->state590 != 10)
    {
        supervisor->flags |= 0x200;
    }
    else
    {
        supervisor->flags &= ~0x200u;
    }

    SetupLoadingPosition position;
    position.z = 0.0f;
    g_GameManager.gameplaySetupState = 1;

    if (supervisor->value594 == 1)
    {
        position.x = 500.0f;
        position.y = 440.0f;
        supervisor->SetupLoadingVmsAndInitCapture(&position);
        supervisor->StartEffect(0);
    }
    else
    {
        position.x = 280.0f;
        position.y = 430.0f;
        supervisor->SetupLoadingVmsAndInitCapture(&position);
    }

    if ((gameManager->flags & 0x60u) >= 0x40u)
        gameManager->flags = (gameManager->flags & ~0x40u) | 0x20u;

    supervisor->ThreadStart((LPTHREAD_START_ROUTINE)GameplaySetupThread, NULL);
    supervisor->UpdateGameTime();
    supervisor->UpdatePlayTime();
    return 0;
}




int GameManagerSetupLayout::DeletedCallback(GameManagerSetupLayout *gameManager)
{
    SupervisorSetupLayout *supervisor =
        reinterpret_cast<SupervisorSetupLayout *>(&g_Supervisor);

    g_GameManager.sides[0].value24 = g_GameManager.sides[0].value20;
    g_GameManager.sides[1].value24 = g_GameManager.sides[1].value20;
    g_GameManager.valueF8 = g_GameManager.valueF4;
    gameManager->flags &= ~4u;

    if (supervisor->state590 != 3 &&
        supervisor->state590 != 12 &&
        supervisor->state590 != 10)
    {
        supervisor->value598 = 1;
    }
    else
    {
        supervisor->value598 = 0;
    }

    memset(g_DeletedScratch, 0, sizeof(g_DeletedScratch));

    SetupStateBuffer *state =
        reinterpret_cast<SetupStateBuffer *>(g_GameManager.sides[0].state);
    if (state != NULL)
    {
        g_DeletedScoreRecord.value0C = state->value08;
        g_DeletedScoreRecord.sideValue14 =
            static_cast<unsigned char>(g_GameManager.sides[0].value20);
        g_DeletedScoreRecord.setupMode15 =
            static_cast<unsigned char>(g_GameManager.setupMode);
        g_DeletedScoreRecord.stateValue2B = state->value14;
        g_DeletedScoreRecord.value10 = 0.0f;
        g_DeletedScoreRecord.magic = 0x52435348u;
        g_DeletedScoreRecord.zero17 = 0;
        g_DeletedScoreRecord.chapterSizeCopy = sizeof(g_DeletedScoreRecord);
        g_DeletedScoreRecord.chapterSize = sizeof(g_DeletedScoreRecord);
        g_DeletedScoreRecord.version = 2;
        g_DeletedScoreRecord.runtimeMarker = 0;
    }

    if ((g_GameManager.flags & 8u) != 0 && supervisor->value598 != 0)
    {
        ReleaseStageObject(gameManager->stageObject);
        gameManager->stageObject = NULL;
    }

    SetupSideState *side = &gameManager->sides[0];
    int sideCount = 2;
    do
    {
        ReleaseSubsystem0(side->subsystem0);
        side->subsystem0 = NULL;
        ReleaseSubsystem1(side->subsystem1);
        side->subsystem1 = NULL;
        ReleaseSubsystem2(side->subsystem2);
        side->subsystem2 = NULL;
        ReleaseSubsystem3(side->subsystem3);
        side->subsystem3 = NULL;
        ReleaseSubsystem4(side->subsystem4);
        side->subsystem4 = NULL;
        ReleaseSubsystem5(side->subsystem5);
        side->subsystem5 = NULL;
        ReleaseSubsystem6(side->subsystem6);
        side->subsystem6 = NULL;
        side++;
    } while (--sideCount != 0);

    ReleaseGameSubsystem(gameManager->gameSubsystem);
    gameManager->gameSubsystem = NULL;
    ReleaseSecondarySubsystem(gameManager->secondarySubsystem);
    gameManager->secondarySubsystem = NULL;
    ReleaseSubsystem3(gameManager->sharedSubsystem);
    gameManager->sharedSubsystem = NULL;

    if (gameManager->heapE8 != NULL)
    {
        free(gameManager->heapE8);
        gameManager->heapE8 = NULL;
    }

    while (g_SoundPlayer.ProcessQueues())
    {
    }

    g_GameManagerChainState = 0;
    return 0;
}


void GameManagerSetupLayout::AdvanceTimedState()
{
    SetupStateBuffer *state =
        reinterpret_cast<SetupStateBuffer *>(g_GameManager.sides[0].state);

    if (state->value00 < 7.0f)
    {
        state->value00 += 1.0f;
        g_SoundPlayer.PlaySoundByIdx(0x1C, 0);
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
    FormatCurrentDateString(g_ReplayPlayTimeText);

    manager->flags |= 4;
    manager->flags &= ~0x1800u;
    manager->value374 = -1;
    previousMode = manager->gameMode;
    manager->valueD0 = 0;
    manager->value348 = 0;
    manager->stageStartupMode = 1;
    manager->value120 = 0xff;
    manager->valueB8 = 2;
    manager->valueCC = 0;
    manager->valueC0 = 0;
    manager->demoFrameCount = 0;
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
            manager->sides[sideIndex].state = static_cast<SetupStateBuffer *>(operator new(0xA0));
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
            if (state->value04 >= 2000000u * state->phase10 + 1000000u)
                g_GameManager.AdvanceTimedState();
        }
        if (manager->gameMode == 1)
        {
            SetupStateBuffer *state =
                reinterpret_cast<SetupStateBuffer *>(manager->sides[0].state);
            if (state->value04 >= 500000u * (state->phase10 + 1))
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

    SupervisorCalculateFps(0);
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

namespace
{
struct GameManagerResetRecord
{
    unsigned char unknown00[0x34];
    unsigned int flags34;
};

struct GameManagerResetView
{
    GameManagerResetRecord records00[3];
    unsigned char unknown0A8[0xB0 - 0xA8];
    int valueB0;
    unsigned char unknown0B4[4];
    int valueB8;
    int valueBC;
    int valueC0;
    unsigned char unknown0C4[0xD0 - 0xC4];
    int valueD0;
    unsigned char unknown0D4[0x110 - 0xD4];
    int value110;
    unsigned char unknown114[0x128 - 0x114];
    int value128;
    int value12C;
    int value130;
    unsigned char unknown134[0x13C - 0x134];
    unsigned char value13C;
    unsigned char value13D;
    unsigned char value13E;
    unsigned char unknown13F[0x380 - 0x13F];
    int value380;
};

typedef char GameManagerResetRecordSize38[
    (sizeof(GameManagerResetRecord) == 0x38) ? 1 : -1];
typedef char GameManagerResetValueB0AtB0[
    (offsetof(GameManagerResetView, valueB0) == 0xB0) ? 1 : -1];
typedef char GameManagerResetValue380At380[
    (offsetof(GameManagerResetView, value380) == 0x380) ? 1 : -1];
}

void __fastcall ResetGameManager(GameManagerSetupLayout *manager)
{
    GameManagerResetView *view =
        reinterpret_cast<GameManagerResetView *>(manager);

    for (int i = 0; i < 3; ++i)
        view->records00[i].flags34 &= ~1u;

    view->valueB0 = 0;
    view->valueB8 = 2;
    view->valueC0 = 0;
    view->valueD0 = 0;
    view->valueBC = 0;
    view->value110 = 0;
    view->value128 = 0;
    view->value12C = 0;
    view->value130 = 0;
    view->value13C = 0;
    view->value13D = 0;
    view->value13E = 0;
    view->value380 = 0;
}

namespace
{
struct GameManagerCutChainSupervisorView
{
    unsigned char unknown000[0x5B8];
    float frameRateMultiplier5B8;
};

typedef char GameManagerCutChainSupervisorRateAt5B8[
    (offsetof(GameManagerCutChainSupervisorView, frameRateMultiplier5B8) == 0x5B8) ? 1 : -1];
}

void __fastcall GameManager_CutChain()
{
    if (g_GameManager.sides[0].state->value08 >= 1000000000u)
        g_GameManager.sides[0].state->value08 = 999999999u;
    g_GameManager.sides[0].state->value04 =
        g_GameManager.sides[0].state->value08;

    if (g_GameManager.sides[1].state->value08 >= 1000000000u)
        g_GameManager.sides[1].state->value08 = 999999999u;
    g_GameManager.sides[1].state->value04 =
        g_GameManager.sides[1].state->value08;

    reinterpret_cast<GameManagerCutChainSupervisorView *>(&g_Supervisor)
        ->frameRateMultiplier5B8 = 1.0f;
    g_Chain.Cut(&g_GameManagerCalcChain);
    g_Chain.Cut(&g_GameManagerDrawChain);
}

void __fastcall FormatCurrentDateString(char *buffer)
{
    time_t now;
    time(&now);
    strftime(buffer, 10, "%y/%m/%d", localtime(&now));
}
