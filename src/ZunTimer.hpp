#pragma once

struct ZunTimer
{
    int previous;
    float subFrame;
    int current;

    void operator=(int value);
};

typedef char ZunTimerSizeIs0C[(sizeof(ZunTimer) == 0x0C) ? 1 : -1];
