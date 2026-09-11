// Target-bound replay/input reconstruction views. Original C++ type names,
// source ownership, and translation-unit partition remain unresolved.

#include <stddef.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef int i32;

struct ReplayInputState
{
    u16 currentInput;
    u16 word02;
    u16 repeatOutput;
    u16 word06;
    u16 word08;
    u8 unknown0A[0x20];
    u16 auxiliary2A;
    u16 historyCurrent;
    u16 historyPrevious;
    u16 historyRepeat;
    u16 historyPressed;
    u16 historyReleased;
    u16 unknown36;
    u16 heldFrames[16];
    u8 unknown58[0x36];

    void Update();
    u16 IsHeld(int mask);
};

typedef char ReplayInputStateSizeIs8E[(sizeof(ReplayInputState) == 0x8E) ? 1 : -1];
typedef char ReplayInputAuxAt2A[(offsetof(ReplayInputState, auxiliary2A) == 0x2A) ? 1 : -1];
typedef char ReplayInputHistoryAt2C[(offsetof(ReplayInputState, historyCurrent) == 0x2C) ? 1 : -1];

struct ReplayRngView
{
    u16 seed;
    u16 unknown02;
    i32 generationCount;
    u16 GetSeed();
    void ResetGenerationCount();
};

struct ReplayInputGateView
{
    u8 unknown000[0xB4];
    u8 sideMode[2];
};

struct GameManagerReplayView
{
    u8 unknown000[0xE8];
    ReplayInputGateView *inputGate;
    u8 unknown0EC[0x44];
    i32 replayPauseRecorded;
};

typedef char GameManagerInputGateAtE8[(offsetof(GameManagerReplayView, inputGate) == 0xE8) ? 1 : -1];
typedef char GameManagerReplayPauseAt130[(offsetof(GameManagerReplayView, replayPauseRecorded) == 0x130) ? 1 : -1];

struct ReplayManagerView
{
    u8 unknown000[0x160];
    u16 frameRngSeed;
    u16 frameEventFlags;
    static int CaptureFrameSyncState(ReplayManagerView *replayManager);
};

typedef char ReplayManagerFrameSeedAt160[(offsetof(ReplayManagerView, frameRngSeed) == 0x160) ? 1 : -1];
typedef char ReplayManagerFrameEventsAt162[(offsetof(ReplayManagerView, frameEventFlags) == 0x162) ? 1 : -1];
typedef char ReplayManagerSizeIs164[(sizeof(ReplayManagerView) == 0x164) ? 1 : -1];

extern ReplayRngView g_ReplayRng;
extern ReplayInputState g_ReplayInputStates[3];
extern GameManagerReplayView g_GameManager;

void ReplayInputState::Update()
{
    u16 currentBits = this->historyCurrent;
    u16 *held = this->heldFrames;
    i32 remaining = 16;
    u16 repeatBit = 1;

    this->repeatOutput = 0;
    do
    {
        if ((currentBits & repeatBit) != 0)
        {
            (*held)++;
            if (*held >= 26)
            {
                this->historyRepeat |= repeatBit;
                *held -= 8;
            }
        }
        else
        {
            *held = 0;
        }
        currentBits >>= 1;
        held++;
        remaining--;
    } while (remaining != 0);

    this->historyPressed =
        (this->historyCurrent ^ this->historyPrevious) & this->historyCurrent;
    this->historyReleased =
        (this->historyCurrent ^ this->historyPrevious) & ~this->historyCurrent;
}

int ReplayManagerView::CaptureFrameSyncState(ReplayManagerView *replayManager)
{
    replayManager->frameEventFlags = 0;
    replayManager->frameRngSeed = g_ReplayRng.GetSeed();
    g_ReplayRng.ResetGenerationCount();

    if (g_GameManager.replayPauseRecorded != 0)
        replayManager->frameEventFlags |= 0x100;

    g_ReplayInputStates[0].historyPrevious = g_ReplayInputStates[0].historyCurrent;
    g_ReplayInputStates[0].historyCurrent = g_ReplayInputStates[0].currentInput;
    g_ReplayInputStates[1].historyPrevious = g_ReplayInputStates[1].historyCurrent;
    g_GameManager.replayPauseRecorded = 0;
    g_ReplayInputStates[1].historyCurrent = g_ReplayInputStates[1].currentInput;
    g_ReplayInputStates[2].historyPrevious = g_ReplayInputStates[2].historyCurrent;
    g_ReplayInputStates[2].historyCurrent = g_ReplayInputStates[2].currentInput;

    g_ReplayInputStates[0].Update();
    g_ReplayInputStates[1].Update();
    g_ReplayInputStates[2].Update();

    ReplayInputGateView *inputGate = g_GameManager.inputGate;
    for (int side = 0; side < 2; side++)
    {
        if (inputGate != NULL && inputGate->sideMode[side] == 1)
        {
            if (g_ReplayInputStates[side].IsHeld(1))
            {
                g_ReplayInputStates[side].auxiliary2A++;
                if (g_ReplayInputStates[side].auxiliary2A >= 8)
                    g_ReplayInputStates[side].auxiliary2A = 8;
            }
            else
            {
                g_ReplayInputStates[side].auxiliary2A = 0;
            }
        }
    }

    return 1;
}
