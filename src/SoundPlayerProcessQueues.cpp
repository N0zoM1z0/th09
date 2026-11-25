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
SoundBufferIdxVolume g_SoundBufferIdxVol[54] = {
    {  0, -2100,   0 },
    {  0, -2400,   0 },
    {  1, -1600,   5 },
    {  1, -1900,   5 },
    {  2, -1500, 100 },
    {  3, -1100, 100 },
    {  4, -1100, 100 },
    {  5, -1900,  50 },
    {  6, -2200,  50 },
    {  7, -2400,  50 },
    {  8, -1500, 100 },
    {  9, -1500, 100 },
    { 10, -1800,  10 },
    { 11, -1800,  10 },
    { 12, -1100, 100 },
    {  5, -1100,  50 },
    { 13, -1300,  50 },
    { 14, -1400,  50 },
    { 15, -1300, 100 },
    {  0, -1900,   0 },
    { 16, -1080,   0 },
    {  0, -1900,   0 },
    {  5,  -900,  20 },
    {  6, -1800,  20 },
    {  7, -1800,  20 },
    { 17, -1100,  50 },
    { 18, -1300,  50 },
    { 19, -1500,  50 },
    { 20,  -500, 140 },
    {  0, -1900,   0 },
    { 21, -1100,  20 },
    { 38,  -300, 100 },
    { 21, -1200,  20 },
    { 17,  -500,  50 },
    { 22,  -800, 100 },
    { 23,  -800, 100 },
    {  0, -1900,   0 },
    { 24, -1100,   0 },
    { 25,  -300, 100 },
    { 26,  -200, 100 },
    { 27,     0, 100 },
    { 27,  -600, 100 },
    {  8,  -400, 100 },
    { 28, -1000, 100 },
    { 29, -1000, 100 },
    { 30,  -400, 100 },
    { 31,  -700,   0 },
    { 32,  -700,   0 },
    { 33,  -350, 100 },
    { 34,  -500, 100 },
    { 35,  -900, 100 },
    { 36,  -900, 100 },
    { 37,  -900, 100 },
    { 12,  -500, 100 },
};
char *g_SfxList[39] = {
    "se_plst00.wav",
    "se_enep00.wav",
    "se_pldead00.wav",
    "se_power0.wav",
    "se_power1.wav",
    "se_tan00.wav",
    "se_tan01.wav",
    "se_tan02.wav",
    "se_ok00.wav",
    "se_cancel00.wav",
    "se_select00.wav",
    "se_gun00.wav",
    "se_cat00.wav",
    "se_lazer00.wav",
    "se_lazer01.wav",
    "se_enep01.wav",
    "se_damage00.wav",
    "se_kira00.wav",
    "se_kira01.wav",
    "se_kira02.wav",
    "se_extend.wav",
    "se_graze.wav",
    "se_pause.wav",
    "se_cardget.wav",
    "se_damage01.wav",
    "se_timeout2.wav",
    "se_invalid.wav",
    "se_slash.wav",
    "se_charge00.wav",
    "se_charge01b.wav",
    "se_exattack.wav",
    "se_eterase.wav",
    "se_gosp.wav",
    "se_life1.wav",
    "se_playerdead.wav",
    "se_chargeup.wav",
    "se_warning.wav",
    "se_timestop0.wav",
    "se_powerup.wav",
};
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

// The active SFX queue is a native twelve-element integer array.
// Its original read/write ordering is preserved by direct element lvalues.

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

// SFX paths, volume rows and active/duplicate sound buffers are native
// C++ arrays. Retain independent reads and original index expressions;
// do not introduce an additional bounds guard.
typedef char SoundBufferIdxVolumeNativeStride[
    (sizeof(SoundBufferIdxVolume) == 0x08) ? 1 : -1];

// Original SoundPlayer owns the 128 source and 128 duplicate buffers.


int SoundPlayer::InitSoundBuffers()
{
    int i;

    if (this->manager == NULL)
        return -1;

    if (this->dsoundHdl == NULL)
        return 0;

    for (i = 0; i < 12; ++i)
        (this->soundQueue[i]) = -1;

    for (i = 0; i < 39; ++i)
    {
        if (this->LoadSound(i, (g_SfxList[i])) != 0)
        {
            g_GameErrorContext.Log(g_SoundLoadErrorMessage, (g_SfxList[i]));
            return -1;
        }
    }

    for (i = 0; i < sizeof(g_SoundBufferIdxVol) / sizeof(g_SoundBufferIdxVol[0]); ++i)
    {
        this->dsoundHdl->DuplicateSoundBuffer(
            (this->soundBuffers[(g_SoundBufferIdxVol[i]).bufferIdx]),
            &(this->duplicateSoundBuffers[i]));
        (this->duplicateSoundBuffers[i])->SetCurrentPosition(0);
        (this->duplicateSoundBuffers[i])->SetVolume((g_SoundBufferIdxVol[i]).volume);
    }

    return 0;
}



int SoundPlayer::LoadSound(int index, char *path)
{
    unsigned char *soundFileData;
    unsigned char *soundFileCursor;
    int fileSize;
    WAVEFORMATEX *wavDataPtr;
    LPVOID audioPtr1;
    LPVOID audioPtr2;
    DWORD audioSize1;
    DWORD audioSize2;
    // The DirectSound lock API owns both output buffers as LPVOID and
    // publishes their lengths as DWORDs, not WAVEFORMATEX records.
    WAVEFORMATEX wavData;
    int formatSize;
    DSBUFFERDESC soundBufferDesc;

    if (this->manager == NULL)
        return 0;

    if ((this->soundBuffers[index]) != NULL)
    {
        (this->soundBuffers[index])->Release();
        (this->soundBuffers[index]) = NULL;
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
            &soundBufferDesc, &(this->soundBuffers[index]), NULL)))
    {
        g_ZunMemory.Free(soundFileData);
        return -1;
    }

    if (FAILED((this->soundBuffers[index])->Lock(
            0,
            formatSize,
            &audioPtr1,
            &audioSize1,
            &audioPtr2,
            &audioSize2,
            0)))
    {
        g_ZunMemory.Free(soundFileData);
        return -1;
    }

    CopyMemory(audioPtr1, wavDataPtr, audioSize1);
    if (audioSize2 != 0)
        CopyMemory(audioPtr2,
            reinterpret_cast<const unsigned char *>(wavDataPtr) + audioSize1,
            audioSize2);

    (this->soundBuffers[index])->Unlock(audioPtr1, audioSize1, audioPtr2, audioSize2);
    g_ZunMemory.Free(soundFileData);
    return 0;
}

// Walk the actual variable-length RIFF chunk stream with a byte cursor.
// Keep the file-supplied chunk length and native 32-bit arithmetic; do not
// introduce a new clamp, checksum or malformed-input branch.
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

        fileSizeExcludingFormat -=
            static_cast<unsigned int>(*formatSize) + 8u;
        soundData += static_cast<unsigned int>(*formatSize) + 8u;
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

// BGM format records, preload slots and filename rows use their real
// pointer/array types. All original side/index expressions stay unguarded.

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

    while ((this->bgmFmtData[index]).name[0] != '\0')
    {
        if (strcmp((this->bgmFmtData[index]).name, buffer) == 0)
            break;
        ++index;
    }

    if ((this->bgmFmtData[index]).name[0] == '\0')
        index = 0;
    return index;
}

void SoundPlayer::FreePreloadedBGM(int index)
{
    if ((this->bgmPreloadAllocations[index]) != NULL)
    {
        g_ZunMemory.Free((this->bgmPreloadAllocations[index]));
        (this->bgmPreloadAllocations[index]) = NULL;
    }
}

int SoundPlayer::ReopenBGM(char *path)
{
    if (this->bgm == NULL)
        return -1;

    int index = this->GetFmtIndexByName(path);
    this->bgm->GetWaveFile()->Reopen(&(this->bgmFmtData[index]));
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

    if ((this->bgmPreloadAllocations[index]) != NULL)
    {
        if (strcmp(path, (this->bgmFileNames[index])) == 0)
            return 0;
    }

    strcpy((g_SoundPlayer.bgmFileNames[index]), path);

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
    SetFilePointer(file, (this->bgmFmtData[formatIndex]).startOffset, NULL, FILE_BEGIN);

    buffer = static_cast<BYTE *>(g_ZunMemory.Alloc(
        (this->bgmFmtData[formatIndex]).preloadAllocSize,
        "./system\\global.h"));
    if (buffer == NULL)
    {
        CloseHandle(file);
        return -1;
    }

    ReadFile(
        file,
        buffer,
        (this->bgmFmtData[formatIndex]).preloadAllocSize,
        &bytesRead,
        NULL);
    CloseHandle(file);

    (this->bgmPreloadFmtData[index]) = &(this->bgmFmtData[formatIndex]);
    (this->bgmPreloadAllocations[index]) = buffer;
    (this->bgmPreloadData[index]) = buffer;
    (this->bgmPreloadAllocSizes[index]) = (this->bgmPreloadFmtData[index])->preloadAllocSize;
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

// QueueCommand writes the original contiguous 0x10C-byte command records;
// each live slot is an element of SoundPlayer::commandQueue.
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
        return this->ReopenBGM((this->bgmFileNames[index]));

    if ((this->bgmPreloadAllocations[index]) == NULL)
        return -1;

    blockAlign = (this->bgmPreloadFmtData[index])->format.nBlockAlign;
    samplesPerSecond = (this->bgmPreloadFmtData[index])->format.nSamplesPerSec;
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
        (this->bgmPreloadData[index]),
        (this->bgmPreloadAllocSizes[index]),
        (this->bgmPreloadFmtData[index]),
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

// The command dispatcher consumes the real 12-row sound queue, 12x128
// pan records, 128 DirectSound buffers, native per-sound volume metadata
// and the BGM filename/format allocations. Keep the live indexing and
// original dispatch/queue ordering without introducing bounds checks.
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

    // Each command has its native signed step word. Increment/decrement
    // that actual member while preserving the original retry transitions.
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
                ++commandCursor->step;
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
        ++commandCursor->step;
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
                --commandCursor->step;
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
        ++commandCursor->step;
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
        ++commandCursor->step;
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
                ++commandCursor->step;
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
        // Shift native 0x10C-byte command records through the real array.
        // Adjacent records do not overlap during the one-record memcpy.
        for (i = 0; i < 31; i++,
             ++commandCursor)
        {
            if (commandCursor->opcode == SOUNDPLAYER_COMMAND_NONE)
                break;
            memcpy(commandCursor,
                   commandCursor + 1,
                   sizeof(*commandCursor));
        }

        if (queuePhase)
            goto loop;
    }

    if (!g_Supervisor.playSounds)
        return this->commandQueue[0].opcode;

    for (queuePhase = 0; queuePhase < 12; queuePhase++)
    {
        if (this->soundQueue[queuePhase] < 0)
            break;

        soundIndex = this->soundQueue[queuePhase];
        this->soundQueue[queuePhase] = -1;

        averagedPan = 0;
        for (j = 0; j < this->soundQueueRequestCounts[queuePhase]; j++)
            averagedPan = static_cast<int>(static_cast<unsigned int>(averagedPan) + this->soundQueuePanData[queuePhase][j]);
        averagedPan /= this->soundQueueRequestCounts[queuePhase];
        this->soundQueueRequestCounts[queuePhase] = 0;

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


void SoundPlayer::FadeOut(float seconds)
{
    if (this->bgm != NULL)
        this->bgm->FadeOut(seconds);
}

// Native sound producers use the same 12-row queue, 12x128 pan array and
// per-sound metadata fields as ProcessQueues. The signed indices and original
// count checks are retained without introducing a new bounds guard.
void SoundPlayer::PlaySoundByIdx(int index, int pan)
{
    int unconsumedMetadata;
    int i;

    unconsumedMetadata = g_SoundBufferIdxVol[index].unconsumedMetadata;
    for (i = 0; i < 12; i++)
    {
        if ((this->soundQueue[i]) < 0)
            break;
        if ((this->soundQueue[i]) == index)
        {
            if (this->soundQueueRequestCounts[i] < 0x80)
                this->soundQueuePanData[i][this->soundQueueRequestCounts[i]++] = pan;
            return;
        }
    }

    if (i >= 12)
        return;

    (this->soundQueue[i]) = index;
    this->unconsumedMetadataBySound[index] = unconsumedMetadata;
    this->soundQueuePanData[i][0] = pan;
    ++this->soundQueueRequestCounts[i];
}

void SoundPlayer::PlaySoundPositionedByIdx(int index, float positionX)
{
    int unconsumedMetadata;
    int pan;
    int i;

    unconsumedMetadata = g_SoundBufferIdxVol[index].unconsumedMetadata;
    pan = (int)(positionX * 6.9444447f);
    for (i = 0; i < 12; i++)
    {
        if ((this->soundQueue[i]) < 0)
            break;
        if ((this->soundQueue[i]) == index)
        {
            if (this->soundQueueRequestCounts[i] < 0x80)
                this->soundQueuePanData[i][this->soundQueueRequestCounts[i]++] = pan;
            return;
        }
    }

    if (i >= 12)
        return;

    (this->soundQueue[i]) = index;
    this->unconsumedMetadataBySound[index] = unconsumedMetadata;
    this->soundQueuePanData[i][0] = pan;
    ++this->soundQueueRequestCounts[i];
}
