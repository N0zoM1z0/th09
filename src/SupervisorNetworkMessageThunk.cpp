struct SupervisorNetworkMessageView
{
    int HandleMessage(
        void *userContext,
        unsigned long messageId,
        void *message);
};

extern SupervisorNetworkMessageView *g_SupervisorNetworkState;

int __stdcall SupervisorNetworkMessageThunk(
    void *userContext,
    unsigned long messageId,
    void *message)
{
    return g_SupervisorNetworkState->HandleMessage(
        userContext, messageId, message);
}
