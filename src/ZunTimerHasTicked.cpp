#include "ZunTimer.hpp"

int ZunTimer::HasTicked()
{
    return current != previous;
}
