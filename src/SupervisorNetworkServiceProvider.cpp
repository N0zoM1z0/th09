#include "SupervisorNetworkState.hpp"
#include "ZunMemory.hpp"
#include "GameErrorContext.hpp"

#include <dplay8.h>
#include <stdio.h>

extern GameErrorContext g_GameErrorContext;

int SupervisorNetworkState::IsServiceProviderAvailable(const void *provider)
{
    HRESULT hr = S_OK;
    DPN_SERVICE_PROVIDER_INFO *providers = NULL;
    DWORD providerCount = 0;
    DWORD bufferSize = 0;

    hr = reinterpret_cast<IDirectPlay8Peer *>(this->peer)->
        EnumServiceProviders(
            &CLSID_DP8SP_TCPIP, NULL, NULL,
            &bufferSize, &providerCount, 0);

    if (hr != DPNERR_BUFFERTOOSMALL)
    {
        printf("Failed Enumerating Service Providers:  0x%x\r\n", hr);
        goto CLEANUP;
    }

    providers = static_cast<DPN_SERVICE_PROVIDER_INFO *>(
        g_ZunMemory.Alloc(bufferSize, "./system\\global.h"));

    if (FAILED(
            hr = reinterpret_cast<IDirectPlay8Peer *>(this->peer)->
                EnumServiceProviders(
                    &CLSID_DP8SP_TCPIP, NULL, providers,
                    &bufferSize, &providerCount, 0)))
    {
        g_GameErrorContext.Fatal(
            "\x83\x54\x81\x5B\x83\x72\x83\x58\x83\x76\x83\x8D\x83\x6F\x83\x43"
            "\x83\x5F\x97\xF1\x8B\x93\x8E\xB8\x94\x73:\t0x%x\r\n",
            hr);
    }
    else if (providerCount == 0)
    {
        hr = E_FAIL;
    }

CLEANUP:
    if (providers != NULL)
        g_ZunMemory.Free(providers);

    return SUCCEEDED(hr);
}
