#include "ZunTimer.hpp"
#include "Supervisor.hpp"

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
