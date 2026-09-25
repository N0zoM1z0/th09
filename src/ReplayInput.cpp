// Target-bound replay/input reconstruction views. Original C++ type names,
// source ownership, and translation-unit partition remain unresolved.

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <windows.h>
#include "Chain.hpp"
#include "ReplayRuntimeLeaves.hpp"
#include "ReplayInputState.hpp"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

struct ReplayBufferLink
{
    u16 *input[3];
    u8 *fps;
    i32 frameCount;
    ReplayBufferLink *next;
};

typedef char ReplayBufferLinkSizeIs18[(sizeof(ReplayBufferLink) == 0x18) ? 1 : -1];
typedef char ReplayBufferLinkCountAt10[(offsetof(ReplayBufferLink, frameCount) == 0x10) ? 1 : -1];
typedef char ReplayBufferLinkNextAt14[(offsetof(ReplayBufferLink, next) == 0x14) ? 1 : -1];

struct ReplayFrameDataStartView
{
    u32 value00;
    u16 stageSeed;
    u8 value06;
    u8 value07;
    u8 value08;
    u8 value09;
    u16 value0A;
    u8 value0C;
    u8 unknown0D[3];
    u32 value10;
    u8 unknown14[0x0C];
};

typedef char ReplayFrameDataStartSizeIs20[(sizeof(ReplayFrameDataStartView) == 0x20) ? 1 : -1];

struct ReplayDataView
{
    ReplayDataView();

    u32 magic;
    u16 formatVersion;
    u8 value06;
    u8 value07;
    u8 unknown008[4];
    i32 fileSize;
    u32 checksum;
    u8 unknown14;
    u8 obfuscationKey;
    u8 unknown16[2];
    i32 compressedSize;
    i32 decompressedSize;
    ReplayFrameDataStartView *frameStart[3][10];
    u8 *fpsStart[10];
    u8 randomPayloadByte;
    u8 value0C1;
    u16 value0C2;
    char playTime[10];
    char playerName[8];
    u8 unknown0D6;
    u8 value0D7;
    u8 unknown0D8[4];
    u8 configSnapshot[0xCC];
    u8 unknown1A8[0x28];
    u32 value1D0;
    u32 value1D4;
    u32 value1D8;
    char exeVersion[6];
    u8 unknown1E2[2];
    u8 value1E4;
    u8 value1E5;
    u8 value1E6;
    u8 value1E7;
    u8 value1E8;
    u8 value1E9;
    u8 unknown1EA[2];
};

typedef char ReplayDataSizeIs1EC[(sizeof(ReplayDataView) == 0x1EC) ? 1 : -1];
typedef char ReplayDataFrame0At20[(offsetof(ReplayDataView, frameStart) == 0x20) ? 1 : -1];
typedef char ReplayDataFrame1At48[(offsetof(ReplayDataView, frameStart) + sizeof(((ReplayDataView *)0)->frameStart[0]) == 0x48) ? 1 : -1];
typedef char ReplayDataFrame2At70[(offsetof(ReplayDataView, frameStart) + 2 * sizeof(((ReplayDataView *)0)->frameStart[0]) == 0x70) ? 1 : -1];
typedef char ReplayDataFpsAt98[(offsetof(ReplayDataView, fpsStart) == 0x98) ? 1 : -1];
typedef char ReplayDataConfigAtDC[(offsetof(ReplayDataView, configSnapshot) == 0xDC) ? 1 : -1];
typedef char ReplayDataVersionAt1DC[(offsetof(ReplayDataView, exeVersion) == 0x1DC) ? 1 : -1];

struct ReplaySideStateView
{
    float value00;
    u32 unknown04;
    u32 value08;
    u32 unknown0C;
    u32 value10;
};

struct ReplayInputGateView
{
    u8 unknown000[0xB4];
    u8 sideMode[2];
};

struct GameManagerReplaySideView
{
    u8 unknown00[0x20];
    i32 character;
    u8 unknown24[4];
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
    u8 unknown0EC[0x28];
    i32 mode114;
    i32 playbackState118;
    i32 difficulty11C;
    u8 unknown120[0x10];
    i32 replayPauseRecorded;
    u32 flags;
    int IsPlaybackStateZero();
    int IsPlaybackStateOne();
    int IsReplayNeutral();
    int IsReplayStateTwo();
};

typedef char GameManagerInputGateAtE8[(offsetof(GameManagerReplayView, inputGate) == 0xE8) ? 1 : -1];
typedef char GameManagerReplayPauseAt130[(offsetof(GameManagerReplayView, replayPauseRecorded) == 0x130) ? 1 : -1];
typedef char GameManagerReplayFlagsAt134[(offsetof(GameManagerReplayView, flags) == 0x134) ? 1 : -1];

struct ZunMemoryReplayView
{
    void *Alloc(size_t size, const char *debugText);
    void Free(void *ptr);
    void *AddToRegistry(void *ptr, size_t size, char *name);
};

struct LzssReplayView
{
    static u8 *Decode(u8 *in, i32 inSize, u8 *out, i32 outSize);
    static u8 *Encode(u8 *in, i32 inSize, i32 *outSize);
};

struct ReplayUserDataHeaderView
{
    u32 magic;
    i32 size;
    u8 type;
    u8 unknown09[3];
};

typedef char ReplayUserDataHeaderSizeIs0C[(sizeof(ReplayUserDataHeaderView) == 0x0C) ? 1 : -1];

struct ReplayManagerView
{
    ReplayManagerView();
    i32 frameCounter;
    i32 inputDelay;
    ReplayDataView *replayData;
    u16 *inputCursor0;
    u16 *inputCursor1;
    u16 *inputCursor2;
    u8 *fpsCursor;
    ReplayBufferLink stageBuffers[10];
    ReplayBufferLink *currentBuffer;
    void *unknown110;
    i32 replayMode;
    const char *replayPath;
    u8 unknown11C[0x34];
    ChainElem *mainChain;
    u32 unknown154;
    ChainElem *playbackControlChain;
    ChainElem *frameSyncChain;
    u16 frameRngSeed;
    u16 frameEventFlags;

    static ReplayManagerView *Create(i32 replayMode, const char *replayPath);
    static int SaveReplay(ReplayManagerView *replayManager, const char *replayPath, const char *replayName);
    void Release();
    static ReplayDataView *LoadReplayData(ReplayDataView *data, i32 fileSize);
    static int RecordInputAndFps(ReplayManagerView *replayManager);
    static int PlaybackInputAndFps(ReplayManagerView *replayManager);
    static int BeginPlaybackStage(ReplayManagerView *replayManager);
    static int BeginRecordingStage(ReplayManagerView *replayManager);
    static int CaptureFrameSyncState(ReplayManagerView *replayManager);
    static int ControlPlaybackFrameAdvance(ReplayManagerView *replayManager);
};

typedef char ReplayManagerInput0At0C[(offsetof(ReplayManagerView, inputCursor0) == 0x0C) ? 1 : -1];
typedef char ReplayManagerFpsAt18[(offsetof(ReplayManagerView, fpsCursor) == 0x18) ? 1 : -1];
typedef char ReplayManagerStageBuffersAt1C[(offsetof(ReplayManagerView, stageBuffers) == 0x1C) ? 1 : -1];
typedef char ReplayManagerCurrentBufferAt10C[(offsetof(ReplayManagerView, currentBuffer) == 0x10C) ? 1 : -1];
typedef char ReplayManagerUnknown110At110[(offsetof(ReplayManagerView, unknown110) == 0x110) ? 1 : -1];
typedef char ReplayManagerModeAt114[(offsetof(ReplayManagerView, replayMode) == 0x114) ? 1 : -1];
typedef char ReplayManagerPathAt118[(offsetof(ReplayManagerView, replayPath) == 0x118) ? 1 : -1];
typedef char ReplayManagerMainChainAt150[(offsetof(ReplayManagerView, mainChain) == 0x150) ? 1 : -1];
typedef char ReplayManagerPlaybackControlAt158[(offsetof(ReplayManagerView, playbackControlChain) == 0x158) ? 1 : -1];
typedef char ReplayManagerFrameSyncAt15C[(offsetof(ReplayManagerView, frameSyncChain) == 0x15C) ? 1 : -1];
typedef char ReplayManagerFrameSeedAt160[(offsetof(ReplayManagerView, frameRngSeed) == 0x160) ? 1 : -1];
typedef char ReplayManagerFrameEventsAt162[(offsetof(ReplayManagerView, frameEventFlags) == 0x162) ? 1 : -1];
typedef char ReplayManagerSizeIs164[(sizeof(ReplayManagerView) == 0x164) ? 1 : -1];

extern Chain g_Chain;
extern ReplayRngView g_ReplayRng;
extern GameManagerReplayView g_GameManager;
extern SupervisorReplayView g_Supervisor;
extern ZunMemoryReplayView g_ZunMemory;
extern i32 g_ReplayStageIndex;
extern u8 *g_ReplayConfigSnapshot;
extern u32 g_ReplayHeaderValue1D4;
extern u32 g_ReplayHeaderValue1D8;
extern u32 g_ReplayHeaderValue0D7;
extern u8 g_ReplayHeaderValue1E4;
extern u8 g_ReplayHeaderValue1E5;
extern u32 g_ReplayHeaderValue1E6;
extern u32 g_ReplayHeaderValue1E7;
extern u32 g_ReplayHeaderValue1E8;
extern u32 g_ReplayHeaderValue1E9;
extern u16 g_ReplayStageSeed;
extern u32 g_ReplayFrame0Value06;
extern u32 g_ReplayFrame1Value06;
extern u32 g_ReplayFrame0Value07;
extern u32 g_ReplayFrame1Value07;
extern u32 g_ReplayFrame0Value0A;
extern u32 g_ReplayFrame1Value0A;
extern ReplaySideStateView *g_ReplaySideState0;
extern ReplaySideStateView *g_ReplaySideState1;
extern u32 g_ReplayFrame0Value0C;
extern i32 g_ReplayFrame0Value09;
extern u8 g_ReplayStageSeen[];

extern char g_ReplayPlayTimeText[];
extern i32 g_ReplayInitialHealth0;
extern i32 g_ReplayInitialHealth1;
extern const char *g_ReplayDifficultyNames[];
extern const char *g_ReplayModeNames[];
extern const char *g_ReplayCharacterNames[];
extern const char g_ReplayInfoPlayerNameFormat[];
extern const char g_ReplayInfoPlayTimeFormat[];
extern const char g_ReplayInfoDifficultyFormat[];
extern const char g_ReplayInfoModeFormat[];
extern const char g_ReplayInfoCharacterFormat[];
extern const char g_ReplayInfoVersusCharactersFormat[];
extern const char g_ReplayInfoInitialHealthFormat[];
extern const char g_ReplayInfoVersionFormat[];
extern const char g_ReplayVersionText[];

namespace FileSystem
{
unsigned char *OpenFile(const char *path, int *fileSize, int isExternalResource);
}


int ReplayManagerView::PlaybackInputAndFps(ReplayManagerView *replayManager)
{
    if ((g_GameManager.flags & 4) != 0 && (g_GameManager.flags & 0x200) == 0)
    {
        g_ReplayInputStates[0].historyPrevious = g_ReplayInputStates[0].historyCurrent;
        g_ReplayInputStates[1].historyPrevious = g_ReplayInputStates[1].historyCurrent;
        g_ReplayInputStates[2].historyPrevious = g_ReplayInputStates[2].historyCurrent;

        g_ReplayInputStates[0].historyCurrent = *replayManager->inputCursor0;
        g_ReplayInputStates[1].historyCurrent = *replayManager->inputCursor1;
        g_ReplayInputStates[2].historyCurrent = *replayManager->inputCursor2;

        g_ReplayInputStates[0].Update();
        g_ReplayInputStates[1].Update();
        g_ReplayInputStates[2].Update();

        u8 *sideMode = g_GameManager.inputGate->sideMode;
        for (int side = 0; side < 2; side++, sideMode++)
        {
            if (*sideMode == 1)
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

        replayManager->inputCursor0++;
        replayManager->inputCursor1++;
        replayManager->inputCursor2++;

        if (replayManager->frameCounter % 30 == 0)
        {
            g_Supervisor.recordedFps = (char)replayManager->fpsCursor[1] & 0x7F;
            g_Supervisor.playbackFpsWarning = (char)replayManager->fpsCursor[1] >> 7;
            replayManager->fpsCursor++;
        }
        replayManager->frameCounter++;
    }
    return 1;
}

ReplayDataView *ReplayManagerView::LoadReplayData(ReplayDataView *data, i32 fileSize)
{
    u8 *obfuscateCursor;
    u8 obfuscateOffset;
    u8 *checksumCursor;
    u32 checksum;
    i32 i;
    ReplayDataView *decodedReplay;
    ReplayDataView *replayData = data;

    if (replayData == NULL)
        goto err1;
    if (replayData->magic != 0x50523954)
        goto err1;
    if (replayData->formatVersion != 2)
        goto err1;
    if (replayData->value06 != 0)
        goto err1;

    obfuscateCursor = (u8 *)replayData + 0x18;
    obfuscateOffset = replayData->obfuscationKey;
    for (i = 0; i < replayData->fileSize - 0x18; i++, obfuscateCursor++)
    {
        *obfuscateCursor -= obfuscateOffset;
        obfuscateOffset += 7;
    }

    checksumCursor = (u8 *)replayData + 0x15;
    checksum = 0x3F000318;
    for (i = 0; i < replayData->fileSize - 0x15; i++, checksumCursor++)
        checksum += *checksumCursor;

    if (checksum != replayData->checksum)
        goto err1;

    i32 decompressedSize = replayData->decompressedSize;
    decodedReplay = (ReplayDataView *)g_ZunMemory.Alloc(
        decompressedSize - replayData->fileSize + fileSize + 0xC0,
        "./system\\global.h");
    memcpy(decodedReplay, replayData, 0xC0);
    LzssReplayView::Decode(
        (u8 *)replayData + 0xC0, replayData->compressedSize,
        (u8 *)decodedReplay + 0xC0, replayData->decompressedSize);
    memcpy(
        (u8 *)decodedReplay + replayData->decompressedSize + 0xC0,
        (u8 *)replayData + replayData->fileSize,
        fileSize - replayData->fileSize);

    if (g_Supervisor.CheckVersion(
            decodedReplay->exeVersion, decodedReplay->value1D4, decodedReplay->value1D8))
        goto err2;

    for (i = 0; i < 10; i++)
    {
        if (decodedReplay->frameStart[0][i] != NULL)
            decodedReplay->frameStart[0][i] = (ReplayFrameDataStartView *)((u8 *)decodedReplay + (u32)decodedReplay->frameStart[0][i]);
        if (decodedReplay->frameStart[1][i] != NULL)
            decodedReplay->frameStart[1][i] = (ReplayFrameDataStartView *)((u8 *)decodedReplay + (u32)decodedReplay->frameStart[1][i]);
        if (decodedReplay->frameStart[2][i] != NULL)
            decodedReplay->frameStart[2][i] = (ReplayFrameDataStartView *)((u8 *)decodedReplay + (u32)decodedReplay->frameStart[2][i]);
        if (decodedReplay->fpsStart[i] != NULL)
            decodedReplay->fpsStart[i] = (u8 *)decodedReplay + (u32)decodedReplay->fpsStart[i];
    }

    g_ZunMemory.Free(data);
    return decodedReplay;

err1:
    g_ZunMemory.Free(data);
    return NULL;

err2:
    g_ZunMemory.Free(data);
    g_ZunMemory.Free(decodedReplay);
    return NULL;
}

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

            replayManager->currentBuffer->input[0] = replayManager->inputCursor0;
            replayManager->currentBuffer->input[1] = replayManager->inputCursor1;
            replayManager->currentBuffer->input[2] = replayManager->inputCursor2;
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

int ReplayManagerView::BeginRecordingStage(ReplayManagerView *replayManager)
{
    ReplayDataView *replayData = replayManager->replayData;
    replayManager->frameCounter = 0;
    replayManager->unknown110 = 0;

    if (replayData == NULL)
    {
        replayData = replayManager->replayData = (ReplayDataView *)g_ZunMemory.AddToRegistry(
            new ReplayDataView, sizeof(ReplayDataView), "ReplayFileHeaderInf");
        memset(replayData, 0, sizeof(ReplayDataView));
        replayData->value07 = 0;
        replayData->value06 = 0;
        replayData->magic = 0x50523954;
        replayData->formatVersion = 2;
        replayData->value0C2 = 0x150;
        replayData->value0C1 = 0x61;
        strcpy(replayData->exeVersion, "0150a");
        replayData->value1D4 = g_ReplayHeaderValue1D4;
        replayData->value1D8 = g_ReplayHeaderValue1D8;
        replayData->value0D7 = g_ReplayHeaderValue0D7;
        replayData->value1E4 = g_ReplayHeaderValue1E4;
        replayData->value1E5 = g_ReplayHeaderValue1E5;
        replayData->value1E6 = g_ReplayHeaderValue1E6;
        replayData->value1E7 = g_ReplayHeaderValue1E7;
        replayManager->replayData->value1E8 = g_ReplayHeaderValue1E8;
        replayManager->replayData->value1E9 = g_ReplayHeaderValue1E9;
        memcpy(replayData->playerName, "NO NAME", 4);
        memcpy(replayData->configSnapshot, g_ReplayConfigSnapshot, sizeof(replayData->configSnapshot));
        for (int i = 0; i < 10; i++)
        {
            replayData->frameStart[0][i] = NULL;
            replayData->frameStart[1][i] = NULL;
            replayData->frameStart[2][i] = NULL;
            replayData->fpsStart[i] = NULL;
        }
    }

    int stage = g_ReplayStageIndex;
    if (replayData->frameStart[0][stage] != NULL)
        g_ZunMemory.Free(replayData->frameStart[0][stage]);
    if (replayData->frameStart[1][stage] != NULL)
        g_ZunMemory.Free(replayData->frameStart[1][stage]);
    if (replayData->frameStart[2][stage] != NULL)
        g_ZunMemory.Free(replayData->frameStart[2][stage]);
    if (replayData->fpsStart[stage] != NULL)
        g_ZunMemory.Free(replayData->fpsStart[stage]);

    ReplayFrameDataStartView *frame0 = (ReplayFrameDataStartView *)g_ZunMemory.AddToRegistry(
        (ReplayFrameDataStartView *)operator new(sizeof(ReplayFrameDataStartView)),
        sizeof(ReplayFrameDataStartView), "ReplayFrameDataStartInf");
    replayData->frameStart[0][stage] = frame0;
    ReplayFrameDataStartView *frame1 = (ReplayFrameDataStartView *)g_ZunMemory.AddToRegistry(
        (ReplayFrameDataStartView *)operator new(sizeof(ReplayFrameDataStartView)),
        sizeof(ReplayFrameDataStartView), "ReplayFrameDataStartInf");
    replayData->frameStart[1][stage] = frame1;
    ReplayFrameDataStartView *frame2 = (ReplayFrameDataStartView *)g_ZunMemory.AddToRegistry(
        (ReplayFrameDataStartView *)operator new(sizeof(ReplayFrameDataStartView)),
        sizeof(ReplayFrameDataStartView), "ReplayFrameDataStartInf");
    replayData->frameStart[2][stage] = frame2;
    u8 *fpsStart = (u8 *)g_ZunMemory.AddToRegistry(
        (u8 *)operator new(1), 1, "ReplayFPSTblStartInf");
    replayData->fpsStart[stage] = fpsStart;

    memset(frame0, 0, sizeof(ReplayFrameDataStartView));
    memset(frame1, 0, sizeof(ReplayFrameDataStartView));
    memset(frame2, 0, sizeof(ReplayFrameDataStartView));
    *fpsStart = 0;

    frame2->stageSeed = g_ReplayStageSeed;
    frame1->stageSeed = g_ReplayStageSeed;
    frame0->stageSeed = g_ReplayStageSeed;
    frame0->value06 = g_ReplayFrame0Value06;
    frame1->value06 = g_ReplayFrame1Value06;
    frame0->value07 = g_ReplayFrame0Value07;
    frame1->value07 = g_ReplayFrame1Value07;
    frame0->value0A = g_ReplayFrame0Value0A;
    frame1->value0A = g_ReplayFrame1Value0A;
    frame0->value08 = (u8)g_ReplaySideState0->value00;
    frame1->value08 = (u8)g_ReplaySideState1->value00;
    frame0->value00 = g_ReplaySideState0->value08;
    frame1->value00 = g_ReplaySideState1->value08;
    frame0->value0C = g_ReplayFrame0Value0C;
    frame0->value09 = g_ReplayFrame0Value09;
    frame0->value10 = g_ReplaySideState0->value10;

    replayManager->inputCursor0 = (u16 *)g_ZunMemory.Alloc(0x1C20, "rep data");
    replayManager->inputCursor1 = (u16 *)g_ZunMemory.Alloc(0x1C20, "rep data");
    replayManager->inputCursor2 = (u16 *)g_ZunMemory.Alloc(0x1C20, "rep data");
    replayManager->fpsCursor = (u8 *)g_ZunMemory.Alloc(0x79, "rep data");

    ReplayBufferLink *first = &replayManager->stageBuffers[stage];
    ReplayBufferLink *link = first;
    replayManager->currentBuffer = first;
    if (link != NULL)
    {
        do
        {
            ReplayBufferLink *current = link;
            if (current->input[0] != NULL)
                g_ZunMemory.Free(current->input[0]);
            if (current->input[1] != NULL)
                g_ZunMemory.Free(current->input[1]);
            if (current->input[2] != NULL)
                g_ZunMemory.Free(current->input[2]);
            if (current->fps != NULL)
                g_ZunMemory.Free(current->fps);
            link = current->next;
            memset(current, 0, sizeof(ReplayBufferLink));
            if (current != first)
                g_ZunMemory.Free(current);
        } while (link != NULL);
    }

    replayManager->currentBuffer->input[0] = replayManager->inputCursor0;
    replayManager->currentBuffer->input[1] = replayManager->inputCursor1;
    replayManager->currentBuffer->input[2] = replayManager->inputCursor2;
    replayManager->currentBuffer->fps = replayManager->fpsCursor;
    replayManager->currentBuffer->next = NULL;
    replayManager->currentBuffer->frameCount = 0;

    memset(replayManager->inputCursor0, 0, 0x1C20);
    memset(replayManager->inputCursor1, 0, 0x1C20);
    memset(replayManager->inputCursor2, 0, 0x1C20);
    memset(replayManager->fpsCursor, 0, 0x79);
    return 0;
}



int ReplayManagerView::BeginPlaybackStage(ReplayManagerView *replayManager)
{
    i32 fileSize;
    replayManager->frameCounter = 0;
    if (replayManager->replayData == NULL)
    {
        replayManager->replayData = (ReplayDataView *)FileSystem::OpenFile(
            replayManager->replayPath, &fileSize, (g_GameManager.flags & 2) == 0);
        replayManager->replayData = LoadReplayData(replayManager->replayData, fileSize);
        if (replayManager->replayData == NULL)
            return -1;
        replayManager->unknown110 = 0;
    }
    i32 stage = g_ReplayStageIndex;
    if (replayManager->replayData->frameStart[0] + stage * 10 == NULL)
        return -1;
    if ((g_GameManager.IsPlaybackStateZero() || g_GameManager.IsPlaybackStateOne()) && stage > 0)
    {
        for (i32 i = 0; i < stage; i++)
            g_ReplayStageSeen[replayManager->replayData->frameStart[1][i]->value06] = 1;
    }
    ReplayFrameDataStartView *frame1 = replayManager->replayData->frameStart[1][stage];
    ReplayFrameDataStartView *frame0 = replayManager->replayData->frameStart[0][stage];
    ReplayFrameDataStartView *frame2 = replayManager->replayData->frameStart[2][stage];
    u8 *fpsStart = replayManager->replayData->fpsStart[stage];
    g_ReplayFrame0Value06 = frame0->value06;
    g_ReplayFrame1Value06 = frame1->value06;
    g_ReplayFrame0Value07 = frame0->value07;
    g_ReplayFrame1Value07 = frame1->value07;
    g_ReplayFrame0Value0A = frame0->value0A;
    g_ReplayFrame1Value0A = frame1->value0A;
    g_ReplayFrame0Value0C = frame0->value0C;
    g_ReplayFrame0Value09 = (signed char)frame0->value09;
    g_ReplayHeaderValue0D7 = replayManager->replayData->value0D7;
    g_ReplayHeaderValue1E6 = replayManager->replayData->value1E6;
    g_ReplayHeaderValue1E7 = replayManager->replayData->value1E7;
    g_ReplayHeaderValue1E8 = replayManager->replayData->value1E8;
    g_ReplayHeaderValue1E9 = replayManager->replayData->value1E9;
    g_ReplaySideState0->value08 = frame0->value00;
    g_ReplaySideState1->value08 = frame1->value00;
    g_ReplaySideState0->unknown04 = frame0->value00;
    g_ReplaySideState1->unknown04 = frame1->value00;
    g_ReplaySideState0->value00 = (float)frame0->value08;
    g_ReplaySideState1->value00 = (float)frame1->value08;
    g_ReplaySideState0->value10 = frame0->value10;
    replayManager->inputCursor1 = (u16 *)((u8 *)frame1 + 0x20);
    replayManager->fpsCursor = fpsStart;
    replayManager->inputCursor0 = (u16 *)((u8 *)frame0 + 0x20);
    replayManager->inputCursor2 = (u16 *)((u8 *)frame2 + 0x20);
    memcpy(g_ReplayConfigSnapshot, replayManager->replayData->configSnapshot, 0xCC);
    g_ReplayRng.SetSeed(frame0->stageSeed);
    replayManager->inputDelay = 0;
    return 0;
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

    int side = 0;
    ReplayInputGateView *inputGate = g_GameManager.inputGate;
    for (; side < 2; side++)
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


ReplayManagerView *ReplayManagerView::Create(i32 replayMode, const char *replayPath)
{
    ReplayManagerView *replayManager = (ReplayManagerView *)g_ZunMemory.AddToRegistry(
        new ReplayManagerView, sizeof(ReplayManagerView), "ReplayInf");
    memset(replayManager, 0, sizeof(ReplayManagerView));
    replayManager->replayMode = replayMode;
    replayManager->replayData = NULL;

    switch (replayMode)
    {
    case 0:
        replayManager->mainChain = g_Chain.CreateElem((ChainCallback)RecordInputAndFps);
        replayManager->mainChain->addedCallback = (ChainLifetimeCallback)BeginRecordingStage;
        replayManager->mainChain->arg = replayManager;
        if (g_Chain.AddToCalcChain(replayManager->mainChain, 26))
            return NULL;

        replayManager->playbackControlChain = NULL;
        replayManager->frameSyncChain = g_Chain.CreateElem((ChainCallback)CaptureFrameSyncState);
        replayManager->frameSyncChain->arg = replayManager;
        g_Chain.AddToCalcChain(replayManager->frameSyncChain, 7);
        CaptureFrameSyncState(replayManager);
        break;

    case 1:
        replayManager->replayPath = replayPath;
        replayManager->mainChain = g_Chain.CreateElem((ChainCallback)PlaybackInputAndFps);
        replayManager->mainChain->addedCallback = (ChainLifetimeCallback)BeginPlaybackStage;
        replayManager->mainChain->arg = replayManager;
        if (g_Chain.AddToCalcChain(replayManager->mainChain, 6))
            return NULL;

        replayManager->playbackControlChain =
            g_Chain.CreateElem((ChainCallback)ControlPlaybackFrameAdvance);
        replayManager->playbackControlChain->arg = replayManager;
        g_Chain.AddToCalcChain(replayManager->playbackControlChain, 27);
        replayManager->frameSyncChain = NULL;
        break;
    }

    return replayManager;
}


static int ReleaseReplayManagerCore(ReplayManagerView *replayManager)
{
    if (replayManager->playbackControlChain != NULL)
    {
        g_Chain.Cut(replayManager->playbackControlChain);
        replayManager->playbackControlChain = NULL;
    }
    if (replayManager->frameSyncChain != NULL)
    {
        g_Chain.Cut(replayManager->frameSyncChain);
        replayManager->frameSyncChain = NULL;
    }
    g_ZunMemory.Free(replayManager->replayData);
    if (replayManager->unknown110 != NULL)
        g_ZunMemory.Free(replayManager->unknown110);
    return 0;
}

void ReplayManagerView::Release()
{
    if (this == NULL)
        return;

    for (int stage = 0; stage < 10; stage++)
    {
        ReplayBufferLink *first = &this->stageBuffers[stage];
        ReplayBufferLink *link = first;
        while (link != NULL)
        {
            ReplayBufferLink *current = link;
            if (current->input[0] != NULL)
                g_ZunMemory.Free(current->input[0]);
            if (current->input[1] != NULL)
                g_ZunMemory.Free(current->input[1]);
            if (current->input[2] != NULL)
                g_ZunMemory.Free(current->input[2]);
            if (current->fps != NULL)
                g_ZunMemory.Free(current->fps);
            link = current->next;
            memset(current, 0, sizeof(ReplayBufferLink));
            if (current != first)
                g_ZunMemory.Free(current);
        }

        if (!g_GameManager.IsReplayNeutral())
        {
            for (int stream = 0; stream < 3; stream++)
            {
                if (this->replayData->frameStart[stream][stage] != NULL)
                    g_ZunMemory.Free(this->replayData->frameStart[stream][stage]);
                this->replayData->frameStart[stream][stage] = NULL;
            }
            if (this->replayData->fpsStart[stage] != NULL)
                g_ZunMemory.Free(this->replayData->fpsStart[stage]);
            this->replayData->fpsStart[stage] = NULL;
        }
    }

    ReleaseReplayManagerCore(this);
    g_Chain.Cut(this->mainChain);
    free(this);
}


void __fastcall ReleaseStageObject(void *object)
{
    ReplayManagerView *replayManager = static_cast<ReplayManagerView *>(object);
    if (replayManager != NULL)
    {
        ReleaseReplayManagerCore(replayManager);
        g_Chain.Cut(replayManager->mainChain);
        free(replayManager);
    }
}


static char *AppendReplayFormat(char *buffer, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    return buffer + strlen(buffer);
}

int ReplayManagerView::SaveReplay(ReplayManagerView *replayManager, const char *replayPath, const char *replayName)
{
    if (replayManager == NULL || replayPath == NULL)
        return 0;

    ReplayDataView replayCopy;
    int allocationSize = 0x101EC;
    {
        int stage = 0;
        ReplayBufferLink *stageRoot = &replayManager->stageBuffers[0];
        for (; stage < 10; stage++, stageRoot++)
        {
            if (replayCopy.frameStart[0][stage] != NULL)
            {
                allocationSize += 0x60;
                for (ReplayBufferLink *link = stageRoot; link != NULL; link = link->next)
                    allocationSize += 0x54D9;
            }
        }
    }

    u8 *tempBuffer = (u8 *)g_ZunMemory.Alloc(allocationSize, "rep tmp");
    replayCopy = *replayManager->replayData;
    int currentOffset = sizeof(ReplayDataView);

    for (int stream = 0; stream < 3; stream++)
    {
        for (int stage = 0; stage < 10; stage++)
        {
            if (replayCopy.frameStart[stream][stage] != NULL)
            {
                memcpy(tempBuffer + currentOffset - 0xC0, replayCopy.frameStart[stream][stage], sizeof(ReplayFrameDataStartView));
                replayCopy.frameStart[stream][stage] = (ReplayFrameDataStartView *)currentOffset;
                currentOffset += sizeof(ReplayFrameDataStartView);
                for (ReplayBufferLink *link = &replayManager->stageBuffers[stage]; link != NULL; link = link->next)
                {
                    int streamSize = 2 * link->frameCount;
                    memcpy(tempBuffer + currentOffset - 0xC0, link->input[stream], streamSize);
                    currentOffset += streamSize;
                }
            }
        }
    }

    for (int stage = 0; stage < 10; stage++)
    {
        if (replayCopy.fpsStart[stage] != NULL)
        {
            replayCopy.fpsStart[stage] = (u8 *)currentOffset;
            for (ReplayBufferLink *link = &replayManager->stageBuffers[stage]; link != NULL; link = link->next)
            {
                int fpsSize = link->frameCount / 30;
                memcpy(tempBuffer + currentOffset - 0xC0, link->fps, fpsSize);
                currentOffset += fpsSize;
            }
        }
    }

    ReplayUserDataHeaderView infoHeader;
    infoHeader.magic = 0x52455355;
    infoHeader.type = 0;
    char infoBuffer[1024];
    memset(infoBuffer, 0, sizeof(infoBuffer));
    char *infoCursor = infoBuffer;
    infoCursor = AppendReplayFormat(infoCursor, g_ReplayInfoPlayerNameFormat, replayName);
    infoCursor = AppendReplayFormat(infoCursor, g_ReplayInfoPlayTimeFormat, g_ReplayPlayTimeText);
    infoCursor = AppendReplayFormat(infoCursor, g_ReplayInfoDifficultyFormat, g_ReplayDifficultyNames[g_GameManager.difficulty11C]);
    int mode = g_GameManager.playbackState118;
    if (mode == 2)
        mode = g_GameManager.mode114 + 2;
    infoCursor = AppendReplayFormat(infoCursor, g_ReplayInfoModeFormat, g_ReplayModeNames[mode]);
    if (!g_GameManager.IsReplayStateTwo())
    {
        infoCursor = AppendReplayFormat(infoCursor, g_ReplayInfoCharacterFormat,
            g_ReplayCharacterNames[g_GameManager.sides[0].character]);
    }
    else
    {
        infoCursor = AppendReplayFormat(infoCursor, g_ReplayInfoVersusCharactersFormat,
            g_ReplayCharacterNames[g_GameManager.sides[0].character],
            g_ReplayCharacterNames[g_GameManager.sides[1].character]);
    }
    infoCursor = AppendReplayFormat(infoCursor, g_ReplayInfoInitialHealthFormat,
        g_ReplayInitialHealth0 * 0.5f, g_ReplayInitialHealth1 * 0.5f);
    infoCursor = AppendReplayFormat(infoCursor, g_ReplayInfoVersionFormat, g_ReplayVersionText);
    infoHeader.size = (int)strlen(infoBuffer) + 13;
    infoHeader.size += infoHeader.size & 1;

    replayCopy.value07 = 1;
    strcpy(replayCopy.playerName, replayName);
    strcpy(replayCopy.playTime, g_ReplayPlayTimeText);
    replayCopy.obfuscationKey = (u8)(g_ReplayRng.GetRandomU16InRange(0x80) + 0x40);
    replayCopy.value1D0 = 30;
    replayCopy.randomPayloadByte = (u8)g_ReplayRng.GetRandomU16InRange(0x100);
    replayCopy.unknown14 = (u8)g_ReplayRng.GetRandomU16InRange(0x100);
    memcpy(tempBuffer, (u8 *)&replayCopy + 0xC0, sizeof(ReplayDataView) - 0xC0);

    replayCopy.decompressedSize = currentOffset - 0xC0;
    u8 *compressedData = LzssReplayView::Encode(tempBuffer, replayCopy.decompressedSize, &replayCopy.compressedSize);
    g_ZunMemory.Free(tempBuffer);
    int compressedSize = replayCopy.compressedSize;

    u32 checksum = 0x3F000318;
    u8 *checksumCursor = &replayCopy.obfuscationKey;
    for (int i = 0; i < 0xAB; i++, checksumCursor++)
        checksum += *checksumCursor;
    for (int i = 0; i < compressedSize; i++)
        checksum += compressedData[i];
    replayCopy.checksum = checksum;

    u8 obfuscationOffset = replayCopy.obfuscationKey;
    u8 *obfuscationCursor = (u8 *)&replayCopy.compressedSize;
    for (int i = 0; i < 0xA8; i++, obfuscationCursor++)
    {
        *obfuscationCursor += obfuscationOffset;
        obfuscationOffset += 7;
    }
    obfuscationCursor = compressedData;
    for (int i = 0; i < compressedSize; i++, obfuscationCursor++)
    {
        *obfuscationCursor += obfuscationOffset;
        obfuscationOffset += 7;
    }
    replayCopy.fileSize = compressedSize + 0xC0;

    replayName = NULL;
    HANDLE file = CreateFileA(replayPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file != INVALID_HANDLE_VALUE)
    {
        WriteFile(file, &replayCopy, 0xC0, (LPDWORD)&replayName, NULL);
        WriteFile(file, compressedData, compressedSize, (LPDWORD)&replayName, NULL);
        WriteFile(file, &infoHeader, sizeof(infoHeader), (LPDWORD)&replayName, NULL);
        WriteFile(file, infoBuffer, infoHeader.size - sizeof(infoHeader), (LPDWORD)&replayName, NULL);
        CloseHandle(file);
        GlobalFree(compressedData);
    }
    return 0;
}


ReplayInputState::ReplayInputState()
{
    keyCodes58[0] = 0;
    keyCodes58[1] = 1;
    keyCodes58[2] = 2;
    keyCodes58[3] = 4;
    keyCodes58[4] = -1;
    keyCodes58[5] = -1;
    keyCodes58[6] = -1;
    keyCodes58[7] = -1;
    keyCodes58[8] = 3;
    keyCodes58[9] = 0x5A;
    keyCodes58[10] = 0x58;
    keyCodes58[11] = 0x10;
    keyCodes58[12] = 0x1B;
    keyCodes58[13] = 0x26;
    keyCodes58[14] = 0x28;
    keyCodes58[15] = 0x25;
    keyCodes58[16] = 0x27;
    keyCodes58[17] = 0x11;
    keyCodes58[18] = 0x2C;
    keyCodes58[19] = 0x2D;
    keyCodes58[20] = 0x2A;
    keyCodes58[21] = 1;
    keyCodes58[22] = 0xC8;
    keyCodes58[23] = 0xD0;
    keyCodes58[24] = 0xCB;
    keyCodes58[25] = 0xCD;
    keyCodes58[26] = 0x1D;
}
