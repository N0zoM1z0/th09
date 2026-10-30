#pragma once

#include <stddef.h>

// The message container is a file-backed, mutable Win32 i386 wire layout.
// The same four-byte entry slot contains an on-disk relative offset until
// FrontMessageRuntimeView::LoadForSideCount converts it to an instruction
// pointer. ReleaseForSideCount consumes only already-converted entries.
struct FrontMessageInstructionView
{
    short time00;
    unsigned char opcode02;
    unsigned char size03;
    int argument04;
};
typedef char FrontMessageInstructionSizeIs08[
    (sizeof(FrontMessageInstructionView) == 0x08) ? 1 : -1];

struct FrontMessageTableEntryView
{
    union
    {
        unsigned int offset00;
        FrontMessageInstructionView *instructions00;
    };
    // Weighted random message selection consumes this signed weight.
    int weight04;
};
typedef char FrontMessageTableEntrySizeIs08[
    (sizeof(FrontMessageTableEntryView) == 0x08) ? 1 : -1];
typedef char FrontMessageTableEntryOffsetAt00[
    (offsetof(FrontMessageTableEntryView, offset00) == 0) ? 1 : -1];
typedef char FrontMessageTableEntryWeightAt04[
    (offsetof(FrontMessageTableEntryView, weight04) == 0x04) ? 1 : -1];

struct FrontMessageTableView
{
    int count00;
    FrontMessageTableEntryView entries04[1];
};
typedef char FrontMessageTableEntriesAt04[
    (offsetof(FrontMessageTableView, entries04) == 0x04) ? 1 : -1];

struct FrontMessageSourceView
{
    unsigned char unknown00[0x0C];
    short musicId0C;
};
typedef char FrontMessageSourceMusicAt0C[
    (offsetof(FrontMessageSourceView, musicId0C) == 0x0C) ? 1 : -1];

struct FrontMessageRuntimeResetView
{
    unsigned char storage[0x1D70];
    int Reset();
};
typedef char FrontMessageRuntimeResetSizeIs1D70[
    (sizeof(FrontMessageRuntimeResetView) == 0x1D70) ? 1 : -1];
