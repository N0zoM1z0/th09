#include "ZunTimer.hpp"
#include "Supervisor.hpp"

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
