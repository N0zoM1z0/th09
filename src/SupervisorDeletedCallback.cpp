#include "Supervisor.hpp"
#include "SupervisorThreadStartView.hpp"
#include "Midi.hpp"
#include "GameManagerRuntimeView.hpp"
#include "PbgArchive.hpp"
#include "ReplayManagerRuntimeView.hpp"
#include "SoundPlayer.hpp"
#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "TextHelper.hpp"
#include "ZunMemory.hpp"

#include <stddef.h>
#include <new>
#include <stdlib.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace
{
struct DeletedMidiOutputView
{
    void PlayFile(int index);
};

// Both the exact startup caller and this full 405-byte delete callback
// own the same original Supervisor device, MIDI, startup VM and version
// fields. Keep the separate non-inline MIDI PlayFile declaration above.
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

typedef char DeletedGameManagerNativeSideAllocationOffsets[
    (offsetof(AsciiGameManagerSideView, runtime) == 0x1C &&
     offsetof(AsciiGameManagerView, sides) == 0 &&
     sizeof(AsciiGameManagerSideView) == 0x38) ? 1 : -1];
}

extern ReplayManagerView *g_DeletedReplayState;

extern int SaveTitleScoreData();
extern void ReleaseEtamaAnm();
extern void SupervisorDeletedReleaseSubsystemC();

int SupervisorDeletedCallback(Supervisor *supervisor)
{
    Supervisor *globalSupervisor = &g_Supervisor;
    Supervisor *callbackSupervisor = supervisor;
    reinterpret_cast<SupervisorThreadStartView *>(&g_Supervisor)->ThreadClose();

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

    g_AnmManager->ReleaseVertexBuffer();
    g_AnmManager->ReleaseAnm(0);
    g_AnmManager->ReleaseAnm(2);
    g_AnmManager->ReleaseSurface(8);

    AsciiManager::CutChain();
    g_SoundPlayer.QueueCommand(4, 0, "dummy");

    if (globalSupervisor->config388.musicMode == 2 && globalSupervisor->midiOutput5BC != NULL)
        reinterpret_cast<DeletedMidiOutputView *>(globalSupervisor->midiOutput5BC)->PlayFile(30);

    g_DeletedReplayState->Release();
    g_DeletedReplayState = NULL;
    TextHelper::ReleaseTextBuffer();

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
        globalSupervisor->dummyMidiTimer384->StopTimer();
        delete globalSupervisor->dummyMidiTimer384;
        globalSupervisor->dummyMidiTimer384 = NULL;
    }

    SaveTitleScoreData();
    ReleaseEtamaAnm();
    SupervisorDeletedReleaseSubsystemC();
    return 0;
}
