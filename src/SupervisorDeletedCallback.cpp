#include "Supervisor.hpp"
#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "ZunMemory.hpp"

#include <stddef.h>
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

struct DeletedSoundPlayerView
{
    void QueueCommand(int opcode, int argument, char *path);
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

struct SupervisorDeletedGlobalLayout
{
    unsigned char unknown000[0x384];
    DeletedDummyMidiTimerView *dummyMidiTimer;
    unsigned char unknown388[0x436 - 0x388];
    unsigned char musicMode;
    unsigned char unknown437[0x584 - 0x437];
    void *startupVm;
    unsigned char unknown588[0x5BC - 0x588];
    DeletedMidiOutputView *midiOutput;
    unsigned char unknown5C0[0x7AC - 0x5C0];
    void *versionData;
};

struct SupervisorDeletedArgumentLayout
{
    unsigned char unknown000[0x0C];
    IDirectInput8A *directInput;
    IDirectInputDevice8A *keyboard;
    IDirectInputDevice8A *controller0;
    IDirectInputDevice8A *controller1;
};

struct DeletedGameManagerLayout
{
    unsigned char unknown000[0x1C];
    void *heap1C;
    unsigned char unknown020[0x54 - 0x20];
    void *heap54;
};

typedef char DeletedDummyTimerAt384[(offsetof(SupervisorDeletedGlobalLayout, dummyMidiTimer) == 0x384) ? 1 : -1];
typedef char DeletedMusicModeAt436[(offsetof(SupervisorDeletedGlobalLayout, musicMode) == 0x436) ? 1 : -1];
typedef char DeletedStartupVmAt584[(offsetof(SupervisorDeletedGlobalLayout, startupVm) == 0x584) ? 1 : -1];
typedef char DeletedMidiAt5BC[(offsetof(SupervisorDeletedGlobalLayout, midiOutput) == 0x5BC) ? 1 : -1];
typedef char DeletedVersionAt7AC[(offsetof(SupervisorDeletedGlobalLayout, versionData) == 0x7AC) ? 1 : -1];
}

extern DeletedSoundPlayerView g_SoundPlayer;
extern DeletedGameManagerLayout g_GameManager;
extern DeletedPbgArchiveView g_PbgArchive;
extern DeletedReplayStateView *g_DeletedReplayState;

extern void SupervisorDeletedReleaseTextBuffer();
extern void SupervisorDeletedReleaseSubsystemA();
extern void SupervisorDeletedReleaseSubsystemB();
extern void SupervisorDeletedReleaseSubsystemC();

int SupervisorDeletedCallback(Supervisor *supervisor)
{
    SupervisorDeletedGlobalLayout *globalSupervisor =
        reinterpret_cast<SupervisorDeletedGlobalLayout *>(&g_Supervisor);
    SupervisorDeletedArgumentLayout *callbackSupervisor =
        reinterpret_cast<SupervisorDeletedArgumentLayout *>(supervisor);
    reinterpret_cast<DeletedSupervisorThreadView *>(&g_Supervisor)->ThreadClose();

    if (globalSupervisor->versionData != NULL)
    {
        g_ZunMemory.Free(globalSupervisor->versionData);
        globalSupervisor->versionData = NULL;
    }

    if (g_GameManager.heap1C != NULL)
        free(g_GameManager.heap1C);
    g_GameManager.heap1C = NULL;

    if (g_GameManager.heap54 != NULL)
        free(g_GameManager.heap54);
    g_GameManager.heap54 = NULL;

    if (globalSupervisor->startupVm != NULL)
    {
        free(globalSupervisor->startupVm);
        globalSupervisor->startupVm = NULL;
    }

    reinterpret_cast<DeletedAnmManagerView *>(g_AnmManager)->ReleaseVertexBuffer();
    reinterpret_cast<DeletedAnmManagerView *>(g_AnmManager)->ReleaseAnm(0);
    reinterpret_cast<DeletedAnmManagerView *>(g_AnmManager)->ReleaseAnm(2);
    reinterpret_cast<DeletedAnmManagerView *>(g_AnmManager)->ReleaseSurface(8);

    AsciiManager::CutChain();
    g_SoundPlayer.QueueCommand(4, 0, "dummy");

    if (globalSupervisor->musicMode == 2 && globalSupervisor->midiOutput != NULL)
        globalSupervisor->midiOutput->PlayFile(30);

    g_DeletedReplayState->Release();
    g_DeletedReplayState = NULL;
    SupervisorDeletedReleaseTextBuffer();

    if (callbackSupervisor->keyboard != NULL)
        callbackSupervisor->keyboard->Unacquire();
    if (callbackSupervisor->keyboard != NULL)
    {
        callbackSupervisor->keyboard->Release();
        callbackSupervisor->keyboard = NULL;
    }

    if (callbackSupervisor->controller0 != NULL)
        callbackSupervisor->controller0->Unacquire();
    if (callbackSupervisor->controller1 != NULL)
        callbackSupervisor->controller1->Unacquire();
    if (callbackSupervisor->controller0 != NULL)
    {
        callbackSupervisor->controller0->Release();
        callbackSupervisor->controller0 = NULL;
    }
    if (callbackSupervisor->controller1 != NULL)
    {
        callbackSupervisor->controller1->Release();
        callbackSupervisor->controller1 = NULL;
    }
    if (callbackSupervisor->directInput != NULL)
    {
        callbackSupervisor->directInput->Release();
        callbackSupervisor->directInput = NULL;
    }

    g_PbgArchive.Release();

    if (globalSupervisor->dummyMidiTimer != NULL)
    {
        globalSupervisor->dummyMidiTimer->StopTimer();
        if (globalSupervisor->dummyMidiTimer != NULL)
            globalSupervisor->dummyMidiTimer->DeleteObject(1);
        globalSupervisor->dummyMidiTimer = NULL;
    }

    SupervisorDeletedReleaseSubsystemA();
    SupervisorDeletedReleaseSubsystemB();
    SupervisorDeletedReleaseSubsystemC();
    return 0;
}
