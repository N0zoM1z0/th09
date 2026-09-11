// Target-bound replay/input reconstruction views. Original C++ type names,
// source ownership, and translation-unit partition remain unresolved.

#include <stddef.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
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

struct ReplayBufferLink
{
    u16 *input0;
    u16 *input1;
    u16 *input2;
    u8 *fps;
    i32 frameCount;
    ReplayBufferLink *next;
};

typedef char ReplayBufferLinkSizeIs18[(sizeof(ReplayBufferLink) == 0x18) ? 1 : -1];
typedef char ReplayBufferLinkCountAt10[(offsetof(ReplayBufferLink, frameCount) == 0x10) ? 1 : -1];
typedef char ReplayBufferLinkNextAt14[(offsetof(ReplayBufferLink, next) == 0x14) ? 1 : -1];

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

struct GameManagerReplaySideView
{
    u8 unknown00[0x28];
    i32 selector;
    u8 unknown2C[0x0C];
};

typedef char GameManagerReplaySideSizeIs38[(sizeof(GameManagerReplaySideView) == 0x38) ? 1 : -1];
typedef char GameManagerReplaySideSelectorAt28[
    (offsetof(GameManagerReplaySideView, selector) == 0x28) ? 1 : -1];

struct GameManagerReplayView
{
    GameManagerReplaySideView sides[2];
    u8 unknown070[0x78];
    ReplayInputGateView *inputGate;
    u8 unknown0EC[0x44];
    i32 replayPauseRecorded;
    u32 flags;
};

typedef char GameManagerInputGateAtE8[(offsetof(GameManagerReplayView, inputGate) == 0xE8) ? 1 : -1];
typedef char GameManagerReplayPauseAt130[(offsetof(GameManagerReplayView, replayPauseRecorded) == 0x130) ? 1 : -1];
typedef char GameManagerReplayFlagsAt134[(offsetof(GameManagerReplayView, flags) == 0x134) ? 1 : -1];

struct SupervisorReplayView
{
    int IsSpeedhackDetected();
    u8 unknown000[0x5C8];
    short recordedFps;
};

typedef char SupervisorReplayFpsAt5C8[(offsetof(SupervisorReplayView, recordedFps) == 0x5C8) ? 1 : -1];

struct ZunMemoryReplayView
{
    void *Alloc(size_t size, const char *debugText);
    void *AddToRegistry(void *ptr, size_t size, char *name);
};

struct ReplayManagerView
{
    i32 frameCounter;
    i32 inputDelay;
    void *replayData;
    u16 *inputCursor0;
    u16 *inputCursor1;
    u16 *inputCursor2;
    u8 *fpsCursor;
    u8 unknown01C[0xF0];
    ReplayBufferLink *currentBuffer;
    u8 unknown110[0x50];
    u16 frameRngSeed;
    u16 frameEventFlags;

    static int RecordInputAndFps(ReplayManagerView *replayManager);
    static int CaptureFrameSyncState(ReplayManagerView *replayManager);
};

typedef char ReplayManagerInput0At0C[(offsetof(ReplayManagerView, inputCursor0) == 0x0C) ? 1 : -1];
typedef char ReplayManagerFpsAt18[(offsetof(ReplayManagerView, fpsCursor) == 0x18) ? 1 : -1];
typedef char ReplayManagerCurrentBufferAt10C[(offsetof(ReplayManagerView, currentBuffer) == 0x10C) ? 1 : -1];
typedef char ReplayManagerFrameSeedAt160[(offsetof(ReplayManagerView, frameRngSeed) == 0x160) ? 1 : -1];
typedef char ReplayManagerFrameEventsAt162[(offsetof(ReplayManagerView, frameEventFlags) == 0x162) ? 1 : -1];
typedef char ReplayManagerSizeIs164[(sizeof(ReplayManagerView) == 0x164) ? 1 : -1];

extern ReplayRngView g_ReplayRng;
extern ReplayInputState g_ReplayInputStates[3];
extern GameManagerReplayView g_GameManager;
extern SupervisorReplayView g_Supervisor;
extern ZunMemoryReplayView g_ZunMemory;

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

int ReplayManagerView::RecordInputAndFps(ReplayManagerView *replayManager)
{
    u32 flags = g_GameManager.flags;

    if ((flags & 4) != 0 && !g_Supervisor.IsSpeedhackDetected())
    {
        if ((flags & 0x200) != 0)
        {
            if (replayManager->inputDelay >= 3)
                return 1;
            replayManager->inputDelay++;
        }

        if (g_GameManager.sides[0].selector == 0)
            *replayManager->inputCursor0 = g_ReplayInputStates[0].historyCurrent;
        if (g_GameManager.sides[1].selector == 0)
            *replayManager->inputCursor1 = g_ReplayInputStates[1].historyCurrent;
        *replayManager->inputCursor2 = g_ReplayInputStates[2].historyCurrent;

        replayManager->currentBuffer->frameCount++;
        replayManager->inputCursor0++;
        replayManager->inputCursor1++;
        replayManager->inputCursor2++;

        if (replayManager->frameCounter % 30 == 0)
        {
            *replayManager->fpsCursor = (u8)g_Supervisor.recordedFps;
            replayManager->fpsCursor++;
        }

        if (replayManager->currentBuffer->frameCount >= 3598)
        {
            replayManager->currentBuffer->next =
                (ReplayBufferLink *)g_ZunMemory.AddToRegistry(
                    new ReplayBufferLink, sizeof(ReplayBufferLink), "ReplayBufLinkInf");
            replayManager->currentBuffer = replayManager->currentBuffer->next;

            replayManager->inputCursor0 = (u16 *)g_ZunMemory.Alloc(0x1C20, "rep data");
            replayManager->inputCursor1 = (u16 *)g_ZunMemory.Alloc(0x1C20, "rep data");
            replayManager->inputCursor2 = (u16 *)g_ZunMemory.Alloc(0x1C20, "rep data");
            replayManager->fpsCursor = (u8 *)g_ZunMemory.Alloc(0x79, "rep data");

            replayManager->currentBuffer->input0 = replayManager->inputCursor0;
            replayManager->currentBuffer->input1 = replayManager->inputCursor1;
            replayManager->currentBuffer->input2 = replayManager->inputCursor2;
            replayManager->currentBuffer->fps = replayManager->fpsCursor;
            replayManager->currentBuffer->next = NULL;
            replayManager->currentBuffer->frameCount = 0;

            memset(replayManager->inputCursor0, 0, 0x1C20);
            memset(replayManager->inputCursor1, 0, 0x1C20);
            memset(replayManager->inputCursor2, 0, 0x1C20);
            memset(replayManager->fpsCursor, 0, 0x79);
        }

        replayManager->frameCounter++;
    }

    return 1;
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
