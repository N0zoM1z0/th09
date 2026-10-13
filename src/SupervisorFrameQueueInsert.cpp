#include "SupervisorFrameQueueView.hpp"
#include "SupervisorFrameQueueStorageView.hpp"
#include <stddef.h>

void SupervisorFrameQueueView::InsertReceivedFrame(
    int side, int frame, int packedInput, short seed)
{
    SupervisorFrameQueueInsertLayout *view =
        reinterpret_cast<SupervisorFrameQueueInsertLayout *>(this);

    view->lastReceived56C[side].frame04 = frame;
    view->lastReceived56C[side].packedInput00 = (short)packedInput;
    view->lastReceived56C[side].seed02 = seed;

    unsigned char *sideBase =
        reinterpret_cast<unsigned char *>(view) + side * 0x78;
    // The two 10-record side queues are actual elements of the same
    // 20-entry array. Keep the scan rooted at that array rather than
    // constructing a pointer from an unrelated byte-offset view.
    SupervisorFrameEntryView *scan = &view->entries47C[side * 10];

    int index;
    for (index = 0; index < 10; ++index, ++scan)
    {
        if (scan->frame04 == 0 || scan->frame04 > frame)
            break;
    }

    int flatIndex = side * 10 + index;
    int *targetFrame = reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(view) +
        offsetof(SupervisorFrameQueueInsertLayout, entries47C) +
        flatIndex * sizeof(SupervisorFrameEntryView) +
        offsetof(SupervisorFrameEntryView, frame04));

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

    char *result =
        reinterpret_cast<char *>(view) +
        flatIndex * sizeof(SupervisorFrameEntryView);

    *reinterpret_cast<short *>(result + 0x47C) = (short)packedInput;
    *targetFrame = frame;
    *reinterpret_cast<short *>(result + 0x47E) = seed;
    *reinterpret_cast<unsigned int *>(result + 0x484) = 0;

}
