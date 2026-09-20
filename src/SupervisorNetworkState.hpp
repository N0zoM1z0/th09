#pragma once

#include <stddef.h>

struct SupervisorNetworkState
{
    void *peer;
    void *deviceAddress;
    unsigned char unknown008[0x08];
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
    unsigned char unknown0C5[0xD4 - 0x0C5];

    SupervisorNetworkState();
    int GetRemotePeerId();
    int SendPacket(void *packet, int size);
    int CreateDeviceAddress();
    int CreateHostAddress();
    unsigned int HostSession();
    unsigned int ConnectSession();
    int ResetSession();
};

typedef char SupervisorNetworkStateSizeIsD4[(sizeof(SupervisorNetworkState) == 0xD4) ? 1 : -1];
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
