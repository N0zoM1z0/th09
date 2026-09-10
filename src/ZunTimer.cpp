#include "ZunTimer.hpp"

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
