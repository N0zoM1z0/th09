#include "Supervisor.hpp"
#include "AnmVmLifecycle.hpp"
#define TH09_GAME_MANAGER_TYPES_ONLY
#include "GameManagerRuntimeView.hpp"
#undef TH09_GAME_MANAGER_TYPES_ONLY
#include "GameErrorContext.hpp"
#include "ZunMemory.hpp"

#include <windows.h>
#include <mmsystem.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <string.h>

// Preserve the original StartupAnmVm constructor C++ symbol while using
// the same 0x2A4-byte native ANM VM as the full ExecuteScript interpreter.
// Its +0x208 XYZ words are already the target-observed AnmVmBase::pos.
struct StartupAnmVm : AnmVm
{
    StartupAnmVm();
};

typedef char StartupAnmVmSizeCheck[
    (sizeof(StartupAnmVm) == 0x2A4 &&
     offsetof(AnmVmBase, pos) == 0x208) ? 1 : -1];

struct StartupAnmLoaded
{
    void InitializeAndSetSprite(StartupAnmVm *vm, int spriteIndex);
};

struct StartupAnmManager
{
    StartupAnmLoaded *PreloadAnm(int slot, const char *filename);
};

struct MidiOutputStartup
{
    unsigned char unknown000[0x300];

    MidiOutputStartup();
    void ReadFileData(int index, const char *filename);
};

struct DummyMidiTimerStartup
{
    unsigned char unknown000[0x14];

    DummyMidiTimerStartup();
    void StartTimer();
};

struct SoundPlayerStartup
{
    unsigned char unknown000[0x6108];
    char currentBgmFileName[0x10C];
    int bgmFileBaseOffset;
    int bgmVolume;
    int sfxVolume;
    int unconsumedBgmAttenuation;

    void InitSoundBuffers();
    int LoadFmt(const char *filename);
    void StartBGM(const char *filename);
};

// Startup and OnUpdate share the authentic 0x7B0-byte Supervisor.
// Keep distinct unresolved StartupAnmVm/StartupAnmLoaded method ABIs.
typedef Supervisor SupervisorStartupLayout;

typedef char SupervisorStartupFlagsAt5D4[
    (offsetof(SupervisorStartupLayout, flags) == 0x5D4) ? 1 : -1];
typedef char SupervisorStartupStateAt6BC[
    (offsetof(SupervisorStartupLayout, startupThreadState) == 0x6BC) ? 1 : -1];
typedef char SupervisorStartupTimeAt798[
    (offsetof(SupervisorStartupLayout, totalPlayTime) == 0x798) ? 1 : -1];

// The same native Supervisor owns DirectInput devices at +0x0C..+0x18,
// two 0x2C DIDEVCAPS at +0x1C and the real HWND at +0x74.
typedef char SupervisorDirectInputWindowAt74[
    (offsetof(Supervisor, window74) == 0x74 &&
     sizeof(DIDEVCAPS) == 0x2C) ? 1 : -1];
typedef char SupervisorDirectInputNativeCapsArray[
    (sizeof(((Supervisor *)0)->controllerCaps1C) ==
     2 * sizeof(DIDEVCAPS)) ? 1 : -1];

// Preserve the original helper's decorated method name while making its
// receiver a zero-offset view of the real Supervisor object.
struct SupervisorThreadCloseView : Supervisor
{
    void ThreadClose();
};

// The startup owner uses the actual two 0x38-byte GameManager sides.
// Its source-facing manager type is the shared native AsciiGameManagerView.

typedef char StartupNativeGameSideAt20[
    (sizeof(AsciiGameManagerSideView) == 0x38 &&
     offsetof(AsciiGameManagerSideView, value20) == 0x20 &&
     offsetof(AsciiGameManagerView, sides) == 0) ? 1 : -1];

extern StartupAnmManager *g_AnmManager;
extern SoundPlayerStartup g_SoundPlayer;
extern AsciiGameManagerView g_GameManager;
extern GameErrorContext g_GameErrorContext;

extern int RegisterAsciiManagerStartup();
extern int PreloadEtamaStartup();
extern int PreloadFrontStartup();
extern int InitializeScoreDataStartup();

extern const GUID g_KeyboardDeviceGuid;
extern const DIDATAFORMAT g_KeyboardDataFormat;
extern const DIDATAFORMAT g_ControllerDataFormat;
extern BOOL CALLBACK StartupEnumControllers(const DIDEVICEINSTANCEA *, void *);
extern BOOL CALLBACK StartupEnumObjects0(const DIDEVICEOBJECTINSTANCEA *, void *);
extern BOOL CALLBACK StartupEnumObjects1(const DIDEVICEOBJECTINSTANCEA *, void *);

extern const char g_DirectInputCreateError[];
extern const char g_DirectInputSetFormatError[];
extern const char g_DirectInputCooperativeError[];
extern const char g_DirectInputAcquireMessage[];
extern const char g_Controller0ConfiguredMessage[];
extern const char g_Controller1ConfiguredMessage[];
extern const char g_StartupErrorAscii[];
extern const char g_StartupErrorEtama[];
extern const char g_StartupErrorFront[];
extern const char g_StartupErrorBgm[];

static int SetupDInputInternal(Supervisor *supervisor)
{
    Supervisor *s = supervisor;
    HINSTANCE instance = (HINSTANCE)GetWindowLongA(s->window74, GWL_HINSTANCE);
    // The two adjacent 0x2C-byte capability records are the real native
    // DIDEVCAPS array stored in Supervisor's +0x1C controller window.
    DIDEVCAPS *controllerCaps =
        reinterpret_cast<DIDEVCAPS *>(&g_Supervisor.controllerCaps1C[0][0]);

    if (supervisor->IsDirectInputDisabled())
        return -1;

    if (DirectInput8Create(instance, 0x800, IID_IDirectInput8A,
                           (void **)&s->directInput0C, NULL) < 0)
    {
        s->directInput0C = NULL;
        g_GameErrorContext.Log(g_DirectInputCreateError);
        return -1;
    }

    if (s->directInput0C->CreateDevice(g_KeyboardDeviceGuid, &s->keyboard10, NULL) < 0)
    {
        if (s->directInput0C != NULL)
        {
            s->directInput0C->Release();
            s->directInput0C = NULL;
        }
        g_GameErrorContext.Log(g_DirectInputCreateError);
        return -1;
    }

    if (s->keyboard10->SetDataFormat(&g_KeyboardDataFormat) < 0)
    {
        if (s->keyboard10 != NULL)
        {
            s->keyboard10->Release();
            s->keyboard10 = NULL;
        }
        if (s->directInput0C != NULL)
        {
            s->directInput0C->Release();
            s->directInput0C = NULL;
        }
        g_GameErrorContext.Log(g_DirectInputSetFormatError);
        return -1;
    }

    if (s->keyboard10->SetCooperativeLevel(s->window74, 0x16) < 0)
    {
        if (s->keyboard10 != NULL)
        {
            s->keyboard10->Release();
            s->keyboard10 = NULL;
        }
        if (s->directInput0C != NULL)
        {
            s->directInput0C->Release();
            s->directInput0C = NULL;
        }
        g_GameErrorContext.Log(g_DirectInputCooperativeError);
        return -1;
    }

    s->keyboard10->Acquire();
    g_GameErrorContext.Log(g_DirectInputAcquireMessage);
    s->directInput0C->EnumDevices(4, StartupEnumControllers, NULL, 1);

    if (s->controller014 != NULL)
    {
        s->controller014->SetDataFormat(&g_ControllerDataFormat);
        s->controller014->SetCooperativeLevel(s->window74, 10);
        controllerCaps[0].dwSize = sizeof(DIDEVCAPS);
        s->controller014->GetCapabilities(&controllerCaps[0]);
        s->controller014->EnumObjects(StartupEnumObjects0, NULL, 0);
        g_GameErrorContext.Log(g_Controller0ConfiguredMessage);
    }

    if (s->controller118 != NULL)
    {
        s->controller118->SetDataFormat(&g_ControllerDataFormat);
        s->controller118->SetCooperativeLevel(s->window74, 10);
        controllerCaps[1].dwSize = sizeof(DIDEVCAPS);
        s->controller118->GetCapabilities(&controllerCaps[1]);
        s->controller118->EnumObjects(StartupEnumObjects1, NULL, 0);
        g_GameErrorContext.Log(g_Controller1ConfiguredMessage);
    }

    return 0;
}

static inline void CloseStartupThread()
{
    reinterpret_cast<SupervisorThreadCloseView *>(&g_Supervisor)->ThreadClose();
}

void Supervisor::StartupThread(Supervisor *unused)
{
    (void)unused;
    g_Supervisor.startupField5A8 = 0;
    g_Supervisor.startupField5A4 = 0;
    g_Supervisor.totalPlayTime = timeGetTime();
    SetupDInputInternal(&g_Supervisor);

    if (g_Supervisor.midiOutput5BC == NULL)
    {
        g_Supervisor.midiOutput5BC = static_cast<MidiOutputStartup *>(
            g_ZunMemory.AddToRegistry(new MidiOutputStartup, sizeof(MidiOutputStartup),
                                      "MidiSysInf"));
    }
    if (g_Supervisor.midiOutput5BC != NULL)
        g_Supervisor.midiOutput5BC->ReadFileData(30, "bgm/init.mid");

    g_SoundPlayer.InitSoundBuffers();
    g_Supervisor.textAnm5CC = g_AnmManager->PreloadAnm(0, "text.anm");
    if (g_Supervisor.textAnm5CC == NULL)
        goto error;

    g_Supervisor.startupVm584 = static_cast<StartupAnmVm *>(
        g_ZunMemory.AddToRegistry(new StartupAnmVm, sizeof(StartupAnmVm), "SprtInf"));
    g_Supervisor.textAnm5CC->InitializeAndSetSprite(g_Supervisor.startupVm584, 31);
    g_Supervisor.startupVm584->pos.x = 320.0f;
    g_Supervisor.startupVm584->pos.y = 240.0f;
    g_Supervisor.startupVm584->pos.z = 0.0f;

    if (RegisterAsciiManagerStartup() != 0)
    {
        if (g_Supervisor.subthreadCloseRequestActive)
            return;
        g_GameErrorContext.Log(g_StartupErrorAscii);
        goto error;
    }

    if (PreloadEtamaStartup() != 0)
    {
        if (g_Supervisor.subthreadCloseRequestActive)
            return;
        g_GameErrorContext.Log(g_StartupErrorEtama);
        goto error;
    }

    if (PreloadFrontStartup() != 0)
    {
        if (g_Supervisor.subthreadCloseRequestActive)
            return;
        g_GameErrorContext.Log(g_StartupErrorFront);
        goto error;
    }

    if (g_SoundPlayer.LoadFmt("bgm/thbgm.fmt") != 0)
    {
        if (g_Supervisor.subthreadCloseRequestActive)
            return;
        g_GameErrorContext.Log(g_StartupErrorBgm);
        goto error;
    }

    g_SoundPlayer.bgmVolume = static_cast<signed char>(g_Supervisor.config388.musicVolume);
    g_SoundPlayer.sfxVolume = static_cast<signed char>(g_Supervisor.config388.sfxVolume);

    float bgmVolume = g_SoundPlayer.bgmVolume * 0.01f;
    if (g_SoundPlayer.sfxVolume != 0)
    {
        bgmVolume = 1.0f - bgmVolume;
        bgmVolume *= bgmVolume;
        bgmVolume *= bgmVolume;
        bgmVolume = 1.0f - bgmVolume;
        g_SoundPlayer.unconsumedBgmAttenuation =
            -5000 - static_cast<int>(bgmVolume * -5000.0f);
    }
    else
    {
        g_SoundPlayer.unconsumedBgmAttenuation = -10000;
    }

    if (g_SoundPlayer.bgmFileBaseOffset == 0)
    {
        if (!g_Supervisor.IsMusicPreloaded())
            g_SoundPlayer.StartBGM("thbgm.dat");
        else
            strcpy(g_SoundPlayer.currentBgmFileName, "thbgm.dat");
    }
    else
    {
        if (!g_Supervisor.IsMusicPreloaded())
            g_SoundPlayer.StartBGM("th09.dat");
        else
            strcpy(g_SoundPlayer.currentBgmFileName, "th09.dat");
    }

    InitializeScoreDataStartup();

    if ((g_Supervisor.flags & 0x40u) != 0)
    {
        g_Supervisor.dummyMidiTimer384 = static_cast<DummyMidiTimerStartup *>(
            g_ZunMemory.AddToRegistry(new DummyMidiTimerStartup,
                                      sizeof(DummyMidiTimerStartup),
                                      "DummyTimerSysInf"));
        if (g_Supervisor.dummyMidiTimer384 != NULL)
            g_Supervisor.dummyMidiTimer384->StartTimer();
    }

    CloseStartupThread();
    g_Supervisor.startupThreadState = 0;
    g_GameManager.sides[0].value20 = 0;
    g_GameManager.sides[1].value20 = 1;
    g_Supervisor.flags &= ~0x100u;
    return;

error:
    CloseStartupThread();
    g_Supervisor.startupThreadState = 2;
    g_Supervisor.flags |= 0x80u;
}
