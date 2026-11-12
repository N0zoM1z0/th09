#include "EffectManager.hpp"
#include "Background.hpp"
#include "CardAttack.hpp"
#include "FrontSide.hpp"
#include "TitleScoreRecordView.hpp"
#include "FrontFactoryConstructView.hpp"
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
#include "GameManagerRuntimeView.hpp"
#include "AnmManager.hpp"
#include "Chain.hpp"
#include "GameConfiguration.hpp"
#include "ScreenEffect.hpp"
#include "ReplayInputState.hpp"
#include "ReplayManagerRuntimeView.hpp"
#include "Supervisor.hpp"
#include "SupervisorThreadStartView.hpp"
#include "SupervisorSetupLayoutView.hpp"
#include "PlayerSharedRuntimeView.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <new>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void __fastcall ResetExAttackRecords(ExAttackController *controller);


// The 0xA0 score/phase buffer is the same concrete side-runtime object
// already carried by the 0x38-byte native GameManager side owner.

typedef char SetupStateBufferSizeIsA0[
    (sizeof(SetupStateBuffer) == 0xA0) ? 1 : -1];
typedef char SetupPlayerCounterA0[
    (offsetof(PlayerLifecycleView, header24) +
     offsetof(PlayerHeaderStateView, state7C) == 0xA0) ? 1 : -1];
typedef char SetupPlayerCounterA4[
    (offsetof(PlayerLifecycleView, header24) +
     offsetof(PlayerHeaderStateView, state80) == 0xA4) ? 1 : -1];

typedef char SetupLoadingPositionSizeCheck[
    (sizeof(SetupLoadingPosition) == 0x0C) ? 1 : -1];

// The gameplay spawn thread receives a real EnemyView from the original
// EnemyManager allocation. Its +0x335C and +0x3380 reward fields already
// belong to that native 0x5430-byte record, not a separate spawn overlay.
typedef char SetupNativeSpawnedEnemyFields[
    (offsetof(EnemyView, itemDropType335C) == 0x335C &&
     offsetof(EnemyView, secondaryFlags3380) == 0x3380) ? 1 : -1];




typedef char SetupScoreRecordViewSizeCheck[
    (sizeof(TitleScoreRecordView) == 0x2C) ? 1 : -1];

#include "GameManagerSetupView.hpp"
#include "GameplaySetupNativeSides.hpp"

// The same original-bound SupervisorSetupLayout is declared in
// SupervisorSetupLayoutView.hpp and shared with the Title loading callback.
typedef char SupervisorSetupLayoutLoadingAnmAt5D0[
    (offsetof(SupervisorSetupLayout, loadingAnm5D0) == 0x5D0) ? 1 : -1];
typedef char SupervisorSetupLayoutLoadingFlagAt740[
    (offsetof(SupervisorSetupLayout, loadingVmsHaveBeenSetup) == 0x740) ? 1 : -1];

typedef char SetupGameManagerInstructionMaskAt120[
    (offsetof(GameManagerSetupLayout, instructionMask120) ==
     offsetof(AsciiGameManagerView, instructionMask120)) ? 1 : -1];
// OnUpdate's replay mode, selectors and pause flag are the same original
// manager words read by the independent replay recording/playback owners.
typedef char SetupGameManagerNativeReplayOffsets[
    (offsetof(GameManagerSetupLayout, gameMode) ==
         offsetof(AsciiGameManagerView, gameMode) &&
     offsetof(GameManagerSetupLayout, replayPauseRecorded) ==
         offsetof(AsciiGameManagerView, replayPauseRecorded130) &&
     offsetof(SetupSideState, selector) ==
         offsetof(AsciiGameManagerSideView, replaySelector28)) ? 1 : -1];

extern Chain g_Chain;
extern ChainElem g_GameManagerCalcChain;
extern ChainElem g_GameManagerDrawChain;
extern char g_ReplayPlayTimeText[];
extern int g_SetupStatus;
extern RngRuntimeView g_Rng;

extern AsciiManager g_AsciiManager;
extern AnmVm g_SupervisorLoadingVms[3];

extern unsigned char g_DeletedScratch[0x2BC0];
extern int g_GameManagerChainState;

extern int g_LastSpawnLane;
extern void __fastcall ReleaseGameSubsystem(void *object);
extern void __fastcall ReleaseSubsystem0(void *object);
extern void __fastcall ReleaseSubsystem4(void *object);

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
// The actual three 0x38-byte GameManager side records are shared by all
// setup/update/cleanup sources. Every signed side index forms a native
// unsigned i386 DWORD-scaled address, not a new bounded alias allocation.
#define TH09_SETUP_LOCAL_SIDE(index) \
    (*reinterpret_cast<SetupSideState *>( \
        reinterpret_cast<unsigned int>(&globalManager.sides[0]) + \
        static_cast<unsigned int>(index) * sizeof(globalManager.sides[0])))
#define TH09_SETUP_GLOBAL_SIDE(index) \
    (*reinterpret_cast<AsciiGameManagerSideView *>( \
        reinterpret_cast<unsigned int>(&g_GameManager.sides[0]) + \
        static_cast<unsigned int>(index) * sizeof(g_GameManager.sides[0])))
// The reset owner traverses all three physical +0x38 side records,
// including the shared EffectManager slot. Its receiver is the passed
// GameManager pointer, not the global side-base used in setup.
#define TH09_SETUP_PARAMETER_SIDE(manager, index) \
    (*reinterpret_cast<SetupSideState *>( \
        reinterpret_cast<unsigned int>(&(manager)->sides[0]) + \
        static_cast<unsigned int>(index) * sizeof((manager)->sides[0])))


int GameManagerSetupLayout::OnUpdate(GameManagerSetupLayout *gameManager)
{
    GameManagerSetupLayout &globalManager =
        *reinterpret_cast<GameManagerSetupLayout *>(&g_GameManager);

    SupervisorSetupLayout *supervisor =
        reinterpret_cast<SupervisorSetupLayout *>(&g_Supervisor);

    // Retail advances these persistent signed frame and reward words with
    // native modulo-32 i386 instructions, not C++ signed-overflow behavior.
    ++*reinterpret_cast<unsigned int *>(&g_GameManager.playtimeFrames12C);

    if (g_GameManager.gameMode != 2 ||
        g_GameManager.sides[0].replaySelector28 != 0 ||
        g_GameManager.sides[1].replaySelector28 != 0)
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

    if (g_GameManager.gameplaySetupState124 != 0)
    {
        if (g_GameManager.gameplaySetupState124 == 2)
            return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
        ++*reinterpret_cast<unsigned int *>(&gameManager->gameplaySetupWaitFrames);
        return CHAIN_CALLBACK_RESULT_BREAK;
    }

    if (globalManager.stageStartupMode != 0)
    {
        g_AnmManager->ReleaseSurface(8);
        supervisor->loadingVmsHaveBeenSetup = 0;
        if (globalManager.stageStartupMode == 1)
            supervisor->PlayMusic(0, 0);
        globalManager.stageStartupMode = 0;
    }

    if (gameManager->menuBlock13D == 0 &&
        gameManager->menuBlock13E == 0 &&
        gameManager->inGameMenu == 0 &&
        (gameManager->flags & 2u) == 0 &&
        (g_ReplayInputStates[2].word06 & 8u) != 0)
    {
        gameManager->inGameMenu = 1;
        reinterpret_cast<AsciiGameManagerView *>(gameManager)->replayPauseRecorded130 = 1;
        g_SoundPlayer.QueueCommand(6, 0, "Pause");
        g_SoundPlayer.PlaySoundByIdx(0x22, 0);
        supervisor->UpdateGameTime();
    }

    if ((gameManager->flags & 2u) != 0)
    {
        if (g_ReplayInputStates[2].word06 != 0)
            supervisor->state590 = 1;

        ++*reinterpret_cast<unsigned int *>(&gameManager->demoFrameCount);
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

    if (!globalManager.gameSubsystem->IsBlocked())
    {
        ++*reinterpret_cast<unsigned int *>(&g_GameManager.valueD0);

        if (++g_GameManager.valueB0 % g_GameManager.valueC4 ==
            g_GameManager.valueC4 - 1 &&
            static_cast<int>(
                ++*reinterpret_cast<unsigned int *>(&g_GameManager.limitB4)) >
                    g_GameManager.valueC8)
        {
            g_GameManager.limitB4 = g_GameManager.valueC8;
        }

        g_GameManager.valueC0++;
        // These difficulty counters belong to each side's subsystem1 object;
        // the separate state buffer is allocated as 0xA0 bytes.
        // These four genuine Player side counters are signed 32-bit words.
        // Match the original i386 INC DWORD stores while keeping the
        // original per-side below-16 bounds tests and read chronology.
        if (g_GameManager.valueB0 % 3600u == 3599u)
        {
            if (globalManager.sides[0].player04->header24.state7C < 16)
                ++*reinterpret_cast<unsigned int *>(
                    &globalManager.sides[0].player04->header24.state7C);
            if (globalManager.sides[0].player04->header24.state80 < 16)
                ++*reinterpret_cast<unsigned int *>(
                    &globalManager.sides[0].player04->header24.state80);
            if (globalManager.sides[1].player04->header24.state7C < 16)
                ++*reinterpret_cast<unsigned int *>(
                    &globalManager.sides[1].player04->header24.state7C);
            if (globalManager.sides[1].player04->header24.state80 < 16)
                ++*reinterpret_cast<unsigned int *>(
                    &globalManager.sides[1].player04->header24.state80);
        }

        if (g_GameManager.valueB8 < 6 &&
            g_GameManager.valueC0 % 720u == 719u)
        {
            ++*reinterpret_cast<unsigned int *>(&g_GameManager.valueB8);
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
            g_GameManager.valueCC = static_cast<int>(
                static_cast<unsigned int>(g_GameManager.valueCC) +
                static_cast<unsigned int>(increase));
        }

        if (g_GameManager.valueCC > 10000)
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

            g_GameManager.valueCC -= 10000;
            CardAttackSpawnPosition position;
            position.x = 0.0f;
            position.z = 0.0f;
            g_LastSpawnLane = lane;
            position.y = -32.0f;

            EnemyView *object =
                static_cast<EnemyView *>(
                    reinterpret_cast<EnemyManagerCardAttackSpawnView *>(globalManager.sides[0].subsystem4)
                        ->SpawnEnemyRecord(19, &position, 830, -2, 10000, 0, 0));
            object->itemDropType335C = lane;
            object->secondaryFlags3380 |= 0x2000u;

            object =
                static_cast<EnemyView *>(
                    reinterpret_cast<EnemyManagerCardAttackSpawnView *>(globalManager.sides[1].subsystem4)
                        ->SpawnEnemyRecord(19, &position, 830, -2, 10000, 0, 0));
            object->secondaryFlags3380 |= 0x2000u;
            object->itemDropType335C = lane;

            g_SoundPlayer.PlaySoundByIdx(0x33, 0);
            globalManager.sides[0].frontSide18->ClearState(1);
            globalManager.sides[1].frontSide18->ClearState(1);
        }
    }

    for (int sideIndex = 0; sideIndex < GameplaySetupNativeSides::PlayableCount; sideIndex++)
    {
        if (TH09_SETUP_LOCAL_SIDE(sideIndex).state->value08 >= 1000000000u)
            TH09_SETUP_LOCAL_SIDE(sideIndex).state->value08 = 999999999u;

        if (TH09_SETUP_LOCAL_SIDE(sideIndex).state->value04 !=
            TH09_SETUP_LOCAL_SIDE(sideIndex).state->value08)
        {
            if (TH09_SETUP_LOCAL_SIDE(sideIndex).state->value08 <
                TH09_SETUP_LOCAL_SIDE(sideIndex).state->value04)
            {
                TH09_SETUP_LOCAL_SIDE(sideIndex).state->value08 =
                    TH09_SETUP_LOCAL_SIDE(sideIndex).state->value04;
            }

            unsigned int step =
                (TH09_SETUP_LOCAL_SIDE(sideIndex).state->value08 -
                 TH09_SETUP_LOCAL_SIDE(sideIndex).state->value04) /
                15u;
            if (step >= 578910u)
                step = 578910u;
            else if (step == 0)
                step = 1;

            if (TH09_SETUP_LOCAL_SIDE(sideIndex).state->value0C < step)
                TH09_SETUP_LOCAL_SIDE(sideIndex).state->value0C = step;
            if (TH09_SETUP_LOCAL_SIDE(sideIndex).state->value04 +
                    TH09_SETUP_LOCAL_SIDE(sideIndex).state->value0C >
                TH09_SETUP_LOCAL_SIDE(sideIndex).state->value08)
            {
                TH09_SETUP_LOCAL_SIDE(sideIndex).state->value0C =
                    TH09_SETUP_LOCAL_SIDE(sideIndex).state->value08 -
                    TH09_SETUP_LOCAL_SIDE(sideIndex).state->value04;
            }
            TH09_SETUP_LOCAL_SIDE(sideIndex).state->value04 +=
                TH09_SETUP_LOCAL_SIDE(sideIndex).state->value0C;
            if (TH09_SETUP_LOCAL_SIDE(sideIndex).state->value04 >=
                TH09_SETUP_LOCAL_SIDE(sideIndex).state->value08)
            {
                TH09_SETUP_LOCAL_SIDE(sideIndex).state->value0C = 0;
                TH09_SETUP_LOCAL_SIDE(sideIndex).state->value04 =
                    TH09_SETUP_LOCAL_SIDE(sideIndex).state->value08;
            }
        }
    }

    if (g_GameManager.gameMode == 0)
    {
        if (globalManager.sides[0].state->value04 >=
            2000000u * globalManager.sides[0].state->phase10 + 1000000u)
        {
            globalManager.AdvanceTimedState();
        }
    }
    if (g_GameManager.gameMode == 1 &&
        globalManager.sides[0].state->value04 >=
            500000u * (static_cast<unsigned int>(
            globalManager.sides[0].state->phase10) + 1u))
    {
        globalManager.AdvanceTimedState();
    }

    ++*reinterpret_cast<unsigned int *>(&gameManager->value348);
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
        globalManager.sides[0].state;
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
    int sideCount = GameplaySetupNativeSides::PlayableCount;
    do
    {
        ReleaseSubsystem0(side->background00);
        side->background00 = NULL;
        PlayerRelease(static_cast<PlayerLifecycleView *>(side->player04));
        side->player04 = NULL;
        EtamaController::Release(static_cast<EtamaController *>(side->etama08));
        side->etama08 = NULL;
        EffectManager::Release(static_cast<EffectManager *>(side->effectManager0C));
        side->effectManager0C = NULL;
        ReleaseSubsystem4(side->enemyManager10);
        side->enemyManager10 = NULL;
        CardAttack::Destroy(static_cast<CardAttack *>(side->cardAttack14));
        side->cardAttack14 = NULL;
        FrontSide::Destroy(static_cast<FrontSide *>(side->frontSide18));
        side->frontSide18 = NULL;
        side++;
    } while (--sideCount != 0);

    ReleaseGameSubsystem(gameManager->gameSubsystem);
    gameManager->gameSubsystem = NULL;
    ExAttackController::Release(static_cast<ExAttackController *>(gameManager->secondarySubsystem));
    gameManager->secondarySubsystem = NULL;
    EffectManager::Release(gameManager->sides[GameplaySetupNativeSides::SharedEffectIndex].effectManager0C);
    gameManager->sides[GameplaySetupNativeSides::SharedEffectIndex].effectManager0C = NULL;

    if (gameManager->configurationE8 != NULL)
    {
        operator delete(gameManager->configurationE8);
        gameManager->configurationE8 = NULL;
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
        globalManager.sides[0].state;

    if (state->value00 < 7.0f)
    {
        state->value00 += 1.0f;
        g_SoundPlayer.PlaySoundByIdx(0x1C, 0);
        state = globalManager.sides[0].state;
    }

    state->phase10 = static_cast<int>(static_cast<unsigned int>(state->phase10) + 1u);

    SetupStateBuffer *thresholdState =
        globalManager.sides[0].state;
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
    globalManager.gameSubsystem->InitializeMessageRuntime();
    ResetExAttackRecords(static_cast<ExAttackController *>(globalManager.secondarySubsystem));

    for (int sideIndex = 0; sideIndex < GameplaySetupNativeSides::PlayableCount; sideIndex++)
    {
        SetupSideState *side = &TH09_SETUP_LOCAL_SIDE(sideIndex);
        side->flags &= ~1u;
        static_cast<Background *>(side->background00)->ClearSpellBackgroundState();
        static_cast<PlayerLifecycleView *>(side->player04)->InitializeAddedState();
        static_cast<EtamaController *>(side->etama08)->ResetProjectiles();
        static_cast<EffectManager *>(side->effectManager0C)->ResetPool();
        static_cast<EnemyManagerView *>(side->enemyManager10)->ResetForGameplayCleanup();
        static_cast<CardAttack *>(side->cardAttack14)->RefreshResource();
        static_cast<FrontSide *>(side->frontSide18)->ResetTransitionState();
    }

    globalManager.sides[GameplaySetupNativeSides::SharedEffectIndex].effectManager0C->ResetPool();

    if (globalManager.gameMode == 2)
    {
        globalManager.limitB4 = static_cast<int>(
            static_cast<unsigned int>(globalManager.limitB4) - 8u);
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
            globalManager.valueC4 = static_cast<int>(
                600u * (static_cast<unsigned int>(stageValue) + 3u));
            break;
        case 1:
            globalManager.limitB4 = static_cast<int>(
                static_cast<unsigned int>(globalManager.counterFC) + 1u);
            globalManager.valueC8 = static_cast<int>(
                static_cast<unsigned int>(globalManager.counterFC) + 10u);
            globalManager.valueC4 = static_cast<int>(
                600u * (static_cast<unsigned int>(stageValue) + 2u));
            break;
        case 2:
            globalManager.limitB4 = static_cast<int>(
                static_cast<unsigned int>(globalManager.counterFC) + 3u);
            globalManager.valueC8 = globalManager.counterFC / 2 + 15;
            globalManager.valueC4 = static_cast<int>(
                600u * static_cast<unsigned int>(stageValue) + 900u);
            break;
        case 3:
            globalManager.limitB4 = static_cast<int>(
                static_cast<unsigned int>(globalManager.counterFC) + 6u);
            globalManager.valueC8 = globalManager.counterFC / 2 + 17;
            globalManager.valueC4 = static_cast<int>(
                600u * static_cast<unsigned int>(stageValue) + 900u);
            break;
        case 4:
            globalManager.limitB4 = 18;
            globalManager.valueC8 = 22;
            globalManager.valueC4 = static_cast<int>(
                600u * (static_cast<unsigned int>(stageValue) + 1u));
            break;
        default:
            break;
        }

        // The retail difficulty and stage limits are computed as native
        // modulo-32 DWORD multiplications and subtractions. Keep the
        // final signed threshold comparisons after restoring their word bits.
        // The pinned build coalesces the per-case limits into this final
        // publication, followed by the conditional minimum publication.
        globalManager.limitB4 = static_cast<int>(
            static_cast<unsigned int>(globalManager.limitB4) -
            2u * static_cast<unsigned int>(stageValue));
        if (globalManager.limitB4 < 1)
            globalManager.limitB4 = 1;
    }

    globalManager.valueB0 = 0;
    globalManager.value348 = 0;
    globalManager.valueCC = static_cast<int>(
        static_cast<unsigned int>(globalManager.valueCC) - 1000u);
    if (globalManager.valueCC < 0)
        globalManager.valueCC = 0;
}

void __fastcall GameplaySetupThread(void *unused)
{
    GameManagerSetupLayout &globalManager =
        *reinterpret_cast<GameManagerSetupLayout *>(&g_GameManager);

    SupervisorSetupLayout *supervisor = reinterpret_cast<SupervisorSetupLayout *>(&g_Supervisor);
    unsigned int previousMode;
    int sideIndex;

    (void)unused;

    ResetGameManager(&globalManager);
    FormatCurrentDateString(g_ReplayPlayTimeText);

    globalManager.flags |= 4;
    globalManager.flags &= ~0x1800u;
    globalManager.value374 = -1;
    previousMode = globalManager.gameMode;
    globalManager.valueD0 = 0;
    globalManager.value348 = 0;
    globalManager.stageStartupMode = 1;
    globalManager.instructionMask120 = 0xff;
    globalManager.valueB8 = 2;
    globalManager.valueCC = 0;
    globalManager.valueC0 = 0;
    globalManager.demoFrameCount = 0;
    globalManager.value100 = 0;
    globalManager.value104 = (previousMode == 2) + 1;
    globalManager.flags &= ~0x4000u;
    globalManager.value108 = 0;
    globalManager.value10C = 0;

    if ((globalManager.flags & 8) == 0)
    {
        if (supervisor->flags & 0x200)
        {
            memset(globalManager.clearD8, 0, sizeof(globalManager.clearD8));
            goto mode_ready;
        }
    }
    else
    {
        memset(globalManager.clearD8, 0, sizeof(globalManager.clearD8));
        if (supervisor->flags & 0x200)
            goto mode_ready;
        previousMode = globalManager.gameMode;
    }

    if (previousMode == 0 || previousMode == 1)
        globalManager.counterFC = static_cast<int>(static_cast<unsigned int>(globalManager.counterFC) + 1u);

mode_ready:
    if (globalManager.configurationE8 != NULL)
    {
        operator delete(globalManager.configurationE8);
        globalManager.configurationE8 = NULL;
    }

    GameConfiguration *allocation =
        static_cast<GameConfiguration *>(operator new(sizeof(GameConfiguration)));
    GameConfiguration *newConfiguration = allocation;
    if (allocation != NULL)
        allocation->Initialize();
    else
        newConfiguration = NULL;
    globalManager.configurationE8 = newConfiguration;
    *globalManager.configurationE8 = supervisor->config388;
    globalManager.speedEC = 1.0f;

    if (supervisor->flags & 0x200)
    {
        g_GameManager.flags &= ~0x2000u;

        for (sideIndex = 0; sideIndex < GameplaySetupNativeSides::PlayableCount; sideIndex++)
        {
            if (TH09_SETUP_GLOBAL_SIDE(sideIndex).state != NULL)
            {
                operator delete(TH09_SETUP_GLOBAL_SIDE(sideIndex).state);
                TH09_SETUP_GLOBAL_SIDE(sideIndex).state = NULL;
            }
            TH09_SETUP_GLOBAL_SIDE(sideIndex).state =
                static_cast<SetupStateBuffer *>(
                    operator new(sizeof(SetupStateBuffer)));
            memset(TH09_SETUP_GLOBAL_SIDE(sideIndex).state, 0, sizeof(SetupStateBuffer));
        }

        if (g_GameManager.gameMode == 0)
            g_GameManager.sides[0].state->value00 =
                static_cast<float>(g_GameManager.configurationE8->valueAC) + 2.0f;
        else
            g_GameManager.sides[0].state->value00 = 0.0f;

        g_GameManager.replayManagerD4->Release();
        g_GameManager.stageObjectD4 = NULL;

        if ((g_GameManager.flags & 8) == 0)
        {
            g_GameManager.seedCopy346 = g_Rng.seed;
            g_GameManager.SelectOpponentConfiguration();
            g_GameManager.replayManagerD4 = ReplayManagerView::Create(0, NULL);
        }
        else
        {
            g_GameManager.replayManagerD4 = ReplayManagerView::Create(
                1, reinterpret_cast<const char *>(&g_GameManager.modeByte145));
            g_GameManager.SelectOpponentConfiguration();
        }
    }
    else
    {
        if (g_GameManager.gameMode == 0)
        {
            SetupStateBuffer *state =
                g_GameManager.sides[0].state;
            if (state->value04 >= 2000000u * state->phase10 + 1000000u)
                globalManager.AdvanceTimedState();
        }
        if (g_GameManager.gameMode == 1)
        {
            SetupStateBuffer *state =
                g_GameManager.sides[0].state;
            if (state->value04 >= 500000u * (static_cast<unsigned int>(state->phase10) + 1u))
                globalManager.AdvanceTimedState();
        }

        if ((g_GameManager.flags & 8) == 0)
        {
            g_GameManager.seedCopy346 = g_Rng.seed;
            g_GameManager.SelectOpponentConfiguration();
            DispatchReplayStage(
                g_GameManager.replayDispatchD4, 0);
        }
        else
        {
            DispatchReplayStage(
                g_GameManager.replayDispatchD4, 1);
            g_GameManager.SelectOpponentConfiguration();
        }

        if (g_GameManager.gameMode == 0 || g_GameManager.gameMode == 1)
            memset(g_GameManager.sides[1].state, 0, sizeof(SetupStateBuffer));
    }

    g_Supervisor.InitializeViewports();
    g_GameManager.valueB0 = 0;
    globalManager.gameSubsystem =
        static_cast<FrontFactoryConstructView *>(CreateGameSubsystem());
    if (globalManager.gameSubsystem == NULL)
        goto setup_failed;

    PreloadPlayerAnmResources();

    for (sideIndex = 0; sideIndex < GameplaySetupNativeSides::PlayableCount; sideIndex++)
    {
        if (TH09_SETUP_GLOBAL_SIDE(sideIndex).selector != 0)
            *reinterpret_cast<unsigned char *>(
                reinterpret_cast<unsigned int>(
                    &g_GameManager.configurationE8->sideFocusMode[0]) +
                static_cast<unsigned int>(sideIndex)) = 0;
        TH09_SETUP_GLOBAL_SIDE(sideIndex).flags &= ~1u;

        TH09_SETUP_GLOBAL_SIDE(sideIndex).background00 = Background::Create(sideIndex);
        if (TH09_SETUP_GLOBAL_SIDE(sideIndex).background00 == NULL)
            goto setup_failed;
        TH09_SETUP_GLOBAL_SIDE(sideIndex).player04 = PlayerRegisterChain(0, sideIndex, TH09_SETUP_GLOBAL_SIDE(sideIndex).selector);
        if (TH09_SETUP_GLOBAL_SIDE(sideIndex).player04 == NULL)
            goto setup_failed;
        TH09_SETUP_GLOBAL_SIDE(sideIndex).etama08 = EtamaController::Register(sideIndex);
        if (TH09_SETUP_GLOBAL_SIDE(sideIndex).etama08 == NULL)
            goto setup_failed;
        TH09_SETUP_GLOBAL_SIDE(sideIndex).effectManager0C = EffectManager::Create(sideIndex, 0x100, 7);
        if (TH09_SETUP_GLOBAL_SIDE(sideIndex).effectManager0C == NULL)
            goto setup_failed;
        TH09_SETUP_GLOBAL_SIDE(sideIndex).enemyManager10 = EnemyManagerView::Create(sideIndex);
        if (TH09_SETUP_GLOBAL_SIDE(sideIndex).enemyManager10 == NULL)
            goto setup_failed;
        TH09_SETUP_GLOBAL_SIDE(sideIndex).cardAttack14 = CardAttack::Create(sideIndex);
        if (TH09_SETUP_GLOBAL_SIDE(sideIndex).cardAttack14 == NULL)
            goto setup_failed;
        TH09_SETUP_GLOBAL_SIDE(sideIndex).frontSide18 = FrontSide::Create(sideIndex);
        if (TH09_SETUP_GLOBAL_SIDE(sideIndex).frontSide18 == NULL)
            goto setup_failed;
    }

    globalManager.sides[GameplaySetupNativeSides::SharedEffectIndex].effectManager0C = EffectManager::Create(GameplaySetupNativeSides::SharedEffectIndex, 0x320, 1);
    if (globalManager.sides[GameplaySetupNativeSides::SharedEffectIndex].effectManager0C == NULL)
        goto setup_failed;

    globalManager.secondarySubsystem = ExAttackController::Register();
    if (globalManager.secondarySubsystem == NULL)
        goto setup_failed;

    switch (g_GameManager.setupMode11C)
    {
    case 4:
        g_GameManager.limitB4 = 18;
        g_GameManager.sides[0].player04->header24.state7C = 10;
        g_GameManager.sides[0].player04->header24.state80 = 10;
        g_GameManager.sides[1].player04->header24.state7C = 10;
        g_GameManager.sides[1].player04->header24.state80 = 10;
        g_GameManager.valueC4 = 600;
        g_GameManager.valueC8 = 22;
        break;

    case 1:
        g_GameManager.limitB4 = 1;
        g_GameManager.sides[0].player04->header24.state7C = 1;
        g_GameManager.sides[0].player04->header24.state80 = 1;
        g_GameManager.sides[1].player04->header24.state7C = 1;
        g_GameManager.sides[1].player04->header24.state80 = 1;
        if (g_GameManager.gameMode == 2)
        {
            g_GameManager.valueC4 = 900;
            g_GameManager.valueC8 = 20;
        }
        else
        {
            g_GameManager.valueC4 = 1200;
            g_GameManager.limitB4 = static_cast<int>(static_cast<unsigned int>(g_GameManager.counterFC) + 1u);
            g_GameManager.valueC8 = static_cast<int>(static_cast<unsigned int>(g_GameManager.counterFC) + 10u);
        }
        break;

    // The mode-1 fixed-rate arm is kept before the mode-3/2 cases in the
    // source switch. VC7.1 then schedules its shared publishing blocks as
    // in the original binary, including the mode-0 short back-jump.
    case 3:
        g_GameManager.limitB4 = 8;
        g_GameManager.sides[0].player04->header24.state7C = 8;
        g_GameManager.sides[0].player04->header24.state80 = 8;
        g_GameManager.sides[1].player04->header24.state7C = 8;
        g_GameManager.sides[1].player04->header24.state80 = 8;
        if (g_GameManager.gameMode == 2)
        {
            g_GameManager.valueC4 = 900;
            g_GameManager.valueC8 = 22;
            break;
        }
        g_GameManager.valueC4 = 720;
        g_GameManager.limitB4 = static_cast<int>(static_cast<unsigned int>(g_GameManager.counterFC) + 6u);
        g_GameManager.valueC8 = g_GameManager.counterFC / 2 + 17;
        break;

    case 2:
        g_GameManager.limitB4 = 6;
        g_GameManager.sides[0].player04->header24.state7C = 4;
        g_GameManager.sides[0].player04->header24.state80 = 4;
        g_GameManager.sides[1].player04->header24.state7C = 4;
        g_GameManager.sides[1].player04->header24.state80 = 4;
        if (g_GameManager.gameMode == 2)
        {
            g_GameManager.valueC4 = 720;
            g_GameManager.valueC8 = 22;
            break;
        }
        g_GameManager.valueC4 = 900;
        g_GameManager.limitB4 = static_cast<int>(static_cast<unsigned int>(g_GameManager.counterFC) + 3u);
        g_GameManager.valueC8 = g_GameManager.counterFC / 2 + 15;
        break;

    case 0:
        g_GameManager.limitB4 = 1;
        g_GameManager.sides[0].player04->header24.state7C = 1;
        g_GameManager.sides[0].player04->header24.state80 = 1;
        g_GameManager.sides[1].player04->header24.state7C = 1;
        g_GameManager.sides[1].player04->header24.state80 = 1;
        g_GameManager.valueC4 = 1800;
        // Mode 2 uses the fixed rate; other modes use round-derived limits.
        if (g_GameManager.gameMode != 2)
        {
            g_GameManager.limitB4 = g_GameManager.counterFC / 3 + 1;
            g_GameManager.valueC8 = g_GameManager.counterFC / 2 + 6;
        }
        else
        {
            g_GameManager.valueC8 = 16;
        }
        break;
    default:
        {}
    }

    SupervisorCalculateFps(0);
    FinishLoading(&g_Supervisor);
    while ((g_GameManager.flags & 0x60u) != 0)
        Sleep(17);

    g_GameManager.gameplaySetupState124 = 0;
    supervisor->runningSubthreadHandle = 0;
    supervisor->subthreadCloseRequestActive = 0;
    supervisor->subthreadActive = 0;
    supervisor->screenTransitionCountdown = 60;
    supervisor->value59C = 0;
    g_SetupStatus = 2;
    supervisor->systemTime = timeGetTime();
    return;

setup_failed:
    g_GameManager.gameplaySetupState124 = 2;
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

void __fastcall ResetGameManager(GameManagerSetupLayout *manager)
{
    for (int i = 0; i < 3; ++i)
        TH09_SETUP_PARAMETER_SIDE(manager, i).flags &= ~1u;

    manager->valueB0 = 0;
    manager->valueB8 = 2;
    manager->valueC0 = 0;
    manager->valueD0 = 0;
    manager->valueBC = 0;
    manager->value110 = 0;
    manager->gameplaySetupWaitFrames = 0;
    manager->playtimeFrames = 0;
    manager->replayPauseRecorded = 0;
    manager->inGameMenu = 0;
    manager->menuBlock13D = 0;
    manager->menuBlock13E = 0;
    reinterpret_cast<AsciiGameManagerView *>(manager)->value380 = 0;
}

// The gameplay cleanup resets the actual native Supervisor frame-time
// multiplier at +0x5B8, the same field consumed by Player movement, ECL,
// bullet integration and Title. There is no extra cut-chain Supervisor.
typedef char GameManagerCutChainNativeSupervisorRateAt5B8[
    (offsetof(Supervisor, framerateMultiplier5B8) == 0x5B8 &&
     sizeof(Supervisor) == 0x7B0) ? 1 : -1];

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

    g_Supervisor.framerateMultiplier5B8 = 1.0f;
    g_Chain.Cut(&g_GameManagerCalcChain);
    g_Chain.Cut(&g_GameManagerDrawChain);
}

void __fastcall FormatCurrentDateString(char *buffer)
{
    time_t now;
    time(&now);
    strftime(buffer, 10, "%y/%m/%d", localtime(&now));
}

#undef TH09_SETUP_LOCAL_SIDE
#undef TH09_SETUP_GLOBAL_SIDE

#undef TH09_SETUP_PARAMETER_SIDE
