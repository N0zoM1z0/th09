#include "SupervisorNetworkState.hpp"

int SupervisorNetworkState::GetRemotePeerId()
{
    return this->peerIds[1u - static_cast<unsigned int>(this->localSide)];
}
