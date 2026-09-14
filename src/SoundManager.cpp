#include "SoundManager.hpp"

#include <stdlib.h>

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
    int bgmVolume;
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

DWORD WaveFileProcessView::GetSize()
{
    return fileSize;
}

int WaveFileProcessView::Read(BYTE *buffer, DWORD sizeToRead, DWORD *sizeRead)
{
    if (isReadingFromMemory)
    {
        if (dataCursor == NULL)
            return CO_E_NOTINITIALIZED;
        if (sizeRead != NULL)
            *sizeRead = 0;

        if (dataCursor + sizeToRead > data + dataSize)
            sizeToRead = dataSize - (DWORD)(dataCursor - data);

        CopyMemory(buffer, dataCursor, sizeToRead);
        dataCursor += sizeToRead;
        if (sizeRead != NULL)
            *sizeRead = sizeToRead;
        return S_OK;
    }
    else
    {
        if (waveFile == NULL)
            return CO_E_NOTINITIALIZED;
        if (buffer == NULL || sizeRead == NULL)
            return E_INVALIDARG;

        UINT bytesIn = sizeToRead;
        if (bytesIn > chunkSize)
            bytesIn = chunkSize;
        chunkSize -= bytesIn;

        DWORD actualSize;
        ReadFile(waveFile, buffer, bytesIn, &actualSize, NULL);
        if (sizeRead != NULL)
            *sizeRead = actualSize;
        return S_OK;
    }
}

int SoundProcessView::RestoreBuffer(LPDIRECTSOUNDBUFFER buffer, BOOL *restored)
{
    int result;

    if (buffer == NULL)
        return CO_E_NOTINITIALIZED;
    if (restored != NULL)
        *restored = FALSE;

    DWORD status;
    if (FAILED(result = buffer->GetStatus(&status)))
        return result;
    if (status & DSBSTATUS_BUFFERLOST)
    {
        do
        {
            result = buffer->Restore();
            if (result == DSERR_BUFFERLOST)
                Sleep(10);
        } while (result = buffer->Restore());

        if (restored != NULL)
            *restored = TRUE;
        return S_OK;
    }
    return S_FALSE;
}

int SoundProcessView::FillBufferWithSound(
    LPDIRECTSOUNDBUFFER buffer, int repeatIfBufferLarger)
{
    int result;
    VOID *lockedBuffer = NULL;
    DWORD lockedBufferSize = 0;
    DWORD waveDataRead = 0;

    if (buffer == NULL)
        return CO_E_NOTINITIALIZED;
    if (FAILED(result = RestoreBuffer(buffer, NULL)))
        return result;
    if (FAILED(result = buffer->Lock(
            0, bufferSize, &lockedBuffer, &lockedBufferSize, NULL, NULL, 0)))
        return result;

    waveFile->ResetFile(false);
    if (FAILED(result = waveFile->Read(
            (BYTE *)lockedBuffer, lockedBufferSize, &waveDataRead)))
        return result;

    if (waveDataRead == 0)
    {
        FillMemory(
            (BYTE *)lockedBuffer,
            lockedBufferSize,
            (BYTE)(waveFile->format->format.wBitsPerSample == 8 ? 128 : 0));
    }
    else if (waveDataRead < lockedBufferSize)
    {
        if (repeatIfBufferLarger)
        {
            DWORD readSoFar = waveDataRead;
            while (readSoFar < lockedBufferSize)
            {
                if (FAILED(result = waveFile->ResetFile(false)))
                    return result;
                result = waveFile->Read(
                    (BYTE *)lockedBuffer + readSoFar,
                    lockedBufferSize - readSoFar,
                    &waveDataRead);
                if (FAILED(result))
                    return result;
                readSoFar += waveDataRead;
            }
        }
        else
        {
            FillMemory(
                (BYTE *)lockedBuffer + waveDataRead,
                lockedBufferSize - waveDataRead,
                (BYTE)(waveFile->format->format.wBitsPerSample == 8 ? 128 : 0));
        }
    }

    buffer->Unlock(lockedBuffer, lockedBufferSize, NULL, 0);
    return S_OK;
}

LPDIRECTSOUNDBUFFER SoundProcessView::GetFreeBuffer()
{
    if (buffers == NULL)
        return NULL;

    DWORD i;
    for (i = 0; i < bufferCount; ++i)
    {
        if (buffers[i] != NULL)
        {
            DWORD status = 0;
            buffers[i]->GetStatus(&status);
            if ((status & DSBSTATUS_PLAYING) == 0)
                break;
        }
    }

    if (i != bufferCount)
        return buffers[i];
    return buffers[rand() % bufferCount];
}

int SoundProcessView::Play(DWORD newPriority, DWORD newFlags)
{
    int result;
    BOOL restored;

    if (buffers == NULL)
        return CO_E_NOTINITIALIZED;

    LPDIRECTSOUNDBUFFER buffer = GetFreeBuffer();
    if (buffer == NULL)
        return E_FAIL;
    if (FAILED(result = RestoreBuffer(buffer, &restored)))
        return result;
    if (restored)
    {
        if (FAILED(result = FillBufferWithSound(buffer, FALSE)))
            return result;
        Reset();
    }

    fadeType = 0;
    currentFadeProgress = 0;
    totalFade = 0;
    SetVolume(0);
    isPlaying = TRUE;
    priority = newPriority;
    flags = newFlags;
    unknown2C = 0;
    unknown30 = 1;
    return buffer->Play(0, newPriority, newFlags);
}

int SoundProcessView::SetVolume(int volume)
{
    float volumeScale = g_SoundPlayer.bgmVolume / 100.0f;

    if (g_SoundPlayer.bgmVolume != 0)
    {
        volumeScale = 1.0f - volumeScale;
        volumeScale = volumeScale * volumeScale;
        volumeScale = 1.0f - volumeScale;
        return buffers[0]->SetVolume((int)((volume + 5000) * volumeScale) - 5000);
    }
    return buffers[0]->SetVolume(DSBVOLUME_MIN);
}

int SoundProcessView::Reset()
{
    if (buffers == NULL)
        return CO_E_NOTINITIALIZED;

    int result = 0;
    for (DWORD i = 0; i < bufferCount; ++i)
        result |= buffers[i]->SetCurrentPosition(0);
    return result;
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

int StreamingSoundProcessView::HandleWaveStreamNotification(int looped)
{
    int result;
    DWORD currentPlayPosition;
    DWORD playDelta;
    VOID *lockedBuffer;
    VOID *lockedBuffer2;
    DWORD lockedBufferSize;
    DWORD lockedBufferSize2;
    DWORD playCursor;
    DWORD writeCursor;

    if (buffers == NULL || waveFile == NULL)
        return CO_E_NOTINITIALIZED;

    buffers[0]->GetCurrentPosition(&playCursor, &writeCursor);
    if ((nextWriteOffset >= writeCursor - notifySize && nextWriteOffset < writeCursor) ||
        (writeCursor - notifySize < 0 && nextWriteOffset >= bufferSize - notifySize))
        return CO_E_FIRST;

    {
        BOOL restored;
        if (FAILED(result = RestoreBuffer(buffers[0], &restored)))
            return result;
        if (restored)
        {
            if (FAILED(result = FillBufferWithSound(buffers[0], FALSE)))
                return result;
            return S_OK;
        }
    }

    {
        DWORD bytesWritten;
        lockedBuffer = NULL;
        lockedBuffer2 = NULL;
        if (FAILED(result = buffers[0]->Lock(
                nextWriteOffset,
                notifySize,
                &lockedBuffer,
                &lockedBufferSize,
                &lockedBuffer2,
                &lockedBufferSize2,
                0)))
            return result;
        if (lockedBuffer2 != NULL)
            return E_UNEXPECTED;

        if (!fillNextNotificationWithSilence)
        {
            if (FAILED(result = waveFile->Read(
                    (BYTE *)lockedBuffer, lockedBufferSize, &bytesWritten)))
                return result;
        }
        else
        {
            FillMemory(
                lockedBuffer,
                lockedBufferSize,
                (BYTE)(waveFile->format->format.wBitsPerSample == 8 ? 128 : 0));
            bytesWritten = lockedBufferSize;
        }

        if (bytesWritten < lockedBufferSize)
        {
            if (!looped)
            {
                FillMemory(
                    (BYTE *)lockedBuffer + bytesWritten,
                    lockedBufferSize - bytesWritten,
                    (BYTE)(waveFile->format->format.wBitsPerSample == 8 ? 128 : 0));
                fillNextNotificationWithSilence = TRUE;
            }
            else
            {
                DWORD readSoFar = bytesWritten;
                while (readSoFar < lockedBufferSize)
                {
                    if (FAILED(result = waveFile->ResetFile(true)))
                        return result;
                    if (FAILED(result = waveFile->Read(
                            (BYTE *)lockedBuffer + readSoFar,
                            lockedBufferSize - readSoFar,
                            &bytesWritten)))
                        return result;
                    readSoFar += bytesWritten;
                }
            }
        }

        buffers[0]->Unlock(lockedBuffer, lockedBufferSize, NULL, 0);
        if (FAILED(result = buffers[0]->GetCurrentPosition(&currentPlayPosition, NULL)))
            return result;

        if (currentPlayPosition < lastPlayPosition)
            playDelta = bufferSize - lastPlayPosition + currentPlayPosition;
        else
            playDelta = currentPlayPosition - lastPlayPosition;
        playProgress += playDelta;
        lastPlayPosition = currentPlayPosition;

        if (fillNextNotificationWithSilence && playProgress >= waveFile->GetSize())
            buffers[0]->Stop();

        nextWriteOffset += lockedBufferSize;
        nextWriteOffset %= bufferSize;
        return S_OK;
    }
}

int StreamingSoundProcessView::Reset()
{
    int result;

    if (buffers[0] == NULL || waveFile == NULL)
        return CO_E_NOTINITIALIZED;

    lastPlayPosition = 0;
    playProgress = 0;
    nextWriteOffset = 0;
    fillNextNotificationWithSilence = FALSE;

    BOOL restored;
    if (FAILED(result = RestoreBuffer(buffers[0], &restored)))
        return result;
    if (restored)
    {
        if (FAILED(result = FillBufferWithSound(buffers[0], FALSE)))
            return result;
    }

    waveFile->ResetFile(false);
    return buffers[0]->SetCurrentPosition(0);
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
