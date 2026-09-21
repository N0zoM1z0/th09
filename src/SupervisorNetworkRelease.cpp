#include "SupervisorNetworkState.hpp"

#include <dplay8.h>

void SupervisorNetworkState::ReleaseDirectPlayResources()
{
    IDirectPlay8Peer *networkPeer =
        reinterpret_cast<IDirectPlay8Peer *>(this->peer);
    if (networkPeer != NULL)
        networkPeer->Close(0);

    if (this->deviceAddress != NULL)
    {
        reinterpret_cast<IUnknown *>(this->deviceAddress)->Release();
        this->deviceAddress = NULL;
    }

    if (this->unknown00C != NULL)
    {
        reinterpret_cast<IUnknown *>(this->unknown00C)->Release();
        this->unknown00C = NULL;
    }

    if (this->hostAddress != NULL)
    {
        reinterpret_cast<IUnknown *>(this->hostAddress)->Release();
        this->hostAddress = NULL;
    }

    if (this->peer != NULL)
    {
        reinterpret_cast<IUnknown *>(this->peer)->Release();
        this->peer = NULL;
    }
}
