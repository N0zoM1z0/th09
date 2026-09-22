// Target-bound TH09 startup reconstruction. The Main* types below are narrow
// views used to preserve target-observed object geometry and ABI; they do not
// claim original source-level class names or complete layouts.
//
// This translation unit deliberately keeps CreateGameWindow file-local. The
// real WinMain caller is what makes VC7.1 choose the target private EBX entry
// transport naturally; no dummy caller or register forcing is used.

#include "GameConfiguration.hpp"
#include "GameErrorContext.hpp"
#include "Midi.hpp"
#include "SupervisorNetworkState.hpp"
#include "ZunMemory.hpp"

#include <windows.h>
#include <mmsystem.h>
#include <objbase.h>
#include <d3d8.h>
#include <string.h>
#include <stdlib.h>

extern "C" BOOL WINAPI WINNLSEnableIME(HWND, BOOL);
extern "C" __declspec(dllimport) HRESULT WINAPI CoInitializeEx(LPVOID, DWORD);

class MainAnmManager
{
  public:
    MainAnmManager();
    ~MainAnmManager();
    void ReleaseSurfaces();

  private:
    unsigned char storage[0x2B2934];
};

class MainSoundPlayer
{
  public:
    int InitializeDSound(HWND gameWindow);
    int Release();
    int ProcessQueues();
};

class MainChainReleaseView
{
  public:
    void Release();
};

class MainReplayRngView
{
  public:
    void SetSeed(unsigned short seed);
};

#pragma pack(push, 4)
struct MainGameWindowView
{
    HWND window00;
    int windowIsClosing04;
    unsigned char unknown08[8];
    signed char framesSinceRedraw10;
    unsigned char unknown11[3];
    LARGE_INTEGER performanceFrequency14;
    unsigned char startupPathDiffers1C;
    unsigned char unknown1D[0x0F];
    double currentTimestamp2C;
    double lastTimestamp34;
    double lastFrameTime3C;

    int Render();
};
#pragma pack(pop)

class MainSupervisorView
{
  public:
    int LoadConfig(char *path);
    int IsWindowed();
    void InitializeCriticalSections();
    void DeleteCriticalSections();

    HINSTANCE instance00;
    IDirect3D8 *d3d04;
    IDirect3DDevice8 *d3dDevice08;
    unsigned char unknown00C[0x350 - 0x0C];
    D3DPRESENT_PARAMETERS present350;
    unsigned char unknown384[4];
    GameConfiguration config388;
    unsigned char unknown454[0x5A4 - 0x454];
    int screenTransitionCountdown5A4;
    unsigned char unknown5A8[0x5D4 - 0x5A8];
    unsigned int flags5D4;
};

typedef char MainGameWindowSizeIs44[
    sizeof(MainGameWindowView) == 0x44 ? 1 : -1];
typedef char MainSupervisorPresentAt350[
    offsetof(MainSupervisorView, present350) == 0x350 ? 1 : -1];
typedef char MainSupervisorConfigAt388[
    offsetof(MainSupervisorView, config388) == 0x388 ? 1 : -1];
typedef char MainSupervisorCountdownAt5A4[
    offsetof(MainSupervisorView, screenTransitionCountdown5A4) == 0x5A4 ? 1 : -1];
typedef char MainSupervisorFlagsAt5D4[
    offsetof(MainSupervisorView, flags5D4) == 0x5D4 ? 1 : -1];

extern MainGameWindowView g_MainGameWindow;
extern MainSupervisorView g_MainSupervisor;
extern MainSoundPlayer g_MainSoundPlayer;
extern MainAnmManager *g_MainAnmManager;
extern MidiOutput *g_MainMidiOutput;
extern MainChainReleaseView g_MainChain;
extern MainReplayRngView g_MainReplayRng;
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
    if (!g_MainSupervisor.IsWindowed())
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
        height =
            GetSystemMetrics(SM_CYDLGFRAME) * 2 +
            GetSystemMetrics(SM_CYCAPTION) + 480;
        window = CreateWindowExA(
            0, "BASE", g_WindowTitle,
            WS_VISIBLE | WS_MINIMIZEBOX | WS_SYSMENU,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            NULL, NULL, instance, NULL);
    }

    g_MainGameWindow.window00 = window;
    g_SupervisorGameWindow = window;
    if (window == NULL)
        return 1;

    ActivateGameWindow(window);
    return 0;
}

int WINAPI WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR commandLine, int showCommand)
{
    MSG message;
    int renderResult = 0;

    if (GetModuleFileNameA(NULL, g_ExecutableDirectory, MAX_PATH) != 0)
    {
        int pathIndex = (int)strlen(g_ExecutableDirectory) - 1;
        if (g_ExecutableDirectory[pathIndex] != '\\')
        {
            do
            {
                if (g_ExecutableDirectory[pathIndex] == '/')
                    break;
                if (pathIndex <= 0)
                    goto executable_path_fallback;
                pathIndex--;
            }
            while (g_ExecutableDirectory[pathIndex] != '\\');
        }

        if (pathIndex <= 0)
        {
        executable_path_fallback:
            strcpy(g_ExecutableDirectory, "./");
        }
        else
        {
            g_ExecutableDirectory[pathIndex] = '/';
            g_ExecutableDirectory[pathIndex + 1] = '\0';
        }
    }
    else
    {
        strcpy(g_ExecutableDirectory, "./");
    }

    g_MainSupervisor.instance00 = hInstance;

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

    g_MainSupervisor.InitializeCriticalSections();
    g_GameErrorContext.Log(g_MainLogStart);

    if (CheckForRunningGameInstance() == -1)
        goto stop;
    if (g_MainSupervisor.LoadConfig((char *)"th09.cfg") != 0)
        goto stop;

    ComputeExecutableChecksum();
    QueryPerformanceFrequency(&g_MainGameWindow.performanceFrequency14);

restart:
    if (SupervisorInitializeD3D() != 0)
        goto stop;
    if (CreateGameWindow(hInstance) != 0)
        goto stop;
    if (InitializeD3DRendering() != 0)
        goto stop;

    g_MainSoundPlayer.InitializeDSound(g_MainGameWindow.window00);
    Controller::GetJoystickCaps();
    SanitizeKeyboardState();

    g_MainAnmManager = (MainAnmManager *)g_ZunMemory.AddToRegistry(
        new MainAnmManager, sizeof(MainAnmManager), (char *)"SprtCtrlInf");

    g_MainReplayRng.SetSeed((unsigned short)timeGetTime());
    CoInitializeEx(NULL, 0);

    g_SupervisorNetworkState =
        (SupervisorNetworkState *)g_ZunMemory.AddToRegistry(
            new SupervisorNetworkState,
            sizeof(SupervisorNetworkState),
            (char *)"NetworkInf");

    PrepareTitleMode4Network(g_SupervisorNetworkState);
    if (g_SupervisorNetworkState->InitializePeer() != 0)
        g_GameErrorContext.Fatal(g_DirectPlayInitFatal);

    if (!g_MainSupervisor.IsWindowed())
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

    g_MainGameWindow.lastFrameTime3C = 0.0;
    renderResult = 0;
    g_MainGameWindow.lastTimestamp34 = 0.0;
    g_MainGameWindow.framesSinceRedraw10 = -4;
    g_MainGameWindow.currentTimestamp2C = 0.0;

    if (g_MainGameWindow.windowIsClosing04 != 0)
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
                g_MainSupervisor.d3dDevice08->TestCooperativeLevel();

            if (cooperativeResult == D3D_OK)
            {
                renderResult = g_MainGameWindow.Render();
                if (renderResult != 0)
                    goto release_runtime;

                g_MainSupervisor.flags5D4 &= ~0x10u;
            }
            else if (cooperativeResult == D3DERR_DEVICENOTRESET)
            {
                g_MainAnmManager->ReleaseSurfaces();
                if (g_MainSupervisor.d3dDevice08->Reset(
                        &g_MainSupervisor.present350) != D3D_OK)
                    goto release_runtime;

                ResetRenderState();
                g_MainSupervisor.screenTransitionCountdown5A4 = 3;
                g_MainSupervisor.flags5D4 |= 0x10u;
            }
        }

        if (g_MainGameWindow.windowIsClosing04 != 0)
            goto release_runtime;
    }

release_runtime:
    g_MainChain.Release();
    while (g_MainSoundPlayer.ProcessQueues() != 0)
        ;

stop:
    Sleep(1000);
    g_MainSoundPlayer.Release();

    if (g_MainAnmManager != NULL)
        delete g_MainAnmManager;
    g_MainAnmManager = NULL;

    if (g_MainSupervisor.d3dDevice08 != NULL)
        g_MainSupervisor.d3dDevice08->Reset(&g_MainSupervisor.present350);

    if (g_MainSupervisor.d3dDevice08 != NULL)
    {
        g_MainSupervisor.d3dDevice08->Release();
        g_MainSupervisor.d3dDevice08 = NULL;
    }

    if (g_MainSupervisor.d3d04 != NULL)
    {
        g_MainSupervisor.d3d04->Release();
        g_MainSupervisor.d3d04 = NULL;
    }

    if (g_SupervisorNetworkState != NULL)
    {
        g_SupervisorNetworkState->ResetSession();
        g_SupervisorNetworkState->ReleaseDirectPlayResources();
        free(g_SupervisorNetworkState);
        g_SupervisorNetworkState = NULL;
    }

    if (g_MainGameWindow.window00 != NULL)
    {
        ShowWindow(g_MainGameWindow.window00, SW_HIDE);
        MoveWindow(g_MainGameWindow.window00, 0, 0, 0, 0, FALSE);
        DestroyWindow(g_MainGameWindow.window00);
        g_MainGameWindow.window00 = NULL;
    }

    ShowCursor(TRUE);

    if (renderResult == 2)
    {
        g_GameErrorContext.ResetContext();
        g_GameErrorContext.Log(g_OptionRestartLog);

        if (!g_MainSupervisor.IsWindowed())
            WINNLSEnableIME(NULL, TRUE);

        int waitFrames = 60;
        do
        {
            if (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessageA(&message);
            }
            waitFrames--;
        } while (waitFrames != 0);

        goto restart;
    }

    FileSystem::WriteDataToFile(
        "th09.cfg", &g_MainSupervisor.config388, sizeof(GameConfiguration));

    if (g_MainMidiOutput != NULL)
    {
        g_MainMidiOutput->StopPlayback();
        if (g_MainMidiOutput != NULL)
            delete g_MainMidiOutput;
        g_MainMidiOutput = NULL;
    }

    g_GameErrorContext.Flush();
    g_MainSupervisor.DeleteCriticalSections();
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
