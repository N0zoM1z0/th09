#pragma once

#include "Supervisor.hpp"
#include <stddef.h>

// The original queue methods use the 0x7B0-byte Supervisor allocation.
// Retain their original C++ decorated receiver name without a second
// zero-storage lookalike; this source role adds no data or virtual methods.
struct SupervisorFrameQueueView : Supervisor
{
    void InsertReceivedFrame(int side, int frame, int packedInput, short seed);
    void InsertPredictedFrame(int side, int frame, short seed);
    unsigned short PopFrame(int side, unsigned short *seed, unsigned int *predicted);
    int AreFrameQueuesSynchronized(int side);
};
typedef char SupervisorQueueNativeReceiverLayout[
    (sizeof(SupervisorFrameQueueView) == sizeof(Supervisor) &&
     offsetof(SupervisorFrameQueueView, entries47C) == 0x47C &&
     offsetof(SupervisorFrameQueueView, lastReceived56C) == 0x56C) ? 1 : -1];
