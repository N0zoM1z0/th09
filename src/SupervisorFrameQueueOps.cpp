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

// The Supervisor owns twenty 0x0C-byte queue entries and two 0x0C
// last-received records. Side selectors are native i386 DWORD indexes.
static __forceinline SupervisorFrameEntryOpsView &NativeFrameQueueEntry(
    Supervisor *owner, unsigned int index)
{
    return *reinterpret_cast<SupervisorFrameEntryOpsView *>(
        reinterpret_cast<unsigned int>(&owner->entries47C[0]) +
        static_cast<unsigned int>(index) * sizeof(owner->entries47C[0]));
}

static __forceinline SupervisorFrameEntryOpsView &NativeLastReceivedFrame(
    Supervisor *owner, int side)
{
    return *reinterpret_cast<SupervisorFrameEntryOpsView *>(
        reinterpret_cast<unsigned int>(&owner->lastReceived56C[0]) +
        static_cast<unsigned int>(side) * sizeof(owner->lastReceived56C[0]));
}

static __forceinline ReplayInputState &NativeNetworkReplayInput(int side)
{
    return *reinterpret_cast<ReplayInputState *>(
        reinterpret_cast<unsigned int>(&g_ReplayInputStates[0]) +
        static_cast<unsigned int>(side) * sizeof(g_ReplayInputStates[0]));
}

// The queue's real storage is the Supervisor-owned 20x0x0C record array.
// These lexical walks preserve native modulo-32 Win32 byte addresses,
// including the two side-local shift windows without C++ pseudo-arrays.
#define TH09_FRAME_QUEUE_PREV(ptr) \
    reinterpret_cast<SupervisorFrameEntryOpsView *>( \
        reinterpret_cast<unsigned int>(ptr) - sizeof(*(ptr)))
#define TH09_FRAME_QUEUE_NEXT(ptr) \
    reinterpret_cast<SupervisorFrameEntryOpsView *>( \
        reinterpret_cast<unsigned int>(ptr) + sizeof(*(ptr)))

void Supervisor::InsertPredictedFrame(
    int side, int frame, short seed)
{
    Supervisor *view = static_cast<Supervisor *>(this);

    unsigned char *scanCursor =
        reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(this) + static_cast<unsigned int>(side) * 0x78u);
    int index = 0;
    unsigned char *sideBase = scanCursor;
    scanCursor = reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(scanCursor) + 0x480u);

    for (; index < 10; ++index, scanCursor = reinterpret_cast<unsigned char *>(
             reinterpret_cast<unsigned int>(scanCursor) + 0x0Cu))
    {
        int queuedFrame = *reinterpret_cast<int *>(scanCursor);
        if (queuedFrame == 0 || queuedFrame > frame)
            break;
    }

    int flatIndex = static_cast<int>(
        static_cast<unsigned int>(side) * 10u + static_cast<unsigned int>(index));
    int *targetFrame = &NativeFrameQueueEntry(view, static_cast<unsigned int>(flatIndex)).frame04;

    if (*targetFrame != 0 && index < 9)
    {
        SupervisorFrameEntryOpsView *dst =
            reinterpret_cast<SupervisorFrameEntryOpsView *>(
                reinterpret_cast<unsigned int>(sideBase) + 0x4E8u);
        int count = 9 - index;
        do
        {
            dst->packedInput00 = TH09_FRAME_QUEUE_PREV(dst)->packedInput00;
            dst->seed02 = TH09_FRAME_QUEUE_PREV(dst)->seed02;
            dst->frame04 = TH09_FRAME_QUEUE_PREV(dst)->frame04;
            dst->predicted08 = TH09_FRAME_QUEUE_PREV(dst)->predicted08;
            dst = TH09_FRAME_QUEUE_PREV(dst);
            --count;
        }
        while (count != 0);
    }

    unsigned char *entryCursor = reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(this) +
        static_cast<unsigned int>(flatIndex) * sizeof(SupervisorFrameEntryOpsView));
    unsigned short packedInput =
        NativeLastReceivedFrame(view, side).packedInput00;
    *targetFrame = frame;
    *reinterpret_cast<unsigned short *>(entryCursor + 0x47C) = packedInput;
    *reinterpret_cast<unsigned short *>(entryCursor + 0x47E) = seed;
    *reinterpret_cast<unsigned int *>(entryCursor + 0x484) = 1;
}

unsigned short Supervisor::PopFrame(
    int side, unsigned short *seed, unsigned int *predicted)
{
    unsigned short input;
    if (NativeFrameQueueEntry(this, static_cast<unsigned int>(side) * 10u).predicted08 == 0)
        input = NativeFrameQueueEntry(this, static_cast<unsigned int>(side) * 10u).packedInput00;
    else
        input = NativeNetworkReplayInput(side).currentInput;

    *seed = NativeFrameQueueEntry(this, static_cast<unsigned int>(side) * 10u).seed02;
    *predicted = NativeFrameQueueEntry(this, static_cast<unsigned int>(side) * 10u).predicted08;

    SupervisorFrameEntryOpsView *entry =
        &NativeFrameQueueEntry(this, static_cast<unsigned int>(side) * 10u);
    int count = 9;
    do
    {
        entry->packedInput00 = TH09_FRAME_QUEUE_NEXT(entry)->packedInput00;
        entry->seed02 = TH09_FRAME_QUEUE_NEXT(entry)->seed02;
        entry->frame04 = TH09_FRAME_QUEUE_NEXT(entry)->frame04;
        entry->predicted08 = TH09_FRAME_QUEUE_NEXT(entry)->predicted08;
        entry = TH09_FRAME_QUEUE_NEXT(entry);
        --count;
    }
    while (count != 0);

    return input;
}

#undef TH09_FRAME_QUEUE_PREV
#undef TH09_FRAME_QUEUE_NEXT
