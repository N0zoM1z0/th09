#pragma once
// Fully target-backed 0x164-byte ReplayManager source type shared by the
// independently exact ReplayInput and Title families, and the open Gameplay
// setup caller. No original class-spelling or original TU claim.
#include <stddef.h>
#include "Chain.hpp"
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

#include "ReplayDataOriginalView.hpp"


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
