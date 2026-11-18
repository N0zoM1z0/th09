#include "SupervisorNetworkState.hpp"

#include <dplay8.h>

int SupervisorNetworkState::SendPacket(void *packet, int size)
{
    if (!this->active)
        return 0;

    unsigned int *remotePeer = &this->peerIds[1u - static_cast<unsigned int>(this->localSide)];
    if (*remotePeer == 0)
        return 0;

    DPN_BUFFER_DESC buffer;
    buffer.dwBufferSize = size;
    buffer.pBufferData = reinterpret_cast<BYTE *>(packet);
    return this->peer->SendTo(
        *remotePeer,
        &buffer,
        1,
        0,
        NULL,
        reinterpret_cast<DPNHANDLE *>(&this->sendAsyncHandle),
        0x9C);
}
