#include "Supervisor.hpp"
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

struct DeletedSupervisorThreadView
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

struct DeletedGameManagerLayout
{
    unsigned char unknown000[0x1C];
    void *heap1C;
    unsigned char unknown020[0x54 - 0x20];
    void *heap54;
};


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

    if (g_GameManager.heap1C != NULL)
        operator delete(g_GameManager.heap1C);
    g_GameManager.heap1C = NULL;

    if (g_GameManager.heap54 != NULL)
        operator delete(g_GameManager.heap54);
    g_GameManager.heap54 = NULL;

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
