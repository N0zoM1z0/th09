#include <stddef.h>
#include "ReplayInputState.hpp"

typedef unsigned short u16;

u16 ReplayInputState::IsHeld(u16 mask)
{
    return historyCurrent & mask;
}
