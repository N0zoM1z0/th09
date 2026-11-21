#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"

#include <stddef.h>
#include <math.h>
#include <windows.h>
#include <d3d8.h>
#include <d3dx8math.h>

float __cdecl SupervisorCameraTan(float value);

// Camera methods use the actual 0xF0-byte record belonging to Supervisor.
// The global D3D device is already the real typed +0x08 member; keep the
// viewport configuration view only for its unresolved method-owner name.
typedef SupervisorCtorArrayElement SupervisorViewportConfiguration;
typedef char SupervisorViewportNativeMemberLayout[
    (sizeof(SupervisorViewportConfiguration) == 0xF0 &&
     offsetof(SupervisorViewportConfiguration, cameraPosition00) == 0 &&
     offsetof(SupervisorViewportConfiguration, viewMatrix4C) == 0x4C &&
     offsetof(SupervisorViewportConfiguration, viewportCC) == 0xCC &&
     offsetof(SupervisorViewportConfiguration, screenShakeOffsetE8) == 0xE8) ? 1 : -1];

struct SupervisorViewportView
{
    unsigned char unknown000[0x08];
    IDirect3DDevice8 *d3dDevice;
    unsigned char unknown00C[0x78 - 0x0C];
    SupervisorViewportConfiguration configurations[3];
    SupervisorViewportConfiguration *current;
    int currentIndex;

    void ApplyScreenCamera(SupervisorViewportConfiguration *configuration);
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

void SupervisorViewportView::ApplyScreenCamera(
    SupervisorViewportConfiguration *configuration)
{
    float fov;
    float aspectRatio;
    float viewportMiddleWidth;
    float viewportMiddleHeight;

    if (g_AnmManager != NULL)
        g_AnmManager->FlushVertexBuffer();

    viewportMiddleWidth = (float)configuration->viewportCC.Width / 2.0f;
    viewportMiddleHeight = (float)configuration->viewportCC.Height / 2.0f;
    aspectRatio = (float)configuration->viewportCC.Width /
                  (float)configuration->viewportCC.Height;
    fov = 3.1415927f / 10.0f;
    D3DXMatrixLookAtLH(
        reinterpret_cast<D3DXMATRIX *>(&configuration->viewMatrix4C),
        reinterpret_cast<D3DXVECTOR3 *>(
            &Float3(viewportMiddleWidth, viewportMiddleHeight,
                    viewportMiddleHeight / SupervisorCameraTan(fov / 2.0f))),
        reinterpret_cast<D3DXVECTOR3 *>(
            &Float3(viewportMiddleWidth, viewportMiddleHeight, 0.0f)),
        reinterpret_cast<D3DXVECTOR3 *>(&Float3(0.0f, -1.0f, 0.0f)));
    D3DXMatrixPerspectiveFovLH(
        reinterpret_cast<D3DXMATRIX *>(&configuration->projectionMatrix8C),
        fov,
        aspectRatio,
        1.0f,
        10000.0f);
    g_Supervisor.d3dDevice08->SetTransform(D3DTS_VIEW, &configuration->viewMatrix4C);
    g_Supervisor.d3dDevice08->SetTransform(
            D3DTS_PROJECTION, &configuration->projectionMatrix8C);

    if (g_AnmManager != NULL) {
        g_AnmManager->frameShakeX1C = configuration->screenShakeOffsetE8.x;
        g_AnmManager->frameShakeY20 = configuration->screenShakeOffsetE8.y;
    }
}

void SupervisorViewportView::ApplyGameplayViewport(
    SupervisorViewportConfiguration *configuration)
{
    if (g_AnmManager != NULL)
        g_AnmManager->FlushVertexBuffer();

    Float3 lookAt = configuration->cameraLookAtOffset0C +
                    configuration->cameraPosition00;
    Float3 eye = configuration->cameraPositionOffset3C +
                 configuration->cameraPosition00;

    D3DXMatrixLookAtLH(
        reinterpret_cast<D3DXMATRIX *>(&configuration->viewMatrix4C),
        reinterpret_cast<D3DXVECTOR3 *>(&eye),
        reinterpret_cast<D3DXVECTOR3 *>(&lookAt),
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraUp18));
    D3DXMatrixPerspectiveFovLH(
        reinterpret_cast<D3DXMATRIX *>(&configuration->projectionMatrix8C),
        configuration->fieldOfView48,
        (float)configuration->viewportCC.Width /
            (float)configuration->viewportCC.Height,
        30.0f,
        1800.0f);
    g_Supervisor.d3dDevice08->SetTransform(D3DTS_VIEW, &configuration->viewMatrix4C);
    g_Supervisor.d3dDevice08->SetTransform(
            D3DTS_PROJECTION, &configuration->projectionMatrix8C);
    D3DXVec3Cross(
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraRight30),
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraLookAtOffset0C),
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraUp18));
    D3DXVec3Normalize(
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraRight30),
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraRight30));

    if (g_AnmManager != NULL) {
        g_AnmManager->frameShakeX1C = configuration->screenShakeOffsetE8.x;
        g_AnmManager->frameShakeY20 = configuration->screenShakeOffsetE8.y;
    }
}

void Supervisor::InitializeViewports()
{
    SupervisorViewportView *supervisor =
        reinterpret_cast<SupervisorViewportView *>(&g_Supervisor);

    supervisor->configurations[2].cameraPosition00 =
        Float3(0.0f, 0.0f, 1000.0f);
    supervisor->configurations[2].cameraLookAtOffset0C =
        Float3(0.0f, 0.0f, 0.0f);
    supervisor->configurations[2].cameraUp18 =
        Float3(0.0f, 1.0f, 0.0f);
    supervisor->configurations[2].fieldOfView48 = 3.1415927f / 6.0f;
    supervisor->configurations[2].viewportCC.X = 0;
    supervisor->configurations[2].viewportCC.Y = 0;
    supervisor->configurations[2].viewportCC.Width = 640;
    supervisor->configurations[2].viewportCC.Height = 480;
    supervisor->configurations[2].viewportCC.MinZ = 0.0f;
    supervisor->configurations[2].viewportCC.MaxZ = 1.0f;
    supervisor->configurations[2].viewportModeE4 = 2;
    supervisor->configurations[2].cameraPositionOffset3C =
        Float3(0.0f, 0.0f, 0.0f);

    supervisor->configurations[0].cameraPosition00 =
        Float3(0.0f, 0.0f, 1000.0f);
    supervisor->configurations[0].cameraLookAtOffset0C =
        Float3(0.0f, 0.0f, 0.0f);
    supervisor->configurations[0].cameraUp18 =
        Float3(0.0f, 1.0f, 0.0f);
    supervisor->configurations[0].fieldOfView48 = 3.1415927f / 6.0f;
    supervisor->configurations[0].viewportCC.X = 16;
    supervisor->configurations[0].viewportCC.Y = 16;
    supervisor->configurations[0].viewportCC.Width = 288;
    supervisor->configurations[0].viewportCC.Height = 448;
    supervisor->configurations[0].viewportCC.MinZ = 0.0f;
    supervisor->configurations[0].viewportCC.MaxZ = 1.0f;
    supervisor->configurations[0].viewportModeE4 = 0;
    supervisor->configurations[0].cameraPositionOffset3C =
        Float3(0.0f, 0.0f, 0.0f);

    supervisor->configurations[1].cameraPosition00 =
        Float3(0.0f, 0.0f, 1000.0f);
    supervisor->configurations[1].cameraLookAtOffset0C =
        Float3(0.0f, 0.0f, 0.0f);
    supervisor->configurations[1].cameraUp18 =
        Float3(0.0f, 1.0f, 0.0f);
    supervisor->configurations[1].fieldOfView48 = 3.1415927f / 6.0f;
    supervisor->configurations[1].viewportCC.X = 336;
    supervisor->configurations[1].viewportCC.Y = 16;
    supervisor->configurations[1].viewportCC.Width = 288;
    supervisor->configurations[1].viewportCC.Height = 448;
    supervisor->configurations[1].viewportCC.MinZ = 0.0f;
    supervisor->configurations[1].viewportCC.MaxZ = 1.0f;
    supervisor->configurations[1].viewportModeE4 = 1;
    supervisor->configurations[1].cameraPositionOffset3C =
        Float3(0.0f, 0.0f, 0.0f);
}

// The three view selectors share the real contiguous 0xF0-byte
// camera array. Preserve the source's signed-to-unsigned selection.
#define TH09_VIEWPORT_NATIVE_AT(owner, ordinal) ((owner)->configurations[static_cast<unsigned int>(ordinal)])

void Supervisor::SelectSide(int index)
{
    SupervisorViewportView *supervisor =
        reinterpret_cast<SupervisorViewportView *>(this);
    supervisor->current = &TH09_VIEWPORT_NATIVE_AT(supervisor, index);
}

void Supervisor::ConfigureScreenViewport(int index)
{
    SupervisorViewportView *supervisor =
        reinterpret_cast<SupervisorViewportView *>(this);
    supervisor->current = &TH09_VIEWPORT_NATIVE_AT(supervisor, index);
    supervisor->ApplyScreenCamera(supervisor->current);
    supervisor->d3dDevice->SetViewport(&supervisor->current->viewportCC);
    supervisor->currentIndex = index;
}

void Supervisor::ConfigureGameplayViewport(int index)
{
    SupervisorViewportView *supervisor =
        reinterpret_cast<SupervisorViewportView *>(this);
    supervisor->current = &TH09_VIEWPORT_NATIVE_AT(supervisor, index);
    supervisor->ApplyGameplayViewport(supervisor->current);
    supervisor->d3dDevice->SetViewport(&supervisor->current->viewportCC);
    supervisor->currentIndex = index;
}

void Supervisor::ApplyCameraMode0()
{
    SupervisorViewportView *supervisor =
        reinterpret_cast<SupervisorViewportView *>(this);
    supervisor->ApplyScreenCamera(supervisor->current);
    supervisor->d3dDevice->SetViewport(&supervisor->current->viewportCC);
}

void Supervisor::ApplyCameraMode1()
{
    SupervisorViewportView *supervisor =
        reinterpret_cast<SupervisorViewportView *>(this);
    supervisor->ApplyGameplayViewport(supervisor->current);
    supervisor->d3dDevice->SetViewport(&supervisor->current->viewportCC);
}

#undef TH09_VIEWPORT_NATIVE_AT
