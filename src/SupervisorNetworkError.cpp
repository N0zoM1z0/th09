#include "GameErrorContext.hpp"

#include <windows.h>
#include <dxerr8.h>
#include <string.h>

extern GameErrorContext g_GameErrorContext;

void __fastcall ReportDirectPlayError(HRESULT error)
{
    char buffer[2048];
    LPSTR systemMessage;

    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        0x400,
        reinterpret_cast<LPSTR>(&systemMessage),
        0,
        NULL);

    if (systemMessage != NULL)
    {
        g_GameErrorContext.Fatal(systemMessage);
        LocalFree(systemMessage);
        return;
    }

    strcpy(buffer, DXGetErrorString8A(error));
    strcat(buffer, "\r\n");
    g_GameErrorContext.Fatal(buffer);
}
