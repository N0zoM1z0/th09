#pragma once

struct SupervisorFrameQueueView
{
    void InsertReceivedFrame(int side, int frame, int packedInput, short seed);
    void InsertPredictedFrame(int side, int frame, short seed);
    unsigned short PopFrame(int side, unsigned short *seed, unsigned int *predicted);
    int AreFrameQueuesSynchronized(int side);
};
