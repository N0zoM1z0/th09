#include "SupervisorNetworkState.hpp"
#include "GameErrorContext.hpp"

#include <dplay8.h>
#include <windows.h>

extern GameErrorContext g_GameErrorContext;
extern const char g_DirectPlayDeviceAddressCreateError[];
extern const char g_DirectPlayDeviceAddressSetProviderError[];
extern const char g_DirectPlayDeviceAddressSetPortError[];

int SupervisorNetworkState::CreateDeviceAddress()
{
    this->peerIds[0] = 0;
    this->peerIds[1] = 0;

    HRESULT result = CoCreateInstance(
        CLSID_DirectPlay8Address,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IDirectPlay8Address,
        &this->deviceAddress);
    if (FAILED(result))
    {
        g_GameErrorContext.Fatal(g_DirectPlayDeviceAddressCreateError);
        return result;
    }

    result =
        reinterpret_cast<IDirectPlay8Address *>(this->deviceAddress)->
            SetSP(&CLSID_DP8SP_TCPIP);
    if (FAILED(result))
    {
        g_GameErrorContext.Fatal(
            g_DirectPlayDeviceAddressSetProviderError,
            result);
        return result;
    }

    result =
        reinterpret_cast<IDirectPlay8Address *>(this->deviceAddress)->
            AddComponent(
        DPNA_KEY_PORT,
        &this->port,
        sizeof(this->port),
        DPNA_DATATYPE_DWORD);
    if (FAILED(result))
        g_GameErrorContext.Fatal(
            g_DirectPlayDeviceAddressSetPortError,
            result);
    return result;
}
