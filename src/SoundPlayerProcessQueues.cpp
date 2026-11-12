#include <windows.h>
#include <dsound.h>
#include <string.h>

#include "ZunMemory.hpp"
#include "GameErrorContext.hpp"
#include "FileSystem.hpp"
#include "SoundPlayer.hpp"

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

SoundPlayer::SoundPlayer()
{
    ZeroMemory(this, sizeof(SoundPlayer));
    for (int i = 0; i < 128; ++i)
        this->unconsumedMetadataBySound[i] = -1;
}

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

// Original i386 builds each active-sound slot address from the real 128
// DirectSound pointers, using modulo-32 index*4 byte arithmetic. Do not
// replace the six genuine live reads or add a new index guard.
// SFX source paths and duplicate-volume metadata are the original
// process-global 39 DWORD path pointers and 54 authentic 0x08-byte rows.
// Retain independent live reads and one unsigned Win32 index stride.
typedef char SoundBufferIdxVolumeNativeStride[
    (sizeof(SoundBufferIdxVolume) == 0x08) ? 1 : -1];
static __forceinline char * &NativeSoundSfxPath(int index)
{
    return *reinterpret_cast<char **>(
        reinterpret_cast<unsigned int>(&g_SfxList[0]) +
        static_cast<unsigned int>(index) * sizeof(g_SfxList[0]));
}
static __forceinline const SoundBufferIdxVolume &NativeSoundIndexVolume(int index)
{
    return *reinterpret_cast<const SoundBufferIdxVolume *>(
        reinterpret_cast<unsigned int>(&g_SoundBufferIdxVol[0]) +
        static_cast<unsigned int>(index) * sizeof(g_SoundBufferIdxVol[0]));
}

static __forceinline LPDIRECTSOUNDBUFFER &NativeActiveSound(
    SoundPlayer *player, int index)
{
    return *reinterpret_cast<LPDIRECTSOUNDBUFFER *>(
        reinterpret_cast<unsigned int>(&player->soundBuffers[0]) +
        static_cast<unsigned int>(index) * sizeof(player->soundBuffers[0]));
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
        if (this->LoadSound(i, NativeSoundSfxPath(i)) != 0)
        {
            g_GameErrorContext.Log(g_SoundLoadErrorMessage, NativeSoundSfxPath(i));
            return -1;
        }
    }

    for (i = 0; i < sizeof(g_SoundBufferIdxVol) / sizeof(g_SoundBufferIdxVol[0]); ++i)
    {
        this->dsoundHdl->DuplicateSoundBuffer(
            NativeActiveSound(this, NativeSoundIndexVolume(i).bufferIdx),
            &this->duplicateSoundBuffers[i]);
        this->duplicateSoundBuffers[i]->SetCurrentPosition(0);
        this->duplicateSoundBuffers[i]->SetVolume(NativeSoundIndexVolume(i).volume);
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

    if (NativeActiveSound(this, index) != NULL)
    {
        NativeActiveSound(this, index)->Release();
        NativeActiveSound(this, index) = NULL;
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
    soundFileCursor = reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(soundFileCursor) + 4u);

    fileSize = *reinterpret_cast<int *>(soundFileCursor);
    soundFileCursor = reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(soundFileCursor) + 4u);

    if (strncmp(reinterpret_cast<char *>(soundFileCursor), "WAVE", 4) != 0)
    {
        g_GameErrorContext.Log(g_NotWavMessage, path);
        g_ZunMemory.Free(soundFileData);
        return -1;
    }
    soundFileCursor = reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(soundFileCursor) + 4u);

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
            &soundBufferDesc, &NativeActiveSound(this, index), NULL)))
    {
        g_ZunMemory.Free(soundFileData);
        return -1;
    }

    if (FAILED(NativeActiveSound(this, index)->Lock(
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
        CopyMemory(audioPtr2, reinterpret_cast<char *>(
            reinterpret_cast<unsigned int>(wavDataPtr) +
            static_cast<unsigned int>(audioSize1)), audioSize2);

    NativeActiveSound(this, index)->Unlock(audioPtr1, audioSize1, audioPtr2, audioSize2);
    g_ZunMemory.Free(soundFileData);
    return 0;
}

// This WAV chunk walk uses untrusted, file-backed word counts. The native
// 1.50a i386 code wraps both the remaining-byte count and the cursor
// advance modulo 2^32 without clamping malformed source chunk sizes.
static WAVEFORMATEX *GetWavFormatData(
    unsigned char *soundData,
    char *formatString,
    int *formatSize,
    unsigned int fileSizeExcludingFormat)
{
    while (fileSizeExcludingFormat > 0)
    {
        *formatSize = *reinterpret_cast<int *>(
            reinterpret_cast<unsigned int>(soundData) + 4u);
        if (strncmp(reinterpret_cast<char *>(soundData), formatString, 4) == 0)
            return reinterpret_cast<WAVEFORMATEX *>(
                reinterpret_cast<unsigned int>(soundData) + 8u);

        fileSizeExcludingFormat -=
            static_cast<unsigned int>(*formatSize) + 8u;
        soundData = reinterpret_cast<unsigned char *>(
            reinterpret_cast<unsigned int>(soundData) +
            static_cast<unsigned int>(*formatSize) + 8u);
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

// The externally loaded BGM format table has genuine 0x34-byte records;
// preload slots and filename rows belong to the real SoundPlayer allocation.
// Original i386 calculates the signed index times each native record width
// as an unsigned DWORD without an extra C++ array bounds condition.
template <typename T>
static __forceinline T &NativeSoundBgmSlot(T *base, int index)
{
    return *reinterpret_cast<T *>(
        reinterpret_cast<unsigned int>(base) +
        static_cast<unsigned int>(index) * sizeof(T));
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

    while (NativeSoundBgmSlot(this->bgmFmtData, index).name[0] != '\0')
    {
        if (strcmp(NativeSoundBgmSlot(this->bgmFmtData, index).name, buffer) == 0)
            break;
        ++index;
    }

    if (NativeSoundBgmSlot(this->bgmFmtData, index).name[0] == '\0')
        index = 0;
    return index;
}

void SoundPlayer::FreePreloadedBGM(int index)
{
    if (NativeSoundBgmSlot(this->bgmPreloadAllocations, index) != NULL)
    {
        g_ZunMemory.Free(NativeSoundBgmSlot(this->bgmPreloadAllocations, index));
        NativeSoundBgmSlot(this->bgmPreloadAllocations, index) = NULL;
    }
}

int SoundPlayer::ReopenBGM(char *path)
{
    if (this->bgm == NULL)
        return -1;

    int index = this->GetFmtIndexByName(path);
    this->bgm->GetWaveFile()->Reopen(&NativeSoundBgmSlot(this->bgmFmtData, index));
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

    if (NativeSoundBgmSlot(this->bgmPreloadAllocations, index) != NULL)
    {
        if (strcmp(path, NativeSoundBgmSlot(this->bgmFileNames, index)) == 0)
            return 0;
    }

    strcpy(NativeSoundBgmSlot(g_SoundPlayer.bgmFileNames, index), path);

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
    SetFilePointer(file, NativeSoundBgmSlot(this->bgmFmtData, formatIndex).startOffset, NULL, FILE_BEGIN);

    buffer = static_cast<BYTE *>(g_ZunMemory.Alloc(
        NativeSoundBgmSlot(this->bgmFmtData, formatIndex).preloadAllocSize,
        "./system\\global.h"));
    if (buffer == NULL)
    {
        CloseHandle(file);
        return -1;
    }

    ReadFile(
        file,
        buffer,
        NativeSoundBgmSlot(this->bgmFmtData, formatIndex).preloadAllocSize,
        &bytesRead,
        NULL);
    CloseHandle(file);

    NativeSoundBgmSlot(this->bgmPreloadFmtData, index) = &NativeSoundBgmSlot(this->bgmFmtData, formatIndex);
    NativeSoundBgmSlot(this->bgmPreloadAllocations, index) = buffer;
    NativeSoundBgmSlot(this->bgmPreloadData, index) = buffer;
    NativeSoundBgmSlot(this->bgmPreloadAllocSizes, index) = NativeSoundBgmSlot(this->bgmPreloadFmtData, index)->preloadAllocSize;
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

// The original SoundPlayer owns 32 contiguous 0x10C-byte command records.
#define TH09_SOUND_NATIVE_COMMAND(owner, slot) \
    (*reinterpret_cast<SoundPlayerCommand *>( \
        reinterpret_cast<unsigned int>(&(owner)->commandQueue[0]) + \
        static_cast<unsigned int>(slot) * sizeof((owner)->commandQueue[0])))

void SoundPlayer::QueueCommand(int opcode, int argument, char *path)
{
    int i;

    for (i = 0; i < 31; i++)
    {
        if (TH09_SOUND_NATIVE_COMMAND(this, i).opcode != SOUNDPLAYER_COMMAND_NONE)
            continue;

        TH09_SOUND_NATIVE_COMMAND(this, i).opcode = opcode;
        TH09_SOUND_NATIVE_COMMAND(this, i).argument = argument;
        strcpy(TH09_SOUND_NATIVE_COMMAND(this, i).path, path);
        TH09_SOUND_NATIVE_COMMAND(this, i).step = 0;
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
        return this->ReopenBGM(NativeSoundBgmSlot(this->bgmFileNames, index));

    if (NativeSoundBgmSlot(this->bgmPreloadAllocations, index) == NULL)
        return -1;

    blockAlign = NativeSoundBgmSlot(this->bgmPreloadFmtData, index)->format.nBlockAlign;
    samplesPerSecond = NativeSoundBgmSlot(this->bgmPreloadFmtData, index)->format.nSamplesPerSec;
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
        NativeSoundBgmSlot(this->bgmPreloadData, index),
        NativeSoundBgmSlot(this->bgmPreloadAllocSizes, index),
        NativeSoundBgmSlot(this->bgmPreloadFmtData, index),
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

void SoundPlayer::UpdateFades()
{
    if (this->bgm != NULL)
    {
        this->bgm->UpdateFadeOut();
        this->bgm->UpdateFadeIn();
        this->bgm->UpdatePartialFadeOut();
        this->bgm->UpdatePartialFadeIn();
    }
}

// Real i386 sound paths use the original queue, 12x128 pan data,
// 0x08-byte volume rows, DirectSound pointers and BGM file allocations.
// Each live selector forms an unsigned native DWORD address without a new guard.
#define TH09_SOUND_NATIVE_QUEUE(index) \
    (*reinterpret_cast<int *>(reinterpret_cast<unsigned int>(&this->soundQueue[0]) + \
        static_cast<unsigned int>(index) * sizeof(this->soundQueue[0])))
#define TH09_SOUND_NATIVE_COUNT(index) \
    (*reinterpret_cast<int *>(reinterpret_cast<unsigned int>(&this->soundQueueRequestCounts[0]) + \
        static_cast<unsigned int>(index) * sizeof(this->soundQueueRequestCounts[0])))
#define TH09_SOUND_NATIVE_PAN(row, index) \
    (*reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned int>(&this->soundQueuePanData[0][0]) + \
        static_cast<unsigned int>(row) * sizeof(this->soundQueuePanData[0]) + \
        static_cast<unsigned int>(index) * sizeof(this->soundQueuePanData[0][0])))
#define TH09_SOUND_NATIVE_BUFFER(index) \
    (*reinterpret_cast<LPDIRECTSOUNDBUFFER *>(reinterpret_cast<unsigned int>(&this->duplicateSoundBuffers[0]) + \
        static_cast<unsigned int>(index) * sizeof(this->duplicateSoundBuffers[0])))
#define TH09_SOUND_NATIVE_VOLUME(index) \
    NativeSoundIndexVolume(index).volume
#define TH09_SOUND_NATIVE_BGM_PATH(index) \
    (*reinterpret_cast<char (*)[256]>(reinterpret_cast<unsigned int>(&this->bgmFileNames[0]) + \
        static_cast<unsigned int>(index) * sizeof(this->bgmFileNames[0])))
#define TH09_SOUND_NATIVE_BGM_FMT(index) \
    (*reinterpret_cast<ThBgmFormatProcessView *>(reinterpret_cast<unsigned int>(&this->bgmFmtData[0]) + \
        static_cast<unsigned int>(index) * sizeof(this->bgmFmtData[0])))

int SoundPlayer::ProcessQueues()
{
    SoundPlayerCommand *commandCursor;
    // Restart flag during command dispatch, then the SFX queue index.
    // These phases share the scalar workspace; shifting commands still uses i.
    int queuePhase;
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
    queuePhase = FALSE;

    // The live signed BGM command step is a native i386 DWORD state word.
    // Advance/retry it modulo 2^32, retaining original state transitions.
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
                ++*reinterpret_cast<unsigned int *>(&commandCursor->step);
                break;
            }
            this->StopBGM();
            this->PreloadBGM(commandCursor->argument, commandCursor->path);
        }
        else
        {
            this->PreloadBGM(commandCursor->argument, commandCursor->path);
        }
        queuePhase = TRUE;
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
            bgmPath = commandCursor->argument >= 0 ? TH09_SOUND_NATIVE_BGM_PATH(commandCursor->argument) : commandCursor->path;
            bgmFormatIndex = this->GetFmtIndexByName(bgmPath);
            this->bgm->GetWaveFile()->Reopen(&TH09_SOUND_NATIVE_BGM_FMT(bgmFormatIndex));
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
        ++*reinterpret_cast<unsigned int *>(&commandCursor->step);
        break;

    case SOUNDPLAYER_COMMAND_START_LOADED_BGM:
        if (this->bgm != NULL)
            this->bgm->Play(0, DSBPLAY_LOOPING);
        goto next_command;

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
                --*reinterpret_cast<unsigned int *>(&commandCursor->step);
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
        ++*reinterpret_cast<unsigned int *>(&commandCursor->step);
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
        ++*reinterpret_cast<unsigned int *>(&commandCursor->step);
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
                ++*reinterpret_cast<unsigned int *>(&commandCursor->step);
                break;
            }
            this->StopBGM();
            this->FreePreloadedBGM(commandCursor->argument);
        }
        else
        {
            this->FreePreloadedBGM(commandCursor->argument);
        }
        queuePhase = TRUE;
        goto next_command;

    default:
        break;

    next_command:
        // Retire each real native 0x10C-byte command record in sequence,
        // preserving the original fixed-capacity shift and memcpy order.
        for (i = 0; i < 31; i++,
             commandCursor = reinterpret_cast<SoundPlayerCommand *>(
                 reinterpret_cast<unsigned int>(commandCursor) +
                 sizeof(*commandCursor)))
        {
            if (commandCursor->opcode == SOUNDPLAYER_COMMAND_NONE)
                break;
            memcpy(commandCursor,
                   reinterpret_cast<const SoundPlayerCommand *>(
                       reinterpret_cast<unsigned int>(commandCursor) +
                       sizeof(*commandCursor)),
                   sizeof(*commandCursor));
        }

        if (queuePhase)
            goto loop;
    }

    if (!g_Supervisor.playSounds)
        return this->commandQueue[0].opcode;

    for (queuePhase = 0; queuePhase < 12; queuePhase++)
    {
        if (TH09_SOUND_NATIVE_QUEUE(queuePhase) < 0)
            break;

        soundIndex = TH09_SOUND_NATIVE_QUEUE(queuePhase);
        TH09_SOUND_NATIVE_QUEUE(queuePhase) = -1;

        averagedPan = 0;
        for (j = 0; j < TH09_SOUND_NATIVE_COUNT(queuePhase); j++)
            averagedPan = static_cast<int>(static_cast<unsigned int>(averagedPan) + TH09_SOUND_NATIVE_PAN(queuePhase, j));
        averagedPan /= TH09_SOUND_NATIVE_COUNT(queuePhase);
        TH09_SOUND_NATIVE_COUNT(queuePhase) = 0;

        if (TH09_SOUND_NATIVE_BUFFER(soundIndex) == NULL)
            continue;

        TH09_SOUND_NATIVE_BUFFER(soundIndex)->Stop();
        TH09_SOUND_NATIVE_BUFFER(soundIndex)->SetCurrentPosition(0);
        TH09_SOUND_NATIVE_BUFFER(soundIndex)->SetPan(averagedPan);

        volumeScale = (float)g_SoundPlayer.sfxVolume / 100.0f;
        if (g_SoundPlayer.sfxVolume != 0)
        {
            volumeScale = 1.0f - volumeScale;
            volumeScale = volumeScale * volumeScale * volumeScale;
            volumeScale = 1.0f - volumeScale;
            TH09_SOUND_NATIVE_BUFFER(soundIndex)->SetVolume(
                (int)((TH09_SOUND_NATIVE_VOLUME(soundIndex) + 5000) * volumeScale) - 5000);
        }
        else
        {
            TH09_SOUND_NATIVE_BUFFER(soundIndex)->SetVolume(-10000);
        }

        TH09_SOUND_NATIVE_BUFFER(soundIndex)->Play(0, 0, 0);
    }

    return this->commandQueue[0].opcode;
}

#undef TH09_SOUND_NATIVE_QUEUE
#undef TH09_SOUND_NATIVE_COUNT
#undef TH09_SOUND_NATIVE_PAN
#undef TH09_SOUND_NATIVE_BUFFER
#undef TH09_SOUND_NATIVE_VOLUME
#undef TH09_SOUND_NATIVE_BGM_PATH
#undef TH09_SOUND_NATIVE_BGM_FMT

void SoundPlayer::FadeOut(float seconds)
{
    if (this->bgm != NULL)
        this->bgm->FadeOut(seconds);
}

// Native request counts are signed DWORD storage but advance as wrapping
// unsigned i386 words, including script- or state-corrupted negative counts.
// The real per-queue indices and original count checks remain live.
// These live producer indices refer to the actual 8-byte SFX metadata
// table, four-byte per-sound state slots and 12x128 pan words. Form
// native i386 byte addresses without imposing the C++ array bounds on
// a signed sound ID or signed, post-incremented pan count. Preserve each
// independent load and write (a callable helper changes VC7.1 codegen).
void SoundPlayer::PlaySoundByIdx(int index, int pan)
{
    int unconsumedMetadata;
    int i;

    unconsumedMetadata = (*reinterpret_cast<const SoundBufferIdxVolume *>(
    reinterpret_cast<unsigned int>(&g_SoundBufferIdxVol[0]) +
    static_cast<unsigned int>(index) * sizeof(g_SoundBufferIdxVol[0]))).unconsumedMetadata;
    for (i = 0; i < 12; i++)
    {
        if (this->soundQueue[i] < 0)
            break;
        if (this->soundQueue[i] == index)
        {
            if (this->soundQueueRequestCounts[i] < 0x80)
                *reinterpret_cast<unsigned int *>(
    reinterpret_cast<unsigned int>(&this->soundQueuePanData[0][0]) +
    static_cast<unsigned int>(i) * sizeof(this->soundQueuePanData[0]) +
    static_cast<unsigned int>((*reinterpret_cast<unsigned int *>(&this->soundQueueRequestCounts[i]))++) *
    sizeof(this->soundQueuePanData[0][0])) = pan;
            return;
        }
    }

    if (i >= 12)
        return;

    this->soundQueue[i] = index;
    *reinterpret_cast<int *>(
    reinterpret_cast<unsigned int>(&this->unconsumedMetadataBySound[0]) +
    static_cast<unsigned int>(index) * sizeof(this->unconsumedMetadataBySound[0])) = unconsumedMetadata;
    this->soundQueuePanData[i][0] = pan;
    (*reinterpret_cast<unsigned int *>(&this->soundQueueRequestCounts[i]))++;
}

void SoundPlayer::PlaySoundPositionedByIdx(int index, float positionX)
{
    int unconsumedMetadata;
    int pan;
    int i;

    unconsumedMetadata = (*reinterpret_cast<const SoundBufferIdxVolume *>(
    reinterpret_cast<unsigned int>(&g_SoundBufferIdxVol[0]) +
    static_cast<unsigned int>(index) * sizeof(g_SoundBufferIdxVol[0]))).unconsumedMetadata;
    pan = (int)(positionX * 6.9444447f);
    for (i = 0; i < 12; i++)
    {
        if (this->soundQueue[i] < 0)
            break;
        if (this->soundQueue[i] == index)
        {
            if (this->soundQueueRequestCounts[i] < 0x80)
                *reinterpret_cast<unsigned int *>(
    reinterpret_cast<unsigned int>(&this->soundQueuePanData[0][0]) +
    static_cast<unsigned int>(i) * sizeof(this->soundQueuePanData[0]) +
    static_cast<unsigned int>((*reinterpret_cast<unsigned int *>(&this->soundQueueRequestCounts[i]))++) *
    sizeof(this->soundQueuePanData[0][0])) = pan;
            return;
        }
    }

    if (i >= 12)
        return;

    this->soundQueue[i] = index;
    *reinterpret_cast<int *>(
    reinterpret_cast<unsigned int>(&this->unconsumedMetadataBySound[0]) +
    static_cast<unsigned int>(index) * sizeof(this->unconsumedMetadataBySound[0])) = unconsumedMetadata;
    this->soundQueuePanData[i][0] = pan;
    (*reinterpret_cast<unsigned int *>(&this->soundQueueRequestCounts[i]))++;
}

#undef TH09_SOUND_NATIVE_COMMAND
