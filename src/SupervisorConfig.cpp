#include "Supervisor.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "ZunMemory.hpp"

#include <windows.h>

#pragma pack(push, 1)
struct ControllerMapping54
{
    unsigned int v00;
    unsigned int v04;
    unsigned int v08;
    unsigned int v0C;
    unsigned short v10;
    unsigned int v12;
    unsigned int v16;
    unsigned int v1A;
    unsigned int v1E;
    unsigned short v22;
    unsigned int v24;
    unsigned int v28;
    unsigned int v2C;
    unsigned int v30;
    unsigned short v34;
};

struct GameConfiguration
{
    ControllerMapping54 player1Mapping;
    ControllerMapping54 player2Mapping;
    unsigned char unknown_006C[0x38];
    unsigned int version;
    short padXAxis;
    short padYAxis;
    unsigned char valueAC;
    unsigned char valueAD;
    unsigned char musicMode;
    unsigned char valueAF;
    unsigned char difficulty;
    unsigned char windowed;
    unsigned char valueB2;
    unsigned char valueB3;
    unsigned char valueB4;
    unsigned char valueB5;
    unsigned char valueB6;
    unsigned char valueB7;
    unsigned char valueB8;
    unsigned char valueB9;
    unsigned char musicVolume;
    unsigned char sfxVolume;
    unsigned char unknown_00BC[0x0C];
    unsigned int options;

    char Initialize();
};
#pragma pack(pop)

typedef char ConfigSizeCheck[(sizeof(GameConfiguration) == 0xCC) ? 1 : -1];
typedef char MappingSizeCheck[(sizeof(ControllerMapping54) == 0x36) ? 1 : -1];

struct SupervisorConfigLayout
{
    unsigned char unknown_0000[0x388];
    GameConfiguration config;
    unsigned char unknown_0454[0x158];
    int disableVsync;
};

extern GameErrorContext g_GameErrorContext;
extern ControllerMapping54 g_ControllerMapping1;
extern ControllerMapping54 g_ControllerMapping2;
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

        g_ControllerMapping1.v00 = globalSupervisor->config.player1Mapping.v00;
        g_ControllerMapping1.v04 = globalSupervisor->config.player1Mapping.v04;
        g_ControllerMapping1.v08 = globalSupervisor->config.player1Mapping.v08;
        g_ControllerMapping1.v0C = globalSupervisor->config.player1Mapping.v0C;
        g_ControllerMapping1.v10 = globalSupervisor->config.player1Mapping.v10;
        g_ControllerMapping1.v12 = globalSupervisor->config.player1Mapping.v12;
        g_ControllerMapping1.v16 = globalSupervisor->config.player1Mapping.v16;
        g_ControllerMapping1.v1A = globalSupervisor->config.player1Mapping.v1A;
        g_ControllerMapping1.v1E = globalSupervisor->config.player1Mapping.v1E;
        g_ControllerMapping1.v22 = globalSupervisor->config.player1Mapping.v22;
        g_ControllerMapping1.v24 = globalSupervisor->config.player1Mapping.v24;
        g_ControllerMapping1.v28 = globalSupervisor->config.player1Mapping.v28;
        g_ControllerMapping1.v2C = globalSupervisor->config.player1Mapping.v2C;
        g_ControllerMapping1.v30 = globalSupervisor->config.player1Mapping.v30;
        g_ControllerMapping1.v34 = globalSupervisor->config.player1Mapping.v34;

        g_ControllerMapping2.v00 = globalSupervisor->config.player2Mapping.v00;
        g_ControllerMapping2.v04 = globalSupervisor->config.player2Mapping.v04;
        g_ControllerMapping2.v08 = globalSupervisor->config.player2Mapping.v08;
        g_ControllerMapping2.v0C = globalSupervisor->config.player2Mapping.v0C;
        g_ControllerMapping2.v10 = globalSupervisor->config.player2Mapping.v10;
        g_ControllerMapping2.v12 = globalSupervisor->config.player2Mapping.v12;
        g_ControllerMapping2.v16 = globalSupervisor->config.player2Mapping.v16;
        g_ControllerMapping2.v1A = globalSupervisor->config.player2Mapping.v1A;
        g_ControllerMapping2.v1E = globalSupervisor->config.player2Mapping.v1E;
        g_ControllerMapping2.v22 = globalSupervisor->config.player2Mapping.v22;
        g_ControllerMapping2.v24 = globalSupervisor->config.player2Mapping.v24;
        g_ControllerMapping2.v28 = globalSupervisor->config.player2Mapping.v28;
        g_ControllerMapping2.v2C = globalSupervisor->config.player2Mapping.v2C;
        g_ControllerMapping2.v30 = globalSupervisor->config.player2Mapping.v30;
        g_ControllerMapping2.v34 = globalSupervisor->config.player2Mapping.v34;
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
