#include "SupervisorNetworkState.hpp"

int SupervisorNetworkState::GetRemotePeerId()
{
    return this->peerIds[static_cast<unsigned int>(
        1u - static_cast<unsigned int>(this->localSide))];
}
