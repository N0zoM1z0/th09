#pragma once

// Shared declaration of the maintained setup owner. Fields remain partial
// target-backed views; original nominal C++ ownership is still provisional.
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
