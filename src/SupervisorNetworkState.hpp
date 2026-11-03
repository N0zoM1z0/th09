#pragma once

#include <stddef.h>

struct SupervisorNetworkState
{
    void *peer;
    void *deviceAddress;
    unsigned char unknown008[0x04];
    void *unknown00C;
    void *hostAddress;
    unsigned int sendAsyncHandle;
    unsigned int connectAsyncHandle;
    unsigned int peerIds[2];
    char hostname[0x80];
    unsigned int port;
    int active;
    int localSide;
    int packetReady;
    int initialFramesQueued;
    int connectionState;
    int syncValue;
    int joinMode;
    unsigned char syncRate;
    unsigned char unknown0C5[3];
    // Native packet image copied by the receive-message callback at +0xC8.
    // This 12-byte slot already belonged to the 0xD4 NetworkState allocation.
    struct LastReceivedFramePacketView
    {
        unsigned char type;
        signed char side;
        unsigned short seed;
        int frame;
        unsigned short input;
        unsigned short checksum;
    } lastReceivedC8;

    SupervisorNetworkState();
    int InitializePeer();
    int GetRemotePeerId();
    int IsServiceProviderAvailable(const void *provider);
    int SendPacket(void *packet, int size);
    void ReleaseDirectPlayResources();
    int CreateDeviceAddress();
    int CreateHostAddress();
    unsigned int HostSession();
    unsigned int ConnectSession();
    int ResetSession();
};

typedef char SupervisorNetworkLastFrameAtC8[
    (offsetof(SupervisorNetworkState, lastReceivedC8) == 0xC8 &&
     sizeof(((SupervisorNetworkState *)0)->lastReceivedC8) == 0x0C) ? 1 : -1];
typedef char SupervisorNetworkStateSizeIsD4[(sizeof(SupervisorNetworkState) == 0xD4) ? 1 : -1];
typedef char SupervisorNetworkStateUnknown00CAt0C[
    (offsetof(SupervisorNetworkState, unknown00C) == 0x0C) ? 1 : -1];
typedef char SupervisorNetworkStateActiveAtA8[
    (offsetof(SupervisorNetworkState, active) == 0xA8) ? 1 : -1];
typedef char SupervisorNetworkStateSideAtAC[
    (offsetof(SupervisorNetworkState, localSide) == 0xAC) ? 1 : -1];
typedef char SupervisorNetworkStatePacketAtB0[
    (offsetof(SupervisorNetworkState, packetReady) == 0xB0) ? 1 : -1];
typedef char SupervisorNetworkStateInitialAtB4[
    (offsetof(SupervisorNetworkState, initialFramesQueued) == 0xB4) ? 1 : -1];
typedef char SupervisorNetworkStateSyncRateAtC4[
    (offsetof(SupervisorNetworkState, syncRate) == 0xC4) ? 1 : -1];

extern SupervisorNetworkState *g_SupervisorNetworkState;
