#include "SupervisorNetworkState.hpp"

#include <dplay8.h>

void SupervisorNetworkState::ReleaseDirectPlayResources()
{
    IDirectPlay8Peer *networkPeer =
        this->peer;
    if (networkPeer != NULL)
        networkPeer->Close(0);

    if (this->deviceAddress != NULL)
    {
        this->deviceAddress->Release();
        this->deviceAddress = NULL;
    }

    if (this->unknown00C != NULL)
    {
        this->unknown00C->Release();
        this->unknown00C = NULL;
    }

    if (this->hostAddress != NULL)
    {
        this->hostAddress->Release();
        this->hostAddress = NULL;
    }

    if (this->peer != NULL)
    {
        this->peer->Release();
        this->peer = NULL;
    }
}
