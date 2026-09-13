#include "SoundManager.hpp"

// Provenance: this translation unit descends from the Microsoft DirectX SDK
// DSUtil.cpp sample family. Packet 117 classifies the unchanged manager core as
// third-party; the target-backed WaveFile and streaming-creation extensions below
// materially consume TH09/ZUN state and are classified independently as game code.

SoundManagerProcessView::SoundManagerProcessView()
{
    directSound = NULL;
}

SoundManagerProcessView::~SoundManagerProcessView()
{
    if (directSound != NULL)
    {
        directSound->Release();
        directSound = NULL;
    }
}

int SoundManagerProcessView::Initialize(
    HWND window,
    DWORD cooperativeLevel,
    DWORD primaryChannels,
    DWORD primaryFrequency,
    DWORD primaryBitRate)
{
    int result;

    if (directSound != NULL)
    {
        directSound->Release();
        directSound = NULL;
    }

    result = DirectSoundCreate8(NULL, &directSound, NULL);
    if (FAILED(result))
        return result;

    result = directSound->SetCooperativeLevel(window, cooperativeLevel);
    if (FAILED(result))
        return result;

    SetPrimaryBufferFormat(primaryChannels, primaryFrequency, primaryBitRate);
    return S_OK;
}

LPDIRECTSOUND SoundManagerProcessView::GetDirectSound()
{
    return directSound;
}

int SoundManagerProcessView::SetPrimaryBufferFormat(
    DWORD primaryChannels,
    DWORD primaryFrequency,
    DWORD primaryBitRate)
{
    int result;
    LPDIRECTSOUNDBUFFER primaryBuffer = NULL;

    if (directSound == NULL)
        return CO_E_NOTINITIALIZED;

    DSBUFFERDESC bufferDescription;
    ZeroMemory(&bufferDescription, sizeof(bufferDescription));
    bufferDescription.dwSize = sizeof(bufferDescription);
    bufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;
    bufferDescription.dwBufferBytes = 0;
    bufferDescription.lpwfxFormat = NULL;

    result = directSound->CreateSoundBuffer(&bufferDescription, &primaryBuffer, NULL);
    if (FAILED(result))
        return result;

    WAVEFORMATEX format;
    ZeroMemory(&format, sizeof(format));
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = (WORD)primaryChannels;
    format.nSamplesPerSec = primaryFrequency;
    format.wBitsPerSample = (WORD)primaryBitRate;
    format.nBlockAlign = format.wBitsPerSample / 8 * format.nChannels;
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

    result = primaryBuffer->SetFormat(&format);
    if (FAILED(result))
        return result;

    if (primaryBuffer != NULL)
    {
        primaryBuffer->Release();
        primaryBuffer = NULL;
    }
    return S_OK;
}

WaveFileProcessView::WaveFileProcessView()
{
    format = NULL;
    mmio = NULL;
    fileSize = 0;
    isReadingFromMemory = FALSE;
}

int WaveFileProcessView::Open(
    char *path,
    ThBgmFormatProcessView *newFormat,
    DWORD newFlags)
{
    flags = newFlags;
    isReadingFromMemory = FALSE;

    if (flags == 1)
    {
        if (path == NULL)
            return E_INVALIDARG;

        waveFile = CreateFileA(
            path,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
            NULL);
        if (waveFile == INVALID_HANDLE_VALUE)
            return E_FAIL;

        format = newFormat;
        ResetFile(false);
        fileSize = chunkSize;
    }
    return S_OK;
}

int WaveFileProcessView::OpenFromMemory(
    BYTE *newData,
    ULONG newDataSize,
    ThBgmFormatProcessView *newFormat,
    DWORD newFlags)
{
    format = newFormat;
    dataSize = newDataSize;
    data = newData;
    dataCursor = data;
    isReadingFromMemory = TRUE;
    if (newFlags != 1)
        return E_NOTIMPL;
    return S_OK;
}


struct SoundPlayerBgmStorageView
{
    unsigned char unknown000[0x6214];
    unsigned int bgmFileBaseOffset;
};

extern SoundPlayerBgmStorageView g_SoundPlayer;

int WaveFileProcessView::Close()
{
    if (flags == 1)
    {
        CloseHandle(waveFile);
        waveFile = INVALID_HANDLE_VALUE;
    }
    return S_OK;
}

WaveFileProcessView::~WaveFileProcessView()
{
    Close();
}

int WaveFileProcessView::ResetFile(bool loop)
{
    DWORD seekResult;

    if (isReadingFromMemory)
    {
        dataCursor = data;
        if (format->totalLength > 0)
            dataSize = format->totalLength;
        if (loop && format->introLength > 0)
            dataCursor += format->introLength;
    }
    else
    {
        if (waveFile == NULL)
            return CO_E_NOTINITIALIZED;

        if (loop && format->introLength > 0)
        {
            seekResult = SetFilePointer(
                waveFile,
                g_SoundPlayer.bgmFileBaseOffset + format->introLength + format->startOffset,
                NULL,
                FILE_BEGIN);
            chunkSize = format->totalLength - format->introLength;
        }
        else
        {
            seekResult = SetFilePointer(
                waveFile,
                g_SoundPlayer.bgmFileBaseOffset + format->startOffset,
                NULL,
                FILE_BEGIN);
            chunkSize = format->totalLength;
        }
    }
    return S_OK;
}

int WaveFileProcessView::Reopen(ThBgmFormatProcessView *newFormat)
{
    if (isReadingFromMemory)
        return E_FAIL;
    if (waveFile == INVALID_HANDLE_VALUE)
        return E_FAIL;

    format = newFormat;
    ResetFile(false);
    fileSize = chunkSize;
    return S_OK;
}

SoundProcessView::SoundProcessView(
    LPDIRECTSOUNDBUFFER *soundBuffers,
    DWORD newBufferSize,
    DWORD newBufferCount,
    WaveFileProcessView *newWaveFile)
{
    DWORD i;

    buffers = new LPDIRECTSOUNDBUFFER[newBufferCount];
    for (i = 0; i < newBufferCount; ++i)
        buffers[i] = soundBuffers[i];

    bufferSize = newBufferSize;
    bufferCount = newBufferCount;
    waveFile = newWaveFile;
    unknown30 = 0;

    FillBufferWithSound(buffers[0], FALSE);
    for (i = 0; i < newBufferCount; ++i)
        buffers[i]->SetCurrentPosition(0);
    isPlaying = FALSE;
}

SoundProcessView::~SoundProcessView()
{
    for (DWORD i = 0; i < bufferCount; ++i)
    {
        if (buffers[i] != NULL)
        {
            buffers[i]->Release();
            buffers[i] = NULL;
        }
    }
    if (buffers != NULL)
    {
        delete[] buffers;
        buffers = NULL;
    }
    if (waveFile != NULL)
    {
        delete waveFile;
        waveFile = NULL;
    }
}

StreamingSoundProcessView::StreamingSoundProcessView(
    LPDIRECTSOUNDBUFFER buffer,
    DWORD newBufferSize,
    WaveFileProcessView *newWaveFile,
    DWORD newNotifySize)
    : SoundProcessView(&buffer, newBufferSize, 1, newWaveFile)
{
    lastPlayPosition = 0;
    playProgress = 0;
    notifySize = newNotifySize;
    nextWriteOffset = 0;
    fillNextNotificationWithSilence = FALSE;
}

StreamingSoundProcessView::~StreamingSoundProcessView()
{
}

int SoundManagerProcessView::CreateStreaming(
    StreamingSoundProcessView **streamingSound,
    char *path,
    DWORD creationFlags,
    GUID algorithm,
    DWORD bufferCount,
    DWORD notifySize,
    HANDLE notifyEvent,
    ThBgmFormatProcessView *format)
{
    int result;

    if (directSound == NULL)
        return CO_E_NOTINITIALIZED;

    LPDIRECTSOUNDBUFFER soundBuffer = NULL;
    WaveFileProcessView *waveFile = NULL;
    DSBPOSITIONNOTIFY *notifications = NULL;
    LPDIRECTSOUNDNOTIFY notify = NULL;

    waveFile = new WaveFileProcessView();
    if (waveFile->Open(path, format, 1) != S_OK)
    {
        delete waveFile;
        return E_FAIL;
    }

    DWORD bufferSize = notifySize * bufferCount;
    DSBUFFERDESC bufferDescription;
    ZeroMemory(&bufferDescription, sizeof(bufferDescription));
    bufferDescription.dwSize = sizeof(bufferDescription);
    bufferDescription.dwFlags = creationFlags | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GLOBALFOCUS |
                                DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME | DSBCAPS_LOCSOFTWARE;
    bufferDescription.dwBufferBytes = bufferSize;
    bufferDescription.guid3DAlgorithm = algorithm;
    bufferDescription.lpwfxFormat = &waveFile->format->format;

    if (FAILED(directSound->CreateSoundBuffer(&bufferDescription, &soundBuffer, NULL)) ||
        FAILED(soundBuffer->QueryInterface(IID_IDirectSoundNotify, (VOID **)&notify)))
        return E_FAIL;

    notifications = new DSBPOSITIONNOTIFY[bufferCount];
    if (notifications == NULL)
        return E_OUTOFMEMORY;

    for (DWORD i = 0; i < bufferCount; ++i)
    {
        notifications[i].dwOffset = notifySize * i + notifySize - 1;
        notifications[i].hEventNotify = notifyEvent;
    }

    result = notify->SetNotificationPositions(bufferCount, notifications);
    if (FAILED(result))
    {
        if (notify != NULL)
        {
            notify->Release();
            notify = NULL;
        }
        delete notifications;
        return E_FAIL;
    }

    if (notify != NULL)
    {
        notify->Release();
        notify = NULL;
    }
    delete notifications;

    *streamingSound = new StreamingSoundProcessView(soundBuffer, bufferSize, waveFile, notifySize);
    CopyMemory(&(*streamingSound)->bufferDescription, &bufferDescription, sizeof(bufferDescription));
    (*streamingSound)->manager = this;
    (*streamingSound)->notifyEvent = notifyEvent;
    (*streamingSound)->isLocked = FALSE;
    return S_OK;
}

int SoundManagerProcessView::CreateStreamingFromMemory(
    StreamingSoundProcessView **streamingSound,
    BYTE *data,
    ULONG dataSize,
    ThBgmFormatProcessView *format,
    DWORD creationFlags,
    GUID algorithm,
    DWORD bufferCount,
    DWORD notifySize,
    HANDLE notifyEvent)
{
    int result;

    if (directSound == NULL)
        return CO_E_NOTINITIALIZED;

    LPDIRECTSOUNDBUFFER soundBuffer = NULL;
    WaveFileProcessView *waveFile = NULL;
    DSBPOSITIONNOTIFY *notifications = NULL;
    LPDIRECTSOUNDNOTIFY notify = NULL;

    waveFile = new WaveFileProcessView();
    waveFile->OpenFromMemory(data, dataSize, format, 0);

    DWORD bufferSize = notifySize * bufferCount;
    DSBUFFERDESC bufferDescription;
    ZeroMemory(&bufferDescription, sizeof(bufferDescription));
    bufferDescription.dwSize = sizeof(bufferDescription);
    bufferDescription.dwFlags = creationFlags | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GLOBALFOCUS |
                                DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLVOLUME | DSBCAPS_LOCSOFTWARE;
    bufferDescription.dwBufferBytes = bufferSize;
    bufferDescription.guid3DAlgorithm = algorithm;
    bufferDescription.lpwfxFormat = &waveFile->format->format;

    if (FAILED(directSound->CreateSoundBuffer(&bufferDescription, &soundBuffer, NULL)) ||
        FAILED(soundBuffer->QueryInterface(IID_IDirectSoundNotify, (VOID **)&notify)))
        return E_FAIL;

    notifications = new DSBPOSITIONNOTIFY[bufferCount];
    if (notifications == NULL)
        return E_OUTOFMEMORY;

    for (DWORD i = 0; i < bufferCount; ++i)
    {
        notifications[i].dwOffset = notifySize * i + notifySize - 1;
        notifications[i].hEventNotify = notifyEvent;
    }

    result = notify->SetNotificationPositions(bufferCount, notifications);
    if (FAILED(result))
    {
        if (notify != NULL)
        {
            notify->Release();
            notify = NULL;
        }
        delete notifications;
        return E_FAIL;
    }

    if (notify != NULL)
    {
        notify->Release();
        notify = NULL;
    }
    delete notifications;

    *streamingSound = new StreamingSoundProcessView(soundBuffer, bufferSize, waveFile, notifySize);
    CopyMemory(&(*streamingSound)->bufferDescription, &bufferDescription, sizeof(bufferDescription));
    (*streamingSound)->manager = this;
    (*streamingSound)->notifyEvent = notifyEvent;
    (*streamingSound)->isLocked = FALSE;
    return S_OK;
}
