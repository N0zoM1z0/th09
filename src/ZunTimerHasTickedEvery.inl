#include "ZunTimer.hpp"

int ZunTimer::HasTickedEvery(int interval)
{
    return current != previous && current % interval == 0;
}
