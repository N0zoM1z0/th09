#include "SoundManager.hpp"

// Provenance: Microsoft DirectX SDK DSUtil.cpp sample family. This maintained
// subset is reconstructed against the TH09 target and keeps only target-backed
// behavior; provenance does not imply authored-game ownership.

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
