#include "SupervisorNetworkState.hpp"
#include "GameErrorContext.hpp"

#include <dplay8.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

extern GameErrorContext g_GameErrorContext;
extern const char g_DirectPlayHostSessionError[];
void __fastcall ReportDirectPlayError(HRESULT error);

namespace
{
const GUID g_Th09NetworkApplicationGuid = {
    0x9FD5EC50,
    0x740A,
    0x4FDB,
    {0x90, 0x9D, 0x2F, 0x24, 0xE1, 0x52, 0x82, 0xEA}
};
}

unsigned int SupervisorNetworkState::HostSession()
{
    wchar_t sessionName[128];
    DPN_APPLICATION_DESC appDesc;
    mbstowcs(sessionName, "TH09YoukaiMatch", 128);
    memset(&appDesc, 0, sizeof(appDesc));
    appDesc.dwSize = sizeof(appDesc);
    appDesc.guidApplication = g_Th09NetworkApplicationGuid;
    appDesc.pwszSessionName = sessionName;

    HRESULT result =
        reinterpret_cast<IDirectPlay8Peer *>(this->peer)->Host(
            &appDesc,
            reinterpret_cast<IDirectPlay8Address **>(&this->deviceAddress),
            1,
            NULL,
            NULL,
            NULL,
            1);
    if (FAILED(result))
    {
        g_GameErrorContext.Fatal(g_DirectPlayHostSessionError, result);
        ReportDirectPlayError(result);
    }
    return result;
}

unsigned int SupervisorNetworkState::ConnectSession()
{
    wchar_t sessionName[128];
    DPN_APPLICATION_DESC appDesc;

    mbstowcs(sessionName, "TH09YoukaiMatch", 128);
    memset(&appDesc, 0, sizeof(appDesc));
    appDesc.dwSize = sizeof(appDesc);
    appDesc.guidApplication = g_Th09NetworkApplicationGuid;
    appDesc.pwszSessionName = sessionName;

    HRESULT result =
        reinterpret_cast<IDirectPlay8Peer *>(this->peer)->Connect(
            &appDesc,
            reinterpret_cast<IDirectPlay8Address *>(this->hostAddress),
            reinterpret_cast<IDirectPlay8Address *>(this->deviceAddress),
            NULL,
            NULL,
            NULL,
            0,
            NULL,
            NULL,
            reinterpret_cast<DPNHANDLE *>(&this->connectAsyncHandle),
            1);
    if (result != DPNERR_PENDING && result != S_OK)
        ReportDirectPlayError(result);
    return result;
}
