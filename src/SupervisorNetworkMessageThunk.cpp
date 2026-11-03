#include "SupervisorNetworkMessageView.hpp"

int __stdcall SupervisorNetworkMessageThunk(
    void *userContext,
    unsigned long messageId,
    void *message)
{
    return static_cast<SupervisorNetworkMessageView *>(
        g_SupervisorNetworkState)->HandleMessage(
            userContext, messageId, message);
}
