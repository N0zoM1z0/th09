#include "Supervisor.hpp"

#include <string.h>

Supervisor::Supervisor()
{
    memset(this, 0, sizeof(*this));
    // The full clear makes this equivalent to the target's flags OR.
    this->flags5D4 = 0x140;
}
