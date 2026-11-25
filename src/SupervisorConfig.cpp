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
extern const char g_ConfigNotFoundMessage[] = "\x83\x52\x83\x93\x83\x74\x83\x42\x83\x4F\x83\x66\x81\x5B\x83\x5E\x82\xAA\x8C\xA9\x82\xC2\x82\xA9\x82\xE7\x82\xC8\x82\xA2\x82\xCC\x82\xC5\x8F\x89\x8A\xFA\x89\xBB\x82\xB5\x82\xDC\x82\xB5\x82\xBD\r\n"; // コンフィグデータが見つからないので初期化しました\r\n
extern const char g_ConfigAbnormalMessage[] = "\x83\x52\x83\x93\x83\x74\x83\x42\x83\x4F\x83\x66\x81\x5B\x83\x5E\x82\xAA\x88\xD9\x8F\xED\x82\xC5\x82\xB5\x82\xBD\x82\xCC\x82\xC5\x8D\xC4\x8F\x89\x8A\xFA\x89\xBB\x82\xB5\x82\xDC\x82\xB5\x82\xBD\r\n"; // コンフィグデータが異常でしたので再初期化しました\r\n
extern const char g_BgmVersionMismatchMessage[] = "BGM \x83\x66\x81\x5B\x83\x5E\x82\xCC\x83\x6F\x81\x5B\x83\x57\x83\x87\x83\x93\x82\xAA\x88\xE1\x82\xA2\x82\xDC\x82\xB7\r\n"; // BGM データのバージョンが違います\r\n
extern const char g_NoFogMessage[] = "\x83\x74\x83\x48\x83\x4F\x82\xCC\x8E\x67\x97\x70\x82\xF0\x97\x7D\x90\xA7\x82\xB5\x82\xDC\x82\xB7\r\n"; // フォグの使用を抑制します\r\n
extern const char g_Force16BitTexturesMessage[] = "16Bit \x82\xCC\x83\x65\x83\x4E\x83\x58\x83\x60\x83\x83\x82\xCC\x8E\x67\x97\x70\x82\xF0\x8B\xAD\x90\xA7\x82\xB5\x82\xDC\x82\xB7\r\n"; // 16Bit のテクスチャの使用を強制します\r\n
extern const char g_WindowedMessage[] = "\x83\x45\x83\x42\x83\x93\x83\x68\x83\x45\x83\x82\x81\x5B\x83\x68\x82\xC5\x8B\x4E\x93\xAE\x82\xB5\x82\xDC\x82\xB7\r\n"; // ウィンドウモードで起動します\r\n
extern const char g_ReferenceRasterizerMessage[] = "\x83\x8A\x83\x74\x83\x40\x83\x8C\x83\x93\x83\x58\x83\x89\x83\x58\x83\x5E\x83\x89\x83\x43\x83\x55\x82\xF0\x8B\xAD\x90\xA7\x82\xB5\x82\xDC\x82\xB7\r\n"; // リファレンスラスタライザを強制します\r\n
extern const char g_DisableDirectInputMessage[] = "\x83\x70\x83\x62\x83\x68\x81\x41\x83\x4C\x81\x5B\x83\x7B\x81\x5B\x83\x68\x82\xCC\x93\xFC\x97\xCD\x82\xC9 DirectInput \x82\xF0\x8E\x67\x97\x70\x82\xB5\x82\xDC\x82\xB9\x82\xF1\r\n"; // パッド、キーボードの入力に DirectInput を使用しません\r\n
extern const char g_PreloadMusicMessage[] = "\x82\x61\x82\x66\x82\x6C\x82\xF0\x83\x81\x83\x82\x83\x8A\x82\xC9\x93\xC7\x82\xDD\x8D\x9E\x82\xDD\x82\xDC\x82\xB7\r\n"; // ＢＧＭをメモリに読み込みます\r\n
extern const char g_DisableVsyncMessage[] = "\x90\x82\x92\xBC\x93\xAF\x8A\xFA\x82\xF0\x8E\xE6\x82\xE8\x82\xDC\x82\xB9\x82\xF1\r\n"; // 垂直同期を取りません\r\n
extern const char g_DisableTextBackgroundMessage[] = "\x95\xB6\x8E\x9A\x95\x60\x89\xE6\x82\xCC\x8A\xC2\x8B\xAB\x82\xF0\x8E\xA9\x93\xAE\x8C\x9F\x8F\x6F\x82\xB5\x82\xDC\x82\xB9\x82\xF1\r\n"; // 文字描画の環境を自動検出しません\r\n
extern const char g_ConfigWriteFailedMessage[] = "\x83\x74\x83\x40\x83\x43\x83\x8B\x82\xAA\x8F\x91\x82\xAB\x8F\x6F\x82\xB9\x82\xDC\x82\xB9\x82\xF1 %s\r\n"; // ファイルが書き出せません %s\r\n
extern const char g_ConfigWriteProtectedMessage[] = "\x83\x74\x83\x48\x83\x8B\x83\x5F\x82\xAA\x8F\x91\x8D\x9E\x82\xDD\x8B\xD6\x8E\x7E\x91\xAE\x90\xAB\x82\xC9\x82\xC8\x82\xC1\x82\xC4\x82\xA2\x82\xE9\x82\xA9\x81\x41\x83\x66\x83\x42\x83\x58\x83\x4E\x82\xAA\x82\xA2\x82\xC1\x82\xCF\x82\xA2\x82\xA2\x82\xC1\x82\xCF\x82\xA2\x82\xC9\x82\xC8\x82\xC1\x82\xC4\x82\xDC\x82\xB9\x82\xF1\x82\xA9\x81\x48\r\n"; // フォルダが書込み禁止属性になっているか、ディスクがいっぱいいっぱいになってませんか？\r\n

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
