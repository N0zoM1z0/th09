#include "SupervisorNetworkState.hpp"
#include "GameErrorContext.hpp"

#include <dplay8.h>
#include <windows.h>

extern GameErrorContext g_GameErrorContext;
extern const char g_DirectPlayPeerCreateError[] = "IDirectPlay8Peer \x8D\xEC\x90\xAC\x8E\xB8\x94\x73:  0x%X\r\n"; // IDirectPlay8Peer 作成失敗:  0x%X\r\n
extern const char g_DirectPlayPeerInitializeError[];
extern const char g_DirectPlayTcpIpUnavailableError[];
extern const char g_DirectPlayInitializedMessage[] = "DirectPlay \x82\xAA\x90\xB3\x8F\xED\x82\xC9\x8F\x89\x8A\xFA\x89\xBB\x82\xB3\x82\xEA\x82\xBD\x82\xE6\x82\xA4\x82\xC9\x8C\xA9\x82\xA6\x82\xBD\r\n"; // DirectPlay が正常に初期化されたように見えた\r\n

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

    result = this->peer->Initialize(
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
