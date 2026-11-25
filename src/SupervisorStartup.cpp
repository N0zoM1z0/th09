#include "Supervisor.hpp"
#include "Midi.hpp"
#include "AnmManager.hpp"
#include "AnmLoaded.hpp"
#include "AnmVmLifecycle.hpp"
#include "SoundPlayer.hpp"
#include "GameManagerRuntimeView.hpp"
#include "GameErrorContext.hpp"
#include "ZunMemory.hpp"

#include <windows.h>
#include <mmsystem.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <string.h>

typedef char StartupAnmVmSizeCheck[
    (sizeof(AnmVm) == 0x2A4 &&
     offsetof(AnmVmBase, pos) == 0x208) ? 1 : -1];

// Startup and OnUpdate share the authentic 0x7B0-byte Supervisor.
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

// The startup owner uses the actual two 0x38-byte GameManager sides.
// Its source-facing manager type is the shared native AsciiGameManagerView.

typedef char StartupNativeGameSideAt20[
    (sizeof(AsciiGameManagerSideView) == 0x38 &&
     offsetof(AsciiGameManagerSideView, value20) == 0x20 &&
     offsetof(AsciiGameManagerView, sides) == 0) ? 1 : -1];

extern GameErrorContext g_GameErrorContext;

extern int RegisterAsciiManagerStartup();
extern int PreloadEtamaStartup();
extern int PreloadFrontStartup();
extern int InitializeScoreDataStartup();

extern const GUID g_KeyboardDeviceGuid = {
    0x6F1D2B61u, 0xD5A0u, 0x11CFu,
    { 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 }
};
extern const DIDATAFORMAT g_KeyboardDataFormat;
extern const DIDATAFORMAT g_ControllerDataFormat;
extern BOOL CALLBACK StartupEnumControllers(const DIDEVICEINSTANCEA *, void *);
extern BOOL CALLBACK StartupEnumObjects0(const DIDEVICEOBJECTINSTANCEA *, void *);
extern BOOL CALLBACK StartupEnumObjects1(const DIDEVICEOBJECTINSTANCEA *, void *);

extern const char g_DirectInputCreateError[] = "DirectInput \x82\xAA\x8E\x67\x97\x70\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\r\n"; // DirectInput が使用できません\r\n
extern const char g_DirectInputSetFormatError[] = "DirectInput SetDataFormat \x82\xAA\x8E\x67\x97\x70\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\r\n"; // DirectInput SetDataFormat が使用できません\r\n
extern const char g_DirectInputCooperativeError[] = "DirectInput SetCooperativeLevel \x82\xAA\x8E\x67\x97\x70\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\r\n"; // DirectInput SetCooperativeLevel が使用できません\r\n
extern const char g_DirectInputAcquireMessage[] = "DirectInput \x82\xCD\x90\xB3\x8F\xED\x82\xC9\x8F\x89\x8A\xFA\x89\xBB\x82\xB3\x82\xEA\x82\xDC\x82\xB5\x82\xBD\r\n"; // DirectInput は正常に初期化されました\r\n
extern const char g_Controller0ConfiguredMessage[] = "\x97\x4C\x8C\xF8\x82\xC8\x83\x70\x83\x62\x83\x68\x82\xF0\x94\xAD\x8C\xA9\x82\xB5\x82\xDC\x82\xB5\x82\xBD\r\n"; // 有効なパッドを発見しました\r\n
extern const char g_Controller1ConfiguredMessage[] = "\x82\xE0\x82\xD0\x82\xC6\x82\xC2\x97\x4C\x8C\xF8\x82\xC8\x83\x70\x83\x62\x83\x68\x82\xF0\x94\xAD\x8C\xA9\x82\xB5\x82\xDC\x82\xB5\x82\xBD\r\n"; // もひとつ有効なパッドを発見しました\r\n
extern const char g_StartupErrorAscii[] = "error : \x95\xB6\x8E\x9A\x82\xCC\x8F\x89\x8A\xFA\x89\xBB\x82\xC9\x8E\xB8\x94\x73\x82\xB5\x82\xDC\x82\xB5\x82\xBD\r\n"; // error : 文字の初期化に失敗しました\r\n
extern const char g_StartupErrorEtama[] = "error : \x93\x47\x92\x65\x82\xCC\x8F\x89\x8A\xFA\x89\xBB\x82\xC9\x8E\xB8\x94\x73\x82\xB5\x82\xDC\x82\xB5\x82\xBD\r\n"; // error : 敵弾の初期化に失敗しました\r\n
extern const char g_StartupErrorFront[] = "error : \x95\x5C\x8E\xA6\x95\xA8\x82\xCC\x8F\x89\x8A\xFA\x89\xBB\x82\xC9\x8E\xB8\x94\x73\x82\xB5\x82\xDC\x82\xB5\x82\xBD\r\n"; // error : 表示物の初期化に失敗しました\r\n
extern const char g_StartupErrorBgm[] = "error : BGM \x82\xCC\x8F\x89\x8A\xFA\x89\xBB\x82\xC9\x8E\xB8\x94\x73\x82\xB5\x82\xDC\x82\xB5\x82\xBD\r\n"; // error : BGM の初期化に失敗しました\r\n

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
        g_Supervisor.midiOutput5BC = static_cast<MidiOutput *>(
            g_ZunMemory.AddToRegistry(new MidiOutput, sizeof(MidiOutput),
                                      "MidiSysInf"));
    }
    if (g_Supervisor.midiOutput5BC != NULL)
        g_Supervisor.midiOutput5BC->ReadFileData(30, "bgm/init.mid");

    g_SoundPlayer.InitSoundBuffers();
    g_Supervisor.textAnm5CC = g_AnmManager->PreloadAnm(0, "text.anm");
    if (g_Supervisor.textAnm5CC == NULL)
        goto error;

    g_Supervisor.startupVm584 = static_cast<AnmVm *>(
        g_ZunMemory.AddToRegistry(new AnmVm, sizeof(AnmVm), "SprtInf"));
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

    if (reinterpret_cast<SoundPlayerLoadFmtView *>(&g_SoundPlayer)->LoadFmt("bgm/thbgm.fmt") != 0)
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
            g_SoundPlayer.StartBGM(const_cast<char *>("thbgm.dat"));
        else
            strcpy(g_SoundPlayer.currentBgmFileName, "thbgm.dat");
    }
    else
    {
        if (!g_Supervisor.IsMusicPreloaded())
            g_SoundPlayer.StartBGM(const_cast<char *>("th09.dat"));
        else
            strcpy(g_SoundPlayer.currentBgmFileName, "th09.dat");
    }

    InitializeScoreDataStartup();

    if ((g_Supervisor.flags & 0x40u) != 0)
    {
        g_Supervisor.dummyMidiTimer384 = static_cast<DummyMidiTimer *>(
            g_ZunMemory.AddToRegistry(new DummyMidiTimer,
                                      sizeof(DummyMidiTimer),
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
