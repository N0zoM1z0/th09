#include "Supervisor.hpp"
#include "Midi.hpp"
#include "GameManagerRuntimeView.hpp"
#include "SoundPlayer.hpp"

#include <stddef.h>
#include <string.h>

struct MidiPlaybackPathView : MidiOutput
{
    void PlayPath(const char *path);
};

namespace
{
// Keep a non-inline facade for PlayFile (MidiOutput::PlayFile is inline in Midi.hpp).
struct SupervisorAudioMidiPlayTrackView
{
    void PlayTrack(int trackId);
};

struct SupervisorAudioGameManagerView
{
    unsigned char unknown000[0x134];
    unsigned int flags;

    int HasFlagBit1();
};

struct SupervisorMusicTrackRecord
{
    int trackId;
    char *path;
    int soundPlayerValue;
    int unlockIndex;
};

typedef char SupervisorAudioMusicModeAt436[
    (offsetof(Supervisor, config388) + offsetof(GameConfiguration, musicMode) == 0x436) ? 1 : -1];
typedef char SupervisorAudioFramerateMultiplierAt5B8[
    (offsetof(Supervisor, framerateMultiplier5B8) == 0x5B8) ? 1 : -1];
typedef char SupervisorAudioMidiAt5BC[
    (offsetof(Supervisor, midiOutput5BC) == 0x5BC) ? 1 : -1];
typedef char SupervisorMusicTrackRecordSizeIs10[
    (sizeof(SupervisorMusicTrackRecord) == 0x10) ? 1 : -1];
}

extern SupervisorMusicTrackRecord g_SupervisorMusicTracks[];
extern unsigned char g_MusicUnlocked[];
extern int g_SoundPlayerBgmValue;

// Native music tracks are 0x10-byte table records; the unlock IDs index
// the real global byte table. Preserve separate live member reads and the
// original unsigned DWORD conversion for an externally supplied unlock ID.

int SupervisorAudioGameManagerView::HasFlagBit1()
{
    return (this->flags >> 1) & 1;
}

int Supervisor::PlayMusic(int trackId, int unused)
{
    unsigned int index;

    (void)unused;

    index = 0;
    SupervisorMusicTrackRecord *track = g_SupervisorMusicTracks;
    do
    {
        if (track->trackId == trackId)
            break;
        ++index;
        ++track;
    } while (index < 19);

    if (index >= 19)
        return 1;

    int unlockIndex = g_SupervisorMusicTracks[index].unlockIndex;
    int resolvedTrackId = g_SupervisorMusicTracks[index].trackId;

    if (g_Supervisor.config388.musicMode == 2)
    {
        if (g_Supervisor.midiOutput5BC != NULL)
            reinterpret_cast<SupervisorAudioMidiPlayTrackView *>(
                g_Supervisor.midiOutput5BC)->PlayTrack(resolvedTrackId);

        if (!g_GameManager.IsReplayNeutral() &&
            !reinterpret_cast<SupervisorAudioGameManagerView *>(&g_GameManager)->HasFlagBit1())
            g_MusicUnlocked[static_cast<unsigned int>(unlockIndex)] = 1;
    }
    else
    {
        if (g_Supervisor.config388.musicMode == 1)
        {
            if (g_Supervisor.IsMusicPreloaded())
                g_SoundPlayer.QueueCommand(4, 0, "dummy");

            g_SoundPlayer.QueueCommand(2, resolvedTrackId, "dummy");

            if (!g_GameManager.IsReplayNeutral() &&
                !reinterpret_cast<SupervisorAudioGameManagerView *>(&g_GameManager)->HasFlagBit1())
                g_MusicUnlocked[static_cast<unsigned int>(unlockIndex)] = 1;
        }

        g_SoundPlayerBgmValue = g_SupervisorMusicTracks[index].soundPlayerValue;
    }

    return 0;
}

int Supervisor::PlayAudio(char *path, int bgmUnlockIndex)
{
    char wavPathBuf[256];
    char *periodLoc;

    if (g_Supervisor.config388.musicMode == 2)
    {
        if (g_Supervisor.midiOutput5BC != NULL)
            reinterpret_cast<MidiPlaybackPathView *>(
                g_Supervisor.midiOutput5BC)->PlayPath(path);

        if (!g_GameManager.IsReplayNeutral() &&
            !reinterpret_cast<SupervisorAudioGameManagerView *>(&g_GameManager)->HasFlagBit1())
            g_MusicUnlocked[static_cast<unsigned int>(bgmUnlockIndex)] = 1;
    }
    else if (g_Supervisor.config388.musicMode == 1)
    {
        strcpy(wavPathBuf, path);
        periodLoc = strrchr(wavPathBuf, '.');
        periodLoc[1] = 'w';
        periodLoc[2] = 'a';
        periodLoc[3] = 'v';
        g_SoundPlayer.QueueCommand(2, -1, wavPathBuf);

        if (!g_GameManager.IsReplayNeutral() &&
            !reinterpret_cast<SupervisorAudioGameManagerView *>(&g_GameManager)->HasFlagBit1())
            g_MusicUnlocked[static_cast<unsigned int>(bgmUnlockIndex)] = 1;
    }
    else
    {
        return -1;
    }

    return 0;
}

int Supervisor::LoadMusic(int trackId)
{
    char wavPathBuf[256];
    char *periodLoc;
    unsigned int index = 0;
    SupervisorMusicTrackRecord *track = g_SupervisorMusicTracks;

    do
    {
        if (track->trackId == trackId)
            break;
        ++index;
        ++track;
    } while (index < 19);

    if (index >= 19)
        return 1;

    if (g_Supervisor.config388.musicMode == 2)
    {
        if (g_Supervisor.midiOutput5BC != NULL)
            g_Supervisor.midiOutput5BC->ReadFileData(
                g_SupervisorMusicTracks[index].trackId,
                g_SupervisorMusicTracks[index].path);
        return 0;
    }
    else if (g_Supervisor.config388.musicMode == 1)
    {
        strcpy(wavPathBuf, g_SupervisorMusicTracks[index].path);
        periodLoc = strrchr(wavPathBuf, '.');
        periodLoc[1] = 'w';
        periodLoc[2] = 'a';
        periodLoc[3] = 'v';
        g_SoundPlayer.QueueCommand(
            1, g_SupervisorMusicTracks[index].trackId, wavPathBuf);
    }

    return 1;
}

int Supervisor::StartLoadedMusic()
{
    if (g_Supervisor.config388.musicMode == 2)
    {
        if (g_Supervisor.midiOutput5BC != NULL)
            g_Supervisor.midiOutput5BC->Play();
        return 0;
    }
    else if (g_Supervisor.config388.musicMode == 1)
    {
        g_SoundPlayer.QueueCommand(10, 0, "dummy");
    }

    return 0;
}

int Supervisor::ReleaseMusic(int trackId)
{
    unsigned int index = 0;
    SupervisorMusicTrackRecord *track = g_SupervisorMusicTracks;

    do
    {
        if (track->trackId == trackId)
            break;
        ++index;
        ++track;
    } while (index < 19);

    if (index >= 19)
        return 1;

    int resolvedTrackId = g_SupervisorMusicTracks[index].trackId;
    if (g_Supervisor.config388.musicMode == 2)
    {
        if (g_Supervisor.midiOutput5BC != NULL)
        {
            g_Supervisor.midiOutput5BC->ReleaseFileData(resolvedTrackId);
            return 0;
        }
    }
    else if (g_Supervisor.config388.musicMode == 1)
    {
        g_SoundPlayer.QueueCommand(9, resolvedTrackId, "dummy");
    }
    else
    {
        return -1;
    }

    return 0;
}

int Supervisor::StopAudio()
{
    if (g_Supervisor.config388.musicMode == 2)
    {
        if (g_Supervisor.midiOutput5BC != NULL)
            g_Supervisor.midiOutput5BC->StopPlayback();
    }
    else if (g_Supervisor.config388.musicMode == 1)
    {
        if (g_Supervisor.IsMusicPreloaded())
            g_SoundPlayer.QueueCommand(4, 0, "dummy");
        else
            g_SoundPlayer.QueueCommand(3, 0, "dummy");
    }
    else
    {
        return -1;
    }

    return 0;
}

int Supervisor::FadeOutMusic(float durationSeconds)
{
    float fadeTime;

    if (g_Supervisor.config388.musicMode == 2)
    {
        if (g_Supervisor.midiOutput5BC != NULL)
            g_Supervisor.midiOutput5BC->SetFadeOut(
                (unsigned int)(1000.0f * durationSeconds));
    }
    else if (g_Supervisor.config388.musicMode == 1)
    {
        if (this->framerateMultiplier5B8 == 0.0f)
            fadeTime = durationSeconds;
        else if (this->framerateMultiplier5B8 > 1.0f)
            fadeTime = durationSeconds;
        else
            fadeTime = durationSeconds / this->framerateMultiplier5B8;

        g_SoundPlayer.QueueCommand(5, (int)fadeTime, "");
    }
    else
    {
        return -1;
    }

    return 0;
}
