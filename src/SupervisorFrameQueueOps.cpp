#include "SupervisorFrameQueueView.hpp"
#include <stddef.h>

struct SupervisorFrameEntryOpsView
{
    unsigned short packedInput00;
    unsigned short seed02;
    int frame04;
    unsigned int predicted08;
};

typedef char SupervisorFrameEntryOpsSizeIs0C[
    (sizeof(SupervisorFrameEntryOpsView) == 0x0C) ? 1 : -1];

struct SupervisorFrameQueueOpsLayout
{
    unsigned char unknown000[0x47C];
    SupervisorFrameEntryOpsView entries47C[20];
    SupervisorFrameEntryOpsView lastReceived56C[2];
};

typedef char SupervisorFrameOpsEntriesAt47C[
    (offsetof(SupervisorFrameQueueOpsLayout, entries47C) == 0x47C) ? 1 : -1];
typedef char SupervisorFrameOpsLastAt56C[
    (offsetof(SupervisorFrameQueueOpsLayout, lastReceived56C) == 0x56C) ? 1 : -1];

struct SupervisorReplayInputHeadOpsView
{
    unsigned short currentInput00;
    unsigned char unknown02[0x8E - 2];
};

extern SupervisorReplayInputHeadOpsView g_ReplayInputStates[3];

void SupervisorFrameQueueView::InsertPredictedFrame(
    int side, int frame, short seed)
{
    SupervisorFrameQueueOpsLayout *view =
        reinterpret_cast<SupervisorFrameQueueOpsLayout *>(this);

    unsigned char *scanCursor =
        reinterpret_cast<unsigned char *>(this) + side * 0x78;
    int index = 0;
    unsigned char *sideBase = scanCursor;
    scanCursor += 0x480;

    for (; index < 10; ++index, scanCursor += 0x0C)
    {
        int queuedFrame = *reinterpret_cast<int *>(scanCursor);
        if (queuedFrame == 0 || queuedFrame > frame)
            break;
    }

    int flatIndex = side * 10 + index;
    int *targetFrame = &view->entries47C[flatIndex].frame04;

    if (*targetFrame != 0 && index < 9)
    {
        SupervisorFrameEntryOpsView *dst =
            reinterpret_cast<SupervisorFrameEntryOpsView *>(sideBase + 0x4E8);
        int count = 9 - index;
        do
        {
            dst->packedInput00 = (dst - 1)->packedInput00;
            dst->seed02 = (dst - 1)->seed02;
            dst->frame04 = (dst - 1)->frame04;
            dst->predicted08 = (dst - 1)->predicted08;
            --dst;
            --count;
        }
        while (count != 0);
    }

    unsigned char *entryCursor =
        reinterpret_cast<unsigned char *>(this) +
        flatIndex * sizeof(SupervisorFrameEntryOpsView);
    unsigned short packedInput =
        view->lastReceived56C[side].packedInput00;
    *targetFrame = frame;
    *reinterpret_cast<unsigned short *>(entryCursor + 0x47C) = packedInput;
    *reinterpret_cast<unsigned short *>(entryCursor + 0x47E) = seed;
    *reinterpret_cast<unsigned int *>(entryCursor + 0x484) = 1;
}

unsigned short SupervisorFrameQueueView::PopFrame(
    int side, unsigned short *seed, unsigned int *predicted)
{
    unsigned short input;
    if (*reinterpret_cast<unsigned int *>(
            reinterpret_cast<unsigned char *>(this) + side * 0x78 + 0x484) == 0)
        input = *reinterpret_cast<unsigned short *>(
            reinterpret_cast<unsigned char *>(this) + side * 0x78 + 0x47C);
    else
        input = g_ReplayInputStates[side].currentInput00;

    *seed = *reinterpret_cast<unsigned short *>(
        reinterpret_cast<unsigned char *>(this) + side * 0x78 + 0x47E);
    *predicted = *reinterpret_cast<unsigned int *>(
        reinterpret_cast<unsigned char *>(this) + side * 0x78 + 0x484);

    SupervisorFrameEntryOpsView *entry =
        reinterpret_cast<SupervisorFrameEntryOpsView *>(
            reinterpret_cast<unsigned char *>(this) + side * 0x78 + 0x47C);
    int count = 9;
    do
    {
        entry->packedInput00 = (entry + 1)->packedInput00;
        entry->seed02 = (entry + 1)->seed02;
        entry->frame04 = (entry + 1)->frame04;
        entry->predicted08 = (entry + 1)->predicted08;
        ++entry;
        --count;
    }
    while (count != 0);

    return input;
}
