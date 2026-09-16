#include "SupervisorNetworkState.hpp"

int SupervisorNetworkState::GetRemotePeerId()
{
    return this->peerIds[1 - this->localSide];
}
