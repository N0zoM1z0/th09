#pragma once

#include "GameConfiguration.hpp"
#include "Float3.hpp"
#include "SupervisorFrameQueueStorageView.hpp"
#include <d3d8.h>

#include <stddef.h>

struct IDirect3D8;
struct IDirect3DDevice8;
struct IDirectInput8A;
struct IDirectInputDevice8A;
struct HWND__;
struct AnmLoaded;
struct StartupAnmVm;
struct AnmVm;
struct StartupAnmLoaded;
struct MidiOutputStartup;
struct DummyMidiTimerStartup;

struct SupervisorCtorArrayElement
{
    // D3D8 viewport state also written by InitializeD3DRendering and consumed
    // by the original gameplay/screen camera update owners.
    Float3 cameraPosition00;
    Float3 cameraLookAtOffset0C;
    Float3 cameraUp18;
    Float3 cameraForward24;
    Float3 cameraRight30;
    Float3 cameraPositionOffset3C;
    float fieldOfView48;
    D3DMATRIX viewMatrix4C;
    D3DMATRIX projectionMatrix8C;
    D3DVIEWPORT8 viewportCC;
    int viewportModeE4;
    union
    {
        Float2 screenShakeOffsetE8;
        struct { float frameOffsetX; float frameOffsetY; };
    };

    // Declaration only: the target callback is a shared/folded three-byte
    // constructor whose original owner and source mapping remain unresolved.
    SupervisorCtorArrayElement();
};

typedef char SupervisorCtorFrameOffsetsAtE8[
    (offsetof(SupervisorCtorArrayElement, cameraPosition00) == 0x00 &&
     offsetof(SupervisorCtorArrayElement, cameraLookAtOffset0C) == 0x0C &&
     offsetof(SupervisorCtorArrayElement, cameraUp18) == 0x18 &&
     offsetof(SupervisorCtorArrayElement, fieldOfView48) == 0x48 &&
     offsetof(SupervisorCtorArrayElement, viewMatrix4C) == 0x4C &&
     offsetof(SupervisorCtorArrayElement, projectionMatrix8C) == 0x8C &&
     offsetof(SupervisorCtorArrayElement, viewportCC) == 0xCC &&
     offsetof(SupervisorCtorArrayElement, viewportModeE4) == 0xE4 &&
     offsetof(SupervisorCtorArrayElement, screenShakeOffsetE8) == 0xE8 &&
     offsetof(SupervisorCtorArrayElement, frameOffsetX) == 0xE8 &&
     offsetof(SupervisorCtorArrayElement, frameOffsetY) == 0xEC) ? 1 : -1];

typedef char SupervisorCtorArrayElementSizeCheck[
    (sizeof(SupervisorCtorArrayElement) == 0xF0) ? 1 : -1];

// Original network service code reads this contiguous eight-dword interval
// from the shared Supervisor, directly before the native frame queues.
struct SupervisorNetworkFrameTimingView
{
    int frameCounter;
    int confirmedFrame;
    int frameMode;
    unsigned int frameStartTime;
    unsigned int lastFrameTime;
    unsigned int waitTime;
    unsigned char unknown470[4];
    unsigned int connectionTime;
};
typedef char SupervisorNetworkFrameTimingSizeIs20[
    (sizeof(SupervisorNetworkFrameTimingView) == 0x20) ? 1 : -1];

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
    // The original queue producer/consumer methods receive this exact
    // Supervisor object; +0x47C/+0x56C own their physical frame records.
    void InsertReceivedFrame(int side, int frame, unsigned short packedInput, short seed);
    void InsertPredictedFrame(int side, int frame, short seed);
    unsigned short PopFrame(int side, unsigned short *seed, unsigned int *predicted);
    int AreFrameQueuesSynchronized(int side);

    unsigned char unknown000[0x04];
    IDirect3D8 *d3d04;
    IDirect3DDevice8 *d3dDevice08;
    IDirectInput8A *directInput0C;
    IDirectInputDevice8A *keyboard10;
    // Two independent exact DirectInput owners bind the same original
    // +0x14/+0x18 controller window. The indexed controller polling path
    // consumes these records as one native 2-element i386 pointer array.
    union
    {
        struct
        {
            IDirectInputDevice8A *controller014;
            IDirectInputDevice8A *controller118;
        };
        IDirectInputDevice8A *controllers14[2];
    };
    unsigned char controllerCaps1C[2][0x2C];
    HWND__ *window74;
    SupervisorCtorArrayElement ctorArray078[3];
    // Shared D3D8 presentation record: the window, FPS, capture and surface
    // loaders all refer to these 52 bytes of the same Supervisor allocation.
    unsigned char unknown348[0x350 - 0x348];
    D3DPRESENT_PARAMETERS presentParameters350;
    DummyMidiTimerStartup *dummyMidiTimer384;
    GameConfiguration config388;
    // These slots are independently observed by the original 1,230-byte
    // GameManager callback and the 214-byte Supervisor loading-VM owner.
    // Twenty frame queue records plus two last-received entries have their
    // own exact original-bound push/pop owners. The complete Supervisor
    // OnUpdate boss also reads these same +0x47C/+0x56C arrays.
    int calcCount454;
    union {
        unsigned char unknown458[0x478 - 0x458];
        SupervisorNetworkFrameTimingView networkTiming458;
    };
    union { int field478; int networkBusy478; };
    SupervisorFrameEntryStorageView entries47C[20];
    SupervisorFrameEntryStorageView lastReceived56C[2];
    union { StartupAnmVm *startupVm584; AnmVm *networkMessageVm584; };
    union { unsigned char unknown588[0x58C - 0x588]; unsigned int networkConnectStartTime588; };
    int wantedState58C;
    union { int state590; int curState590; };
    union { int value594; int wantedState2; };
    int value598;
    int value59C;
    unsigned char unknown5A0[4];
    union { int screenTransitionCountdown; int startupField5A4; };
    union { int startupField5A8; int suppressFpsDisplay; };
    int disableVsync;
    // D3D initialization publishes both refresh negotiation and the last
    // measured frame tick into this actual Supervisor storage window.
    int couldSetRefreshRate5B0;
    int lastFrameTime5B4;
    union { float framerateMultiplier5B8; float gameSpeed; };
    MidiOutputStartup *midiOutput5BC;
    unsigned char unknown5C0[0x5CC - 0x5C0];
    StartupAnmLoaded *textAnm5CC;
    union { AnmLoaded *loadingAnm5D0; void *loadingAnm; };
    union { volatile unsigned int flags5D4; unsigned int flags; signed char flagsLow5D4; };
    // The retail renderer writes the complete D3D8 caps record here.
    D3DCAPS8 d3dCaps5D8;
    // Win32 thread methods use the same +0x6AC handle cell as game setup's
    // raw state resets. Preserve both native representations at one address.
    union { int runningSubthreadHandle; void *runningSubthreadHandlePtr; };
    union { unsigned char unknown6B0[4]; unsigned long runningSubthreadId6B0; };
    int subthreadCloseRequestActive;
    int subthreadActive;
    union { int startupThreadState6BC; int startupThreadState; };
    unsigned char unknown6C0[0x740 - 0x6C0];
    int loadingVmsHaveBeenSetup;
    // The Title result/FPs callback observes six native status dwords in
    // this exact +0x77C..+0x793 window. Keep the raw cover for other owners.
    union
    {
        unsigned char unknown744[0x794 - 0x744];
        struct
        {
            unsigned char unknown744_77C[0x77C - 0x744];
            int recordingFpsWarning77C;
            int playbackFpsWarning780;
            int recordingFpsState784;
            int recordingFpsState788;
            int recordingFpsState78C;
            int recordingFpsState790;
        };
    };
    int frameState794;
    unsigned int totalPlayTime;
    unsigned int systemTime;
    int exeChecksum7A0;
    int exeSize7A4;
    // The exact version-data loader publishes the length at +0x7A8;
    // the exact version checker consumes that same native four-byte slot.
    union { unsigned char unknown7A8[4]; unsigned int versionDataSize7A8; };
    void *versionData7AC;
};

typedef char SupervisorD3DAt04[
    (offsetof(Supervisor, d3d04) == 0x04) ? 1 : -1];
typedef char SupervisorDeviceAt08[
    (offsetof(Supervisor, d3dDevice08) == 0x08) ? 1 : -1];
typedef char SupervisorCtorArrayAt078[
    (offsetof(Supervisor, ctorArray078) == 0x78) ? 1 : -1];
typedef char SupervisorNativePresentAt350[
    (offsetof(Supervisor, presentParameters350) == 0x350 &&
     sizeof(((Supervisor *)0)->presentParameters350) == 0x34 &&
     offsetof(Supervisor, presentParameters350.BackBufferFormat) == 0x358 &&
     offsetof(Supervisor, dummyMidiTimer384) == 0x384 &&
     offsetof(Supervisor, screenTransitionCountdown) == 0x5A4 &&
     offsetof(Supervisor, disableVsync) == 0x5AC &&
     offsetof(Supervisor, flags5D4) == 0x5D4) ? 1 : -1];
typedef char SupervisorConfigAt388[
    (offsetof(Supervisor, config388) == 0x388) ? 1 : -1];
typedef char SupervisorFrameMultiplierAt5B8[
    (offsetof(Supervisor, framerateMultiplier5B8) == 0x5B8) ? 1 : -1];
typedef char SupervisorNativeD3DInitFields[
    (offsetof(Supervisor, couldSetRefreshRate5B0) == 0x5B0 &&
     offsetof(Supervisor, lastFrameTime5B4) == 0x5B4 &&
     offsetof(Supervisor, d3dCaps5D8) == 0x5D8 &&
     sizeof(D3DCAPS8) == 0xD4) ? 1 : -1];
typedef char SupervisorFlagsAt5D4[
    (offsetof(Supervisor, flags5D4) == 0x5D4) ? 1 : -1];
typedef char SupervisorSizeIs7B0[(sizeof(Supervisor) == 0x7B0) ? 1 : -1];
// Original deleted-callback tail and config music mode are also part of this
// same global Supervisor, not a separate +0x7AC lifetime view.
typedef char SupervisorNativeVersionSuffix[
    (sizeof(Supervisor) == 0x7B0 &&
     offsetof(Supervisor, versionDataSize7A8) == 0x7A8 &&
     offsetof(Supervisor, versionData7AC) == 0x7AC &&
     offsetof(Supervisor, exeSize7A4) == 0x7A4 &&
     offsetof(Supervisor, exeChecksum7A0) == 0x7A0) ? 1 : -1];
typedef char SupervisorNativeDeletedFields[
    (offsetof(Supervisor, versionData7AC) == 0x7AC &&
     offsetof(Supervisor, config388) +
        offsetof(GameConfiguration, musicMode) == 0x436) ? 1 : -1];

typedef char SupervisorNativeDInputFields[
    (offsetof(Supervisor, directInput0C) == 0x0C &&
     offsetof(Supervisor, keyboard10) == 0x10 &&
     offsetof(Supervisor, controller014) == 0x14 &&
     offsetof(Supervisor, controller118) == 0x18 &&
     offsetof(Supervisor, controllers14) == 0x14 &&
     sizeof(((Supervisor *)0)->controllers14) == 8 &&
     offsetof(Supervisor, controllerCaps1C) == 0x1C &&
     sizeof(((Supervisor *)0)->controllerCaps1C[0]) == 0x2C &&
     offsetof(Supervisor, window74) == 0x74 &&
     offsetof(Supervisor, ctorArray078) == 0x78) ? 1 : -1];

typedef char SupervisorNativeStartupAndAddedFields[
    (offsetof(Supervisor, dummyMidiTimer384) == 0x384 &&
     offsetof(Supervisor, startupVm584) == 0x584 &&
     offsetof(Supervisor, startupField5A4) == 0x5A4 &&
     offsetof(Supervisor, startupField5A8) == 0x5A8 &&
     offsetof(Supervisor, suppressFpsDisplay) == 0x5A8 &&
     offsetof(Supervisor, disableVsync) == 0x5AC &&
     offsetof(Supervisor, gameSpeed) == 0x5B8 &&
     offsetof(Supervisor, midiOutput5BC) == 0x5BC &&
     offsetof(Supervisor, textAnm5CC) == 0x5CC &&
     offsetof(Supervisor, loadingAnm) == 0x5D0 &&
     offsetof(Supervisor, startupThreadState) == 0x6BC) ? 1 : -1];

// Assert the entire network service's recovered slot ownership, including
// the two 0x0C*10 side queues and two published last-received records.
typedef char SupervisorNativeNetworkServiceFields[
    (offsetof(Supervisor, networkTiming458) == 0x458 &&
     offsetof(Supervisor, networkTiming458) +
         offsetof(SupervisorNetworkFrameTimingView, frameMode) == 0x460 &&
     offsetof(Supervisor, networkTiming458) +
         offsetof(SupervisorNetworkFrameTimingView, connectionTime) == 0x474 &&
     offsetof(Supervisor, networkBusy478) == 0x478 &&
     offsetof(Supervisor, entries47C) == 0x47C &&
     sizeof(((Supervisor *)0)->entries47C) == 0xF0 &&
     offsetof(Supervisor, lastReceived56C) == 0x56C &&
     sizeof(((Supervisor *)0)->lastReceived56C) == 0x18 &&
     offsetof(Supervisor, networkMessageVm584) == 0x584 &&
     offsetof(Supervisor, networkConnectStartTime588) == 0x588 &&
     offsetof(Supervisor, flagsLow5D4) == 0x5D4) ? 1 : -1];

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
