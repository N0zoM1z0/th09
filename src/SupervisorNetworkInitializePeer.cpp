#include "SupervisorNetworkState.hpp"
#include "GameErrorContext.hpp"

#include <dplay8.h>
#include <windows.h>

extern GameErrorContext g_GameErrorContext;
extern const char g_DirectPlayPeerCreateError[];
extern const char g_DirectPlayPeerInitializeError[];
extern const char g_DirectPlayTcpIpUnavailableError[];
extern const char g_DirectPlayInitializedMessage[];

int __stdcall SupervisorNetworkMessageThunk(
    void *userContext,
    unsigned long messageId,
    void *message);
void __fastcall ReportDirectPlayError(HRESULT error);

int SupervisorNetworkState::InitializePeer()
{
    HRESULT result = CoCreateInstance(
        CLSID_DirectPlay8Peer,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IDirectPlay8Peer,
        reinterpret_cast<void **>(&this->peer));
    if (FAILED(result))
    {
        g_GameErrorContext.Fatal(g_DirectPlayPeerCreateError, result);
        ReportDirectPlayError(result);
        return result;
    }

    result = reinterpret_cast<IDirectPlay8Peer *>(this->peer)->Initialize(
        NULL,
        reinterpret_cast<PFNDPNMESSAGEHANDLER>(
            SupervisorNetworkMessageThunk),
        0);
    if (FAILED(result))
    {
        g_GameErrorContext.Fatal(g_DirectPlayPeerInitializeError, result);
        ReportDirectPlayError(result);
        return result;
    }

    if (!this->IsServiceProviderAvailable(&CLSID_DP8SP_TCPIP))
    {
        result = E_FAIL;
        g_GameErrorContext.Fatal(g_DirectPlayTcpIpUnavailableError);
    }
    else
    {
        g_GameErrorContext.Log(g_DirectPlayInitializedMessage);
    }

    return result;
}
