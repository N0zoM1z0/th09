#include "Supervisor.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "ZunMemory.hpp"

#include <windows.h>

#include "GameConfiguration.hpp"

struct SupervisorConfigLayout
{
    unsigned char unknown_0000[0x388];
    GameConfiguration config;
    unsigned char unknown_0454[0x158];
    int disableVsync;
};

extern GameErrorContext g_GameErrorContext;
extern const char g_ConfigNotFoundMessage[];
extern const char g_ConfigAbnormalMessage[];
extern const char g_BgmVersionMismatchMessage[];
extern const char g_NoFogMessage[];
extern const char g_Force16BitTexturesMessage[];
extern const char g_WindowedMessage[];
extern const char g_ReferenceRasterizerMessage[];
extern const char g_DisableDirectInputMessage[];
extern const char g_PreloadMusicMessage[];
extern const char g_DisableVsyncMessage[];
extern const char g_DisableTextBackgroundMessage[];
extern const char g_ConfigWriteFailedMessage[];
extern const char g_ConfigWriteProtectedMessage[];

int Supervisor::LoadConfig(char *configFile)
{
    SupervisorConfigLayout *globalSupervisor =
        reinterpret_cast<SupervisorConfigLayout *>(&g_Supervisor);
    SupervisorConfigLayout *supervisor =
        reinterpret_cast<SupervisorConfigLayout *>(this);
    int bgmBuffer[4];
    HANDLE bgmHandle;
    DWORD bytesRead;
    unsigned char *configFileBuffer;
    int fileSize;

    globalSupervisor->config.Initialize();
    configFileBuffer = FileSystem::OpenFile(configFile, &fileSize, 1);
    if (configFileBuffer == NULL)
    {
        g_GameErrorContext.Log(g_ConfigNotFoundMessage);
    SET_DEFAULT:
        globalSupervisor->config.Initialize();
        bgmHandle = CreateFileA("./thbgm.dat", GENERIC_READ, FILE_SHARE_READ, NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                                NULL);
        if (bgmHandle != INVALID_HANDLE_VALUE)
        {
            ReadFile(bgmHandle, bgmBuffer, sizeof(bgmBuffer), &bytesRead, NULL);
            CloseHandle(bgmHandle);
            if (bgmBuffer[0] != 0x5641575A || bgmBuffer[1] != 1 ||
                bgmBuffer[2] != 0x900)
            {
                g_GameErrorContext.Fatal(g_BgmVersionMismatchMessage);
                return -1;
            }
            globalSupervisor->config.musicMode = 1;
        }
        else
        {
            globalSupervisor->config.musicMode = 2;
        }
    }
    else
    {
        globalSupervisor->config = *(GameConfiguration *)configFileBuffer;
        g_ZunMemory.Free(configFileBuffer);
        if (globalSupervisor->config.valueAC >= 3 ||
            globalSupervisor->config.valueAD >= 2 ||
            globalSupervisor->config.musicMode >= 3 ||
            globalSupervisor->config.difficulty >= 6 ||
            globalSupervisor->config.valueAF >= 2 ||
            globalSupervisor->config.windowed >= 2 ||
            globalSupervisor->config.valueB2 >= 3 ||
            globalSupervisor->config.valueB3 >= 3 ||
            globalSupervisor->config.version != 0x90003 ||
            fileSize != sizeof(GameConfiguration))
        {
            g_GameErrorContext.Log(g_ConfigAbnormalMessage);
            goto SET_DEFAULT;
        }

        g_ControllerMapping.primaryBindings[0].v00 = globalSupervisor->config.controllerMapping.bindings[0].v00;
        g_ControllerMapping.primaryBindings[0].v04 = globalSupervisor->config.controllerMapping.bindings[0].v04;
        g_ControllerMapping.primaryBindings[0].v08 = globalSupervisor->config.controllerMapping.bindings[0].v08;
        g_ControllerMapping.primaryBindings[0].v0C = globalSupervisor->config.controllerMapping.bindings[0].v0C;
        g_ControllerMapping.primaryBindings[0].v10 = globalSupervisor->config.controllerMapping.bindings[0].v10;
        g_ControllerMapping.primaryBindings[1].v00 = globalSupervisor->config.controllerMapping.bindings[1].v00;
        g_ControllerMapping.primaryBindings[1].v04 = globalSupervisor->config.controllerMapping.bindings[1].v04;
        g_ControllerMapping.primaryBindings[1].v08 = globalSupervisor->config.controllerMapping.bindings[1].v08;
        g_ControllerMapping.primaryBindings[1].v0C = globalSupervisor->config.controllerMapping.bindings[1].v0C;
        g_ControllerMapping.primaryBindings[1].v10 = globalSupervisor->config.controllerMapping.bindings[1].v10;
        g_ControllerMapping.primaryBindings[2].v00 = globalSupervisor->config.controllerMapping.bindings[2].v00;
        g_ControllerMapping.primaryBindings[2].v04 = globalSupervisor->config.controllerMapping.bindings[2].v04;
        g_ControllerMapping.primaryBindings[2].v08 = globalSupervisor->config.controllerMapping.bindings[2].v08;
        g_ControllerMapping.primaryBindings[2].v0C = globalSupervisor->config.controllerMapping.bindings[2].v0C;
        g_ControllerMapping.primaryBindings[2].v10 = globalSupervisor->config.controllerMapping.bindings[2].v10;
        g_ControllerMapping.secondaryBindings[0].v00 = globalSupervisor->config.controllerMapping.bindings[3].v00;
        g_ControllerMapping.secondaryBindings[0].v04 = globalSupervisor->config.controllerMapping.bindings[3].v04;
        g_ControllerMapping.secondaryBindings[0].v08 = globalSupervisor->config.controllerMapping.bindings[3].v08;
        g_ControllerMapping.secondaryBindings[0].v0C = globalSupervisor->config.controllerMapping.bindings[3].v0C;
        g_ControllerMapping.secondaryBindings[0].v10 = globalSupervisor->config.controllerMapping.bindings[3].v10;
        g_ControllerMapping.secondaryBindings[1].v00 = globalSupervisor->config.controllerMapping.bindings[4].v00;
        g_ControllerMapping.secondaryBindings[1].v04 = globalSupervisor->config.controllerMapping.bindings[4].v04;
        g_ControllerMapping.secondaryBindings[1].v08 = globalSupervisor->config.controllerMapping.bindings[4].v08;
        g_ControllerMapping.secondaryBindings[1].v0C = globalSupervisor->config.controllerMapping.bindings[4].v0C;
        g_ControllerMapping.secondaryBindings[1].v10 = globalSupervisor->config.controllerMapping.bindings[4].v10;
        g_ControllerMapping.secondaryBindings[2].v00 = globalSupervisor->config.controllerMapping.bindings[5].v00;
        g_ControllerMapping.secondaryBindings[2].v04 = globalSupervisor->config.controllerMapping.bindings[5].v04;
        g_ControllerMapping.secondaryBindings[2].v08 = globalSupervisor->config.controllerMapping.bindings[5].v08;
        g_ControllerMapping.secondaryBindings[2].v0C = globalSupervisor->config.controllerMapping.bindings[5].v0C;
        g_ControllerMapping.secondaryBindings[2].v10 = globalSupervisor->config.controllerMapping.bindings[5].v10;
    }

    supervisor->disableVsync = 0;
    if (this->IsFogDisabled())
        g_GameErrorContext.Log(g_NoFogMessage);
    if (this->Is16BitTexturesForced())
        g_GameErrorContext.Log(g_Force16BitTexturesMessage);
    if (this->IsWindowed())
        g_GameErrorContext.Log(g_WindowedMessage);
    if (this->IsReferenceRasterizerForced())
        g_GameErrorContext.Log(g_ReferenceRasterizerMessage);
    if (this->IsDirectInputDisabled())
        g_GameErrorContext.Log(g_DisableDirectInputMessage);
    if (this->IsMusicPreloaded())
        g_GameErrorContext.Log(g_PreloadMusicMessage);
    if (this->IsVsyncDisabled())
    {
        g_GameErrorContext.Log(g_DisableVsyncMessage);
        globalSupervisor->disableVsync = 1;
    }
    if (this->IsTextBackgroundDetectionDisabled())
        g_GameErrorContext.Log(g_DisableTextBackgroundMessage);

    if (FileSystem::WriteDataToFile(configFile, &globalSupervisor->config,
                                    sizeof(GameConfiguration)) != 0)
    {
        g_GameErrorContext.Fatal(g_ConfigWriteFailedMessage, configFile);
        g_GameErrorContext.Fatal(g_ConfigWriteProtectedMessage);
        return -1;
    }
    return 0;
}
