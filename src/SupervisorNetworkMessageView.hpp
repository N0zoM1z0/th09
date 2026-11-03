#pragma once

#include "SupervisorNetworkState.hpp"
#include <stddef.h>

// Source owner identity in the original callback and exact 15-byte thunk is
// SupervisorNetworkMessageView. This view names the same 0xD4-byte native
// session allocation, not an independently allocated lookalike.
struct SupervisorNetworkMessageView : SupervisorNetworkState
{
    int HandleMessage(void *userContext, unsigned long messageId, void *message);
};
typedef char SupervisorNetworkMessageNativeStorage[
    (sizeof(SupervisorNetworkMessageView) == sizeof(SupervisorNetworkState) &&
     offsetof(SupervisorNetworkMessageView, lastReceivedC8) == 0xC8 &&
     offsetof(SupervisorNetworkMessageView, peer) == 0 &&
     offsetof(SupervisorNetworkMessageView, localSide) == 0xAC) ? 1 : -1];
