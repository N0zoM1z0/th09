#include "ReplayRuntimeLeaves.hpp"
#include "Supervisor.hpp"
#include "SupervisorFrameQueueView.hpp"
#include "SupervisorNetworkState.hpp"

#include <dplay8.h>
#include <new>
#include <stddef.h>
#include <string.h>

struct NetworkFramePacketView
{
    unsigned char type;
    signed char side;
    unsigned short seed;
    int frame;
    unsigned short input;
    unsigned short checksum;
};
typedef char NetworkFramePacketViewSizeIs0C[
    (sizeof(NetworkFramePacketView) == 0x0C) ? 1 : -1];

struct NetworkHandshakePacketView
{
    unsigned char type;
    signed char side;
    unsigned short seed;
    unsigned char syncRate;
    unsigned char unknown05[3];
    unsigned int syncValue;
};
typedef char NetworkHandshakePacketViewSizeIs0C[
    (sizeof(NetworkHandshakePacketView) == 0x0C) ? 1 : -1];

struct NetworkAckPacketView
{
    unsigned char type;
    unsigned char unknown01;
    unsigned short value02;
    unsigned int value04;
};

struct SupervisorNetworkMessageView
{
    IDirectPlay8Peer *peer00;
    void *deviceAddress04;
    unsigned char unknown008[0x04];
    void *unknown00C;
    void *hostAddress10;
    unsigned int sendAsyncHandle14;
    unsigned int connectAsyncHandle18;
    DPNID peerIds1C[2];
    char hostname24[0x80];
    unsigned int portA4;
    int activeA8;
    int localSideAC;
    int packetReadyB0;
    int initialFramesQueuedB4;
    int connectionStateB8;
    int syncValueBC;
    int joinModeC0;
    unsigned char syncRateC4;
    unsigned char unknown0C5[3];
    NetworkFramePacketView lastReceivedC8;

    int HandleMessage(
        void *userContext,
        unsigned long messageId,
        void *message);
};
typedef char SupervisorNetworkMessageViewSizeIsD4[
    (sizeof(SupervisorNetworkMessageView) == 0xD4) ? 1 : -1];

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

                reinterpret_cast<SupervisorFrameQueueView *>(&g_Supervisor)
                    ->InsertReceivedFrame(
                        packet->side,
                        packet->frame,
                        packet->input,
                        static_cast<short>(packet->seed));
            }
        }
        else if (data[0] == 1)
        {
            NetworkAckPacketView *packet =
                reinterpret_cast<NetworkAckPacketView *>(data);

            if (g_NetworkHandshakeToken == static_cast<int>(packet->value04) &&
                packet->value04 == packet->value02)
            {
                packetReadyB0 = 1;
                g_NetworkLastReceiveTime = g_NetworkCurrentTime;
                break;
            }
        }
        else if (data[0] == 2)
        {
            NetworkHandshakePacketView *packet =
                reinterpret_cast<NetworkHandshakePacketView *>(data);

            if (packet->side == localSideAC)
                localSideAC = 1 - packet->side;

            if (localSideAC == 1)
            {
                g_ReplayRng.SetSeed(packet->seed);
                syncRateC4 = packet->syncRate;
                syncValueBC = packet->syncValue;

                if (60 / syncRateC4 > 1)
                {
                    int frame = -9998;
                    while (frame + 9998 < 60 / syncRateC4 - 1)
                    {
                        reinterpret_cast<SupervisorFrameQueueView *>(
                            &g_Supervisor)->InsertReceivedFrame(
                                0,
                                frame,
                                0,
                                static_cast<short>(g_ReplayRng.GetSeed()));
                        reinterpret_cast<SupervisorFrameQueueView *>(
                            &g_Supervisor)->InsertReceivedFrame(
                                1,
                                frame++,
                                0,
                                static_cast<short>(g_ReplayRng.GetSeed()));
                    }
                }
            }
        }

        g_NetworkLastReceiveTime = g_NetworkCurrentTime;
        break;
    }

    case DPN_MSGID_CREATE_PLAYER:
    {
        g_Supervisor.EnterCriticalSectionWrapper(4);
        DPNMSG_CREATE_PLAYER *msg =
            static_cast<DPNMSG_CREATE_PLAYER *>(message);

        DWORD size = 0;
        peer00->GetPeerInfo(msg->dpnidPlayer, 0, &size, 0);

        DPN_PLAYER_INFO *info =
            static_cast<DPN_PLAYER_INFO *>(::operator new(size));
        memset(info, 0, size);
        info->dwSize = sizeof(DPN_PLAYER_INFO);

        HRESULT result =
            peer00->GetPeerInfo(msg->dpnidPlayer, info, &size, 0);
        if (result != 0)
        {
            ::operator delete(info);
            g_Supervisor.LeaveCriticalSectionWrapper(4);
            return result;
        }

        if ((info->dwPlayerFlags & DPNPLAYER_LOCAL) != 0)
            peerIds1C[localSideAC] = msg->dpnidPlayer;
        else
            peerIds1C[1 - localSideAC] = msg->dpnidPlayer;

        ::operator delete(info);
        break;
    }

    case DPN_MSGID_DESTROY_PLAYER:
    {
        g_Supervisor.EnterCriticalSectionWrapper(4);
        DPNMSG_DESTROY_PLAYER *msg =
            static_cast<DPNMSG_DESTROY_PLAYER *>(message);

        if (peerIds1C[0] == msg->dpnidPlayer)
            peerIds1C[0] = 0;
        if (peerIds1C[1] == msg->dpnidPlayer)
            peerIds1C[1] = 0;
        break;
    }

    case DPN_MSGID_SEND_COMPLETE:
        g_Supervisor.EnterCriticalSectionWrapper(4);
        packetReadyB0 = 1;
        break;

    case DPN_MSGID_CONNECT_COMPLETE:
    {
        g_Supervisor.EnterCriticalSectionWrapper(4);
        DPNMSG_CONNECT_COMPLETE *msg =
            static_cast<DPNMSG_CONNECT_COMPLETE *>(message);
        if (msg->hResultCode == 0 && connectionStateB8 == 1)
        {
            connectionStateB8 = 3;
            localSideAC = 1;
        }
        break;
    }

    default:
        return 0;
    }

    g_Supervisor.LeaveCriticalSectionWrapper(4);
    return 0;
}
