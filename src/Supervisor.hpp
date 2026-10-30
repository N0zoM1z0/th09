#pragma once

#include "GameConfiguration.hpp"

#include <stddef.h>

struct IDirect3D8;
struct IDirect3DDevice8;

struct SupervisorCtorArrayElement
{
    unsigned char unknown000[0xF0];

    // Declaration only: the target callback is a shared/folded three-byte
    // constructor whose original owner and source mapping remain unresolved.
    SupervisorCtorArrayElement();
};

typedef char SupervisorCtorArrayElementSizeCheck[
    (sizeof(SupervisorCtorArrayElement) == 0xF0) ? 1 : -1];

class Supervisor
{
  public:
    Supervisor();
    static int OnUpdate(Supervisor *supervisor);
    static int AddedCallback(Supervisor *supervisor);
    static void StartupThread(Supervisor *unused);
    void TickTimer(int *current, float *subFrame);
    int TakeSnapshot(const char *filePath);
    int LoadConfig(char *configFile);
    int IsFogDisabled();
    int DisableFog();
    int EnableFog();
    void SetRenderState(int state, int value);
    int Is16BitTexturesForced();
    int IsWindowed();
    int IsReferenceRasterizerForced();
    int IsDirectInputDisabled();
    int IsMusicPreloaded();
    int PlayMusic(int trackId, int unused);
    int PlayAudio(char *path, int bgmUnlockIndex);
    int LoadMusic(int trackId);
    int StartLoadedMusic();
    int ReleaseMusic(int trackId);
    int StopAudio();
    int FadeOutMusic(float durationSeconds);
    int IsVsyncDisabled();
    int IsTextBackgroundDetectionDisabled();
    void InitializeViewports();
    void ConfigureGameplayViewport(int index);
    void ConfigureScreenViewport(int index);
    void SelectSide(int index);
    void ApplyCameraMode0();
    void ApplyCameraMode1();
    void EnterCriticalSectionWrapper(int id);
    void LeaveCriticalSectionWrapper(int id);

    unsigned char unknown000[0x04];
    IDirect3D8 *d3d04;
    IDirect3DDevice8 *d3dDevice08;
    unsigned char unknown00C[0x78 - 0x0C];
    SupervisorCtorArrayElement ctorArray078[3];
    unsigned char unknown348[0x40];
    GameConfiguration config388;
    unsigned char unknown454[0x164];
    float framerateMultiplier5B8;
    unsigned char unknown5BC[0x18];
    volatile unsigned int flags5D4;
    unsigned char unknown5D8[0x1D8];
};

typedef char SupervisorD3DAt04[
    (offsetof(Supervisor, d3d04) == 0x04) ? 1 : -1];
typedef char SupervisorDeviceAt08[
    (offsetof(Supervisor, d3dDevice08) == 0x08) ? 1 : -1];
typedef char SupervisorCtorArrayAt078[
    (offsetof(Supervisor, ctorArray078) == 0x78) ? 1 : -1];
typedef char SupervisorConfigAt388[
    (offsetof(Supervisor, config388) == 0x388) ? 1 : -1];
typedef char SupervisorFrameMultiplierAt5B8[
    (offsetof(Supervisor, framerateMultiplier5B8) == 0x5B8) ? 1 : -1];
typedef char SupervisorFlagsAt5D4[
    (offsetof(Supervisor, flags5D4) == 0x5D4) ? 1 : -1];
typedef char SupervisorSizeIs7B0[(sizeof(Supervisor) == 0x7B0) ? 1 : -1];

extern Supervisor g_Supervisor;
