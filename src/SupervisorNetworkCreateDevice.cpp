#include "SupervisorNetworkState.hpp"
#include "GameErrorContext.hpp"

#include <dplay8.h>
#include <windows.h>

extern GameErrorContext g_GameErrorContext;
extern const char g_DirectPlayDeviceAddressCreateError[] = "Device Address\x8D\xEC\x90\xAC\x8E\xB8\x94\x73\r\n"; // Device Address作成失敗\r\n
extern const char g_DirectPlayDeviceAddressSetProviderError[] = "\x83\x54\x81\x5B\x83\x72\x83\x58\x83\x76\x83\x8D\x83\x6F\x83\x43\x83\x5F\x90\xDD\x92\xE8\x8E\xB8\x94\x73:\t0x%X\r\n"; // サービスプロバイダ設定失敗:\t0x%X\r\n
extern const char g_DirectPlayDeviceAddressSetPortError[] = "\x83\x7C\x81\x5B\x83\x67\x90\xDD\x92\xE8\x8E\xB8\x94\x73:\t0x%X\r\n"; // ポート設定失敗:\t0x%X\r\n

int SupervisorNetworkState::CreateDeviceAddress()
{
    this->peerIds[0] = 0;
    this->peerIds[1] = 0;

    HRESULT result = CoCreateInstance(
        CLSID_DirectPlay8Address,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IDirectPlay8Address,
        reinterpret_cast<void **>(&this->deviceAddress));
    if (FAILED(result))
    {
        g_GameErrorContext.Fatal(g_DirectPlayDeviceAddressCreateError);
        return result;
    }

    result =
        this->deviceAddress->
            SetSP(&CLSID_DP8SP_TCPIP);
    if (FAILED(result))
    {
        g_GameErrorContext.Fatal(
            g_DirectPlayDeviceAddressSetProviderError,
            result);
        return result;
    }

    result =
        this->deviceAddress->
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
