// Defined once by the ECL/Enemy carrier; original TU ownership is unknown.
#include "ZunTimer.hpp"

unsigned int ZunTimer::operator>(int value)
{
    return this->current > value;
}
