#ifndef TH09_SOUND_MANAGER_HPP
#define TH09_SOUND_MANAGER_HPP

#include <windows.h>
#include <dsound.h>

struct StreamingSoundProcessView;
struct ThBgmFormatProcessView;

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
