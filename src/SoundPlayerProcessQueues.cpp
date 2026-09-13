#include <windows.h>
#include <dsound.h>
#include <string.h>

#include "ZunMemory.hpp"
#include "GameErrorContext.hpp"
#include "FileSystem.hpp"

struct ThBgmFormatProcessView
{
    char name[16];
    int startOffset;
    DWORD preloadAllocSize;
    int introLength;
    int totalLength;
    WAVEFORMATEX format;
};

typedef char ThBgmFormatProcessViewSize[(sizeof(ThBgmFormatProcessView) == 0x34) ? 1 : -1];
typedef char ThBgmFormatProcessViewStartOffsetAt10[
    (offsetof(ThBgmFormatProcessView, startOffset) == 0x10) ? 1 : -1];
typedef char ThBgmFormatProcessViewPreloadSizeAt14[
    (offsetof(ThBgmFormatProcessView, preloadAllocSize) == 0x14) ? 1 : -1];
typedef char ThBgmFormatProcessViewSamplesAt24[
    (offsetof(ThBgmFormatProcessView, format.nSamplesPerSec) == 0x24) ? 1 : -1];
typedef char ThBgmFormatProcessViewBlockAlignAt2C[
    (offsetof(ThBgmFormatProcessView, format.nBlockAlign) == 0x2C) ? 1 : -1];

struct WaveFileProcessView
{
    ThBgmFormatProcessView *GetFormat();
    int Reopen(ThBgmFormatProcessView *format);
};

struct StreamingSoundProcessView
{
    virtual ~StreamingSoundProcessView();

    unsigned char unknown004[0x34 - 0x4];
    int isPlaying;
    unsigned char unknown038[0x78 - 0x38];
    int isLocked;

    LPDIRECTSOUNDBUFFER GetBuffer(unsigned int index);
    WaveFileProcessView *GetWaveFile();
    int Reset();
    int FillBufferWithSound(LPDIRECTSOUNDBUFFER buffer, int looped);
    void InitSoundBuffers();
    void Play(unsigned int priority, unsigned int flags);
    void Stop();
    void Pause();
    void Unpause();
    void SetVolume(int volume);
    int HandleWaveStreamNotification(int looped);
};

typedef char StreamingSoundIsPlayingAt34[
    (offsetof(StreamingSoundProcessView, isPlaying) == 0x34) ? 1 : -1];
typedef char StreamingSoundIsLockedAt78[
    (offsetof(StreamingSoundProcessView, isLocked) == 0x78) ? 1 : -1];


class SoundManagerProcessView
{
  public:
    LPDIRECTSOUND8 directSound;

    SoundManagerProcessView();
    ~SoundManagerProcessView();
    int Initialize(
        HWND window,
        DWORD cooperativeLevel,
        DWORD primaryChannels,
        DWORD primaryFrequency,
        DWORD primaryBitRate);
    LPDIRECTSOUND GetDirectSound();

    int CreateStreaming(
        StreamingSoundProcessView **streamingSound,
        char *path,
        DWORD creationFlags,
        GUID algorithm,
        DWORD bufferCount,
        DWORD notifySize,
        HANDLE notifyEvent,
        ThBgmFormatProcessView *format);
    int CreateStreamingFromMemory(
        StreamingSoundProcessView **streamingSound,
        BYTE *data,
        ULONG dataSize,
        ThBgmFormatProcessView *format,
        DWORD creationFlags,
        GUID algorithm,
        DWORD bufferCount,
        DWORD notifySize,
        HANDLE notifyEvent);
};

typedef char SoundManagerProcessViewSize[(sizeof(SoundManagerProcessView) == 4) ? 1 : -1];

struct SoundPlayerCommand
{
    int opcode;
    int argument;
    int step;
    char path[256];
};

struct SoundBufferIdxVolume
{
    int bufferIdx;
    short volume;
    short unconsumedMetadata;
};

struct SupervisorProcessQueuesView
{
    unsigned char unknown000[0x74];
    HWND window;
    unsigned char unknown078[0x436 - 0x78];
    unsigned char musicMode;
    unsigned char playSounds;

    int IsMusicPreloaded();
};

typedef char SupervisorProcessQueuesWindowAt74[
    (offsetof(SupervisorProcessQueuesView, window) == 0x74) ? 1 : -1];
typedef char SupervisorProcessQueuesMusicModeAt436[
    (offsetof(SupervisorProcessQueuesView, musicMode) == 0x436) ? 1 : -1];
typedef char SupervisorProcessQueuesPlaySoundsAt437[
    (offsetof(SupervisorProcessQueuesView, playSounds) == 0x437) ? 1 : -1];

extern SupervisorProcessQueuesView g_Supervisor;
extern SoundBufferIdxVolume g_SoundBufferIdxVol[54];
extern char *g_SfxList[39];
extern const char g_SoundLoadErrorMessage[];
extern const char g_NotWavRiffMessage[];
extern const char g_NotWavMessage[];
extern const char g_DirectSoundInitializeErrorMessage[];
extern const char g_DirectSoundInitializeSuccessMessage[];
extern GameErrorContext g_GameErrorContext;

static WAVEFORMATEX *GetWavFormatData(
    unsigned char *soundData,
    char *formatString,
    int *formatSize,
    unsigned int fileSizeExcludingFormat);

enum SoundPlayerCommandOpcode
{
    SOUNDPLAYER_COMMAND_NONE = 0,
    SOUNDPLAYER_COMMAND_PRELOAD_BGM = 1,
    SOUNDPLAYER_COMMAND_LOAD_BGM = 2,
    SOUNDPLAYER_COMMAND_STOP_BGM = 3,
    SOUNDPLAYER_COMMAND_RELEASE_BGM = 4,
    SOUNDPLAYER_COMMAND_FADE_OUT = 5,
    SOUNDPLAYER_COMMAND_PAUSE = 6,
    SOUNDPLAYER_COMMAND_UNPAUSE = 7,
    SOUNDPLAYER_COMMAND_SET_VOLUME = 8,
    SOUNDPLAYER_COMMAND_FREE_PRELOADED_BGM = 9,
    SOUNDPLAYER_COMMAND_START_LOADED_BGM = 10,
};

class SoundPlayer
{
  public:
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
    int ProcessQueues();
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

extern SoundPlayer g_SoundPlayer;

int SoundPlayer::InitializeDSound(HWND gameWindow)
{
    DSBUFFERDESC bufferDescription;
    WAVEFORMATEX waveFormat;
    LPVOID audioBuffer1Start;
    DWORD audioBuffer1Length;
    LPVOID audioBuffer2Start;
    DWORD audioBuffer2Length;
    int i;

    ZeroMemory(this, sizeof(SoundPlayer));

    for (i = 0; i < 128; ++i)
        this->unconsumedMetadataBySound[i] = -1;
    for (i = 0; i < 12; ++i)
        this->soundQueue[i] = -1;

    this->manager = new SoundManagerProcessView();
    if (this->manager->Initialize(gameWindow, 2, 2, 44100, 16) < 0)
    {
        g_GameErrorContext.Log(g_DirectSoundInitializeErrorMessage);
        if (this->manager != NULL)
        {
            delete this->manager;
            this->manager = NULL;
        }
        return -1;
    }

    this->dsoundHdl = this->manager->GetDirectSound();
    this->bgmThreadHandle = NULL;

    ZeroMemory(&bufferDescription, sizeof(bufferDescription));
    bufferDescription.dwSize = sizeof(bufferDescription);
    bufferDescription.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCSOFTWARE;
    bufferDescription.dwBufferBytes = 0x8000;

    ZeroMemory(&waveFormat, sizeof(waveFormat));
    waveFormat.cbSize = 0;
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = 2;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.nAvgBytesPerSec = 176400;
    waveFormat.nBlockAlign = 4;
    waveFormat.wBitsPerSample = 16;
    bufferDescription.lpwfxFormat = &waveFormat;

    if (this->dsoundHdl->CreateSoundBuffer(
            &bufferDescription, &this->initSoundBuffer, NULL) < 0)
        return -1;

    if (this->initSoundBuffer->Lock(
            0,
            0x8000,
            &audioBuffer1Start,
            &audioBuffer1Length,
            &audioBuffer2Start,
            &audioBuffer2Length,
            0) < 0)
        return -1;

    ZeroMemory(audioBuffer1Start, 0x8000);
    this->initSoundBuffer->Unlock(
        audioBuffer1Start,
        audioBuffer1Length,
        audioBuffer2Start,
        audioBuffer2Length);
    this->initSoundBuffer->Play(0, 0, 1);
    this->bgmVolume = 100;
    this->sfxVolume = 100;
    SetTimer(gameWindow, 0, 250, NULL);
    this->gameWindow = gameWindow;
    g_GameErrorContext.Log(g_DirectSoundInitializeSuccessMessage);
    return 0;
}

int SoundPlayer::InitSoundBuffers()
{
    int i;

    if (this->manager == NULL)
        return -1;

    if (this->dsoundHdl == NULL)
        return 0;

    for (i = 0; i < 12; ++i)
        this->soundQueue[i] = -1;

    for (i = 0; i < 39; ++i)
    {
        if (this->LoadSound(i, g_SfxList[i]) != 0)
        {
            g_GameErrorContext.Log(g_SoundLoadErrorMessage, g_SfxList[i]);
            return -1;
        }
    }

    for (i = 0; i < sizeof(g_SoundBufferIdxVol) / sizeof(g_SoundBufferIdxVol[0]); ++i)
    {
        this->dsoundHdl->DuplicateSoundBuffer(
            this->soundBuffers[g_SoundBufferIdxVol[i].bufferIdx],
            &this->duplicateSoundBuffers[i]);
        this->duplicateSoundBuffers[i]->SetCurrentPosition(0);
        this->duplicateSoundBuffers[i]->SetVolume(g_SoundBufferIdxVol[i].volume);
    }

    return 0;
}

int SoundPlayer::LoadSound(int index, char *path)
{
    unsigned char *soundFileData;
    unsigned char *soundFileCursor;
    int fileSize;
    WAVEFORMATEX *wavDataPtr;
    WAVEFORMATEX *audioPtr1;
    WAVEFORMATEX *audioPtr2;
    DWORD audioSize1;
    DWORD audioSize2;
    WAVEFORMATEX wavData;
    int formatSize;
    DSBUFFERDESC soundBufferDesc;

    if (this->manager == NULL)
        return 0;

    if (this->soundBuffers[index] != NULL)
    {
        this->soundBuffers[index]->Release();
        this->soundBuffers[index] = NULL;
    }

    soundFileData = FileSystem::OpenFile(path, NULL, 0);
    soundFileCursor = soundFileData;
    if (soundFileCursor == NULL)
        return -1;

    if (strncmp(reinterpret_cast<char *>(soundFileCursor), "RIFF", 4) != 0)
    {
        g_GameErrorContext.Log(g_NotWavRiffMessage, path);
        g_ZunMemory.Free(soundFileData);
        return -1;
    }
    soundFileCursor += 4;

    fileSize = *reinterpret_cast<int *>(soundFileCursor);
    soundFileCursor += 4;

    if (strncmp(reinterpret_cast<char *>(soundFileCursor), "WAVE", 4) != 0)
    {
        g_GameErrorContext.Log(g_NotWavMessage, path);
        g_ZunMemory.Free(soundFileData);
        return -1;
    }
    soundFileCursor += 4;

    wavDataPtr = GetWavFormatData(soundFileCursor, "fmt ", &formatSize, fileSize - 12);
    if (wavDataPtr == NULL)
    {
        g_GameErrorContext.Log(g_NotWavMessage, path);
        g_ZunMemory.Free(soundFileData);
        return -1;
    }
    wavData = *wavDataPtr;

    wavDataPtr = GetWavFormatData(soundFileCursor, "data", &formatSize, fileSize - 12);
    if (wavDataPtr == NULL)
    {
        g_GameErrorContext.Log(g_NotWavMessage, path);
        g_ZunMemory.Free(soundFileData);
        return -1;
    }

    ZeroMemory(&soundBufferDesc, sizeof(soundBufferDesc));
    soundBufferDesc.dwSize = sizeof(soundBufferDesc);
    soundBufferDesc.dwFlags =
        DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_LOCSOFTWARE;
    soundBufferDesc.dwBufferBytes = formatSize;
    soundBufferDesc.lpwfxFormat = &wavData;
    if (FAILED(this->dsoundHdl->CreateSoundBuffer(
            &soundBufferDesc, &this->soundBuffers[index], NULL)))
    {
        g_ZunMemory.Free(soundFileData);
        return -1;
    }

    if (FAILED(this->soundBuffers[index]->Lock(
            0,
            formatSize,
            reinterpret_cast<LPVOID *>(&audioPtr1),
            reinterpret_cast<LPDWORD>(&audioSize1),
            reinterpret_cast<LPVOID *>(&audioPtr2),
            reinterpret_cast<LPDWORD>(&audioSize2),
            0)))
    {
        g_ZunMemory.Free(soundFileData);
        return -1;
    }

    CopyMemory(audioPtr1, wavDataPtr, audioSize1);
    if (audioSize2 != 0)
        CopyMemory(audioPtr2, reinterpret_cast<char *>(wavDataPtr) + audioSize1, audioSize2);

    this->soundBuffers[index]->Unlock(audioPtr1, audioSize1, audioPtr2, audioSize2);
    g_ZunMemory.Free(soundFileData);
    return 0;
}

static WAVEFORMATEX *GetWavFormatData(
    unsigned char *soundData,
    char *formatString,
    int *formatSize,
    unsigned int fileSizeExcludingFormat)
{
    while (fileSizeExcludingFormat > 0)
    {
        *formatSize = *reinterpret_cast<int *>(soundData + 4);
        if (strncmp(reinterpret_cast<char *>(soundData), formatString, 4) == 0)
            return reinterpret_cast<WAVEFORMATEX *>(soundData + 8);

        fileSizeExcludingFormat -= *formatSize + 8;
        soundData += *formatSize + 8;
    }
    return NULL;
}

int SoundPlayer::Release()
{
    int i;

    if (this->bgmFmtData != NULL)
        g_ZunMemory.Free(this->bgmFmtData);

    for (i = 0; i < 128; ++i)
    {
        if (this->duplicateSoundBuffers[i] != NULL)
        {
            this->duplicateSoundBuffers[i]->Release();
            this->duplicateSoundBuffers[i] = NULL;
        }
        if (this->soundBuffers[i] != NULL)
        {
            this->soundBuffers[i]->Release();
            this->soundBuffers[i] = NULL;
        }
    }

    if (this->manager == NULL)
        return 0;

    KillTimer(this->gameWindow, 1);
    this->StopBGM();
    this->dsoundHdl = NULL;
    this->initSoundBuffer->Stop();
    if (this->initSoundBuffer != NULL)
    {
        this->initSoundBuffer->Release();
        this->initSoundBuffer = NULL;
    }
    if (this->bgm != NULL)
    {
        delete this->bgm;
        this->bgm = NULL;
    }
    if (this->manager != NULL)
    {
        delete this->manager;
        this->manager = NULL;
    }

    for (i = 0; i < 19; ++i)
        this->FreePreloadedBGM(i);

    return 0;
}

int SoundPlayer::StartBGM(char *path)
{
    int result;
    ThBgmFormatProcessView *format;
    DWORD blockAlign;
    DWORD samplesPerSecond;
    DWORD notifySize;

    strcpy(this->currentBgmFileName, path);

    if (this->manager == NULL)
        return -1;

    if (this->dsoundHdl == NULL)
        return -1;

    this->StopBGM();

    format = this->bgmFmtData;
    blockAlign = format->format.nBlockAlign;
    samplesPerSecond = format->format.nSamplesPerSec;
    notifySize = samplesPerSecond * 4 * blockAlign / 16;
    notifySize -= notifySize % blockAlign;
    this->bgmUpdateEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
    this->bgmThreadHandle = CreateThread(
        NULL,
        0,
        SoundPlayer::BGMPlayerThread,
        g_Supervisor.window,
        0,
        &this->bgmThreadId);
    result = this->manager->CreateStreaming(
        &this->bgm,
        path,
        DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY,
        GUID_NULL,
        16,
        notifySize,
        this->bgmUpdateEvent,
        format);
    if (result < 0)
        return -1;

    return 0;
}

int SoundPlayer::GetFmtIndexByName(char *name)
{
    char *position;
    int index = 0;
    char buffer[128];

    position = strrchr(name, '/');
    if (position == NULL)
        position = strrchr(name, '\\');

    if (position == NULL)
        strcpy(buffer, name);
    else
        strcpy(buffer, position + 1);

    while (this->bgmFmtData[index].name[0] != '\0')
    {
        if (strcmp(this->bgmFmtData[index].name, buffer) == 0)
            break;
        ++index;
    }

    if (this->bgmFmtData[index].name[0] == '\0')
        index = 0;
    return index;
}

void SoundPlayer::FreePreloadedBGM(int index)
{
    if (this->bgmPreloadAllocations[index] != NULL)
    {
        g_ZunMemory.Free(this->bgmPreloadAllocations[index]);
        this->bgmPreloadAllocations[index] = NULL;
    }
}

int SoundPlayer::ReopenBGM(char *path)
{
    if (this->bgm == NULL)
        return -1;

    int index = this->GetFmtIndexByName(path);
    this->bgm->GetWaveFile()->Reopen(&this->bgmFmtData[index]);
    return 0;
}

DWORD WINAPI SoundPlayer::BGMPlayerThread(LPVOID parameter)
{
    DWORD waitResult;
    MSG message;
    int stopped = 0;

    do
    {
        waitResult = MsgWaitForMultipleObjects(
            1, &g_SoundPlayer.bgmUpdateEvent, FALSE, INFINITE, QS_ALLEVENTS);
        if (g_SoundPlayer.bgm == NULL)
            stopped = 1;

        switch (waitResult)
        {
        case WAIT_OBJECT_0:
            if (g_SoundPlayer.bgm != NULL && g_SoundPlayer.bgm->isPlaying)
            {
                g_SoundPlayer.bgm->isLocked = TRUE;
                g_SoundPlayer.bgm->HandleWaveStreamNotification(1);
                g_SoundPlayer.bgm->isLocked = FALSE;
            }
            break;
        case WAIT_OBJECT_0 + 1:
            if (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
            {
                do
                {
                    if (message.message == WM_QUIT)
                        stopped = 1;
                } while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE));
            }
            break;
        }
    } while (!stopped);

    return 0;
}

int SoundPlayer::PreloadBGM(int index, char *path)
{
    HANDLE file;
    int formatIndex;
    BYTE *buffer;
    DWORD bytesRead;

    if (this->bgmPreloadAllocations[index] != NULL)
    {
        if (strcmp(path, this->bgmFileNames[index]) == 0)
            return 0;
    }

    strcpy(g_SoundPlayer.bgmFileNames[index], path);

    if (!g_Supervisor.IsMusicPreloaded())
        return 0;

    if (this->manager == NULL)
        return 0;

    this->FreePreloadedBGM(index);

    file = CreateFileA(
        this->currentBgmFileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
        NULL);
    if (file == INVALID_HANDLE_VALUE)
        return -1;

    formatIndex = this->GetFmtIndexByName(path);
    SetFilePointer(file, this->bgmFmtData[formatIndex].startOffset, NULL, FILE_BEGIN);

    buffer = static_cast<BYTE *>(g_ZunMemory.Alloc(
        this->bgmFmtData[formatIndex].preloadAllocSize,
        "./system\\global.h"));
    if (buffer == NULL)
    {
        CloseHandle(file);
        return -1;
    }

    ReadFile(
        file,
        buffer,
        this->bgmFmtData[formatIndex].preloadAllocSize,
        &bytesRead,
        NULL);
    CloseHandle(file);

    this->bgmPreloadFmtData[index] = &this->bgmFmtData[formatIndex];
    this->bgmPreloadAllocations[index] = buffer;
    this->bgmPreloadData[index] = buffer;
    this->bgmPreloadAllocSizes[index] = this->bgmPreloadFmtData[index]->preloadAllocSize;
    return 0;
}

void SoundPlayer::StopBGM()
{
    if (this->bgm != NULL)
    {
        this->bgm->Stop();
        if (this->bgmThreadHandle != NULL)
        {
            PostThreadMessageA(this->bgmThreadId, WM_QUIT, 0, 0);
            while (WaitForSingleObject(this->bgmThreadHandle, 256))
                PostThreadMessageA(this->bgmThreadId, WM_QUIT, 0, 0);

            CloseHandle(this->bgmThreadHandle);
            CloseHandle(this->bgmUpdateEvent);
            this->bgmThreadHandle = NULL;
        }

        if (this->bgm != NULL)
        {
            delete this->bgm;
            this->bgm = NULL;
        }
    }
}

void SoundPlayer::QueueCommand(int opcode, int argument, char *path)
{
    int i;

    for (i = 0; i < 31; i++)
    {
        if (this->commandQueue[i].opcode != SOUNDPLAYER_COMMAND_NONE)
            continue;

        this->commandQueue[i].opcode = opcode;
        this->commandQueue[i].argument = argument;
        strcpy(this->commandQueue[i].path, path);
        this->commandQueue[i].step = 0;
        break;
    }
}

int SoundPlayer::LoadBGM(int index)
{
    int result;
    DWORD blockAlign;
    DWORD samplesPerSecond;
    DWORD notifySize;

    if (this->manager == NULL)
        return -1;

    if (g_Supervisor.musicMode == 0)
        return -1;

    if (this->dsoundHdl == NULL)
        return -1;

    if (!g_Supervisor.IsMusicPreloaded())
        return this->ReopenBGM(this->bgmFileNames[index]);

    if (this->bgmPreloadAllocations[index] == NULL)
        return -1;

    blockAlign = this->bgmPreloadFmtData[index]->format.nBlockAlign;
    samplesPerSecond = this->bgmPreloadFmtData[index]->format.nSamplesPerSec;
    notifySize = samplesPerSecond * 4 * blockAlign / 16;
    notifySize -= notifySize % blockAlign;

    this->bgmUpdateEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
    this->bgmThreadHandle = CreateThread(
        NULL,
        0,
        SoundPlayer::BGMPlayerThread,
        g_Supervisor.window,
        0,
        &this->bgmThreadId);

    result = this->manager->CreateStreamingFromMemory(
        &this->bgm,
        this->bgmPreloadData[index],
        this->bgmPreloadAllocSizes[index],
        this->bgmPreloadFmtData[index],
        DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY,
        GUID_NULL,
        16,
        notifySize,
        this->bgmUpdateEvent);
    if (result < 0)
        return -1;

    this->loadedBgmSlot = index;
    return 0;
}

int SoundPlayer::ProcessQueues()
{
    SoundPlayerCommand *commandCursor;
    BOOL restartCommandProcessing;
    LPDIRECTSOUNDBUFFER preloadBuffer;
    LPDIRECTSOUNDBUFFER reopenedBuffer;
    char *bgmPath;
    int bgmFormatIndex;
    int i;
    int j;
    int soundIndex;
    int averagedPan;
    float volumeScale;

    if (this->manager == NULL)
        return 0;

    commandCursor = this->commandQueue;

loop:
    restartCommandProcessing = FALSE;

    switch (commandCursor->opcode)
    {
    case SOUNDPLAYER_COMMAND_SET_VOLUME:
        if (this->bgm != NULL)
            this->bgm->SetVolume(this->bgmVolume);
        goto next_command;

    case SOUNDPLAYER_COMMAND_PRELOAD_BGM:
        if (g_Supervisor.IsMusicPreloaded())
        {
            if (commandCursor->step != 0)
            {
                commandCursor->step++;
                break;
            }
            this->StopBGM();
            this->PreloadBGM(commandCursor->argument, commandCursor->path);
        }
        else
        {
            this->PreloadBGM(commandCursor->argument, commandCursor->path);
        }
        restartCommandProcessing = TRUE;
        goto next_command;

    case SOUNDPLAYER_COMMAND_LOAD_BGM:
        if (g_Supervisor.IsMusicPreloaded() && commandCursor->argument >= 0)
        {
            if (commandCursor->step == 0)
            {
                if (this->LoadBGM(commandCursor->argument) != 0)
                    goto next_command;
            }
            else if (commandCursor->step == 2)
            {
                if (this->bgm != NULL)
                {
                    if (FAILED(this->bgm->Reset()))
                        goto next_command;
                }
            }
            else if (commandCursor->step == 5)
            {
                preloadBuffer = this->bgm->GetBuffer(0);
                commandCursor->argument = this->bgm->GetWaveFile()->GetFormat()->totalLength != 0;
                if (FAILED(this->bgm->FillBufferWithSound(preloadBuffer, commandCursor->argument)))
                    goto next_command;
            }
            else if (commandCursor->step == 7)
            {
                this->bgm->Play(0, DSBPLAY_LOOPING);
            }
            else if (commandCursor->step >= 20)
            {
                goto next_command;
            }
        }
        else if (this->bgm == NULL)
        {
            goto next_command;
        }
        else if (commandCursor->step == 0)
        {
            this->bgm->Stop();
        }
        else if (commandCursor->step == 1)
        {
            if (this->bgm->isLocked)
                break;
            this->bgm->InitSoundBuffers();
        }
        else if (commandCursor->step == 2)
        {
            bgmPath = commandCursor->argument >= 0 ? this->bgmFileNames[commandCursor->argument] : commandCursor->path;
            bgmFormatIndex = this->GetFmtIndexByName(bgmPath);
            this->bgm->GetWaveFile()->Reopen(&this->bgmFmtData[bgmFormatIndex]);
        }
        else if (commandCursor->step == 3)
        {
            reopenedBuffer = this->bgm->GetBuffer(0);
            this->bgm->Reset();
            commandCursor->argument = this->bgm->GetWaveFile()->GetFormat()->totalLength != 0;
            if (FAILED(this->bgm->FillBufferWithSound(reopenedBuffer, commandCursor->argument)))
                goto next_command;
        }
        else if (commandCursor->step == 4)
        {
            this->bgm->Play(0, DSBPLAY_LOOPING);
        }
        else if (commandCursor->step >= 7)
        {
            goto next_command;
        }
        commandCursor->step++;
        break;

    case SOUNDPLAYER_COMMAND_RELEASE_BGM:
        if (this->bgm == NULL)
            goto next_command;
        if (commandCursor->step == 0)
        {
            this->bgm->Stop();
        }
        else if (commandCursor->step == 1)
        {
            if (this->bgmThreadHandle == NULL)
                goto next_command;
            PostThreadMessageA(this->bgmThreadId, WM_QUIT, 0, 0);
        }
        else if (commandCursor->step == 2)
        {
            if (WaitForSingleObject(this->bgmThreadHandle, 256))
            {
                PostThreadMessageA(this->bgmThreadId, WM_QUIT, 0, 0);
                commandCursor->step--;
            }
            else
            {
                this->bgmThreadHandle = NULL;
            }
        }
        else if (commandCursor->step == 3)
        {
            CloseHandle(this->bgmThreadHandle);
            CloseHandle(this->bgmUpdateEvent);
            this->bgmThreadHandle = NULL;
            if (this->bgm != NULL)
            {
                delete this->bgm;
                this->bgm = NULL;
            }
        }
        else if (commandCursor->step == 10)
        {
            goto next_command;
        }
        commandCursor->step++;
        break;

    case SOUNDPLAYER_COMMAND_STOP_BGM:
        if (this->bgm == NULL)
            goto next_command;
        if (commandCursor->step == 0)
        {
            this->bgm->Stop();
        }
        else if (commandCursor->step == 1)
        {
            goto next_command;
        }
        commandCursor->step++;
        break;

    case SOUNDPLAYER_COMMAND_FADE_OUT:
        g_SoundPlayer.FadeOut((float)commandCursor->argument);
        goto next_command;

    case SOUNDPLAYER_COMMAND_PAUSE:
        if (g_Supervisor.musicMode == 1)
        {
            if (this->bgm->isLocked)
                break;
            if (this->bgm != NULL)
                this->bgm->Pause();
        }
        goto next_command;

    case SOUNDPLAYER_COMMAND_UNPAUSE:
        if (g_Supervisor.musicMode == 1)
        {
            if (this->bgm->isLocked)
                break;
            if (this->bgm != NULL)
                this->bgm->Unpause();
        }
        goto next_command;

    case SOUNDPLAYER_COMMAND_FREE_PRELOADED_BGM:
        if (g_Supervisor.IsMusicPreloaded())
        {
            if (commandCursor->step != 0)
            {
                commandCursor->step++;
                break;
            }
            this->StopBGM();
        }
        this->FreePreloadedBGM(commandCursor->argument);
        restartCommandProcessing = TRUE;
        goto next_command;

    case SOUNDPLAYER_COMMAND_START_LOADED_BGM:
        if (this->bgm != NULL)
            this->bgm->Play(0, DSBPLAY_LOOPING);
        goto next_command;

    default:
        break;

    next_command:
        for (i = 0; i < 31; i++, commandCursor++)
        {
            if (commandCursor->opcode == SOUNDPLAYER_COMMAND_NONE)
                break;
            memcpy(commandCursor, commandCursor + 1, sizeof(*commandCursor));
        }

        if (restartCommandProcessing)
            goto loop;
    }

    if (!g_Supervisor.playSounds)
        return this->commandQueue[0].opcode;

    for (i = 0; i < 12; i++)
    {
        if (this->soundQueue[i] < 0)
            break;

        soundIndex = this->soundQueue[i];
        this->soundQueue[i] = -1;

        averagedPan = 0;
        for (j = 0; j < this->soundQueueRequestCounts[i]; j++)
            averagedPan += this->soundQueuePanData[i][j];
        averagedPan /= this->soundQueueRequestCounts[i];
        this->soundQueueRequestCounts[i] = 0;

        if (this->duplicateSoundBuffers[soundIndex] == NULL)
            continue;

        this->duplicateSoundBuffers[soundIndex]->Stop();
        this->duplicateSoundBuffers[soundIndex]->SetCurrentPosition(0);
        this->duplicateSoundBuffers[soundIndex]->SetPan(averagedPan);

        volumeScale = (float)g_SoundPlayer.sfxVolume / 100.0f;
        if (g_SoundPlayer.sfxVolume != 0)
        {
            volumeScale = 1.0f - volumeScale;
            volumeScale = volumeScale * volumeScale * volumeScale;
            volumeScale = 1.0f - volumeScale;
            this->duplicateSoundBuffers[soundIndex]->SetVolume(
                (int)((g_SoundBufferIdxVol[soundIndex].volume + 5000) * volumeScale) - 5000);
        }
        else
        {
            this->duplicateSoundBuffers[soundIndex]->SetVolume(-10000);
        }

        this->duplicateSoundBuffers[soundIndex]->Play(0, 0, 0);
    }

    return this->commandQueue[0].opcode;
}
