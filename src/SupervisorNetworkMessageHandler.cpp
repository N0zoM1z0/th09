#include "ReplayRuntimeLeaves.hpp"
#include "Supervisor.hpp"
#include "SupervisorFrameQueueView.hpp"
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

extern ReplayRngView g_ReplayRng;
extern int g_NetworkHandshakeToken;
extern unsigned int g_NetworkLastReceiveTime;
extern unsigned int g_NetworkCurrentTime;

int SupervisorNetworkMessageView::HandleMessage(
    void *userContext,
    unsigned long messageId,
    void *message)
{
    switch (messageId)
    {
    case DPN_MSGID_RECEIVE:
    {
        g_Supervisor.EnterCriticalSectionWrapper(4);
        DPNMSG_RECEIVE *msg = static_cast<DPNMSG_RECEIVE *>(message);
        unsigned char *data =
            static_cast<unsigned char *>(msg->pReceiveData);

        if (data[0] == 0)
        {
            NetworkFramePacketView *packet =
                reinterpret_cast<NetworkFramePacketView *>(data);

            if (packet->side != g_SupervisorNetworkState->localSide)
            {
                lastReceivedC8 = *packet;

                // The original reads the frame before the input word. Keep
                // these actual packet reads visible before the queue call.
                int receivedFrame = static_cast<volatile int &>(packet->frame);
                unsigned short receivedSeed = packet->seed;
                int receivedInput = static_cast<volatile unsigned short &>(packet->input);
                reinterpret_cast<SupervisorFrameQueueView *>(&g_Supervisor)
                    ->InsertReceivedFrame(packet->side, receivedFrame, receivedInput,
                        static_cast<short>(receivedSeed));
            }
        }
        else if (data[0] == 1)
        {
            NetworkAckPacketView *packet =
                reinterpret_cast<NetworkAckPacketView *>(data);

            if (g_NetworkHandshakeToken == static_cast<int>(packet->value04) &&
                packet->value04 == packet->value02)
            {
                packetReady = 1;
                g_NetworkLastReceiveTime = g_NetworkCurrentTime;
                g_Supervisor.LeaveCriticalSectionWrapper(4);
                return 0;
            }
        }
        else if (data[0] == 2)
        {
            NetworkHandshakePacketView *packet =
                reinterpret_cast<NetworkHandshakePacketView *>(data);

            if (packet->side == localSide)
                localSide = 1 - packet->side;

            if (localSide == 1)
            {
                g_ReplayRng.SetSeed(packet->seed);
                syncRate = packet->syncRate;
                syncValue = packet->syncValue;

                if (60 / syncRate > 1)
                {
                    for (int index = 0; index < 60 / syncRate - 1; ++index)
                    {
                        reinterpret_cast<SupervisorFrameQueueView *>(
                            &g_Supervisor)->InsertReceivedFrame(
                                0,
                                index - 9998,
                                0,
                                static_cast<short>(g_ReplayRng.GetSeed()));
                        reinterpret_cast<SupervisorFrameQueueView *>(
                            &g_Supervisor)->InsertReceivedFrame(
                                1,
                                index - 9998,
                                0,
                                static_cast<short>(g_ReplayRng.GetSeed()));
                    }
                }
            }
        }

        g_NetworkLastReceiveTime = g_NetworkCurrentTime;
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        return 0;
    }

    case DPN_MSGID_CREATE_PLAYER:
    {
        g_Supervisor.EnterCriticalSectionWrapper(4);
        DPNMSG_CREATE_PLAYER *msg =
            static_cast<DPNMSG_CREATE_PLAYER *>(message);

        DWORD size = 0;
        static_cast<IDirectPlay8Peer *>(peer)->GetPeerInfo(msg->dpnidPlayer, 0, &size, 0);

        DPN_PLAYER_INFO *info =
            static_cast<DPN_PLAYER_INFO *>(::operator new(size));
        memset(info, 0, size);
        info->dwSize = sizeof(DPN_PLAYER_INFO);

        HRESULT result =
            static_cast<IDirectPlay8Peer *>(peer)->GetPeerInfo(msg->dpnidPlayer, info, &size, 0);
        if (result != 0)
        {
            ::operator delete(info);
            g_Supervisor.LeaveCriticalSectionWrapper(4);
            return result;
        }

        if ((info->dwPlayerFlags & DPNPLAYER_LOCAL) != 0)
            peerIds[localSide] = msg->dpnidPlayer;
        else
            peerIds[1 - localSide] = msg->dpnidPlayer;

        ::operator delete(info);
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        return 0;
    }

    case DPN_MSGID_DESTROY_PLAYER:
    {
        g_Supervisor.EnterCriticalSectionWrapper(4);
        DPNMSG_DESTROY_PLAYER *msg =
            static_cast<DPNMSG_DESTROY_PLAYER *>(message);

        if (peerIds[0] == msg->dpnidPlayer)
            peerIds[0] = 0;
        if (peerIds[1] == msg->dpnidPlayer)
            peerIds[1] = 0;
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        return 0;
    }

    case DPN_MSGID_SEND_COMPLETE:
        g_Supervisor.EnterCriticalSectionWrapper(4);
        packetReady = 1;
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        return 0;

    case DPN_MSGID_CONNECT_COMPLETE:
    {
        g_Supervisor.EnterCriticalSectionWrapper(4);
        DPNMSG_CONNECT_COMPLETE *msg =
            static_cast<DPNMSG_CONNECT_COMPLETE *>(message);
        if (msg->hResultCode == 0 && connectionState == 1)
        {
            connectionState = 3;
            localSide = 1;
        }
        g_Supervisor.LeaveCriticalSectionWrapper(4);
        return 0;
    }

    default:
        return 0;
    }
}
