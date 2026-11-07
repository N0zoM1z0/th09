#include "SupervisorFrameQueueStorageView.hpp"
#include "Supervisor.hpp"
#include "ReplayInputState.hpp"
#include <stddef.h>

// PopFrame reads the first word of the actual 0x8E-byte Replay input
// state. The original global array and stride are shared with Player,
// Replay playback and GameManager. This is not another private input type.
typedef char SupervisorNativeFrameInputState[
    (sizeof(ReplayInputState) == 0x8E &&
     offsetof(ReplayInputState, currentInput) == 0x00) ? 1 : -1];

void Supervisor::InsertPredictedFrame(
    int side, int frame, short seed)
{
    Supervisor *view = static_cast<Supervisor *>(this);

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

unsigned short Supervisor::PopFrame(
    int side, unsigned short *seed, unsigned int *predicted)
{
    unsigned short input;
    if (static_cast<Supervisor *>(this)->entries47C[side * 10].predicted08 == 0)
        input = static_cast<Supervisor *>(this)->entries47C[side * 10].packedInput00;
    else
        input = g_ReplayInputStates[side].currentInput;

    *seed = static_cast<Supervisor *>(this)->entries47C[side * 10].seed02;
    *predicted = static_cast<Supervisor *>(this)->entries47C[side * 10].predicted08;

    SupervisorFrameEntryOpsView *entry =
        &static_cast<Supervisor *>(this)->entries47C[side * 10];
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
