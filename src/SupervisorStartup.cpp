#include "Supervisor.hpp"
#include "GameErrorContext.hpp"
#include "ZunMemory.hpp"

#include <windows.h>
#include <mmsystem.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <string.h>

struct StartupAnmVm
{
    unsigned char unknown000[0x208];
    float x;
    float y;
    float z;
    unsigned char unknown214[0x2A4 - 0x214];

    StartupAnmVm();
};

typedef char StartupAnmVmSizeCheck[(sizeof(StartupAnmVm) == 0x2A4) ? 1 : -1];

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

struct SupervisorStartupLayout
{
    unsigned char unknown000[0x384];
    DummyMidiTimerStartup *dummyMidiTimer;
    unsigned char unknown388[0x442 - 0x388];
    unsigned char musicVolume;
    unsigned char sfxVolume;
    unsigned char unknown444[0x584 - 0x444];
    StartupAnmVm *startupVm;
    unsigned char unknown588[0x5A4 - 0x588];
    int startupField5A4;
    int startupField5A8;
    unsigned char unknown5AC[0x5B8 - 0x5AC];
    float gameSpeed;
    MidiOutputStartup *midiOutput;
    unsigned char unknown5C0[0x5CC - 0x5C0];
    StartupAnmLoaded *textAnm;
    unsigned char unknown5D0[4];
    unsigned int flags;
    unsigned char unknown5D8[0x6B4 - 0x5D8];
    int subthreadCloseRequestActive;
    int subthreadActive;
    int startupThreadState;
    unsigned char unknown6C0[0x798 - 0x6C0];
    DWORD totalPlayTime;
};

typedef char SupervisorStartupFlagsAt5D4[
    (offsetof(SupervisorStartupLayout, flags) == 0x5D4) ? 1 : -1];
typedef char SupervisorStartupStateAt6BC[
    (offsetof(SupervisorStartupLayout, startupThreadState) == 0x6BC) ? 1 : -1];
typedef char SupervisorStartupTimeAt798[
    (offsetof(SupervisorStartupLayout, totalPlayTime) == 0x798) ? 1 : -1];

struct SupervisorDirectInputPrefix
{
    unsigned char unknown000[0x0C];
    IDirectInput8A *directInput;
    IDirectInputDevice8A *keyboard;
    IDirectInputDevice8A *controller0;
    IDirectInputDevice8A *controller1;
    DIDEVCAPS controllerCaps[2];
    HWND window;
};

typedef char SupervisorDirectInputWindowAt74[
    (offsetof(SupervisorDirectInputPrefix, window) == 0x74) ? 1 : -1];

struct SupervisorThreadCloseView
{
    void ThreadClose();
};

struct StartupGameManagerSide
{
    unsigned char unknown000[0x20];
    int value20;
    unsigned char unknown024[0x38 - 0x24];
};

struct StartupGameManager
{
    StartupGameManagerSide sides[2];
};

extern StartupAnmManager *g_AnmManager;
extern SoundPlayerStartup g_SoundPlayer;
extern StartupGameManager g_GameManager;
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
    SupervisorDirectInputPrefix *s =
        reinterpret_cast<SupervisorDirectInputPrefix *>(supervisor);
    HINSTANCE instance = (HINSTANCE)GetWindowLongA(s->window, GWL_HINSTANCE);

    if (supervisor->IsDirectInputDisabled())
        return -1;

    if (DirectInput8Create(instance, 0x800, IID_IDirectInput8A,
                           (void **)&s->directInput, NULL) < 0)
        goto create_error;

    if (s->directInput->CreateDevice(g_KeyboardDeviceGuid, &s->keyboard, NULL) < 0)
    {
        if (s->directInput != NULL)
            s->directInput->Release();
    create_error:
        s->directInput = NULL;
        g_GameErrorContext.Log(g_DirectInputCreateError);
        return -1;
    }

    if (s->keyboard->SetDataFormat(&g_KeyboardDataFormat) < 0)
    {
        if (s->keyboard != NULL)
        {
            s->keyboard->Release();
            s->keyboard = NULL;
        }
        if (s->directInput != NULL)
        {
            s->directInput->Release();
            s->directInput = NULL;
        }
        g_GameErrorContext.Log(g_DirectInputSetFormatError);
        return -1;
    }

    if (s->keyboard->SetCooperativeLevel(s->window, 0x16) < 0)
    {
        if (s->keyboard != NULL)
        {
            s->keyboard->Release();
            s->keyboard = NULL;
        }
        if (s->directInput != NULL)
        {
            s->directInput->Release();
            s->directInput = NULL;
        }
        g_GameErrorContext.Log(g_DirectInputCooperativeError);
        return -1;
    }

    s->keyboard->Acquire();
    g_GameErrorContext.Log(g_DirectInputAcquireMessage);
    s->directInput->EnumDevices(4, StartupEnumControllers, NULL, 1);

    if (s->controller0 != NULL)
    {
        s->controller0->SetDataFormat(&g_ControllerDataFormat);
        s->controller0->SetCooperativeLevel(s->window, 10);
        s->controllerCaps[0].dwSize = sizeof(DIDEVCAPS);
        s->controller0->GetCapabilities(&s->controllerCaps[0]);
        s->controller0->EnumObjects(StartupEnumObjects0, NULL, 0);
        g_GameErrorContext.Log(g_Controller0ConfiguredMessage);
    }

    if (s->controller1 != NULL)
    {
        s->controller1->SetDataFormat(&g_ControllerDataFormat);
        s->controller1->SetCooperativeLevel(s->window, 10);
        s->controllerCaps[1].dwSize = sizeof(DIDEVCAPS);
        s->controller1->GetCapabilities(&s->controllerCaps[1]);
        s->controller1->EnumObjects(StartupEnumObjects1, NULL, 0);
        g_GameErrorContext.Log(g_Controller1ConfiguredMessage);
    }

    return 0;
}

static inline SupervisorStartupLayout *StartupLayout()
{
    return reinterpret_cast<SupervisorStartupLayout *>(&g_Supervisor);
}

static inline void CloseStartupThread()
{
    reinterpret_cast<SupervisorThreadCloseView *>(&g_Supervisor)->ThreadClose();
}

void Supervisor::StartupThread(Supervisor *unused)
{
    (void)unused;
    StartupLayout()->startupField5A8 = 0;
    StartupLayout()->startupField5A4 = 0;
    StartupLayout()->totalPlayTime = timeGetTime();
    SetupDInputInternal(&g_Supervisor);

    if (StartupLayout()->midiOutput == NULL)
    {
        StartupLayout()->midiOutput = static_cast<MidiOutputStartup *>(
            g_ZunMemory.AddToRegistry(new MidiOutputStartup, sizeof(MidiOutputStartup),
                                      "MidiSysInf"));
    }
    if (StartupLayout()->midiOutput != NULL)
        StartupLayout()->midiOutput->ReadFileData(30, "bgm/init.mid");

    g_SoundPlayer.InitSoundBuffers();
    StartupLayout()->textAnm = g_AnmManager->PreloadAnm(0, "text.anm");
    if (StartupLayout()->textAnm == NULL)
        goto error;

    StartupLayout()->startupVm = static_cast<StartupAnmVm *>(
        g_ZunMemory.AddToRegistry(new StartupAnmVm, sizeof(StartupAnmVm), "SprtInf"));
    StartupLayout()->textAnm->InitializeAndSetSprite(StartupLayout()->startupVm, 31);
    StartupLayout()->startupVm->x = 320.0f;
    StartupLayout()->startupVm->y = 240.0f;
    StartupLayout()->startupVm->z = 0.0f;

    if (RegisterAsciiManagerStartup() != 0)
    {
        if (StartupLayout()->subthreadCloseRequestActive)
            return;
        g_GameErrorContext.Log(g_StartupErrorAscii);
        goto error;
    }

    if (PreloadEtamaStartup() != 0)
    {
        if (StartupLayout()->subthreadCloseRequestActive)
            return;
        g_GameErrorContext.Log(g_StartupErrorEtama);
        goto error;
    }

    if (PreloadFrontStartup() != 0)
    {
        if (StartupLayout()->subthreadCloseRequestActive)
            return;
        g_GameErrorContext.Log(g_StartupErrorFront);
        goto error;
    }

    if (g_SoundPlayer.LoadFmt("bgm/thbgm.fmt") != 0)
    {
        if (StartupLayout()->subthreadCloseRequestActive)
            return;
        g_GameErrorContext.Log(g_StartupErrorBgm);
        goto error;
    }

    g_SoundPlayer.bgmVolume = static_cast<signed char>(StartupLayout()->musicVolume);
    g_SoundPlayer.sfxVolume = static_cast<signed char>(StartupLayout()->sfxVolume);

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

    if ((StartupLayout()->flags & 0x40u) != 0)
    {
        StartupLayout()->dummyMidiTimer = static_cast<DummyMidiTimerStartup *>(
            g_ZunMemory.AddToRegistry(new DummyMidiTimerStartup,
                                      sizeof(DummyMidiTimerStartup),
                                      "DummyTimerSysInf"));
        if (StartupLayout()->dummyMidiTimer != NULL)
            StartupLayout()->dummyMidiTimer->StartTimer();
    }

    CloseStartupThread();
    StartupLayout()->startupThreadState = 0;
    g_GameManager.sides[0].value20 = 0;
    g_GameManager.sides[1].value20 = 1;
    StartupLayout()->flags &= ~0x100u;
    return;

error:
    CloseStartupThread();
    StartupLayout()->startupThreadState = 2;
    StartupLayout()->flags |= 0x80u;
}
