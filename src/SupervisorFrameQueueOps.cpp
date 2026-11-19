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

// The Supervisor owns twenty actual 0x0C-byte queue elements and two
// last-received records. No duplicate queue representation or range gate.
static __forceinline SupervisorFrameEntryOpsView &NativeFrameQueueEntry(
    Supervisor *owner, unsigned int index)
{
    return owner->entries47C[index];
}

static __forceinline SupervisorFrameEntryOpsView &NativeLastReceivedFrame(
    Supervisor *owner, int side)
{
    return owner->lastReceived56C[side];
}

static __forceinline ReplayInputState &NativeNetworkReplayInput(int side)
{
    return g_ReplayInputStates[side];
}

// Queue shifts walk the real array one 0x0C-byte record at a time.
// Preserve each independent field copy and its original iteration order.
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
            dst->packedInput00 = (dst - 1)->packedInput00;
            dst->seed02 = (dst - 1)->seed02;
            dst->frame04 = (dst - 1)->frame04;
            dst->predicted08 = (dst - 1)->predicted08;
            dst = (dst - 1);
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
        entry->packedInput00 = (entry + 1)->packedInput00;
        entry->seed02 = (entry + 1)->seed02;
        entry->frame04 = (entry + 1)->frame04;
        entry->predicted08 = (entry + 1)->predicted08;
        entry = (entry + 1);
        --count;
    }
    while (count != 0);

    return input;
}
