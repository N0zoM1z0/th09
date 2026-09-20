#include "GameErrorContext.hpp"
#include <d3d8.h>

struct SupervisorD3DCreateView
{
    unsigned char unknown00[0x04];
    IDirect3D8 *d3d04;
};

extern SupervisorD3DCreateView g_Supervisor;
extern GameErrorContext g_GameErrorContext;

int SupervisorInitializeD3D()
{
    g_Supervisor.d3d04 = Direct3DCreate8(D3D_SDK_VERSION);
    if (g_Supervisor.d3d04 == NULL)
    {
        g_GameErrorContext.Fatal(
            "Direct3D \x83\x49\x83\x75\x83\x57\x83\x46\x83\x4e\x83\x67"
            "\x82\xcd\x89\xbd\x8c\xcc\x82\xa9\x8d\xec\x90\xac\x8f\x6f"
            "\x97\x88\x82\xc8\x82\xa9\x82\xc1\x82\xbd\r\n");
        return 1;
    }
    return 0;
}
