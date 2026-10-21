#include "ZunTimer.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

typedef char ZunTimerCurrentAt08[
    (offsetof(ZunTimer, current) == 0x08) ? 1 : -1];

void ZunTimer::Initialize()
{
    this->current = 0;
    this->previous = -999999;
    this->subFrame = 0.0f;
}

void ZunTimer::SetCurrent(int value)
{
    this->current = value;
    this->subFrame = (float)value;
    this->previous = -999999;
}

void ZunTimer::operator=(int value)
{
    this->SetCurrent(value);
}

int ZunTimer::Tick()
{
    this->previous = this->current;
    g_Supervisor.TickTimer(&this->current, &this->subFrame);
    return this->current;
}

// Byte-compatible with the existing shared getter at 0x00435F00.
ZunTimer::operator int()
{
    return this->current;
}

ZunTimer::operator float()
{
    return this->subFrame;
}

void ZunTimer::operator++(int)
{
    this->Tick();
}

unsigned int ZunTimer::operator<=(int value)
{
    return this->current <= value;
}


void ZunTimer::Increment(float value)
{
    this->previous = this->current;
    if (g_Supervisor.framerateMultiplier5B8 > 0.99f)
        this->subFrame += value;
    else
        this->subFrame += g_Supervisor.framerateMultiplier5B8 * value;

    this->current = (int)this->subFrame;
    if ((g_Supervisor.flags5D4 & 0x20) != 0)
    {
        this->previous = -999999;
        this->subFrame += (float)this->current;
    }
}

void ZunTimer::Increment(int value)
{
    this->Increment((float)value);
}

void ZunTimer::Decrement(float value)
{
    this->Increment(-value);
}

void ZunTimer::Decrement(int value)
{
    this->Increment(-value);
}

void ZunTimer::operator+=(int value)
{
    this->Increment(value);
}

void ZunTimer::operator+=(float value)
{
    this->Increment(value);
}

void ZunTimer::operator-=(float value)
{
    this->Decrement(value);
}

void ZunTimer::operator--(int)
{
    this->Decrement(1);
}

int ZunTimer::operator%(int value)
{
    return this->current % value;
}
