#include "SupervisorFrameQueueView.hpp"
#include <stddef.h>

struct SupervisorFrameEntryView
{
    short packedInput00;
    short seed02;
    int frame04;
    int value08;
};

typedef char SupervisorFrameEntrySizeIs0C[
    (sizeof(SupervisorFrameEntryView) == 0x0C) ? 1 : -1];

struct SupervisorFrameQueueInsertLayout
{
    unsigned char unknown000[0x47C];
    SupervisorFrameEntryView entries47C[20];
    SupervisorFrameEntryView lastReceived56C[2];
};

typedef char SupervisorInsertEntriesAt47C[
    (offsetof(SupervisorFrameQueueInsertLayout, entries47C) == 0x47C) ? 1 : -1];
typedef char SupervisorInsertLastReceivedAt56C[
    (offsetof(SupervisorFrameQueueInsertLayout, lastReceived56C) == 0x56C) ? 1 : -1];

void SupervisorFrameQueueView::InsertReceivedFrame(
    int side, int frame, int packedInput, short seed)
{
    SupervisorFrameQueueInsertLayout *view =
        reinterpret_cast<SupervisorFrameQueueInsertLayout *>(this);

    view->lastReceived56C[side].frame04 = frame;
    view->lastReceived56C[side].packedInput00 = (short)packedInput;
    view->lastReceived56C[side].seed02 = seed;

    unsigned char *sideBase =
        reinterpret_cast<unsigned char *>(this) + side * 0x78;
    SupervisorFrameEntryView *scan =
        reinterpret_cast<SupervisorFrameEntryView *>(sideBase + 0x47C);

    int index;
    for (index = 0; index < 10; ++index, ++scan)
    {
        if (scan->frame04 == 0 || scan->frame04 > frame)
            break;
    }

    int flatIndex = side * 10 + index;
    int *targetFrame = &view->entries47C[flatIndex].frame04;

    if (*targetFrame != 0 && index < 9)
    {
        SupervisorFrameEntryView *dst =
            reinterpret_cast<SupervisorFrameEntryView *>(sideBase + 0x4E8);
        int count = 9 - index;
        do
        {
            dst->packedInput00 = (dst - 1)->packedInput00;
            dst->seed02 = (dst - 1)->seed02;
            dst->frame04 = (dst - 1)->frame04;
            dst->value08 = (dst - 1)->value08;
            --dst;
            --count;
        }
        while (count != 0);
    }

    view->entries47C[flatIndex].packedInput00 = (short)packedInput;
    *targetFrame = frame;
    view->entries47C[flatIndex].seed02 = seed;
    view->entries47C[flatIndex].value08 = 0;
}
