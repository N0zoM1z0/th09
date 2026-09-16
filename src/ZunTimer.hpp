#pragma once

struct ZunTimer
{
    int previous;
    float subFrame;
    int current;

    ZunTimer();
    void Initialize();
    void SetCurrent(int value);
    int Tick();
    operator float();
    void operator++(int);
    void operator--(int);
    int operator%(int value);
    void operator+=(int value);
    void operator+=(float value);
    void operator-=(float value);
    void Increment(int value);
    void Increment(float value);
    void Decrement(int value);
    void Decrement(float value);
    unsigned int operator>=(int value);
    unsigned int operator<(int value);
    unsigned int operator<=(int value);
    int operator==(int value);
    unsigned int operator>(int value);
    void operator=(int value);
};

typedef char ZunTimerSizeIs0C[(sizeof(ZunTimer) == 0x0C) ? 1 : -1];
