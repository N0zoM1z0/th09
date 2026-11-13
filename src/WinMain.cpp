// Target-bound TH09 startup reconstruction. The Main* types below are narrow
// views used to preserve target-observed object geometry and ABI; they do not
// claim original source-level class names or complete layouts.
//
// This translation unit deliberately keeps CreateGameWindow file-local. The
// real WinMain caller is what makes VC7.1 choose the target private EBX entry
// transport naturally; no dummy caller or register forcing is used.

#include "GameConfiguration.hpp"
#include "GameWindow.hpp"
#include "SoundPlayer.hpp"
#include "Chain.hpp"
#define TH09_ANM_MANAGER_LIFECYCLE_LAYOUT
#include "AnmManager.hpp"
#undef TH09_ANM_MANAGER_LIFECYCLE_LAYOUT
#include "Supervisor.hpp"
#include "GameErrorContext.hpp"
#include "Midi.hpp"
#include "SupervisorNetworkState.hpp"
#include "ZunMemory.hpp"
#include "RngRuntimeLeaves.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <objbase.h>
#include <d3d8.h>
#include <string.h>
#include <new>
#include <stdlib.h>

extern "C" BOOL WINAPI WINNLSEnableIME(HWND, BOOL);
extern "C" __declspec(dllimport) HRESULT WINAPI CoInitializeEx(LPVOID, DWORD);

// Startup allocates the original fully laid out 0x2B2934-byte AnmManager,
// not an independent giant byte-storage alias.
typedef char WinMainNativeAnmManagerSize[
    (sizeof(AnmManager) == 0x2B2934) ? 1 : -1];

// The main loop uses the actual process-global SoundPlayer with its
// 0x6224-byte allocation, already bound by the full queue/audio owners.
// Use the actual window-closure DWORD in the same 0x44-byte GameWindow
// allocation as Render/Present, rather than a byte-prefix type overlay.
typedef char WinMainNativeGameWindowLayout[
    (sizeof(GameWindow) == 0x44 &&
     offsetof(GameWindow, windowIsClosing04) == 0x04 &&
     offsetof(GameWindow, performanceFrequency14) == 0x14 &&
     offsetof(GameWindow, lastFrameTime3C) == 0x3C) ? 1 : -1];

// Real Supervisor source allocation and all member/callee identities are
// independently shared with D3D startup, network and critical-section owners.
typedef char WinMainNativeSupervisorView[
    (offsetof(Supervisor, instance00) == 0x00 &&
     offsetof(Supervisor, d3d04) == 0x04 &&
     offsetof(Supervisor, d3dDevice08) == 0x08 &&
     offsetof(Supervisor, presentParameters350) == 0x350 &&
     offsetof(Supervisor, config388) == 0x388 &&
     offsetof(Supervisor, screenTransitionCountdown) == 0x5A4 &&
     offsetof(Supervisor, flags) == 0x5D4) ? 1 : -1];

extern MidiOutput *g_MainMidiOutput;
extern Chain g_Chain;
extern RngRuntimeView g_ReplayRng;
extern ZunMemory g_ZunMemory;
extern GameErrorContext g_GameErrorContext;

extern char g_ExecutableDirectory[MAX_PATH];
extern int g_ScreenSaverActive;
extern int g_LowPowerActive;
extern int g_PowerOffActive;
extern int g_WindowActivateWParam;
extern int g_WindowInactive;
extern HWND g_SupervisorGameWindow;
extern const char g_WindowTitle[];

extern const char g_MainLogStart[];
extern const char g_DirectPlayInitFatal[];
extern const char g_OptionRestartLog[];

LRESULT CALLBACK GameWindowProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void __stdcall ActivateGameWindow(HWND hWnd);

int __fastcall CheckForRunningGameInstance();
int __fastcall ComputeExecutableChecksum();
int SupervisorInitializeD3D();
int __fastcall InitializeD3DRendering();
void __fastcall ResetRenderState();
int SupervisorRegisterChain();
void __fastcall PrepareTitleMode4Network(void *networkState);
void __fastcall SanitizeKeyboardState();

namespace Controller
{
unsigned short __fastcall GetJoystickCaps();
}

namespace FileSystem
{
int __fastcall WriteDataToFile(const char *path, void *data, size_t size);
}

// The original signed system-metric products and frame/path counters
// use i386 modulo-32 arithmetic before their existing signed tests.
static int CreateGameWindow(HINSTANCE instance)
{
    WNDCLASSA baseClass;
    int width;
    int height;

    ZeroMemory(&baseClass, sizeof(baseClass));
    baseClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    baseClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    baseClass.hInstance = instance;
    baseClass.lpfnWndProc = GameWindowProc;
    g_WindowActivateWParam = TRUE;
    g_WindowInactive = FALSE;
    baseClass.lpszClassName = "BASE";
    RegisterClassA(&baseClass);

    HWND window;
    if (!g_Supervisor.IsWindowed())
    {
        width = 640;
        height = 480;
        window = CreateWindowExA(
            0, "BASE", g_WindowTitle, WS_OVERLAPPEDWINDOW,
            0, 0, width, height, NULL, NULL, instance, NULL);
    }
    else
    {
        width = static_cast<int>(
            static_cast<unsigned int>(GetSystemMetrics(SM_CXDLGFRAME)) * 2u + 640u);
        height = static_cast<int>(
            static_cast<unsigned int>(GetSystemMetrics(SM_CYDLGFRAME)) * 2u +
            static_cast<unsigned int>(GetSystemMetrics(SM_CYCAPTION)) + 480u);
        window = CreateWindowExA(
            0, "BASE", g_WindowTitle,
            WS_VISIBLE | WS_MINIMIZEBOX | WS_SYSMENU,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            NULL, NULL, instance, NULL);
    }

    g_GameWindow.window00 = window;
    g_SupervisorGameWindow = window;
    if (window == NULL)
        return 1;

    ActivateGameWindow(window);
    return 0;
}

// Retail executable-path scanning uses an i386 DWORD address even when
// strlen(path)-1 is signed. Do not turn the original initial byte read into
// a checked C++ array access or cache the live characters across the loop.
#define TH09_NATIVE_EXE_PATH_CHAR(index) \
    (*reinterpret_cast<char *>( \
        reinterpret_cast<unsigned int>(&g_ExecutableDirectory[0]) + \
        static_cast<unsigned int>(index)))

int WINAPI WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR commandLine, int showCommand)
{
    MSG message;
    int renderResult = 0;

    if (GetModuleFileNameA(NULL, g_ExecutableDirectory, MAX_PATH) != 0)
    {
        int pathIndex = (int)strlen(g_ExecutableDirectory) - 1;
        if (TH09_NATIVE_EXE_PATH_CHAR(pathIndex) != '\\')
        {
            do
            {
                if (TH09_NATIVE_EXE_PATH_CHAR(pathIndex) == '/')
                    break;
                if (pathIndex <= 0)
                    goto executable_path_fallback;
                --*reinterpret_cast<unsigned int *>(&pathIndex);
            }
            while (TH09_NATIVE_EXE_PATH_CHAR(pathIndex) != '\\');
        }

        if (pathIndex <= 0)
        {
        executable_path_fallback:
            strcpy(g_ExecutableDirectory, "./");
        }
        else
        {
            TH09_NATIVE_EXE_PATH_CHAR(pathIndex) = '/';
            TH09_NATIVE_EXE_PATH_CHAR(static_cast<unsigned int>(pathIndex) + 1u) = '\0';
        }
    }
    else
    {
        strcpy(g_ExecutableDirectory, "./");
    }

    g_Supervisor.instance00 = hInstance;

    SystemParametersInfoA(
        SPI_GETSCREENSAVEACTIVE, 0, &g_ScreenSaverActive, 0);
    SystemParametersInfoA(
        SPI_GETLOWPOWERACTIVE, 0, &g_LowPowerActive, 0);
    SystemParametersInfoA(
        SPI_GETPOWEROFFACTIVE, 0, &g_PowerOffActive, 0);
    SystemParametersInfoA(
        SPI_SETSCREENSAVEACTIVE, 0, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(
        SPI_SETLOWPOWERACTIVE, 0, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(
        SPI_SETPOWEROFFACTIVE, 0, NULL, SPIF_SENDCHANGE);

    g_Supervisor.InitializeCriticalSections();
    g_GameErrorContext.Log(g_MainLogStart);

    if (CheckForRunningGameInstance() == -1)
        goto stop;
    if (g_Supervisor.LoadConfig((char *)"th09.cfg") != 0)
        goto stop;

    ComputeExecutableChecksum();
    QueryPerformanceFrequency(&g_GameWindow.performanceFrequency14);

restart:
    if (SupervisorInitializeD3D() != 0)
        goto stop;
    if (CreateGameWindow(hInstance) != 0)
        goto stop;
    if (InitializeD3DRendering() != 0)
        goto stop;

    g_SoundPlayer.InitializeDSound(g_GameWindow.window00);
    Controller::GetJoystickCaps();
    SanitizeKeyboardState();

    g_AnmManager = (AnmManager *)g_ZunMemory.AddToRegistry(
        new AnmManager, sizeof(AnmManager), (char *)"SprtCtrlInf");

    g_ReplayRng.SetSeed((unsigned short)timeGetTime());
    CoInitializeEx(NULL, 0);

    g_SupervisorNetworkState =
        (SupervisorNetworkState *)g_ZunMemory.AddToRegistry(
            new SupervisorNetworkState,
            sizeof(SupervisorNetworkState),
            (char *)"NetworkInf");

    PrepareTitleMode4Network(g_SupervisorNetworkState);
    if (g_SupervisorNetworkState->InitializePeer() != 0)
        g_GameErrorContext.Fatal(g_DirectPlayInitFatal);

    if (!g_Supervisor.IsWindowed())
    {
        WINNLSEnableIME(NULL, FALSE);
        ShowCursor(FALSE);
        SetCursor(NULL);
    }

    renderResult = SupervisorRegisterChain();
    if (renderResult != 0)
    {
        if (renderResult != -1)
            renderResult = 2;
        goto release_runtime;
    }

    g_GameWindow.lastFrameTime3C = 0.0;
    renderResult = 0;
    g_GameWindow.lastTimestamp34 = 0.0;
    g_GameWindow.framesSinceRedraw10 = -4;
    g_GameWindow.currentTimestamp2C = 0.0;

    if (g_GameWindow.windowIsClosing04 != 0)
        goto release_runtime;

    for (;;)
    {
        if (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
        else
        {
            HRESULT cooperativeResult =
                g_Supervisor.d3dDevice08->TestCooperativeLevel();

            if (cooperativeResult == D3D_OK)
            {
                renderResult = g_GameWindow.Render();
                if (renderResult != 0)
                    goto release_runtime;

                g_Supervisor.flags &= ~0x10u;
            }
            else if (cooperativeResult == D3DERR_DEVICENOTRESET)
            {
                g_AnmManager->ReleaseSurfaces();
                if (g_Supervisor.d3dDevice08->Reset(
                        &g_Supervisor.presentParameters350) != D3D_OK)
                    goto release_runtime;

                ResetRenderState();
                g_Supervisor.screenTransitionCountdown = 3;
                g_Supervisor.flags |= 0x10u;
            }
        }

        if (g_GameWindow.windowIsClosing04 != 0)
            goto release_runtime;
    }

release_runtime:
    g_Chain.Release();
    while (g_SoundPlayer.ProcessQueues() != 0)
        ;

stop:
    Sleep(1000);
    g_SoundPlayer.Release();

    if (g_AnmManager != NULL)
        delete g_AnmManager;
    g_AnmManager = NULL;

    if (g_Supervisor.d3dDevice08 != NULL)
        g_Supervisor.d3dDevice08->Reset(&g_Supervisor.presentParameters350);

    if (g_Supervisor.d3dDevice08 != NULL)
    {
        g_Supervisor.d3dDevice08->Release();
        g_Supervisor.d3dDevice08 = NULL;
    }

    if (g_Supervisor.d3d04 != NULL)
    {
        g_Supervisor.d3d04->Release();
        g_Supervisor.d3d04 = NULL;
    }

    if (g_SupervisorNetworkState != NULL)
    {
        g_SupervisorNetworkState->ResetSession();
        g_SupervisorNetworkState->ReleaseDirectPlayResources();
        operator delete(g_SupervisorNetworkState);
        g_SupervisorNetworkState = NULL;
    }

    if (g_GameWindow.window00 != NULL)
    {
        ShowWindow(g_GameWindow.window00, SW_HIDE);
        MoveWindow(g_GameWindow.window00, 0, 0, 0, 0, FALSE);
        DestroyWindow(g_GameWindow.window00);
        g_GameWindow.window00 = NULL;
    }

    ShowCursor(TRUE);

    if (renderResult == 2)
    {
        g_GameErrorContext.ResetContext();
        g_GameErrorContext.Log(g_OptionRestartLog);

        if (!g_Supervisor.IsWindowed())
            WINNLSEnableIME(NULL, TRUE);

        int waitFrames = 60;
        do
        {
            if (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessageA(&message);
            }
            --*reinterpret_cast<unsigned int *>(&waitFrames);
        } while (waitFrames != 0);

        goto restart;
    }

    FileSystem::WriteDataToFile(
        "th09.cfg", &g_Supervisor.config388, sizeof(GameConfiguration));

    if (g_MainMidiOutput != NULL)
    {
        g_MainMidiOutput->StopPlayback();
        if (g_MainMidiOutput != NULL)
            delete g_MainMidiOutput;
        g_MainMidiOutput = NULL;
    }

    g_GameErrorContext.Flush();
    g_Supervisor.DeleteCriticalSections();
    CoUninitialize();

    SystemParametersInfoA(
        SPI_SETSCREENSAVEACTIVE, g_ScreenSaverActive, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(
        SPI_SETLOWPOWERACTIVE, g_LowPowerActive, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(
        SPI_SETPOWEROFFACTIVE, g_PowerOffActive, NULL, SPIF_SENDCHANGE);
    WINNLSEnableIME(NULL, TRUE);

    return 0;
}

#undef TH09_NATIVE_EXE_PATH_CHAR
