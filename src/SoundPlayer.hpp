#pragma once

#include <windows.h>
#include <dsound.h>

#include "SoundManager.hpp"

struct SoundPlayerCommand
{
    int opcode;
    int argument;
    int step;
    char path[256];
};

class SoundPlayer
{
  public:
    SoundPlayer();
    LPDIRECTSOUND dsoundHdl;
    int unknown004;
    LPDIRECTSOUNDBUFFER soundBuffers[128];
    LPDIRECTSOUNDBUFFER duplicateSoundBuffers[128];
    int unconsumedMetadataBySound[128];
    LPDIRECTSOUNDBUFFER initSoundBuffer;
    HWND gameWindow;
    SoundManagerProcessView *manager;
    DWORD bgmThreadId;
    HANDLE bgmThreadHandle;
    int unknown61C;
    int soundQueue[12];
    int soundQueueRequestCounts[12];
    unsigned int soundQueuePanData[12][128];
    ThBgmFormatProcessView *bgmPreloadFmtData[16];
    unsigned char *bgmPreloadAllocations[16];
    unsigned char *bgmPreloadData[16];
    DWORD bgmPreloadAllocSizes[16];
    unsigned int loadedBgmSlot;
    ThBgmFormatProcessView *bgmFmtData;
    SoundPlayerCommand commandQueue[32];
    char bgmFileNames[32][256];
    char currentBgmFileName[256];
    StreamingSoundProcessView *bgm;
    HANDLE bgmUpdateEvent;
    int unknown6210;
    unsigned int bgmFileBaseOffset;
    int bgmVolume;
    int sfxVolume;
    int unconsumedBgmAttenuation;

    int InitializeDSound(HWND gameWindow);
    int InitSoundBuffers();
    int Release();
    int StartBGM(char *path);
    int LoadSound(int index, char *path);
    void PlaySoundByIdx(int index, int pan);
    void PlaySoundPositionedByIdx(int index, float positionX);
    int ProcessQueues();
    void UpdateFades();
    void QueueCommand(int opcode, int argument, char *path);
    int PreloadBGM(int index, char *path);
    int LoadBGM(int index);
    int ReopenBGM(char *path);
    void FreePreloadedBGM(int index);
    void StopBGM();
    void FadeOut(float seconds);
    int GetFmtIndexByName(char *name);
    static DWORD WINAPI BGMPlayerThread(LPVOID parameter);
};

typedef char SoundPlayerProcessViewSize[(sizeof(SoundPlayer) == 0x6224) ? 1 : -1];
typedef char SoundPlayerMetadataAt408[
    (offsetof(SoundPlayer, unconsumedMetadataBySound) == 0x408) ? 1 : -1];

extern SoundPlayer g_SoundPlayer;
