// SPDX-License-Identifier: GPL-3.0-only
// Adapted from Cirno-Labs/th09 src/network.cpp Network::HandleMessage,
// commit d7653152becabb8c86f2f43d49015ecc11bd0652.
// Original MIT portions retain their grant in LICENSE; see THIRD_PARTY_NOTICES.md.
#include "RngRuntimeLeaves.hpp"
#include "Supervisor.hpp"
#include "SupervisorNetworkState.hpp"
#include "SupervisorNetworkMessageView.hpp"
#include "SupervisorNetworkPackets.hpp"

#include <dplay8.h>
#include <new>
#include <stddef.h>
#include <string.h>

// The received frame is a field of the native NetworkState, not a second
// anonymous 0xD4-byte network owner. Packet signedness and size are preserved.
typedef SupervisorNetworkState::LastReceivedFramePacketView NetworkFramePacketView;
typedef char NetworkFramePacketViewSizeIs0C[
    (sizeof(NetworkFramePacketView) == 0x0C) ? 1 : -1];

// The receiver reads exactly the same twelve-byte packet as the sender.
typedef SupervisorNetworkHandshakeWire NetworkHandshakePacketView;
typedef char NetworkHandshakePacketViewSizeIs0C[
    (sizeof(NetworkHandshakePacketView) == 0x0C) ? 1 : -1];

struct NetworkAckPacketView
{
    unsigned char type;
    unsigned char unknown01;
    unsigned short value02;
    unsigned int value04;
};

extern RngRuntimeView g_ReplayRng;
extern int g_NetworkHandshakeToken;
extern unsigned int g_NetworkLastReceiveTime;
extern unsigned int g_NetworkCurrentTime;

// DirectPlay local/remote sides select the two actual peer ID words.
// The retail signed side expression is evaluated as an i386 DWORD and
// then scaled by the native four-byte pointer/ID row width.
#define TH09_NETWORK_NATIVE_PEER_ID(index) \
    (*reinterpret_cast<DPNID *>( \
        reinterpret_cast<unsigned int>(&this->peerIds[0]) + \
        static_cast<unsigned int>(index) * sizeof(this->peerIds[0])))

int SupervisorNetworkMessageView::HandleMessage(void *userContext, unsigned long messageId, void *message)
{
    NetworkFramePacketView *packet;
    DPN_PLAYER_INFO *info;
    DWORD size;
    HRESULT hr;
    int i;

    switch (messageId)
    {
    case DPN_MSGID_RECEIVE:
        g_Supervisor.EnterCriticalSectionWrapper(4);
        packet = (NetworkFramePacketView *)((DPNMSG_RECEIVE *)message)->pReceiveData;
        if (packet->type == 0)
        {
            if (packet->side != g_SupervisorNetworkState->localSide)
            {
                this->lastReceivedC8 = *packet;
                g_Supervisor.InsertReceivedFrame(packet->side, packet->frame, packet->input, packet->seed);
            }
        }
        else if (packet->type == 1)
        {
            if (g_NetworkHandshakeToken == packet->frame && packet->frame == packet->seed)
            {
                this->packetReady = 1;
            }
        }
        else if (packet->type == 2)
        {
            if (packet->side == this->localSide)
            {
                this->localSide = static_cast<int>(
                    1u - static_cast<unsigned int>(packet->side));
            }
            if (this->localSide == 1)
            {
                g_ReplayRng.SetSeed(packet->seed);
                this->syncRate = packet->frame;
                this->syncValue = packet->packedInput;
                if (60 / this->syncRate > 1)
                {
                    for (i = 0; i < 60 / this->syncRate - 1; i++)
                    {
                        g_Supervisor.InsertReceivedFrame(0, static_cast<int>(static_cast<unsigned int>(i) - 9998u), 0, g_ReplayRng.GetSeed());
                        g_Supervisor.InsertReceivedFrame(1, static_cast<int>(static_cast<unsigned int>(i) - 9998u), 0, g_ReplayRng.GetSeed());
                    }
                }
            }
        }
        g_NetworkLastReceiveTime = g_NetworkCurrentTime;
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        break;
    case DPN_MSGID_CREATE_PLAYER:
        g_Supervisor.EnterCriticalSectionWrapper(4);
        size = 0;
        static_cast<IDirectPlay8Peer *>(peer)->GetPeerInfo(((DPNMSG_CREATE_PLAYER *)message)->dpnidPlayer, NULL, &size, 0);
        info = static_cast<DPN_PLAYER_INFO *>(::operator new(size));
        memset(info, 0, size);
        info->dwSize = sizeof(DPN_PLAYER_INFO);
        hr = static_cast<IDirectPlay8Peer *>(peer)->GetPeerInfo(((DPNMSG_CREATE_PLAYER *)message)->dpnidPlayer, info, &size, 0);
        if (hr != DPN_OK)
        {
            delete info;
            g_Supervisor.LeaveCriticalSectionWrapper(4);
            return hr;
        }
        if (info->dwPlayerFlags & DPNPLAYER_LOCAL)
        {
            TH09_NETWORK_NATIVE_PEER_ID(this->localSide) = ((DPNMSG_CREATE_PLAYER *)message)->dpnidPlayer;
        }
        else
        {
            TH09_NETWORK_NATIVE_PEER_ID(
                static_cast<int>(1u - static_cast<unsigned int>(this->localSide))) = ((DPNMSG_CREATE_PLAYER *)message)->dpnidPlayer;
        }
        delete info;
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        break;
    case DPN_MSGID_DESTROY_PLAYER:
        g_Supervisor.EnterCriticalSectionWrapper(4);
        if (this->peerIds[0] == ((DPNMSG_DESTROY_PLAYER *)message)->dpnidPlayer)
        {
            this->peerIds[0] = 0;
        }
        if (this->peerIds[1] == ((DPNMSG_DESTROY_PLAYER *)message)->dpnidPlayer)
        {
            this->peerIds[1] = 0;
        }
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        break;
    case DPN_MSGID_SEND_COMPLETE:
        g_Supervisor.EnterCriticalSectionWrapper(4);
        this->packetReady = 1;
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        break;
    case DPN_MSGID_CONNECT_COMPLETE:
        g_Supervisor.EnterCriticalSectionWrapper(4);
        if (((DPNMSG_CONNECT_COMPLETE *)message)->hResultCode == DPN_OK && this->connectionState == 1)
        {
            this->connectionState = 3;
            this->localSide = 1;
        }
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        break;
    }
    return 0;
}

#undef TH09_NETWORK_NATIVE_PEER_ID
