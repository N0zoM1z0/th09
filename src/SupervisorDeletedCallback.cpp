#include "Supervisor.hpp"
// This TU preserves the original DeletedGameManagerLayout global symbol type.
// Import only the native side layout, without redeclaring the other global.
#define TH09_GAME_MANAGER_TYPES_ONLY
#include "GameManagerRuntimeView.hpp"
#undef TH09_GAME_MANAGER_TYPES_ONLY
#include "SoundPlayer.hpp"
#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "ZunMemory.hpp"

#include <stddef.h>
#include <new>
#include <stdlib.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace
{
struct DeletedAnmManagerView
{
    void ReleaseVertexBuffer();
    void ReleaseAnm(int slot);
    void ReleaseSurface(int slot);
};

struct DeletedMidiOutputView
{
    void PlayFile(int index);
};

struct DeletedDummyMidiTimerView
{
    void StopTimer();
    void DeleteObject(int deleting);
};

struct DeletedPbgArchiveView
{
    void Release();
};

struct DeletedReplayStateView
{
    void Release();
};

// The deleted callback's distinct ThreadClose symbol also receives the
// same original Supervisor object; no separate receiver layout exists.
struct DeletedSupervisorThreadView : Supervisor
{
    void ThreadClose();
};

// Both the exact startup caller and this full 405-byte delete callback
// own the same original Supervisor device, MIDI, startup VM and version
// fields. Keep the separate MIDI/ThreadClose method identities below.
typedef char DeletedDummyTimerAt384[
    (offsetof(Supervisor, dummyMidiTimer384) == 0x384) ? 1 : -1];
typedef char DeletedMusicModeAt436[
    (offsetof(Supervisor, config388) +
         offsetof(GameConfiguration, musicMode) == 0x436) ? 1 : -1];
typedef char DeletedStartupVmAt584[
    (offsetof(Supervisor, startupVm584) == 0x584) ? 1 : -1];
typedef char DeletedMidiAt5BC[
    (offsetof(Supervisor, midiOutput5BC) == 0x5BC) ? 1 : -1];
typedef char DeletedVersionAt7AC[
    (offsetof(Supervisor, versionData7AC) == 0x7AC) ? 1 : -1];
typedef char DeletedDevicesAt0C[
    (offsetof(Supervisor, directInput0C) == 0x0C &&
     offsetof(Supervisor, keyboard10) == 0x10 &&
     offsetof(Supervisor, controller014) == 0x14 &&
     offsetof(Supervisor, controller118) == 0x18) ? 1 : -1];

// The deleted callback releases the +0x1C score/phase allocations from
// each original 0x38 GameManager side, as used by the exact replay owners.
// Retain the original DeletedGameManagerLayout C++ global symbol identity.
struct DeletedGameManagerLayout : AsciiGameManagerView
{
};
typedef char DeletedGameManagerNativeSideAllocationOffsets[
    (offsetof(AsciiGameManagerSideView, runtime) == 0x1C &&
     offsetof(AsciiGameManagerView, sides) == 0 &&
     sizeof(AsciiGameManagerSideView) == 0x38) ? 1 : -1];


}

extern DeletedGameManagerLayout g_GameManager;
extern DeletedPbgArchiveView g_PbgArchive;
extern DeletedReplayStateView *g_DeletedReplayState;

extern void SupervisorDeletedReleaseTextBuffer();
extern int SaveTitleScoreData();
extern void SupervisorDeletedReleaseSubsystemB();
extern void SupervisorDeletedReleaseSubsystemC();

int SupervisorDeletedCallback(Supervisor *supervisor)
{
    Supervisor *globalSupervisor = &g_Supervisor;
    Supervisor *callbackSupervisor = supervisor;
    reinterpret_cast<DeletedSupervisorThreadView *>(&g_Supervisor)->ThreadClose();

    if (globalSupervisor->versionData7AC != NULL)
    {
        g_ZunMemory.Free(globalSupervisor->versionData7AC);
        globalSupervisor->versionData7AC = NULL;
    }

    if (g_GameManager.sides[0].runtime != NULL)
        operator delete(g_GameManager.sides[0].runtime);
    g_GameManager.sides[0].runtime = NULL;

    if (g_GameManager.sides[1].runtime != NULL)
        operator delete(g_GameManager.sides[1].runtime);
    g_GameManager.sides[1].runtime = NULL;

    if (globalSupervisor->startupVm584 != NULL)
    {
        operator delete(globalSupervisor->startupVm584);
        globalSupervisor->startupVm584 = NULL;
    }

    reinterpret_cast<DeletedAnmManagerView *>(g_AnmManager)->ReleaseVertexBuffer();
    reinterpret_cast<DeletedAnmManagerView *>(g_AnmManager)->ReleaseAnm(0);
    reinterpret_cast<DeletedAnmManagerView *>(g_AnmManager)->ReleaseAnm(2);
    reinterpret_cast<DeletedAnmManagerView *>(g_AnmManager)->ReleaseSurface(8);

    AsciiManager::CutChain();
    g_SoundPlayer.QueueCommand(4, 0, "dummy");

    if (globalSupervisor->config388.musicMode == 2 && globalSupervisor->midiOutput5BC != NULL)
        reinterpret_cast<DeletedMidiOutputView *>(globalSupervisor->midiOutput5BC)->PlayFile(30);

    g_DeletedReplayState->Release();
    g_DeletedReplayState = NULL;
    SupervisorDeletedReleaseTextBuffer();

    if (callbackSupervisor->keyboard10 != NULL)
        callbackSupervisor->keyboard10->Unacquire();
    if (callbackSupervisor->keyboard10 != NULL)
    {
        callbackSupervisor->keyboard10->Release();
        callbackSupervisor->keyboard10 = NULL;
    }

    if (callbackSupervisor->controller014 != NULL)
        callbackSupervisor->controller014->Unacquire();
    if (callbackSupervisor->controller118 != NULL)
        callbackSupervisor->controller118->Unacquire();
    if (callbackSupervisor->controller014 != NULL)
    {
        callbackSupervisor->controller014->Release();
        callbackSupervisor->controller014 = NULL;
    }
    if (callbackSupervisor->controller118 != NULL)
    {
        callbackSupervisor->controller118->Release();
        callbackSupervisor->controller118 = NULL;
    }
    if (callbackSupervisor->directInput0C != NULL)
    {
        callbackSupervisor->directInput0C->Release();
        callbackSupervisor->directInput0C = NULL;
    }

    g_PbgArchive.Release();

    if (globalSupervisor->dummyMidiTimer384 != NULL)
    {
        reinterpret_cast<DeletedDummyMidiTimerView *>(globalSupervisor->dummyMidiTimer384)->StopTimer();
        if (globalSupervisor->dummyMidiTimer384 != NULL)
            reinterpret_cast<DeletedDummyMidiTimerView *>(globalSupervisor->dummyMidiTimer384)->DeleteObject(1);
        globalSupervisor->dummyMidiTimer384 = NULL;
    }

    SaveTitleScoreData();
    SupervisorDeletedReleaseSubsystemB();
    SupervisorDeletedReleaseSubsystemC();
    return 0;
}
