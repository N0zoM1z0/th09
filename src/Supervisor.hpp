#pragma once

#include "GameConfiguration.hpp"
#include "SupervisorFrameQueueStorageView.hpp"

#include <stddef.h>

struct IDirect3D8;
struct IDirect3DDevice8;
struct AnmLoaded;

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
    // These slots are independently observed by the original 1,230-byte
    // GameManager callback and the 214-byte Supervisor loading-VM owner.
    // Twenty frame queue records plus two last-received entries have their
    // own exact original-bound push/pop owners. The complete Supervisor
    // OnUpdate boss also reads these same +0x47C/+0x56C arrays.
    int calcCount454;
    unsigned char unknown458[0x478 - 0x458];
    int field478;
    SupervisorFrameEntryStorageView entries47C[20];
    SupervisorFrameEntryStorageView lastReceived56C[2];
    unsigned char unknown584[0x58C - 0x584];
    int wantedState58C;
    union { int state590; int curState590; };
    union { int value594; int wantedState2; };
    int value598;
    int value59C;
    unsigned char unknown5A0[4];
    int screenTransitionCountdown;
    unsigned char unknown5A8[0x5B8 - 0x5A8];
    float framerateMultiplier5B8;
    unsigned char unknown5BC[0x5D0 - 0x5BC];
    AnmLoaded *loadingAnm5D0;
    union { volatile unsigned int flags5D4; unsigned int flags; signed char flagsLow5D4; };
    unsigned char unknown5D8[0x6AC - 0x5D8];
    int runningSubthreadHandle;
    unsigned char unknown6B0[4];
    int subthreadCloseRequestActive;
    int subthreadActive;
    int startupThreadState6BC;
    unsigned char unknown6C0[0x740 - 0x6C0];
    int loadingVmsHaveBeenSetup;
    unsigned char unknown744[0x794 - 0x744];
    int frameState794;
    unsigned int totalPlayTime;
    unsigned int systemTime;
    int exeChecksum7A0;
    int exeSize7A4;
    unsigned char unknown7A8[0x7B0 - 0x7A8];
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
typedef char SupervisorNativeUpdateFields[
    (offsetof(Supervisor, calcCount454) == 0x454 &&
     offsetof(Supervisor, field478) == 0x478 &&
     offsetof(Supervisor, entries47C) == 0x47C &&
     offsetof(Supervisor, lastReceived56C) == 0x56C &&
     offsetof(Supervisor, wantedState58C) == 0x58C &&
     offsetof(Supervisor, curState590) == 0x590 &&
     offsetof(Supervisor, wantedState2) == 0x594 &&
     offsetof(Supervisor, flagsLow5D4) == 0x5D4 &&
     offsetof(Supervisor, startupThreadState6BC) == 0x6BC &&
     offsetof(Supervisor, frameState794) == 0x794 &&
     offsetof(Supervisor, exeChecksum7A0) == 0x7A0 &&
     offsetof(Supervisor, exeSize7A4) == 0x7A4) ? 1 : -1];

typedef char SupervisorNativeSetupFields[
    (offsetof(Supervisor, state590) == 0x590 &&
     offsetof(Supervisor, value598) == 0x598 &&
     offsetof(Supervisor, screenTransitionCountdown) == 0x5A4 &&
     offsetof(Supervisor, loadingAnm5D0) == 0x5D0 &&
     offsetof(Supervisor, flags) == 0x5D4 &&
     offsetof(Supervisor, runningSubthreadHandle) == 0x6AC &&
     offsetof(Supervisor, subthreadCloseRequestActive) == 0x6B4 &&
     offsetof(Supervisor, subthreadActive) == 0x6B8 &&
     offsetof(Supervisor, loadingVmsHaveBeenSetup) == 0x740 &&
     offsetof(Supervisor, totalPlayTime) == 0x798 &&
     offsetof(Supervisor, systemTime) == 0x79C) ? 1 : -1];

extern Supervisor g_Supervisor;
