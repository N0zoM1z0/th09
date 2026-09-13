#ifndef TH09_SOUND_MANAGER_HPP
#define TH09_SOUND_MANAGER_HPP

#include <stddef.h>
#include <windows.h>
#include <dsound.h>

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
typedef char ThBgmFormatProcessViewFormatAt20[
    (offsetof(ThBgmFormatProcessView, format) == 0x20) ? 1 : -1];
typedef char ThBgmFormatProcessViewSamplesAt24[
    (offsetof(ThBgmFormatProcessView, format.nSamplesPerSec) == 0x24) ? 1 : -1];
typedef char ThBgmFormatProcessViewBlockAlignAt2C[
    (offsetof(ThBgmFormatProcessView, format.nBlockAlign) == 0x2C) ? 1 : -1];

struct WaveFileProcessView
{
    void *mmio;
    unsigned char unknown004[0x08 - 0x04];
    DWORD chunkSize;
    unsigned char unknown00C[0x2C - 0x0C];
    DWORD fileSize;
    unsigned char unknown030[0x78 - 0x30];
    DWORD flags;
    BOOL isReadingFromMemory;
    BYTE *data;
    BYTE *dataCursor;
    ULONG dataSize;
    HANDLE waveFile;
    ThBgmFormatProcessView *format;

    WaveFileProcessView();
    ~WaveFileProcessView();
    int Open(char *path, ThBgmFormatProcessView *newFormat, DWORD newFlags);
    int OpenFromMemory(
        BYTE *newData,
        ULONG newDataSize,
        ThBgmFormatProcessView *newFormat,
        DWORD newFlags);
    int Close();
    int ResetFile(bool loop);
    ThBgmFormatProcessView *GetFormat();
    int Reopen(ThBgmFormatProcessView *newFormat);
};

typedef char WaveFileProcessViewSize[(sizeof(WaveFileProcessView) == 0x94) ? 1 : -1];
typedef char WaveFileProcessViewChunkSizeAt08[
    (offsetof(WaveFileProcessView, chunkSize) == 0x08) ? 1 : -1];
typedef char WaveFileProcessViewFlagsAt78[
    (offsetof(WaveFileProcessView, flags) == 0x78) ? 1 : -1];
typedef char WaveFileProcessViewMemoryModeAt7C[
    (offsetof(WaveFileProcessView, isReadingFromMemory) == 0x7C) ? 1 : -1];
typedef char WaveFileProcessViewDataAt80[
    (offsetof(WaveFileProcessView, data) == 0x80) ? 1 : -1];
typedef char WaveFileProcessViewWaveHandleAt8C[
    (offsetof(WaveFileProcessView, waveFile) == 0x8C) ? 1 : -1];
typedef char WaveFileProcessViewFormatAt90[
    (offsetof(WaveFileProcessView, format) == 0x90) ? 1 : -1];

class SoundManagerProcessView;

struct SoundProcessView
{
    virtual ~SoundProcessView();

    LPDIRECTSOUNDBUFFER *buffers;
    DWORD bufferSize;
    WaveFileProcessView *waveFile;
    DWORD bufferCount;
    int unknown14;
    int unknown18;
    int unknown1C;
    DWORD unknown20;
    DWORD unknown24;
    DWORD unknown28;
    DWORD unknown2C;
    DWORD unknown30;
    BOOL isPlaying;
    DSBUFFERDESC bufferDescription;
    SoundManagerProcessView *manager;

    SoundProcessView(
        LPDIRECTSOUNDBUFFER *soundBuffers,
        DWORD newBufferSize,
        DWORD newBufferCount,
        WaveFileProcessView *newWaveFile);
    int FillBufferWithSound(LPDIRECTSOUNDBUFFER buffer, int looped);
};

typedef char SoundProcessViewSize[(sizeof(SoundProcessView) == 0x60) ? 1 : -1];
typedef char SoundProcessBuffersAt04[(offsetof(SoundProcessView, buffers) == 0x04) ? 1 : -1];
typedef char SoundProcessBufferSizeAt08[(offsetof(SoundProcessView, bufferSize) == 0x08) ? 1 : -1];
typedef char SoundProcessWaveFileAt0C[(offsetof(SoundProcessView, waveFile) == 0x0C) ? 1 : -1];
typedef char SoundProcessBufferCountAt10[(offsetof(SoundProcessView, bufferCount) == 0x10) ? 1 : -1];
typedef char SoundProcessUnknown30At30[(offsetof(SoundProcessView, unknown30) == 0x30) ? 1 : -1];
typedef char SoundProcessIsPlayingAt34[(offsetof(SoundProcessView, isPlaying) == 0x34) ? 1 : -1];
typedef char SoundProcessDsbdAt38[(offsetof(SoundProcessView, bufferDescription) == 0x38) ? 1 : -1];
typedef char SoundProcessManagerAt5C[(offsetof(SoundProcessView, manager) == 0x5C) ? 1 : -1];

struct StreamingSoundProcessView : SoundProcessView
{
    DWORD lastPlayPosition;
    DWORD playProgress;
    DWORD nextWriteOffset;
    BOOL fillNextNotificationWithSilence;
    DWORD notifySize;
    HANDLE notifyEvent;
    int isLocked;

    StreamingSoundProcessView(
        LPDIRECTSOUNDBUFFER buffer,
        DWORD bufferSize,
        WaveFileProcessView *waveFile,
        DWORD notifySize);
    virtual ~StreamingSoundProcessView();

    LPDIRECTSOUNDBUFFER GetBuffer(unsigned int index);
    WaveFileProcessView *GetWaveFile();
    int Reset();
    void InitSoundBuffers();
    void Play(unsigned int priority, unsigned int flags);
    void Stop();
    void Pause();
    void Unpause();
    void SetVolume(int volume);
    int HandleWaveStreamNotification(int looped);
};

typedef char StreamingSoundProcessViewSize[(sizeof(StreamingSoundProcessView) == 0x7C) ? 1 : -1];
typedef char StreamingSoundIsPlayingAt34[
    (offsetof(StreamingSoundProcessView, isPlaying) == 0x34) ? 1 : -1];
typedef char StreamingSoundDsbdAt38[
    (offsetof(StreamingSoundProcessView, bufferDescription) == 0x38) ? 1 : -1];
typedef char StreamingSoundManagerAt5C[
    (offsetof(StreamingSoundProcessView, manager) == 0x5C) ? 1 : -1];
typedef char StreamingSoundNotifySizeAt70[
    (offsetof(StreamingSoundProcessView, notifySize) == 0x70) ? 1 : -1];
typedef char StreamingSoundEventAt74[
    (offsetof(StreamingSoundProcessView, notifyEvent) == 0x74) ? 1 : -1];
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
    int SetPrimaryBufferFormat(
        DWORD primaryChannels,
        DWORD primaryFrequency,
        DWORD primaryBitRate);

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

#endif
