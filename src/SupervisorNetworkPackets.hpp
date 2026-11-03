#pragma once

#include <stddef.h>

// The native DirectPlay handshake packet has one 0x0C-byte wire layout,
// independently consumed by the TH09 network service and receive callback.
// The original receiver sign-extends the side byte; packet senders publish
// only side 0 or 1, which retain the same byte representation.
struct SupervisorNetworkHandshakeWire
{
    unsigned char type;
    signed char side;
    unsigned short seed;
    unsigned char syncRate;
    unsigned char unknown05[3];
    unsigned int syncValue;
};

typedef char SupervisorNetworkHandshakeWireSizeIs0C[
    (sizeof(SupervisorNetworkHandshakeWire) == 0x0C) ? 1 : -1];
typedef char SupervisorNetworkHandshakeWireFields[
    (offsetof(SupervisorNetworkHandshakeWire, side) == 0x01 &&
     offsetof(SupervisorNetworkHandshakeWire, seed) == 0x02 &&
     offsetof(SupervisorNetworkHandshakeWire, syncRate) == 0x04 &&
     offsetof(SupervisorNetworkHandshakeWire, syncValue) == 0x08) ? 1 : -1];
