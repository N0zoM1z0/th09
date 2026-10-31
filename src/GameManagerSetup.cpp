#include "EffectManager.hpp"
#include "Background.hpp"
#include "CardAttack.hpp"
#include "FrontSide.hpp"
#include "TitleScoreRecordView.hpp"
#include "FrontMessageOwnerView.hpp"
#include "EnemyManager.hpp"
#include "BulletManager.hpp"
#include "ExAttackController.hpp"
#include "RngRuntimeLeaves.hpp"
#include "SoundPlayer.hpp"
#include "EnemyManagerCardAttackSpawn.hpp"
#include "PlayerLifecycleView.hpp"
#include "PlayerRegisterChain.hpp"
// Partial target-facing reconstruction of the TH09 gameplay setup thread.
// The local layout views name only fields exercised by this bounded packet;
// they do not claim the complete original GameManager or Supervisor layouts.
// Provisional helper names describe observed call-site roles only.

#include "AnmCaptureRequest.hpp"
#include "AsciiManager.hpp"
#include "AnmManager.hpp"
#include "Chain.hpp"
#include "GameConfiguration.hpp"
#include "GameManagerOpponentSelectionView.hpp"
#include "GameManagerEclStateView.hpp"
#include "ScreenEffect.hpp"
#include "ReplayInputState.hpp"
#include "ReplayManagerRuntimeView.hpp"
#include "Supervisor.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <new>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void __fastcall ResetExAttackRecords(ExAttackController *controller);


struct SetupStateBuffer
{
    float value00;
    unsigned int value04;
    unsigned int value08;
    unsigned int value0C;
    int phase10;
    unsigned char value14;
    unsigned char unknown_15[0x8B];
};

typedef char SetupStateBufferSizeIsA0[
    (sizeof(SetupStateBuffer) == 0xA0) ? 1 : -1];
typedef char SetupPlayerCounterA0[
    (offsetof(PlayerLifecycleView, header24) +
     offsetof(PlayerHeaderStateView, state7C) == 0xA0) ? 1 : -1];
typedef char SetupPlayerCounterA4[
    (offsetof(PlayerLifecycleView, header24) +
     offsetof(PlayerHeaderStateView, state80) == 0xA4) ? 1 : -1];

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

struct SideAuxSetupView
{
    void Trigger(int index);
};

struct GameSubsystemSetupView
{
    int IsActive();
};




typedef char SetupScoreRecordViewSizeCheck[
    (sizeof(TitleScoreRecordView) == 0x2C) ? 1 : -1];

#include "GameManagerSetupView.hpp"

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

typedef char SetupGameManagerInstructionMaskAt120[
    (offsetof(GameManagerSetupLayout, instructionMask120) ==
     offsetof(GameManagerEclStateView, instructionMask120)) ? 1 : -1];

extern Chain g_Chain;
extern ChainElem g_GameManagerCalcChain;
extern ChainElem g_GameManagerDrawChain;
extern GameConfiguration *g_GameConfiguration;
extern char g_ReplayPlayTimeText[];
extern int g_SetupStatus;
extern RngRuntimeView g_Rng;

extern AsciiManager g_AsciiManager;
extern AnmVm g_SupervisorLoadingVms[3];

extern unsigned char g_DeletedScratch[0x2BC0];
extern int g_GameManagerChainState;

extern int g_LastSpawnLane;
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
struct ReplayStageDispatchView;
extern void __fastcall DispatchReplayStage(ReplayStageDispatchView *manager, int replayMode);
extern void __fastcall FinishLoading(Supervisor *supervisor);
extern void *__fastcall CreateGameSubsystem();
extern int __fastcall PreloadPlayerAnmResources();
extern void __fastcall SupervisorCalculateFps(int shouldDraw);
extern void __fastcall GameplaySetupThread(void *unused);
int GameManagerSetupLayout::OnUpdate(GameManagerSetupLayout *gameManager)
{
    GameManagerSetupLayout &globalManager =
        *reinterpret_cast<GameManagerSetupLayout *>(&g_GameManager);

    SupervisorSetupLayout *supervisor =
        reinterpret_cast<SupervisorSetupLayout *>(&g_Supervisor);

    globalManager.playtimeFrames++;

    if (globalManager.gameMode != 2 ||
        globalManager.sides[0].selector != 0 ||
        globalManager.sides[1].selector != 0)
    {
        // The source is the distinct third ReplayInputState: its five word
        // members cannot alias either destination [0] or [1]. Bind each real
        // source lvalue so VC7.1 allocates the original AX/CX/DX/DI/SI five
        // word capture, followed by the two ordered five-word publications.
        const unsigned short &w0 = g_ReplayInputStates[2].currentInput;
        const unsigned short &w1 = g_ReplayInputStates[2].word02;
        const unsigned short &w2 = g_ReplayInputStates[2].repeatOutput;
        const unsigned short &w3 = g_ReplayInputStates[2].word06;
        const unsigned short &w4 = g_ReplayInputStates[2].word08;
        g_ReplayInputStates[0].currentInput = w0;
        g_ReplayInputStates[0].word02 = w1;
        g_ReplayInputStates[0].repeatOutput = w2;
        g_ReplayInputStates[0].word06 = w3;
        g_ReplayInputStates[0].word08 = w4;
        g_ReplayInputStates[1].currentInput = w0;
        g_ReplayInputStates[1].word02 = w1;
        g_ReplayInputStates[1].repeatOutput = w2;
        g_ReplayInputStates[1].word06 = w3;
        g_ReplayInputStates[1].word08 = w4;

    }

    if (globalManager.gameplaySetupState != 0)
    {
        if (globalManager.gameplaySetupState == 2)
            return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
        gameManager->gameplaySetupWaitFrames++;
        return CHAIN_CALLBACK_RESULT_BREAK;
    }

    if (globalManager.stageStartupMode != 0)
    {
        g_AnmManager->ReleaseSurface(8);
        supervisor->loadingVmsHaveBeenSetup = 0;
        if (globalManager.stageStartupMode == 1)
            supervisor->StartStageAudio(0, 0);
        globalManager.stageStartupMode = 0;
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
            ScreenEffect::RegisterChain(
                SCREEN_EFFECT_FULL_FADE_OUT, 120, 0, 0, 0, 0x23, 2);
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

    if (!reinterpret_cast<GameSubsystemSetupView *>(globalManager.gameSubsystem)->IsActive())
    {
        globalManager.valueD0++;

        if (++globalManager.valueB0 % globalManager.valueC4 ==
            globalManager.valueC4 - 1 &&
            ++globalManager.limitB4 > globalManager.valueC8)
        {
            globalManager.limitB4 = globalManager.valueC8;
        }

        globalManager.valueC0++;
        // These difficulty counters belong to each side's subsystem1 object;
        // the separate state buffer is allocated as 0xA0 bytes.
        if (globalManager.valueB0 % 3600u == 3599u)
        {
            if (static_cast<PlayerLifecycleView *>(globalManager.sides[0].subsystem1)->header24.state7C < 16)
                static_cast<PlayerLifecycleView *>(globalManager.sides[0].subsystem1)->header24.state7C++;
            if (static_cast<PlayerLifecycleView *>(globalManager.sides[0].subsystem1)->header24.state80 < 16)
                static_cast<PlayerLifecycleView *>(globalManager.sides[0].subsystem1)->header24.state80++;
            if (static_cast<PlayerLifecycleView *>(globalManager.sides[1].subsystem1)->header24.state7C < 16)
                static_cast<PlayerLifecycleView *>(globalManager.sides[1].subsystem1)->header24.state7C++;
            if (static_cast<PlayerLifecycleView *>(globalManager.sides[1].subsystem1)->header24.state80 < 16)
                static_cast<PlayerLifecycleView *>(globalManager.sides[1].subsystem1)->header24.state80++;
        }

        if (globalManager.valueB8 < 6 &&
            globalManager.valueC0 % 720u == 719u)
        {
            globalManager.valueB8++;
        }

        if ((globalManager.flags & 0x1800u) == 0 &&
            (globalManager.sides[0].flags & 1u) == 0)
        {
            unsigned int elapsed = gameManager->valueB0;
            int increase;
            if (elapsed < 1800u)
                increase = 3;
            else if (elapsed < 3600u)
                increase = 4;
            else
                increase = elapsed < 5400u ? 6 : 7;
            globalManager.valueCC += increase;
        }

        if (globalManager.valueCC > 10000)
        {
            // Sample an unsigned four-lane value before the later int adjustment.
            unsigned short laneSample =
                g_Rng.NextU16() % 4;
            int lane = laneSample;
            if (lane == g_LastSpawnLane)
            {
                lane++;
                if (lane >= 4)
                    lane = 0;
            }

            globalManager.valueCC -= 10000;
            CardAttackSpawnPosition position;
            position.x = 0.0f;
            position.z = 0.0f;
            g_LastSpawnLane = lane;
            position.y = -32.0f;

            SpawnedSideObjectView *object =
                static_cast<SpawnedSideObjectView *>(
                    reinterpret_cast<EnemyManagerCardAttackSpawnView *>(globalManager.sides[0].subsystem4)
                        ->SpawnEnemyRecord(19, &position, 830, -2, 10000, 0, 0));
            object->selector335C = lane;
            object->flags3380 |= 0x2000u;

            object =
                static_cast<SpawnedSideObjectView *>(
                    reinterpret_cast<EnemyManagerCardAttackSpawnView *>(globalManager.sides[1].subsystem4)
                        ->SpawnEnemyRecord(19, &position, 830, -2, 10000, 0, 0));
            object->flags3380 |= 0x2000u;
            object->selector335C = lane;

            g_SoundPlayer.PlaySoundByIdx(0x33, 0);
            reinterpret_cast<SideAuxSetupView *>(globalManager.sides[0].subsystem6)->Trigger(1);
            reinterpret_cast<SideAuxSetupView *>(globalManager.sides[1].subsystem6)->Trigger(1);
        }
    }

    for (int sideIndex = 0; sideIndex < 2; sideIndex++)
    {
        if (globalManager.sides[sideIndex].state->value08 >= 1000000000u)
            globalManager.sides[sideIndex].state->value08 = 999999999u;

        if (globalManager.sides[sideIndex].state->value04 !=
            globalManager.sides[sideIndex].state->value08)
        {
            if (globalManager.sides[sideIndex].state->value08 <
                globalManager.sides[sideIndex].state->value04)
            {
                globalManager.sides[sideIndex].state->value08 =
                    globalManager.sides[sideIndex].state->value04;
            }

            unsigned int step =
                (globalManager.sides[sideIndex].state->value08 -
                 globalManager.sides[sideIndex].state->value04) /
                15u;
            if (step >= 578910u)
                step = 578910u;
            else if (step == 0)
                step = 1;

            if (globalManager.sides[sideIndex].state->value0C < step)
                globalManager.sides[sideIndex].state->value0C = step;
            if (globalManager.sides[sideIndex].state->value04 +
                    globalManager.sides[sideIndex].state->value0C >
                globalManager.sides[sideIndex].state->value08)
            {
                globalManager.sides[sideIndex].state->value0C =
                    globalManager.sides[sideIndex].state->value08 -
                    globalManager.sides[sideIndex].state->value04;
            }
            globalManager.sides[sideIndex].state->value04 +=
                globalManager.sides[sideIndex].state->value0C;
            if (globalManager.sides[sideIndex].state->value04 >=
                globalManager.sides[sideIndex].state->value08)
            {
                globalManager.sides[sideIndex].state->value0C = 0;
                globalManager.sides[sideIndex].state->value04 =
                    globalManager.sides[sideIndex].state->value08;
            }
        }
    }

    if (globalManager.gameMode == 0)
    {
        if (globalManager.sides[0].state->value04 >=
            2000000u * globalManager.sides[0].state->phase10 + 1000000u)
        {
            globalManager.AdvanceTimedState();
        }
    }
    if (globalManager.gameMode == 1 &&
        globalManager.sides[0].state->value04 >=
            500000u * (globalManager.sides[0].state->phase10 + 1))
    {
        globalManager.AdvanceTimedState();
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
    GameManagerSetupLayout &globalManager =
        *reinterpret_cast<GameManagerSetupLayout *>(&g_GameManager);

    GameManagerSetupLayout *manager = &globalManager;

    globalManager.value348 = 0;
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
    GameManagerSetupLayout &globalManager =
        *reinterpret_cast<GameManagerSetupLayout *>(&g_GameManager);

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
    globalManager.gameplaySetupState = 1;

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
    GameManagerSetupLayout &globalManager =
        *reinterpret_cast<GameManagerSetupLayout *>(&g_GameManager);

    SupervisorSetupLayout *supervisor =
        reinterpret_cast<SupervisorSetupLayout *>(&g_Supervisor);

    globalManager.sides[0].value24 = globalManager.sides[0].value20;
    globalManager.sides[1].value24 = globalManager.sides[1].value20;
    globalManager.valueF8 = globalManager.valueF4;
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
        reinterpret_cast<SetupStateBuffer *>(globalManager.sides[0].state);
    if (state != NULL)
    {
        g_TitleScoreRecord.score = state->value08;
        g_TitleScoreRecord.category0 =
            static_cast<unsigned char>(globalManager.sides[0].value20);
        g_TitleScoreRecord.category1 =
            static_cast<unsigned char>(globalManager.setupMode);
        g_TitleScoreRecord.scoreDigit = state->value14;
        g_TitleScoreRecord.value10 = 0.0f;
        g_TitleScoreRecord.magic = 0x52435348u;
        g_TitleScoreRecord.unknown17 = 0;
        g_TitleScoreRecord.chapterSizeCopy = sizeof(g_TitleScoreRecord);
        g_TitleScoreRecord.chapterSize = sizeof(g_TitleScoreRecord);
        g_TitleScoreRecord.version = 2;
        g_TitleScoreRecord.runtimeMarker = 0;
    }

    if ((globalManager.flags & 8u) != 0 && supervisor->value598 != 0)
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
        operator delete(gameManager->heapE8);
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
    GameManagerSetupLayout &globalManager =
        *reinterpret_cast<GameManagerSetupLayout *>(&g_GameManager);

    SetupStateBuffer *state =
        reinterpret_cast<SetupStateBuffer *>(globalManager.sides[0].state);

    if (state->value00 < 7.0f)
    {
        state->value00 += 1.0f;
        g_SoundPlayer.PlaySoundByIdx(0x1C, 0);
        state = reinterpret_cast<SetupStateBuffer *>(globalManager.sides[0].state);
    }

    state->phase10++;

    SetupStateBuffer *thresholdState =
        reinterpret_cast<SetupStateBuffer *>(globalManager.sides[0].state);
    if (globalManager.gameMode == 1)
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
    GameManagerSetupLayout &globalManager =
        *reinterpret_cast<GameManagerSetupLayout *>(&g_GameManager);

    ResetGameManager(&globalManager);
    g_AsciiManager.Reset();
    static_cast<FrontMessageOwnerView *>(globalManager.gameSubsystem)->InitializeMessageRuntime();
    ResetExAttackRecords(static_cast<ExAttackController *>(globalManager.secondarySubsystem));

    for (int sideIndex = 0; sideIndex < 2; sideIndex++)
    {
        SetupSideState *side = &globalManager.sides[sideIndex];
        side->flags &= ~1u;
        static_cast<Background *>(side->subsystem0)->ClearSpellBackgroundState();
        static_cast<PlayerLifecycleView *>(side->subsystem1)->InitializeAddedState();
        static_cast<EtamaController *>(side->subsystem2)->ResetProjectiles();
        static_cast<EffectManager *>(side->subsystem3)->ResetPool();
        static_cast<EnemyManagerView *>(side->subsystem4)->ResetForGameplayCleanup();
        static_cast<CardAttack *>(side->subsystem5)->RefreshResource();
        static_cast<FrontSide *>(side->subsystem6)->ResetTransitionState();
    }

    static_cast<EffectManager *>(globalManager.sharedSubsystem)->ResetPool();

    if (globalManager.gameMode == 2)
    {
        globalManager.limitB4 -= 8;
        if (globalManager.limitB4 < globalManager.value100)
            globalManager.limitB4 = globalManager.value100;
    }
    else
    {
        int stageValue = globalManager.value100;

        switch (globalManager.setupMode)
        {
        case 0:
            globalManager.limitB4 = globalManager.counterFC / 3 + 1;
            globalManager.valueC8 = globalManager.counterFC / 2 + 6;
            globalManager.valueC4 = 600 * (stageValue + 3);
            break;
        case 1:
            globalManager.limitB4 = globalManager.counterFC + 1;
            globalManager.valueC8 = globalManager.counterFC + 10;
            globalManager.valueC4 = 600 * (stageValue + 2);
            break;
        case 2:
            globalManager.limitB4 = globalManager.counterFC + 3;
            globalManager.valueC8 = globalManager.counterFC / 2 + 15;
            globalManager.valueC4 = 600 * stageValue + 900;
            break;
        case 3:
            globalManager.limitB4 = globalManager.counterFC + 6;
            globalManager.valueC8 = globalManager.counterFC / 2 + 17;
            globalManager.valueC4 = 600 * stageValue + 900;
            break;
        case 4:
            globalManager.limitB4 = 18;
            globalManager.valueC8 = 22;
            globalManager.valueC4 = 600 * (stageValue + 1);
            break;
        default:
            break;
        }

        // The pinned build coalesces the per-case limits into this final
        // publication, followed by the conditional minimum publication.
        globalManager.limitB4 -= 2 * stageValue;
        if (globalManager.limitB4 < 1)
            globalManager.limitB4 = 1;
    }

    globalManager.valueB0 = 0;
    globalManager.value348 = 0;
    globalManager.valueCC -= 1000;
    if (globalManager.valueCC < 0)
        globalManager.valueCC = 0;
}

void __fastcall GameplaySetupThread(void *unused)
{
    GameManagerSetupLayout &globalManager =
        *reinterpret_cast<GameManagerSetupLayout *>(&g_GameManager);

    GameManagerSetupLayout *manager = &globalManager;
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
    manager->instructionMask120 = 0xff;
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
        operator delete(g_GameConfiguration);
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
                operator delete(manager->sides[sideIndex].state);
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

        reinterpret_cast<ReplayManagerView *>(manager->stageObject)
            ->Release();
        manager->stageObject = NULL;

        if ((manager->flags & 8) == 0)
        {
            manager->seedCopy346 = g_Rng.seed;
            reinterpret_cast<GameManagerOpponentSelectionView *>(&globalManager)
                ->SelectOpponentConfiguration();
            manager->stageObject = ReplayManagerView::Create(0, NULL);
        }
        else
        {
            manager->stageObject = ReplayManagerView::Create(
                1, reinterpret_cast<const char *>(&manager->modeByte145));
            reinterpret_cast<GameManagerOpponentSelectionView *>(&globalManager)
                ->SelectOpponentConfiguration();
        }
    }
    else
    {
        if (manager->gameMode == 0)
        {
            SetupStateBuffer *state =
                reinterpret_cast<SetupStateBuffer *>(manager->sides[0].state);
            if (state->value04 >= 2000000u * state->phase10 + 1000000u)
                globalManager.AdvanceTimedState();
        }
        if (manager->gameMode == 1)
        {
            SetupStateBuffer *state =
                reinterpret_cast<SetupStateBuffer *>(manager->sides[0].state);
            if (state->value04 >= 500000u * (state->phase10 + 1))
                globalManager.AdvanceTimedState();
        }

        if ((manager->flags & 8) == 0)
        {
            manager->seedCopy346 = g_Rng.seed;
            reinterpret_cast<GameManagerOpponentSelectionView *>(&globalManager)
                ->SelectOpponentConfiguration();
            DispatchReplayStage(
                static_cast<ReplayStageDispatchView *>(manager->stageObject), 0);
        }
        else
        {
            DispatchReplayStage(
                static_cast<ReplayStageDispatchView *>(manager->stageObject), 1);
            reinterpret_cast<GameManagerOpponentSelectionView *>(&globalManager)
                ->SelectOpponentConfiguration();
        }

        if (manager->gameMode == 0 || manager->gameMode == 1)
            memset(manager->sides[1].state, 0, 0xA0);
    }

    g_Supervisor.InitializeViewports();
    manager->valueB0 = 0;
    manager->gameSubsystem = CreateGameSubsystem();
    if (manager->gameSubsystem == NULL)
        goto setup_failed;

    PreloadPlayerAnmResources();

    for (sideIndex = 0; sideIndex < 2; sideIndex++)
    {
        SetupSideState *side = &manager->sides[sideIndex];

        if (side->selector != 0)
            g_GameConfiguration->sideFocusMode[sideIndex] = 0;
        side->flags &= ~1u;

        side->subsystem0 = Background::Create(sideIndex);
        if (side->subsystem0 == NULL)
            goto setup_failed;
        side->subsystem1 = PlayerRegisterChain(0, sideIndex, side->selector);
        if (side->subsystem1 == NULL)
            goto setup_failed;
        side->subsystem2 = EtamaController::Register(sideIndex);
        if (side->subsystem2 == NULL)
            goto setup_failed;
        side->subsystem3 = EffectManager::Create(sideIndex, 0x100, 7);
        if (side->subsystem3 == NULL)
            goto setup_failed;
        side->subsystem4 = EnemyManagerView::Create(sideIndex);
        if (side->subsystem4 == NULL)
            goto setup_failed;
        side->subsystem5 = CardAttack::Create(sideIndex);
        if (side->subsystem5 == NULL)
            goto setup_failed;
        side->subsystem6 = FrontSide::Create(sideIndex);
        if (side->subsystem6 == NULL)
            goto setup_failed;
    }

    manager->sharedSubsystem = EffectManager::Create(2, 0x320, 1);
    if (manager->sharedSubsystem == NULL)
        goto setup_failed;

    manager->secondarySubsystem = ExAttackController::Register();
    if (manager->secondarySubsystem == NULL)
        goto setup_failed;

    switch (manager->setupMode)
    {
    case 4:
        manager->limitB4 = 18;
        static_cast<PlayerLifecycleView *>(manager->sides[0].subsystem1)->header24.state7C = 10;
        static_cast<PlayerLifecycleView *>(manager->sides[0].subsystem1)->header24.state80 = 10;
        static_cast<PlayerLifecycleView *>(manager->sides[1].subsystem1)->header24.state7C = 10;
        static_cast<PlayerLifecycleView *>(manager->sides[1].subsystem1)->header24.state80 = 10;
        manager->valueC4 = 600;
        manager->valueC8 = 22;
        break;

    case 3:
        manager->limitB4 = 8;
        static_cast<PlayerLifecycleView *>(manager->sides[0].subsystem1)->header24.state7C = 8;
        static_cast<PlayerLifecycleView *>(manager->sides[0].subsystem1)->header24.state80 = 8;
        static_cast<PlayerLifecycleView *>(manager->sides[1].subsystem1)->header24.state7C = 8;
        static_cast<PlayerLifecycleView *>(manager->sides[1].subsystem1)->header24.state80 = 8;
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
        static_cast<PlayerLifecycleView *>(manager->sides[0].subsystem1)->header24.state7C = 4;
        static_cast<PlayerLifecycleView *>(manager->sides[0].subsystem1)->header24.state80 = 4;
        static_cast<PlayerLifecycleView *>(manager->sides[1].subsystem1)->header24.state7C = 4;
        static_cast<PlayerLifecycleView *>(manager->sides[1].subsystem1)->header24.state80 = 4;
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
        static_cast<PlayerLifecycleView *>(manager->sides[0].subsystem1)->header24.state7C = 1;
        static_cast<PlayerLifecycleView *>(manager->sides[0].subsystem1)->header24.state80 = 1;
        static_cast<PlayerLifecycleView *>(manager->sides[1].subsystem1)->header24.state7C = 1;
        static_cast<PlayerLifecycleView *>(manager->sides[1].subsystem1)->header24.state80 = 1;
        if (manager->gameMode == 2)
        {
            manager->valueC4 = 900;
            manager->valueC8 = 20;
        }
        else
        {
            manager->valueC4 = 1200;
            manager->limitB4 = manager->counterFC + 1;
            manager->valueC8 = manager->counterFC + 10;
        }
        break;

    case 0:
        manager->limitB4 = 1;
        static_cast<PlayerLifecycleView *>(manager->sides[0].subsystem1)->header24.state7C = 1;
        static_cast<PlayerLifecycleView *>(manager->sides[0].subsystem1)->header24.state80 = 1;
        static_cast<PlayerLifecycleView *>(manager->sides[1].subsystem1)->header24.state7C = 1;
        static_cast<PlayerLifecycleView *>(manager->sides[1].subsystem1)->header24.state80 = 1;
        manager->valueC4 = 1800;
        // For every gameMode other than 2, keep the counter-derived limits
        // and leave this switch immediately. The fixed mode-2 rate follows
        // below; both paths retain the same actual field writes.
        if (manager->gameMode != 2)
        {
            manager->limitB4 = manager->counterFC / 3 + 1;
            manager->valueC8 = manager->counterFC / 2 + 6;
            break;
        }
        manager->valueC8 = 16;
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
};

typedef char GameManagerResetRecordSize38[
    (sizeof(GameManagerResetRecord) == 0x38) ? 1 : -1];
typedef char GameManagerResetValueB0AtB0[
    (offsetof(GameManagerResetView, valueB0) == 0xB0) ? 1 : -1];
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
    reinterpret_cast<GameManagerEclStateView *>(manager)->value380 = 0;
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
    GameManagerSetupLayout &globalManager =
        *reinterpret_cast<GameManagerSetupLayout *>(&g_GameManager);

    if (globalManager.sides[0].state->value08 >= 1000000000u)
        globalManager.sides[0].state->value08 = 999999999u;
    globalManager.sides[0].state->value04 =
        globalManager.sides[0].state->value08;

    if (globalManager.sides[1].state->value08 >= 1000000000u)
        globalManager.sides[1].state->value08 = 999999999u;
    globalManager.sides[1].state->value04 =
        globalManager.sides[1].state->value08;

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
