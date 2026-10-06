#include "ReplayInputState.hpp"

unsigned short ReplayInputState::WasPressed(unsigned short buttons)
{
    return historyPressed & buttons;
}
