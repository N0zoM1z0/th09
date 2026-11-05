#pragma once
#include "GameManagerRuntimeView.hpp"

// Shared declaration of the maintained setup owner. Fields remain partial
// target-backed views; original nominal C++ ownership is still provisional.

// Setup resets three physical 0x38-byte records but creates player-side
// controllers only for records 0/1. Record 2 carries the shared EffectManager.
// The unused third-record members have no asserted gameplay role.
struct ReplayManagerView;
struct ReplayStageDispatchView;
typedef AsciiGameManagerSideView SetupSideState;
typedef char SetupSideStateSizeCheck[(sizeof(SetupSideState) == 0x38) ? 1 : -1];

struct GameManagerSetupLayout
{
    SetupSideState sides[3];
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
    union { void *stageObject; ReplayManagerView *replayManagerD4; ReplayStageDispatchView *replayDispatchD4; };
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
    unsigned int instructionMask120;
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

    void AdvanceTimedState();
    static int OnUpdate(GameManagerSetupLayout *gameManager);
    static int OnDraw(GameManagerSetupLayout *gameManager);
    static int AddedCallback(GameManagerSetupLayout *gameManager);
    static int DeletedCallback(GameManagerSetupLayout *gameManager);
    static int RegisterChain();
    static void CleanupGameplayState();
};

// The original reset walks flag words at +0x34/+0x6C/+0xA4. The setup,
// cleanup and delete owners share the EffectManager slot at +0x7C.
typedef char SetupThirdRecordNativeFields[
    (sizeof(GameManagerSetupLayout) == 0x390 &&
     offsetof(GameManagerSetupLayout, sides) + 2 * sizeof(SetupSideState) +
         offsetof(SetupSideState, effectManager0C) == 0x7C &&
     offsetof(GameManagerSetupLayout, sides) + 2 * sizeof(SetupSideState) +
         offsetof(SetupSideState, flags) == 0xA4 &&
     offsetof(GameManagerSetupLayout, gameSubsystem) == 0xA8 &&
     offsetof(GameManagerSetupLayout, secondarySubsystem) == 0xAC) ? 1 : -1];
