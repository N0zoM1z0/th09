#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "Supervisor.hpp"

#include <stddef.h>
#include <math.h>
#include <windows.h>
#include <d3d8.h>

struct D3DXVECTOR3 : public D3DVECTOR {};
struct D3DXMATRIX : public D3DMATRIX {};

D3DXMATRIX *WINAPI D3DXMatrixLookAtLH(
    D3DXMATRIX *out,
    const D3DXVECTOR3 *eye,
    const D3DXVECTOR3 *at,
    const D3DXVECTOR3 *up);
D3DXMATRIX *WINAPI D3DXMatrixPerspectiveFovLH(
    D3DXMATRIX *out, float fovY, float aspect, float nearZ, float farZ);
D3DXVECTOR3 *__fastcall D3DXVec3Cross(
    D3DXVECTOR3 *out, const D3DXVECTOR3 *first, const D3DXVECTOR3 *second);
D3DXVECTOR3 *WINAPI D3DXVec3Normalize(
    D3DXVECTOR3 *out, const D3DXVECTOR3 *vector);
float __cdecl SupervisorCameraTan(float value);

struct SupervisorViewportConfiguration
{
    Float3 cameraPosition;
    Float3 cameraLookAtOffset;
    Float3 cameraUp;
    Float3 cameraForward;
    Float3 cameraRight;
    Float3 cameraPositionOffset;
    float fieldOfView;
    D3DXMATRIX viewMatrix;
    D3DXMATRIX projectionMatrix;
    D3DVIEWPORT8 viewport;
    int viewportMode;
    Float2 screenShakeOffset;
};

typedef char SupervisorViewportConfigurationSizeIsF0[
    (sizeof(SupervisorViewportConfiguration) == 0xF0) ? 1 : -1];
typedef char SupervisorViewportCameraPositionAt00[
    (offsetof(SupervisorViewportConfiguration, cameraPosition) == 0x00) ? 1 : -1];
typedef char SupervisorViewportCameraLookAtAt0C[
    (offsetof(SupervisorViewportConfiguration, cameraLookAtOffset) == 0x0C) ? 1 : -1];
typedef char SupervisorViewportCameraUpAt18[
    (offsetof(SupervisorViewportConfiguration, cameraUp) == 0x18) ? 1 : -1];
typedef char SupervisorViewportCameraRightAt30[
    (offsetof(SupervisorViewportConfiguration, cameraRight) == 0x30) ? 1 : -1];
typedef char SupervisorViewportCameraOffsetAt3C[
    (offsetof(SupervisorViewportConfiguration, cameraPositionOffset) == 0x3C) ? 1 : -1];
typedef char SupervisorViewportFovAt48[
    (offsetof(SupervisorViewportConfiguration, fieldOfView) == 0x48) ? 1 : -1];
typedef char SupervisorViewportViewMatrixAt4C[
    (offsetof(SupervisorViewportConfiguration, viewMatrix) == 0x4C) ? 1 : -1];
typedef char SupervisorViewportProjectionAt8C[
    (offsetof(SupervisorViewportConfiguration, projectionMatrix) == 0x8C) ? 1 : -1];
typedef char SupervisorViewportAtCC[
    (offsetof(SupervisorViewportConfiguration, viewport) == 0xCC) ? 1 : -1];
typedef char SupervisorViewportShakeAtE8[
    (offsetof(SupervisorViewportConfiguration, screenShakeOffset) == 0xE8) ? 1 : -1];

struct SupervisorViewportAnmManagerView
{
    unsigned char unknown000[0x1C];
    Float2 screenShakeOffset;
};

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

    viewportMiddleWidth = (float)configuration->viewport.Width / 2.0f;
    viewportMiddleHeight = (float)configuration->viewport.Height / 2.0f;
    aspectRatio = (float)configuration->viewport.Width /
                  (float)configuration->viewport.Height;
    fov = 3.1415927f / 10.0f;
    D3DXMatrixLookAtLH(
        &configuration->viewMatrix,
        reinterpret_cast<D3DXVECTOR3 *>(
            &Float3(viewportMiddleWidth, viewportMiddleHeight,
                    viewportMiddleHeight / SupervisorCameraTan(fov / 2.0f))),
        reinterpret_cast<D3DXVECTOR3 *>(
            &Float3(viewportMiddleWidth, viewportMiddleHeight, 0.0f)),
        reinterpret_cast<D3DXVECTOR3 *>(&Float3(0.0f, -1.0f, 0.0f)));
    D3DXMatrixPerspectiveFovLH(
        &configuration->projectionMatrix,
        fov,
        aspectRatio,
        1.0f,
        10000.0f);
    reinterpret_cast<SupervisorViewportView *>(&g_Supervisor)
        ->d3dDevice->SetTransform(D3DTS_VIEW, &configuration->viewMatrix);
    reinterpret_cast<SupervisorViewportView *>(&g_Supervisor)
        ->d3dDevice->SetTransform(
            D3DTS_PROJECTION, &configuration->projectionMatrix);

    if (g_AnmManager != NULL) {
        reinterpret_cast<SupervisorViewportAnmManagerView *>(g_AnmManager)
            ->screenShakeOffset = configuration->screenShakeOffset;
    }
}

void SupervisorViewportView::ApplyGameplayViewport(
    SupervisorViewportConfiguration *configuration)
{
    if (g_AnmManager != NULL)
        g_AnmManager->FlushVertexBuffer();

    Float3 lookAt = configuration->cameraLookAtOffset +
                    configuration->cameraPosition;
    Float3 eye = configuration->cameraPositionOffset +
                 configuration->cameraPosition;

    D3DXMatrixLookAtLH(
        &configuration->viewMatrix,
        reinterpret_cast<D3DXVECTOR3 *>(&eye),
        reinterpret_cast<D3DXVECTOR3 *>(&lookAt),
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraUp));
    D3DXMatrixPerspectiveFovLH(
        &configuration->projectionMatrix,
        configuration->fieldOfView,
        (float)configuration->viewport.Width /
            (float)configuration->viewport.Height,
        30.0f,
        1800.0f);
    reinterpret_cast<SupervisorViewportView *>(&g_Supervisor)
        ->d3dDevice->SetTransform(D3DTS_VIEW, &configuration->viewMatrix);
    reinterpret_cast<SupervisorViewportView *>(&g_Supervisor)
        ->d3dDevice->SetTransform(
            D3DTS_PROJECTION, &configuration->projectionMatrix);
    D3DXVec3Cross(
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraRight),
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraLookAtOffset),
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraUp));
    D3DXVec3Normalize(
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraRight),
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraRight));

    if (g_AnmManager != NULL) {
        reinterpret_cast<SupervisorViewportAnmManagerView *>(g_AnmManager)
            ->screenShakeOffset = configuration->screenShakeOffset;
    }
}

void Supervisor::InitializeViewports()
{
    SupervisorViewportView *supervisor =
        reinterpret_cast<SupervisorViewportView *>(&g_Supervisor);

    supervisor->configurations[2].cameraPosition =
        Float3(0.0f, 0.0f, 1000.0f);
    supervisor->configurations[2].cameraLookAtOffset =
        Float3(0.0f, 0.0f, 0.0f);
    supervisor->configurations[2].cameraUp =
        Float3(0.0f, 1.0f, 0.0f);
    supervisor->configurations[2].fieldOfView = 3.1415927f / 6.0f;
    supervisor->configurations[2].viewport.X = 0;
    supervisor->configurations[2].viewport.Y = 0;
    supervisor->configurations[2].viewport.Width = 640;
    supervisor->configurations[2].viewport.Height = 480;
    supervisor->configurations[2].viewport.MinZ = 0.0f;
    supervisor->configurations[2].viewport.MaxZ = 1.0f;
    supervisor->configurations[2].viewportMode = 2;
    supervisor->configurations[2].cameraPositionOffset =
        Float3(0.0f, 0.0f, 0.0f);

    supervisor->configurations[0].cameraPosition =
        Float3(0.0f, 0.0f, 1000.0f);
    supervisor->configurations[0].cameraLookAtOffset =
        Float3(0.0f, 0.0f, 0.0f);
    supervisor->configurations[0].cameraUp =
        Float3(0.0f, 1.0f, 0.0f);
    supervisor->configurations[0].fieldOfView = 3.1415927f / 6.0f;
    supervisor->configurations[0].viewport.X = 16;
    supervisor->configurations[0].viewport.Y = 16;
    supervisor->configurations[0].viewport.Width = 288;
    supervisor->configurations[0].viewport.Height = 448;
    supervisor->configurations[0].viewport.MinZ = 0.0f;
    supervisor->configurations[0].viewport.MaxZ = 1.0f;
    supervisor->configurations[0].viewportMode = 0;
    supervisor->configurations[0].cameraPositionOffset =
        Float3(0.0f, 0.0f, 0.0f);

    supervisor->configurations[1].cameraPosition =
        Float3(0.0f, 0.0f, 1000.0f);
    supervisor->configurations[1].cameraLookAtOffset =
        Float3(0.0f, 0.0f, 0.0f);
    supervisor->configurations[1].cameraUp =
        Float3(0.0f, 1.0f, 0.0f);
    supervisor->configurations[1].fieldOfView = 3.1415927f / 6.0f;
    supervisor->configurations[1].viewport.X = 336;
    supervisor->configurations[1].viewport.Y = 16;
    supervisor->configurations[1].viewport.Width = 288;
    supervisor->configurations[1].viewport.Height = 448;
    supervisor->configurations[1].viewport.MinZ = 0.0f;
    supervisor->configurations[1].viewport.MaxZ = 1.0f;
    supervisor->configurations[1].viewportMode = 1;
    supervisor->configurations[1].cameraPositionOffset =
        Float3(0.0f, 0.0f, 0.0f);
}

void Supervisor::ConfigureGameplayViewport(int index)
{
    SupervisorViewportView *supervisor =
        reinterpret_cast<SupervisorViewportView *>(this);
    supervisor->current = &supervisor->configurations[index];
    supervisor->ApplyGameplayViewport(supervisor->current);
    supervisor->d3dDevice->SetViewport(&supervisor->current->viewport);
    supervisor->currentIndex = index;
}

void Supervisor::ApplyCameraMode0()
{
    SupervisorViewportView *supervisor =
        reinterpret_cast<SupervisorViewportView *>(this);
    supervisor->ApplyScreenCamera(supervisor->current);
    supervisor->d3dDevice->SetViewport(&supervisor->current->viewport);
}

void Supervisor::ApplyCameraMode1()
{
    SupervisorViewportView *supervisor =
        reinterpret_cast<SupervisorViewportView *>(this);
    supervisor->ApplyGameplayViewport(supervisor->current);
    supervisor->d3dDevice->SetViewport(&supervisor->current->viewport);
}
