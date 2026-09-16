#include "ZunTimer.hpp"
#include "Supervisor.hpp"

#include <stddef.h>

namespace
{
struct ZunTimerSupervisorLayout
{
    unsigned char unknown000[0x5B8];
    float gameSpeed5B8;
    unsigned char unknown5BC[0x5D4 - 0x5BC];
    unsigned int flags5D4;
};

typedef char ZunTimerSupervisorGameSpeedAt5B8[
    (offsetof(ZunTimerSupervisorLayout, gameSpeed5B8) == 0x5B8) ? 1 : -1];
typedef char ZunTimerSupervisorFlagsAt5D4[
    (offsetof(ZunTimerSupervisorLayout, flags5D4) == 0x5D4) ? 1 : -1];
}

ZunTimer::ZunTimer()
{
    this->Initialize();
}

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

ZunTimer::operator float()
{
    return this->subFrame;
}

void ZunTimer::operator++(int)
{
    this->Tick();
}

unsigned int ZunTimer::operator>=(int value)
{
    return this->current >= value;
}

unsigned int ZunTimer::operator<(int value)
{
    return this->current < value;
}

unsigned int ZunTimer::operator<=(int value)
{
    return this->current <= value;
}

int ZunTimer::operator==(int value)
{
    return this->current == value;
}

void ZunTimer::Increment(float value)
{
    ZunTimerSupervisorLayout *supervisor =
        reinterpret_cast<ZunTimerSupervisorLayout *>(&g_Supervisor);

    this->previous = this->current;
    if (supervisor->gameSpeed5B8 > 0.99f)
        this->subFrame += value;
    else
        this->subFrame += supervisor->gameSpeed5B8 * value;

    this->current = (int)this->subFrame;
    if ((supervisor->flags5D4 & 0x20) != 0)
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

unsigned int ZunTimer::operator>(int value)
{
    return this->current > value;
}
