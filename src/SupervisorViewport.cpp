#include "Supervisor.hpp"

#include <stddef.h>
#include <windows.h>
#include <d3d8.h>

struct SupervisorViewportConfiguration
{
    unsigned char unknown000[0xCC];
    D3DVIEWPORT8 viewport;
    unsigned char unknown0E4[0x0C];
};

typedef char SupervisorViewportConfigurationSizeIsF0[
    (sizeof(SupervisorViewportConfiguration) == 0xF0) ? 1 : -1];
typedef char SupervisorViewportAtCC[
    (offsetof(SupervisorViewportConfiguration, viewport) == 0xCC) ? 1 : -1];

struct SupervisorViewportView
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice;
    unsigned char unknown00C[0x78 - 0x0C];
    SupervisorViewportConfiguration configurations[3];
    SupervisorViewportConfiguration *current;
    int currentIndex;

    void ApplyGameplayViewport(SupervisorViewportConfiguration *configuration);
};

typedef char SupervisorViewportDeviceAt08[
    (offsetof(SupervisorViewportView, d3dDevice) == 0x08) ? 1 : -1];
typedef char SupervisorViewportConfigurationsAt78[
    (offsetof(SupervisorViewportView, configurations) == 0x78) ? 1 : -1];
typedef char SupervisorViewportCurrentAt348[
    (offsetof(SupervisorViewportView, current) == 0x348) ? 1 : -1];
typedef char SupervisorViewportIndexAt34C[
    (offsetof(SupervisorViewportView, currentIndex) == 0x34C) ? 1 : -1];

void Supervisor::ConfigureGameplayViewport(int index)
{
    SupervisorViewportView *supervisor =
        reinterpret_cast<SupervisorViewportView *>(this);
    supervisor->current = &supervisor->configurations[index];
    supervisor->ApplyGameplayViewport(supervisor->current);
    supervisor->d3dDevice->SetViewport(&supervisor->current->viewport);
    supervisor->currentIndex = index;
}
