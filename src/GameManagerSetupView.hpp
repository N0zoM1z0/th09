#pragma once
#include "GameManagerRuntimeView.hpp"

// Shared declaration of the maintained setup owner. Fields remain partial
// target-backed views; original nominal C++ ownership is still provisional.

// Setup resets three physical 0x38-byte records but creates player-side
// controllers only for records 0/1. Record 2 carries the shared EffectManager.
// The unused third-record members have no asserted gameplay role.
struct ReplayManagerView;
struct ReplayStageDispatchView;
struct PlayerSharedRuntimeView;
struct FrontFactoryConstructView;
typedef AsciiGameManagerSideView SetupSideState;
typedef char SetupSideStateSizeCheck[(sizeof(SetupSideState) == 0x38) ? 1 : -1];

struct GameManagerSetupLayout
{
    // Runtime and replay own two playable records; the setup allocation
    // includes one additional physical record for the shared EffectManager.
    enum
    {
        SideRecordCount = sizeof(((AsciiGameManagerView *)0)->sides) /
                          sizeof(((AsciiGameManagerView *)0)->sides[0]) + 1
    };
    SetupSideState sides[SideRecordCount];
    // The +0xA8 gameplay subsystem is the exact native Front allocation
    // returned by CreateGameSubsystem. Its other receiver projection still
    // shares this physical pointer cell for the update path.
    union { FrontFactoryConstructView *gameSubsystem; PlayerSharedRuntimeView *playerSharedRuntimeA8; };
    void *secondarySubsystem;
    unsigned int valueB0;
    int limitB4;
    int valueB8;
    int valueBC;
    unsigned int valueC0;
    unsigned int valueC4;
    int valueC8;
    int valueCC;
    int valueD0;
    union { void *stageObject; ReplayManagerView *replayManagerD4; ReplayStageDispatchView *replayDispatchD4; };
    unsigned char clearD8[0x10];
    GameConfiguration *configurationE8;
    float speedEC;
    unsigned char unknown_F0[4];
    int valueF4;
    int valueF8;
    int counterFC;
    int value100;
    int value104;
    int value108;
    int value10C;
    int value110;
    unsigned char unknown114[4];
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
     offsetof(GameManagerSetupLayout, playerSharedRuntimeA8) == 0xA8 &&
     offsetof(GameManagerSetupLayout, secondarySubsystem) == 0xAC) ? 1 : -1];

// Reset clears these original dwords independently of the adjacent rank
// ceiling/rate and replay-mode fields; their higher-level roles stay unknown.
typedef char SetupResetNativeWords[
    (offsetof(GameManagerSetupLayout, valueBC) == 0xBC &&
     offsetof(GameManagerSetupLayout, value110) == 0x110 &&
     sizeof(((GameManagerSetupLayout *)0)->valueBC) == 4 &&
     sizeof(((GameManagerSetupLayout *)0)->value110) == 4) ? 1 : -1];

// Setup allocates/copies the configuration in the same +0xE8 pointer cell
// read by Replay, then releases it from the deleted callback.
typedef char SetupNativeConfigurationAtE8[
    (offsetof(GameManagerSetupLayout, configurationE8) == 0xE8 &&
     offsetof(GameManagerSetupLayout, configurationE8) ==
         offsetof(AsciiGameManagerView, configurationE8) &&
     sizeof(((GameManagerSetupLayout *)0)->configurationE8) == 4) ? 1 : -1];
