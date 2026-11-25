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

extern MidiOutput *g_MidiOutput;
extern Chain g_Chain;
extern RngRuntimeView g_ReplayRng;
extern ZunMemory g_ZunMemory;
extern GameErrorContext g_GameErrorContext;

namespace FileSystem { extern char g_ExecutableDirectory[MAX_PATH]; }
extern HWND g_SupervisorGameWindow;
extern const char g_WindowTitle[] = "\x93\x8C\x95\xFB\x89\xD4\x89\x66\x92\xCB\x81\x40\x81\x60 Phantasmagoria of Flower View. ver 1.50a"; // 東方花映塚　～ Phantasmagoria of Flower View. ver 1.50a

extern const char g_MainLogStart[] = "\x93\x8C\x95\xFB\x93\xAE\x8D\xEC\x8B\x4C\x98\x5E --------------------------------------------- \r\n"; // 東方動作記録 --------------------------------------------- \r\n
extern const char g_DirectPlayInitFatal[] = "DirectPlay \x8F\x89\x8A\xFA\x89\xBB\x8E\xB8\x94\x73\r\n"; // DirectPlay 初期化失敗\r\n
extern const char g_OptionRestartLog[] = "\x8D\xC4\x8B\x4E\x93\xAE\x82\xF0\x97\x76\x82\xB7\x82\xE9\x83\x49\x83\x76\x83\x56\x83\x87\x83\x93\x82\xAA\x95\xCF\x8D\x58\x82\xB3\x82\xEA\x82\xBD\x82\xCC\x82\xC5\x8D\xC4\x8B\x4E\x93\xAE\x82\xB5\x82\xDC\x82\xB7\r\n"; // 再起動を要するオプションが変更されたので再起動します\r\n

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

// The Win32 window decoration sizes are ordinary signed screen metrics;
// build the bounded 640x480 client area's window dimensions directly.
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
    g_GameWindow.isActive08 = TRUE;
    g_GameWindow.isInactive0C = FALSE;
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
        width = GetSystemMetrics(SM_CXDLGFRAME) * 2 + 640;
        height = GetSystemMetrics(SM_CYDLGFRAME) * 2 +
            GetSystemMetrics(SM_CYCAPTION) + 480;
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

// GetModuleFileNameA reports a nonempty path on success. Its bounded
// length supplies real indices into the executable-directory byte buffer.


int WINAPI WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR commandLine, int showCommand)
{
    MSG message;
    int renderResult = 0;

    if (GetModuleFileNameA(NULL, FileSystem::g_ExecutableDirectory, MAX_PATH) != 0)
    {
        int pathIndex = (int)strlen(FileSystem::g_ExecutableDirectory) - 1;
        if (FileSystem::g_ExecutableDirectory[pathIndex] != '\\')
        {
            do
            {
                if (FileSystem::g_ExecutableDirectory[pathIndex] == '/')
                    break;
                if (pathIndex <= 0)
                    goto executable_path_fallback;
                --pathIndex;
            }
            while (FileSystem::g_ExecutableDirectory[pathIndex] != '\\');
        }

        if (pathIndex <= 0)
        {
        executable_path_fallback:
            strcpy(FileSystem::g_ExecutableDirectory, "./");
        }
        else
        {
            FileSystem::g_ExecutableDirectory[pathIndex] = '/';
            FileSystem::g_ExecutableDirectory[pathIndex + 1] = '\0';
        }
    }
    else
    {
        strcpy(FileSystem::g_ExecutableDirectory, "./");
    }

    g_Supervisor.instance00 = hInstance;

    SystemParametersInfoA(
        SPI_GETSCREENSAVEACTIVE, 0, &g_GameWindow.screenSaverActive20, 0);
    SystemParametersInfoA(
        SPI_GETLOWPOWERACTIVE, 0, &g_GameWindow.lowPowerActive24, 0);
    SystemParametersInfoA(
        SPI_GETPOWEROFFACTIVE, 0, &g_GameWindow.powerOffActive28, 0);
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
            --waitFrames;
        } while (waitFrames != 0);

        goto restart;
    }

    FileSystem::WriteDataToFile(
        "th09.cfg", &g_Supervisor.config388, sizeof(GameConfiguration));

    if (g_MidiOutput != NULL)
    {
        g_MidiOutput->StopPlayback();
        if (g_MidiOutput != NULL)
            delete g_MidiOutput;
        g_MidiOutput = NULL;
    }

    g_GameErrorContext.Flush();
    g_Supervisor.DeleteCriticalSections();
    CoUninitialize();

    SystemParametersInfoA(
        SPI_SETSCREENSAVEACTIVE, g_GameWindow.screenSaverActive20, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(
        SPI_SETLOWPOWERACTIVE, g_GameWindow.lowPowerActive24, NULL, SPIF_SENDCHANGE);
    SystemParametersInfoA(
        SPI_SETPOWEROFFACTIVE, g_GameWindow.powerOffActive28, NULL, SPIF_SENDCHANGE);
    WINNLSEnableIME(NULL, TRUE);

    return 0;
}
