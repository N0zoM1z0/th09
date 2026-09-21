#include "SupervisorNetworkState.hpp"
#include "GameErrorContext.hpp"

#include <dplay8.h>
#include <windows.h>

extern GameErrorContext g_GameErrorContext;

int __stdcall SupervisorNetworkMessageThunk(
    void *userContext,
    unsigned long messageId,
    void *message);
void __fastcall ReportDirectPlayError(HRESULT error);

int SupervisorNetworkState::ResetSession()
{
    reinterpret_cast<IDirectPlay8Peer *>(this->peer)->
        CancelAsyncOperation(NULL, DPNCANCEL_ALL_OPERATIONS);
    reinterpret_cast<IDirectPlay8Peer *>(this->peer)->Close(0);

    if (this->deviceAddress != NULL)
    {
        reinterpret_cast<IUnknown *>(this->deviceAddress)->Release();
        this->deviceAddress = NULL;
    }
    if (this->unknown00C != NULL)
    {
        reinterpret_cast<IUnknown *>(this->unknown00C)->Release();
        this->unknown00C = NULL;
    }
    if (this->hostAddress != NULL)
    {
        reinterpret_cast<IUnknown *>(this->hostAddress)->Release();
        this->hostAddress = NULL;
    }

    Sleep(100);

    HRESULT result =
        reinterpret_cast<IDirectPlay8Peer *>(this->peer)->Initialize(
            NULL,
            reinterpret_cast<PFNDPNMESSAGEHANDLER>(
                SupervisorNetworkMessageThunk),
            0);
    if (FAILED(result))
    {
        g_GameErrorContext.Fatal(
            "\x49\x44\x69\x72\x65\x63\x74\x50\x6C\x61\x79\x38\x50\x65\x65\x72"
            "\x20\x8F\x89\x8A\xFA\x89\xBB\x8E\xB8\x94\x73\x3A\x20\x20\x30\x78\x25\x58\x0D\x0A",
            result);
        ReportDirectPlayError(result);
        return 1;
    }

    if (!this->IsServiceProviderAvailable(&CLSID_DP8SP_TCPIP))
    {
        g_GameErrorContext.Fatal(
            "\x43\x4C\x53\x49\x44\x5F\x44\x50\x38\x53\x50\x5F\x54\x43\x50\x49\x50"
            "\x20\x8E\x67\x97\x70\x95\x73\x0D\x0A");
        return 1;
    }

    this->peerIds[0] = 0;
    this->peerIds[1] = 0;
    return 1;
}
