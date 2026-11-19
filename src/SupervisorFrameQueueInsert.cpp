// SPDX-License-Identifier: GPL-3.0-only
// Adapted from Cirno-Labs/th09 src/app.cpp, App::AddRecord,
// commit d7653152becabb8c86f2f43d49015ecc11bd0652.
// Original MIT portions retain their grant in LICENSE; see THIRD_PARTY_NOTICES.md.
#include "SupervisorFrameQueueStorageView.hpp"
#include "Supervisor.hpp"

// Both the incoming record and the 20-entry queue belong to the native
// Supervisor object. Preserve the original unsigned flat side/slot index
// calculation and all repeated field reads, with no new range check.

static __forceinline SupervisorFrameEntryView &NativeReceivedFrame(Supervisor *view, int side)
{
    return view->lastReceived56C[side];
}

static __forceinline SupervisorFrameEntryView &NativeQueueFrame(Supervisor *view, int side, int index)
{
    return view->entries47C[static_cast<unsigned int>(side) * 10u +
                            static_cast<unsigned int>(index)];
}


void Supervisor::InsertReceivedFrame(
    int side, int frame, unsigned short packedInput, short seed)
{
    int i;
    int j;
    Supervisor *view = static_cast<Supervisor *>(this);
    NativeReceivedFrame(view, side).frame04 = frame;
    NativeReceivedFrame(view, side).packedInput00 = (short)packedInput;
    NativeReceivedFrame(view, side).seed02 = seed;
    for (i = 0; i < 10; i++)
    {
        if (NativeQueueFrame(view, side, i).frame04 == 0 ||
            NativeQueueFrame(view, side, i).frame04 > frame)
            break;
    }
    if (NativeQueueFrame(view, side, i).frame04 != 0 && i < 9)
    {
        for (j = 9; j > i; j--)
        {
            NativeQueueFrame(view, side, j).packedInput00 = NativeQueueFrame(view, side, j - 1).packedInput00;
            NativeQueueFrame(view, side, j).seed02 = NativeQueueFrame(view, side, j - 1).seed02;
            NativeQueueFrame(view, side, j).frame04 = NativeQueueFrame(view, side, j - 1).frame04;
            NativeQueueFrame(view, side, j).value08 = NativeQueueFrame(view, side, j - 1).value08;
        }
    }
    NativeQueueFrame(view, side, i).packedInput00 = (short)packedInput;
    NativeQueueFrame(view, side, i).frame04 = frame;
    NativeQueueFrame(view, side, i).seed02 = seed;
    NativeQueueFrame(view, side, i).value08 = 0;
}
