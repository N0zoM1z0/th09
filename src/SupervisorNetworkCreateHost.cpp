#include "SupervisorNetworkState.hpp"
#include "GameErrorContext.hpp"

#include <dplay8.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

extern GameErrorContext g_GameErrorContext;
extern const char g_DirectPlayHostAddressCreateError[];
extern const char g_DirectPlayHostAddressSetProviderError[];
extern const char g_DirectPlayHostAddressSetHostnameError[];
extern const char g_DirectPlayHostAddressSetPortError[];

int SupervisorNetworkState::CreateHostAddress()
{
    wchar_t hostname[256];
    mbstowcs(hostname, this->hostname, 256);

    HRESULT result = CoCreateInstance(
        CLSID_DirectPlay8Address,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IDirectPlay8Address,
        &this->hostAddress);
    if (FAILED(result))
    {
        g_GameErrorContext.Fatal(g_DirectPlayHostAddressCreateError);
        return result;
    }

    result =
        reinterpret_cast<IDirectPlay8Address *>(this->hostAddress)->
            SetSP(&CLSID_DP8SP_TCPIP);
    if (FAILED(result))
    {
        g_GameErrorContext.Fatal(
            g_DirectPlayHostAddressSetProviderError,
            result);
        return result;
    }

    result =
        reinterpret_cast<IDirectPlay8Address *>(this->hostAddress)->
            AddComponent(
                DPNA_KEY_HOSTNAME,
                hostname,
                (wcslen(hostname) + 1) * sizeof(wchar_t),
                DPNA_DATATYPE_STRING);
    if (FAILED(result))
    {
        g_GameErrorContext.Fatal(g_DirectPlayHostAddressSetHostnameError);
        return result;
    }

    result =
        reinterpret_cast<IDirectPlay8Address *>(this->hostAddress)->
            AddComponent(
                DPNA_KEY_PORT,
                &this->port,
                sizeof(this->port),
                DPNA_DATATYPE_DWORD);
    if (FAILED(result))
        g_GameErrorContext.Fatal(g_DirectPlayHostAddressSetPortError);
    return result;
}
