#pragma once

struct ZunTimer
{
    int previous;
    float subFrame;
    int current;

    void SetCurrent(int value);
    int Tick();
    operator float();
    void operator++(int);
    void operator=(int value);
};

typedef char ZunTimerSizeIs0C[(sizeof(ZunTimer) == 0x0C) ? 1 : -1];
