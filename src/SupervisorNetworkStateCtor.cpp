#include "SupervisorNetworkState.hpp"
#include <string.h>

SupervisorNetworkState::SupervisorNetworkState()
{
    memset(this, 0, sizeof(*this));
    syncRate = 60;
}
