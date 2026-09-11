#include "Supervisor.hpp"

#include <string.h>

Supervisor::Supervisor()
{
    memset(this, 0, sizeof(*this));
    this->flags5D4 |= 0x140;
}
