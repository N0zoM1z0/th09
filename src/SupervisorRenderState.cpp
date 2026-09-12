#include "AnmManager.hpp"
#include "Supervisor.hpp"

#include <stddef.h>
#include <windows.h>
#include <d3d8.h>

struct SupervisorRenderStateLayout
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice;
    unsigned char unknown00C[0x794 - 0x0C];
    int fogState;
};

typedef char SupervisorRenderStateDeviceAt08[
    (offsetof(SupervisorRenderStateLayout, d3dDevice) == 0x08) ? 1 : -1];
typedef char SupervisorRenderStateFogAt794[
    (offsetof(SupervisorRenderStateLayout, fogState) == 0x794) ? 1 : -1];

int Supervisor::IsFogDisabled()
{
    return (this->config388.options >> 2) & 1;
}

int Supervisor::DisableFog()
{
    SupervisorRenderStateLayout *supervisor =
        reinterpret_cast<SupervisorRenderStateLayout *>(this);

    g_AnmManager->FlushVertexBuffer();
    if (supervisor->fogState != 0) {
        supervisor->fogState = 0;
        return supervisor->d3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }
    return 0;
}

void Supervisor::SetRenderState(int state, int value)
{
    SupervisorRenderStateLayout *supervisor =
        reinterpret_cast<SupervisorRenderStateLayout *>(this);

    g_AnmManager->FlushVertexBuffer();
    supervisor->d3dDevice->SetRenderState(
        static_cast<D3DRENDERSTATETYPE>(state), static_cast<DWORD>(value));
}
