// Target-bound replay/input reconstruction views. Original C++ type names,
// source ownership, and translation-unit partition remain unresolved.

#include <stddef.h>
#include <string.h>
#include <new>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <windows.h>
#include "Chain.hpp"
#include "ZunMemory.hpp"
#include "Lzss.hpp"
#include "ReplayRuntimeLeaves.hpp"
#include "RngRuntimeLeaves.hpp"
#include "ReplayInputState.hpp"
#include "GameManagerRuntimeView.hpp"
#include "GameConfiguration.hpp"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

#include "ReplayManagerRuntimeView.hpp"

#include "ReplayNativeInputGateCapacity.hpp"

// Replay side pointers at 0x4A7DAC/0x4A7DE4 alias the true
// GameManager.sides[0/1].runtime fields (+0x1C per 0x38-byte side).
// Their pointee is the native 0xA0-byte side score/phase buffer.
typedef char ReplayNativeSideRuntimeFields[
    (sizeof(AsciiGameSideRuntimeView) == 0xA0 &&
     offsetof(AsciiGameSideRuntimeView, value00) == 0x00 &&
     offsetof(AsciiGameSideRuntimeView, value04) == 0x04 &&
     offsetof(AsciiGameSideRuntimeView, value08) == 0x08 &&
     offsetof(AsciiGameSideRuntimeView, phase10) == 0x10 &&
     offsetof(AsciiGameManagerSideView, runtime) == 0x1C) ? 1 : -1];

// The native GameManager +0xE8 configuration pointer is the same original
// owner used for the complete snapshot and the two input-mode bytes at +0xB4.
typedef char ReplayNativeInputGateConfigModes[
    (offsetof(AsciiGameManagerView, replayInputGateE8) == 0xE8 &&
     offsetof(GameConfiguration, sideFocusMode) == 0xB4) ? 1 : -1];

// The two 0x38-byte side records, +0xE8 input gate and +0x114/+0x118
// replay mode fields are actually part of the original-bound GameManager.
// Do not create another unrelated manager ABI in the replay interpreter.
typedef char ReplayNativeSideAndGateOffsets[
    (sizeof(AsciiGameManagerSideView) == 0x38 &&
     offsetof(AsciiGameManagerSideView, character20) == 0x20 &&
     offsetof(AsciiGameManagerSideView, replaySelector28) == 0x28 &&
     offsetof(AsciiGameManagerView, replayInputGateE8) == 0xE8 &&
     offsetof(AsciiGameManagerView, replayMode114) == 0x114 &&
     offsetof(AsciiGameManagerView, gameMode) == 0x118 &&
     offsetof(AsciiGameManagerView, difficulty) == 0x11C &&
     offsetof(AsciiGameManagerView, replayPauseRecorded130) == 0x130 &&
     offsetof(AsciiGameManagerView, flags) == 0x134) ? 1 : -1];

struct ReplayUserDataHeaderView
{
    u32 magic;
    i32 size;
    u8 type;
    u8 unknown09[3];
};

typedef char ReplayUserDataHeaderSizeIs0C[(sizeof(ReplayUserDataHeaderView) == 0x0C) ? 1 : -1];

// These original byte ranges are bounded by the physical 0xC0 wire header.
// They do not cover the post-header payload nor its compressed stream.
typedef char ReplaySaveHeaderCodecSpans[
    (sizeof(ReplayHeaderView) == 0xC0 &&
     offsetof(ReplayHeaderView, obfuscationKey) == 0x15 &&
     offsetof(ReplayHeaderView, compressedSize) == 0x18 &&
     sizeof(ReplayHeaderView) - offsetof(ReplayHeaderView, obfuscationKey) == 0xAB &&
     sizeof(ReplayHeaderView) - offsetof(ReplayHeaderView, compressedSize) == 0xA8 &&
     sizeof(ReplayUserDataHeaderView) + 1 == 13) ? 1 : -1];


// The recording payload owns the same native 0xCC GameConfiguration
// object as the live runtime. Record and restore are ordinary value copies.
typedef char ReplayNativeConfigurationSnapshotIdentity[
    (sizeof(((ReplayDataView *)0)->configSnapshot) == sizeof(GameConfiguration) &&
     sizeof(*((AsciiGameManagerView *)0)->configurationE8) == sizeof(GameConfiguration) &&
     sizeof(GameConfiguration) == 0xCC &&
     offsetof(ReplayDataView, configSnapshot) == 0xDC) ? 1 : -1];

// Recording and playback use the original GameManager members, including
// side +0x2C/+0x30 words and the signed stage byte widened at +0x138.
// The serialized header has one native byte/word map. Preserve the original
// *signed* fileSize subtraction when turning offsets into C++ offsetof values.
typedef char ReplayCodecNativeWireHeaderOffsets[
    (offsetof(ReplayDataView, obfuscationKey) == 0x15 &&
     offsetof(ReplayDataView, compressedSize) == 0x18 &&
     offsetof(ReplayDataView, randomPayloadByte) == 0xC0 &&
     sizeof(ReplayHeaderView) == 0xC0) ? 1 : -1];

// Replay playback's original address test forms the address of one
// 0x28-byte frame table row in the genuine 3x10 pointer header. This is
// NOT a test of the pointer stored at frameStart[0][stage].
typedef char ReplayNativePlaybackFrameRowAddress[
    (offsetof(ReplayDataView, frameStart) == 0x20 &&
     sizeof(((ReplayDataView *)0)->frameStart[0]) == 0x28 &&
     sizeof(((ReplayDataView *)0)->frameStart[0]) ==
         ReplayRecordingCapacity::StageCount * sizeof(ReplayFrameDataStartView *) &&
     sizeof(((ReplayDataView *)0)->frameStart) ==
         ReplayRecordingCapacity::InputStreamCount * 0x28) ? 1 : -1];

typedef char ReplayNativeRestoredStateOffsets[
    (offsetof(AsciiGameManagerSideView, value2C) == 0x2C &&
     offsetof(AsciiGameManagerSideView, value30) == 0x30 &&
     offsetof(AsciiGameManagerView, clearD8) == 0xD8 &&
     offsetof(AsciiGameManagerView, configurationE8) == 0xE8 &&
     offsetof(AsciiGameManagerView, valueF4) == 0xF4 &&
     offsetof(AsciiGameManagerView, field0FC) == 0xFC &&
     offsetof(AsciiGameManagerView, value138) == 0x138 &&
     offsetof(AsciiGameManagerView, seedCopy346) == 0x346) ? 1 : -1];

extern Chain g_Chain;
// The seed methods and the shared generator project the same eight-byte state.
extern RngRuntimeView g_ReplayRng;
extern SupervisorReplayView g_Supervisor;
extern u32 g_ReplayHeaderValue1D4;
extern u32 g_ReplayHeaderValue1D8;

extern char g_ReplayPlayTimeText[];
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
        g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].historyPrevious =
            g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].historyCurrent;

        g_ReplayInputStates[0].historyCurrent = *replayManager->inputCursors[0];
        g_ReplayInputStates[1].historyCurrent = *replayManager->inputCursors[1];
        g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].historyCurrent =
            *replayManager->inputCursors[2];

        g_ReplayInputStates[0].Update();
        g_ReplayInputStates[1].Update();
        g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].Update();

        u8 *sideMode = g_GameManager.configurationE8->sideFocusMode;
        for (int side = 0; side < ReplayNativeInputGateCapacity::SideCount; side++, sideMode++)
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

        replayManager->inputCursors[0] = reinterpret_cast<u16 *>(
            reinterpret_cast<unsigned int>(replayManager->inputCursors[0]) +
            static_cast<unsigned int>(sizeof(u16)));
        replayManager->inputCursors[1] = reinterpret_cast<u16 *>(
            reinterpret_cast<unsigned int>(replayManager->inputCursors[1]) +
            static_cast<unsigned int>(sizeof(u16)));
        replayManager->inputCursors[2] = reinterpret_cast<u16 *>(
            reinterpret_cast<unsigned int>(replayManager->inputCursors[2]) +
            static_cast<unsigned int>(sizeof(u16)));

        if (replayManager->frameCounter % ReplayRecordingCapacity::FramesPerFpsSample == 0)
        {
            g_Supervisor.recordedFps = (char)replayManager->fpsCursor[1] & 0x7F;
            g_Supervisor.playbackFpsWarning = (char)replayManager->fpsCursor[1] >> 7;
            replayManager->fpsCursor = reinterpret_cast<u8 *>(
                reinterpret_cast<unsigned int>(replayManager->fpsCursor) + sizeof(u8));
        }
        replayManager->frameCounter = static_cast<int>(static_cast<unsigned int>(replayManager->frameCounter) + 1u);
    }
    return 1;
}

ReplayDataView *ReplayManagerView::LoadReplayData(ReplayDataView *data, i32 fileSize)
{
    unsigned int obfuscateAddress;
    u8 obfuscateOffset;
    unsigned int checksumAddress;
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

    obfuscateAddress = reinterpret_cast<unsigned int>(&replayData->compressedSize);
    obfuscateOffset = replayData->obfuscationKey;
    // The original Win32 length subtraction wraps before the signed bound
    // comparison; the serialized fields themselves remain signed DWORDs.
    for (i = 0; i < static_cast<i32>(
             static_cast<u32>(replayData->fileSize) -
             static_cast<u32>(offsetof(ReplayDataView, compressedSize)));
         i++, obfuscateAddress++)
    {
        *reinterpret_cast<u8 *>(obfuscateAddress) -= obfuscateOffset;
        obfuscateOffset += 7;
    }

    checksumAddress = reinterpret_cast<unsigned int>(&replayData->obfuscationKey);
    checksum = 0x3F000318;
    for (i = 0; i < static_cast<i32>(
             static_cast<u32>(replayData->fileSize) -
             static_cast<u32>(offsetof(ReplayDataView, obfuscationKey)));
         i++, checksumAddress++)
        checksum += *reinterpret_cast<const u8 *>(checksumAddress);

    if (checksum != replayData->checksum)
        goto err1;

    // Allocation size and trailing copy count are also native 32-bit word
    // expressions, even when an input header contains extreme signed values.
    i32 decompressedSize = replayData->decompressedSize;
    decodedReplay = (ReplayDataView *)g_ZunMemory.Alloc(
        (static_cast<u32>(decompressedSize) - static_cast<u32>(replayData->fileSize)) +
        static_cast<u32>(fileSize) + sizeof(ReplayHeaderView),
        "./system\\global.h");
    memcpy(decodedReplay, replayData, sizeof(ReplayHeaderView));
    // The packed stream begins at the payload byte inside the flat wire buffer.
    Lzss::Decode(
        reinterpret_cast<u8 *>(
            reinterpret_cast<unsigned int>(replayData) +
            static_cast<unsigned int>(offsetof(ReplayDataView, randomPayloadByte))),
        replayData->compressedSize,
        reinterpret_cast<u8 *>(
            reinterpret_cast<unsigned int>(decodedReplay) +
            static_cast<unsigned int>(offsetof(ReplayDataView, randomPayloadByte))),
        replayData->decompressedSize);
    memcpy(
        reinterpret_cast<u8 *>(
            reinterpret_cast<unsigned int>(decodedReplay) +
            static_cast<unsigned int>(offsetof(ReplayDataView, randomPayloadByte)) +
            static_cast<unsigned int>(replayData->decompressedSize)),
        reinterpret_cast<u8 *>(
            reinterpret_cast<unsigned int>(replayData) +
            static_cast<unsigned int>(replayData->fileSize)),
        static_cast<u32>(fileSize) - static_cast<u32>(replayData->fileSize));

    if (g_Supervisor.CheckVersion(
            decodedReplay->exeVersion, decodedReplay->value1D4, decodedReplay->value1D8))
        goto err2;

    for (i = 0; i < ReplayRecordingCapacity::StageCount; i++)
    {
        if (decodedReplay->frameOffset[0][i] != 0)
            decodedReplay->frameStart[0][i] = reinterpret_cast<ReplayFrameDataStartView *>(
                reinterpret_cast<unsigned int>(decodedReplay) +
                static_cast<unsigned int>(decodedReplay->frameOffset[0][i]));
        if (decodedReplay->frameOffset[1][i] != 0)
            decodedReplay->frameStart[1][i] = reinterpret_cast<ReplayFrameDataStartView *>(
                reinterpret_cast<unsigned int>(decodedReplay) +
                static_cast<unsigned int>(decodedReplay->frameOffset[1][i]));
        if (decodedReplay->frameOffset[2][i] != 0)
            decodedReplay->frameStart[2][i] = reinterpret_cast<ReplayFrameDataStartView *>(
                reinterpret_cast<unsigned int>(decodedReplay) +
                static_cast<unsigned int>(decodedReplay->frameOffset[2][i]));
        if (decodedReplay->fpsOffset[i] != 0)
            decodedReplay->fpsStart[i] = reinterpret_cast<u8 *>(
                reinterpret_cast<unsigned int>(decodedReplay) +
                static_cast<unsigned int>(decodedReplay->fpsOffset[i]));
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
            replayManager->inputDelay = static_cast<int>(static_cast<unsigned int>(replayManager->inputDelay) + 1u);
        }

        if (g_GameManager.sides[0].replaySelector28 == 0)
            *replayManager->inputCursors[0] = g_ReplayInputStates[0].historyCurrent;
        if (g_GameManager.sides[1].replaySelector28 == 0)
            *replayManager->inputCursors[1] = g_ReplayInputStates[1].historyCurrent;
        *replayManager->inputCursors[2] =
            g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].historyCurrent;

        replayManager->currentBuffer->frameCount = static_cast<int>(static_cast<unsigned int>(replayManager->currentBuffer->frameCount) + 1u);
        replayManager->inputCursors[0] = reinterpret_cast<u16 *>(
            reinterpret_cast<unsigned int>(replayManager->inputCursors[0]) +
            static_cast<unsigned int>(sizeof(u16)));
        replayManager->inputCursors[1] = reinterpret_cast<u16 *>(
            reinterpret_cast<unsigned int>(replayManager->inputCursors[1]) +
            static_cast<unsigned int>(sizeof(u16)));
        replayManager->inputCursors[2] = reinterpret_cast<u16 *>(
            reinterpret_cast<unsigned int>(replayManager->inputCursors[2]) +
            static_cast<unsigned int>(sizeof(u16)));

        if (replayManager->frameCounter % ReplayRecordingCapacity::FramesPerFpsSample == 0)
        {
            *replayManager->fpsCursor = (u8)g_Supervisor.recordedFps;
            replayManager->fpsCursor = reinterpret_cast<u8 *>(
                reinterpret_cast<unsigned int>(replayManager->fpsCursor) + sizeof(u8));
        }

        if (replayManager->currentBuffer->frameCount >= ReplayRecordingCapacity::LinkRolloverFrames)
        {
            replayManager->currentBuffer->next =
                (ReplayBufferLink *)g_ZunMemory.AddToRegistry(
                    new ReplayBufferLink, sizeof(ReplayBufferLink), "ReplayBufLinkInf");
            replayManager->currentBuffer = replayManager->currentBuffer->next;

            replayManager->inputCursors[0] = (u16 *)g_ZunMemory.Alloc(ReplayRecordingCapacity::InputStreamBytes, "rep data");
            replayManager->inputCursors[1] = (u16 *)g_ZunMemory.Alloc(ReplayRecordingCapacity::InputStreamBytes, "rep data");
            replayManager->inputCursors[2] = (u16 *)g_ZunMemory.Alloc(ReplayRecordingCapacity::InputStreamBytes, "rep data");
            replayManager->fpsCursor = (u8 *)g_ZunMemory.Alloc(ReplayRecordingCapacity::FpsStreamBytes, "rep data");

            replayManager->currentBuffer->input[0] = replayManager->inputCursors[0];
            replayManager->currentBuffer->input[1] = replayManager->inputCursors[1];
            replayManager->currentBuffer->input[2] = replayManager->inputCursors[2];
            replayManager->currentBuffer->fps = replayManager->fpsCursor;
            replayManager->currentBuffer->next = NULL;
            replayManager->currentBuffer->frameCount = 0;

            memset(replayManager->inputCursors[0], 0, ReplayRecordingCapacity::InputStreamBytes);
            memset(replayManager->inputCursors[1], 0, ReplayRecordingCapacity::InputStreamBytes);
            memset(replayManager->inputCursors[2], 0, ReplayRecordingCapacity::InputStreamBytes);
            memset(replayManager->fpsCursor, 0, ReplayRecordingCapacity::FpsStreamBytes);
        }

        replayManager->frameCounter = static_cast<int>(static_cast<unsigned int>(replayManager->frameCounter) + 1u);
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
        replayData->value0D7 = g_GameManager.difficulty;
        replayData->value1E4 = g_GameManager.gameMode;
        replayData->value1E5 = g_GameManager.replayMode114;
        replayData->value1E6 = g_GameManager.value378;
        replayData->value1E7 = g_GameManager.value37C;
        replayManager->replayData->value1E8 = g_GameManager.sides[0].value30;
        replayManager->replayData->value1E9 = g_GameManager.sides[1].value30;
        memcpy(replayData->playerName, "NO NAME", 4);
        replayData->configSnapshot = *g_GameManager.configurationE8;
        for (int i = 0; i < ReplayRecordingCapacity::StageCount; i++)
        {
            replayData->frameStart[0][i] = NULL;
            replayData->frameStart[1][i] = NULL;
            replayData->frameStart[2][i] = NULL;
            replayData->fpsStart[i] = NULL;
        }
    }

    int stage = g_GameManager.field0FC;
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

    frame2->stageSeed = g_GameManager.seedCopy346;
    frame1->stageSeed = g_GameManager.seedCopy346;
    frame0->stageSeed = g_GameManager.seedCopy346;
    frame0->value06 = g_GameManager.sides[0].character20;
    frame1->value06 = g_GameManager.sides[1].character20;
    frame0->value07 = g_GameManager.sides[0].replaySelector28;
    frame1->value07 = g_GameManager.sides[1].replaySelector28;
    frame0->value0A = g_GameManager.sides[0].value2C;
    frame1->value0A = g_GameManager.sides[1].value2C;
    frame0->value08 = (u8)g_GameManager.sides[0].runtime->value00;
    frame1->value08 = (u8)g_GameManager.sides[1].runtime->value00;
    frame0->value00 = g_GameManager.sides[0].runtime->value08;
    frame1->value00 = g_GameManager.sides[1].runtime->value08;
    frame0->value0C = g_GameManager.valueF4;
    frame0->value09 = g_GameManager.value138;
    frame0->value10 = g_GameManager.sides[0].runtime->phase10;

    replayManager->inputCursors[0] = (u16 *)g_ZunMemory.Alloc(ReplayRecordingCapacity::InputStreamBytes, "rep data");
    replayManager->inputCursors[1] = (u16 *)g_ZunMemory.Alloc(ReplayRecordingCapacity::InputStreamBytes, "rep data");
    replayManager->inputCursors[2] = (u16 *)g_ZunMemory.Alloc(ReplayRecordingCapacity::InputStreamBytes, "rep data");
    replayManager->fpsCursor = (u8 *)g_ZunMemory.Alloc(ReplayRecordingCapacity::FpsStreamBytes, "rep data");

    ReplayBufferLink *first = &replayManager->stageBuffers[stage];
    ReplayBufferLink *current = &replayManager->stageBuffers[stage];
    replayManager->currentBuffer = first;
    while (current != NULL)
    {
        ReplayBufferLink *link = current;
        if (link->input[0] != NULL)
            g_ZunMemory.Free(link->input[0]);
        if (link->input[1] != NULL)
            g_ZunMemory.Free(link->input[1]);
        if (link->input[2] != NULL)
            g_ZunMemory.Free(link->input[2]);
        if (link->fps != NULL)
            g_ZunMemory.Free(link->fps);
        current = link->next;
        memset(link, 0, sizeof(ReplayBufferLink));
        if (link != first)
            g_ZunMemory.Free(link);
    }

    replayManager->currentBuffer->input[0] = replayManager->inputCursors[0];
    replayManager->currentBuffer->input[1] = replayManager->inputCursors[1];
    replayManager->currentBuffer->input[2] = replayManager->inputCursors[2];
    replayManager->currentBuffer->fps = replayManager->fpsCursor;
    replayManager->currentBuffer->next = NULL;
    replayManager->currentBuffer->frameCount = 0;

    memset(replayManager->inputCursors[0], 0, ReplayRecordingCapacity::InputStreamBytes);
    memset(replayManager->inputCursors[1], 0, ReplayRecordingCapacity::InputStreamBytes);
    memset(replayManager->inputCursors[2], 0, ReplayRecordingCapacity::InputStreamBytes);
    memset(replayManager->fpsCursor, 0, ReplayRecordingCapacity::FpsStreamBytes);
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
    i32 stage = g_GameManager.field0FC;
    // The original tests the frame-table ROW ADDRESS (0x20 + stage*0x28),
    // not the pointer stored in frameStart[0][stage]. Keep its observed
    // address test; changing to a data-pointer check alters game behavior.
    // Use the native 32-bit row address, not frameStart[stage]: the latter
    // subscripts the three-row C++ array out of bounds for stages 3..9.
    const unsigned int rowAddress =
        reinterpret_cast<unsigned int>(&replayManager->replayData->frameStart[0][0]) +
        static_cast<unsigned int>(stage) *
        static_cast<unsigned int>(sizeof(replayManager->replayData->frameStart[0]));
    if (rowAddress == 0u)
        return -1;
    if ((g_GameManager.IsGameMode0() || g_GameManager.IsGameMode1()) && stage > 0)
    {
        for (i32 i = 0; i < stage; i++)
            g_GameManager.clearD8[(*reinterpret_cast<ReplayFrameDataStartView **>(
                reinterpret_cast<unsigned int>(&replayManager->replayData->frameStart[1][0]) +
                static_cast<unsigned int>(i) * sizeof(replayManager->replayData->frameStart[1][0])))->value06] = 1;
    }
    // Original playback reads frame/FPS table pointers by native Win32
    // 32-bit byte offsets; the stage DWORD is not a bounded C++ subscript.
    // Preserve separate physical row bases and the original read order.
    ReplayFrameDataStartView *frame1 = (*reinterpret_cast<ReplayFrameDataStartView **>(
        reinterpret_cast<unsigned int>(&replayManager->replayData->frameStart[1][0]) +
        static_cast<unsigned int>(stage) * sizeof(replayManager->replayData->frameStart[1][0])));
    ReplayFrameDataStartView *frame0 = (*reinterpret_cast<ReplayFrameDataStartView **>(
        reinterpret_cast<unsigned int>(&replayManager->replayData->frameStart[0][0]) +
        static_cast<unsigned int>(stage) * sizeof(replayManager->replayData->frameStart[0][0])));
    ReplayFrameDataStartView *frame2 = (*reinterpret_cast<ReplayFrameDataStartView **>(
        reinterpret_cast<unsigned int>(&replayManager->replayData->frameStart[2][0]) +
        static_cast<unsigned int>(stage) * sizeof(replayManager->replayData->frameStart[2][0])));
    u8 *fpsStart = (*reinterpret_cast<u8 **>(
        reinterpret_cast<unsigned int>(&replayManager->replayData->fpsStart[0]) +
        static_cast<unsigned int>(stage) * sizeof(replayManager->replayData->fpsStart[0])));
    g_GameManager.sides[0].character20 = frame0->value06;
    g_GameManager.sides[1].character20 = frame1->value06;
    g_GameManager.sides[0].replaySelector28 = frame0->value07;
    g_GameManager.sides[1].replaySelector28 = frame1->value07;
    g_GameManager.sides[0].value2C = frame0->value0A;
    g_GameManager.sides[1].value2C = frame1->value0A;
    g_GameManager.valueF4 = frame0->value0C;
    g_GameManager.value138 = (signed char)frame0->value09;
    g_GameManager.difficulty = replayManager->replayData->value0D7;
    g_GameManager.value378 = replayManager->replayData->value1E6;
    g_GameManager.value37C = replayManager->replayData->value1E7;
    // The original loads header 1E8 into a byte-sized value between
    // publishing 1E7 and reading the side-0 runtime owner. Record that
    // source-level chronology explicitly; VC7.1 still chooses the same
    // nonexact register ordering, so the full-owner oracle remains required.
    const u8 side0Value1E8 = replayManager->replayData->value1E8;
    // Retain the original source-visible publication order: VC7.1 hoists
    // the ordinary pointer read across value1E7 when volatile is omitted.
    AsciiGameSideRuntimeView *volatile &side0RuntimeSlot =
        g_GameManager.sides[0].runtime;
    AsciiGameSideRuntimeView *firstSide0 = side0RuntimeSlot;
    g_GameManager.sides[0].value30 = side0Value1E8;
    g_GameManager.sides[1].value30 = replayManager->replayData->value1E9;
    firstSide0->value08 = frame0->value00;
    g_GameManager.sides[1].runtime->value08 = frame1->value00;
    g_GameManager.sides[0].runtime->value04 = frame0->value00;
    g_GameManager.sides[1].runtime->value04 = frame1->value00;
    g_GameManager.sides[0].runtime->value00 = (float)frame0->value08;
    g_GameManager.sides[1].runtime->value00 = (float)frame1->value08;
    g_GameManager.sides[0].runtime->phase10 = frame0->value10;
    // Saved frames contain a 0x20-byte wire header followed by input samples.
    // The samples are byte-addressed payloads, not another array element
    // of the C++ frame-header type; retain native Win32 address addition.
    replayManager->inputCursors[1] = reinterpret_cast<u16 *>(
        reinterpret_cast<unsigned int>(frame1) + sizeof(*frame1));
    replayManager->fpsCursor = fpsStart;
    replayManager->inputCursors[0] = reinterpret_cast<u16 *>(
        reinterpret_cast<unsigned int>(frame0) + sizeof(*frame0));
    replayManager->inputCursors[2] = reinterpret_cast<u16 *>(
        reinterpret_cast<unsigned int>(frame2) + sizeof(*frame2));
    *g_GameManager.configurationE8 = replayManager->replayData->configSnapshot;
    g_ReplayRng.SetSeed(frame0->stageSeed);
    replayManager->inputDelay = 0;
    return 0;
}

int ReplayManagerView::CaptureFrameSyncState(ReplayManagerView *replayManager)
{
    replayManager->frameEventFlags = 0;
    replayManager->frameRngSeed = g_ReplayRng.GetSeed();
    g_ReplayRng.ResetGenerationCount();

    if (g_GameManager.replayPauseRecorded130 != 0)
        replayManager->frameEventFlags |= 0x100;

    g_ReplayInputStates[0].historyPrevious = g_ReplayInputStates[0].historyCurrent;
    g_ReplayInputStates[0].historyCurrent = g_ReplayInputStates[0].currentInput;
    g_ReplayInputStates[1].historyPrevious = g_ReplayInputStates[1].historyCurrent;
    g_GameManager.replayPauseRecorded130 = 0;
    g_ReplayInputStates[1].historyCurrent = g_ReplayInputStates[1].currentInput;
    g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].historyPrevious =
        g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].historyCurrent;
    g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].historyCurrent =
        g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].currentInput;

    g_ReplayInputStates[0].Update();
    g_ReplayInputStates[1].Update();
    g_ReplayInputStates[ReplayNativeInputGateCapacity::SharedInputStateIndex].Update();

    int side = 0;
    GameConfiguration *inputGate = g_GameManager.configurationE8;
    for (; side < ReplayNativeInputGateCapacity::SideCount; side++)
    {
        if (inputGate != NULL && inputGate->sideFocusMode[side] == 1)
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

    for (int stage = 0; stage < ReplayRecordingCapacity::StageCount; stage++)
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
            for (int stream = 0; stream < ReplayRecordingCapacity::InputStreamCount; stream++)
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
    operator delete(this);
}


void __fastcall ReleaseStageObject(void *object)
{
    ReplayManagerView *replayManager = static_cast<ReplayManagerView *>(object);
    if (replayManager != NULL)
    {
        ReleaseReplayManagerCore(replayManager);
        g_Chain.Cut(replayManager->mainChain);
        operator delete(replayManager);
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

    enum ReplaySaveWireLengths
    {
        ChecksumHeaderBytes = sizeof(ReplayHeaderView) -
                              offsetof(ReplayHeaderView, obfuscationKey),
        ObfuscatedHeaderBytes = sizeof(ReplayHeaderView) -
                                offsetof(ReplayHeaderView, compressedSize),
        UserInfoWithNulBytes = sizeof(ReplayUserDataHeaderView) + 1,
    };
    // The original serialized replay and ten-stage source buffer are
    // addressed through live native 32-bit frame/FPS/record byte offsets.
    // Keep distinct stream and FPS table reads, and do not add bounds gates.
    ReplayDataView replayCopy;
    // Retail reserves serialized replay bytes with native modulo-32 arithmetic.
    unsigned int allocationSize = (sizeof(ReplayDataView) + ReplayRecordingCapacity::InitialPayloadBytes);
    int loopIndex;
    {
        loopIndex = 0;
        // Walk the inline stage roots separately from each immutable source
        // link chain. Saving reads their recorded data but never mutates it.
        const ReplayBufferLink *stageRoot = &replayManager->stageBuffers[0];
        for (; loopIndex < ReplayRecordingCapacity::StageCount; loopIndex++, stageRoot++)
        {
            if ((*reinterpret_cast<ReplayFrameDataStartView **>(
                reinterpret_cast<unsigned int>(&replayCopy.frameStart[0][0]) +
                static_cast<unsigned int>(loopIndex) * sizeof(replayCopy.frameStart[0][0]))) != NULL)
            {
                allocationSize += ReplayRecordingCapacity::StageHeadersBytes;
                for (const ReplayBufferLink *link = stageRoot; link != NULL; link = link->next)
                    allocationSize += ReplayRecordingCapacity::LinkBytes;
            }
        }
    }

    u8 *tempBuffer = (u8 *)g_ZunMemory.Alloc(allocationSize, "rep tmp");
    replayCopy = *replayManager->replayData;
    // This offset is a serialized byte count, not a C++ array index.
    unsigned int currentOffset = sizeof(ReplayDataView);

    for (int stream = 0; stream < ReplayRecordingCapacity::InputStreamCount; stream++)
    {
        for (loopIndex = 0; loopIndex < ReplayRecordingCapacity::StageCount; loopIndex++)
        {
            const ReplayFrameDataStartView *frameHeader = (*reinterpret_cast<ReplayFrameDataStartView **>(
                reinterpret_cast<unsigned int>(&replayCopy.frameStart[0][0]) +
                (static_cast<unsigned int>(stream) * ReplayRecordingCapacity::StageCount +
                 static_cast<unsigned int>(loopIndex)) * sizeof(replayCopy.frameStart[0][0])));
            if (frameHeader != NULL)
            {
                const ReplayBufferLink *link = reinterpret_cast<const ReplayBufferLink *>(
                reinterpret_cast<unsigned int>(&replayManager->stageBuffers[0]) +
                static_cast<unsigned int>(loopIndex) * sizeof(replayManager->stageBuffers[0]));
                // A packed byte address can cross the header and its payload.
                memcpy(
                    reinterpret_cast<unsigned char *>(
                        reinterpret_cast<unsigned int>(tempBuffer) +
                        static_cast<unsigned int>(currentOffset) -
                        sizeof(ReplayHeaderView)),
                    frameHeader, sizeof(*frameHeader));
                (*reinterpret_cast<unsigned int *>(
                reinterpret_cast<unsigned int>(&replayCopy.frameOffset[0][0]) +
                (static_cast<unsigned int>(stream) * ReplayRecordingCapacity::StageCount +
                 static_cast<unsigned int>(loopIndex)) * sizeof(replayCopy.frameOffset[0][0]))) = currentOffset;
                currentOffset += sizeof(*frameHeader);
                for (; link != NULL; link = link->next)
                {
                    unsigned int streamSize =
                        static_cast<unsigned int>(ReplayRecordingCapacity::InputFrameBytes) *
                        static_cast<unsigned int>(link->frameCount);
                    memcpy(
                        reinterpret_cast<unsigned char *>(
                            reinterpret_cast<unsigned int>(tempBuffer) +
                            static_cast<unsigned int>(currentOffset) -
                            sizeof(ReplayHeaderView)),
                        link->input[stream], streamSize);
                    currentOffset += streamSize;
                }
            }
        }
    }

    // Each stage owns a distinct FPS chain; link contents remain live reads.
    for (loopIndex = 0; loopIndex < ReplayRecordingCapacity::StageCount; loopIndex++)
    {
        if ((*reinterpret_cast<u8 **>(
                reinterpret_cast<unsigned int>(&replayCopy.fpsStart[0]) +
                static_cast<unsigned int>(loopIndex) * sizeof(replayCopy.fpsStart[0]))) != NULL)
        {
            const ReplayBufferLink *link = reinterpret_cast<const ReplayBufferLink *>(
                reinterpret_cast<unsigned int>(&replayManager->stageBuffers[0]) +
                static_cast<unsigned int>(loopIndex) * sizeof(replayManager->stageBuffers[0]));
            (*reinterpret_cast<unsigned int *>(
                reinterpret_cast<unsigned int>(&replayCopy.fpsOffset[0]) +
                static_cast<unsigned int>(loopIndex) * sizeof(replayCopy.fpsOffset[0]))) = currentOffset;
            for (; link != NULL; link = link->next)
            {
                int fpsSize = link->frameCount / ReplayRecordingCapacity::FramesPerFpsSample;
                memcpy(
                    reinterpret_cast<unsigned char *>(
                        reinterpret_cast<unsigned int>(tempBuffer) +
                        static_cast<unsigned int>(currentOffset) -
                        sizeof(ReplayHeaderView)),
                    link->fps, fpsSize);
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
    infoCursor = AppendReplayFormat(infoCursor, g_ReplayInfoDifficultyFormat, g_ReplayDifficultyNames[g_GameManager.difficulty]);
    int mode = g_GameManager.gameMode;
    if (mode == 2)
        // Retail adds to a native DWORD before indexing the replay mode names.
        mode = static_cast<int>(
            static_cast<unsigned int>(g_GameManager.replayMode114) + 2u);
    infoCursor = AppendReplayFormat(infoCursor, g_ReplayInfoModeFormat, g_ReplayModeNames[mode]);
    if (!g_GameManager.IsGameMode2())
    {
        infoCursor = AppendReplayFormat(infoCursor, g_ReplayInfoCharacterFormat,
            g_ReplayCharacterNames[g_GameManager.sides[0].character20]);
    }
    else
    {
        infoCursor = AppendReplayFormat(infoCursor, g_ReplayInfoVersusCharactersFormat,
            g_ReplayCharacterNames[g_GameManager.sides[0].character20],
            g_ReplayCharacterNames[g_GameManager.sides[1].character20]);
    }
    infoCursor = AppendReplayFormat(infoCursor, g_ReplayInfoInitialHealthFormat,
        g_GameManager.value378 * 0.5f, g_GameManager.value37C * 0.5f);
    infoCursor = AppendReplayFormat(infoCursor, g_ReplayInfoVersionFormat, g_ReplayVersionText);
    // The resulting wire header length is a signed 32-bit field; the retail
    // ADD itself wraps as a native DWORD before its value is stored.
    infoHeader.size = static_cast<int>(
        static_cast<unsigned int>(strlen(infoBuffer)) +
        static_cast<unsigned int>(UserInfoWithNulBytes));
    // Padding the user info to an even length is another native DWORD ADD.
    infoHeader.size = static_cast<int>(
        static_cast<unsigned int>(infoHeader.size) +
        (static_cast<unsigned int>(infoHeader.size) & 1u));

    replayCopy.value07 = 1;
    strcpy(replayCopy.playerName, replayName);
    strcpy(replayCopy.playTime, g_ReplayPlayTimeText);
    replayCopy.obfuscationKey = (u8)(g_ReplayRng.GetRandomU16InRange(0x80) + 0x40);
    replayCopy.value1D0 = ReplayRecordingCapacity::FramesPerFpsSample;
    replayCopy.randomPayloadByte = (u8)g_ReplayRng.GetRandomU16InRange(0x100);
    replayCopy.unknown14 = (u8)g_ReplayRng.GetRandomU16InRange(0x100);
    // The initial payload extends beyond the one-byte offset anchor.
    memcpy(tempBuffer,
        reinterpret_cast<const u8 *>(
            reinterpret_cast<unsigned int>(&replayCopy) +
            static_cast<unsigned int>(offsetof(ReplayDataView, randomPayloadByte))),
        sizeof(ReplayPayloadView));

    replayCopy.decompressedSize = currentOffset - sizeof(ReplayHeaderView);
    u8 *compressedData = Lzss::Encode(tempBuffer, replayCopy.decompressedSize, &replayCopy.compressedSize);
    g_ZunMemory.Free(tempBuffer);
    int compressedSize = replayCopy.compressedSize;

    u32 checksum = 0x3F000318;
    // Header checksums begin inside a scalar member and traverse neighboring
    // wire fields; advancing a native address word preserves that layout.
    unsigned int checksumAddress =
        reinterpret_cast<unsigned int>(&replayCopy.obfuscationKey);
    for (loopIndex = 0; loopIndex < ChecksumHeaderBytes; loopIndex++, checksumAddress++)
        checksum += *reinterpret_cast<const u8 *>(checksumAddress);
    for (loopIndex = 0; loopIndex < compressedSize; loopIndex++)
        checksum += compressedData[loopIndex];
    replayCopy.checksum = checksum;

    u8 obfuscationOffset = replayCopy.obfuscationKey;
    // The obfuscated header and compressed payload are counted byte streams.
    unsigned int obfuscationAddress =
        reinterpret_cast<unsigned int>(&replayCopy.compressedSize);
    for (loopIndex = 0; loopIndex < ObfuscatedHeaderBytes; loopIndex++, ++obfuscationAddress)
    {
        *reinterpret_cast<u8 *>(obfuscationAddress) += obfuscationOffset;
        obfuscationOffset += 7;
    }
    obfuscationAddress = reinterpret_cast<unsigned int>(compressedData);
    for (loopIndex = 0; loopIndex < compressedSize; loopIndex++, ++obfuscationAddress)
    {
        *reinterpret_cast<u8 *>(obfuscationAddress) += obfuscationOffset;
        obfuscationOffset += 7;
    }
    replayCopy.fileSize = compressedSize + sizeof(ReplayHeaderView);

    {
        DWORD bytesWritten = 0;
        HANDLE file = CreateFileA(replayPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file != INVALID_HANDLE_VALUE)
        {
            WriteFile(file, &replayCopy, sizeof(ReplayHeaderView), &bytesWritten, NULL);
            WriteFile(file, compressedData, compressedSize, &bytesWritten, NULL);
            WriteFile(file, &infoHeader, sizeof(infoHeader), &bytesWritten, NULL);
            WriteFile(file, infoBuffer, infoHeader.size - sizeof(infoHeader), &bytesWritten, NULL);
            CloseHandle(file);
            GlobalFree(compressedData);
        }
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
