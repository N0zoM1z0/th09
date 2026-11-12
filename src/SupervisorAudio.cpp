#include "Supervisor.hpp"
#include "SoundPlayer.hpp"

#include <stddef.h>
#include <string.h>

namespace
{
struct SupervisorAudioMidiOutputView
{
    void PlayTrack(int trackId);
    void PlayPath(const char *path);
    int ReadFileData(int trackId, const char *path);
    void ReleaseFileData(int trackId);
    int Play();
    void StopPlayback();
    void SetFadeOut(unsigned int milliseconds);
};

struct SupervisorAudioGameManagerView
{
    unsigned char unknown000[0x134];
    unsigned int flags;

    int IsReplayMode();
    int HasFlagBit1();
};

struct SupervisorAudioLayout
{
    unsigned char unknown000[0x436];
    unsigned char musicMode;
    unsigned char unknown437[0x5B8 - 0x437];
    float framerateMultiplier;
    SupervisorAudioMidiOutputView *midiOutput;
};

struct SupervisorMusicTrackRecord
{
    int trackId;
    char *path;
    int soundPlayerValue;
    int unlockIndex;
};

typedef char SupervisorAudioMusicModeAt436[
    (offsetof(SupervisorAudioLayout, musicMode) == 0x436) ? 1 : -1];
typedef char SupervisorAudioFramerateMultiplierAt5B8[
    (offsetof(SupervisorAudioLayout, framerateMultiplier) == 0x5B8) ? 1 : -1];
typedef char SupervisorAudioMidiAt5BC[
    (offsetof(SupervisorAudioLayout, midiOutput) == 0x5BC) ? 1 : -1];
typedef char SupervisorMusicTrackRecordSizeIs10[
    (sizeof(SupervisorMusicTrackRecord) == 0x10) ? 1 : -1];
}

extern SupervisorAudioGameManagerView g_GameManager;
extern SupervisorMusicTrackRecord g_SupervisorMusicTracks[];
extern unsigned char g_MusicUnlocked[];
extern int g_SoundPlayerBgmValue;

// Unlock IDs arrive as signed words from the music table or caller.
// The original retail music descriptor is a 0x10-byte native table
// entry. Keep each independent resolved index lookup as a live read.
#define TH09_SUPERVISOR_MUSIC_TRACK(idx) \
    (*reinterpret_cast<const SupervisorMusicTrackRecord *>( \
        reinterpret_cast<unsigned int>(&g_SupervisorMusicTracks[0]) + \
        static_cast<unsigned int>(idx) * sizeof(g_SupervisorMusicTracks[0])))

#define TH09_SUPERVISOR_MUSIC_UNLOCK(id) \
    (*reinterpret_cast<unsigned char *>( \
        reinterpret_cast<unsigned int>(&g_MusicUnlocked[0]) + \
        static_cast<unsigned int>(id)))

int SupervisorAudioGameManagerView::HasFlagBit1()
{
    return (this->flags >> 1) & 1;
}

int Supervisor::PlayMusic(int trackId, int unused)
{
    unsigned int index;
    SupervisorAudioLayout *supervisor =
        reinterpret_cast<SupervisorAudioLayout *>(&g_Supervisor);

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

    int unlockIndex = TH09_SUPERVISOR_MUSIC_TRACK(index).unlockIndex;
    int resolvedTrackId = TH09_SUPERVISOR_MUSIC_TRACK(index).trackId;

    if (supervisor->musicMode == 2)
    {
        if (supervisor->midiOutput != NULL)
            supervisor->midiOutput->PlayTrack(resolvedTrackId);

        if (!g_GameManager.IsReplayMode() && !g_GameManager.HasFlagBit1())
            TH09_SUPERVISOR_MUSIC_UNLOCK(unlockIndex) = 1;
    }
    else
    {
        if (supervisor->musicMode == 1)
        {
            if (g_Supervisor.IsMusicPreloaded())
                g_SoundPlayer.QueueCommand(4, 0, "dummy");

            g_SoundPlayer.QueueCommand(2, resolvedTrackId, "dummy");

            if (!g_GameManager.IsReplayMode() && !g_GameManager.HasFlagBit1())
                TH09_SUPERVISOR_MUSIC_UNLOCK(unlockIndex) = 1;
        }

        g_SoundPlayerBgmValue = TH09_SUPERVISOR_MUSIC_TRACK(index).soundPlayerValue;
    }

    return 0;
}

int Supervisor::PlayAudio(char *path, int bgmUnlockIndex)
{
    char wavPathBuf[256];
    char *periodLoc;
    SupervisorAudioLayout *supervisor =
        reinterpret_cast<SupervisorAudioLayout *>(&g_Supervisor);

    if (supervisor->musicMode == 2)
    {
        if (supervisor->midiOutput != NULL)
            supervisor->midiOutput->PlayPath(path);

        if (!g_GameManager.IsReplayMode() && !g_GameManager.HasFlagBit1())
            TH09_SUPERVISOR_MUSIC_UNLOCK(bgmUnlockIndex) = 1;
    }
    else if (supervisor->musicMode == 1)
    {
        strcpy(wavPathBuf, path);
        periodLoc = strrchr(wavPathBuf, '.');
        periodLoc[1] = 'w';
        periodLoc[2] = 'a';
        periodLoc[3] = 'v';
        g_SoundPlayer.QueueCommand(2, -1, wavPathBuf);

        if (!g_GameManager.IsReplayMode() && !g_GameManager.HasFlagBit1())
            TH09_SUPERVISOR_MUSIC_UNLOCK(bgmUnlockIndex) = 1;
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
    SupervisorAudioLayout *supervisor =
        reinterpret_cast<SupervisorAudioLayout *>(&g_Supervisor);

    do
    {
        if (track->trackId == trackId)
            break;
        ++index;
        ++track;
    } while (index < 19);

    if (index >= 19)
        return 1;

    if (supervisor->musicMode == 2)
    {
        if (supervisor->midiOutput != NULL)
            supervisor->midiOutput->ReadFileData(
                TH09_SUPERVISOR_MUSIC_TRACK(index).trackId,
                TH09_SUPERVISOR_MUSIC_TRACK(index).path);
        return 0;
    }
    else if (supervisor->musicMode == 1)
    {
        strcpy(wavPathBuf, TH09_SUPERVISOR_MUSIC_TRACK(index).path);
        periodLoc = strrchr(wavPathBuf, '.');
        periodLoc[1] = 'w';
        periodLoc[2] = 'a';
        periodLoc[3] = 'v';
        g_SoundPlayer.QueueCommand(
            1, TH09_SUPERVISOR_MUSIC_TRACK(index).trackId, wavPathBuf);
    }

    return 1;
}

int Supervisor::StartLoadedMusic()
{
    SupervisorAudioLayout *supervisor =
        reinterpret_cast<SupervisorAudioLayout *>(&g_Supervisor);

    if (supervisor->musicMode == 2)
    {
        if (supervisor->midiOutput != NULL)
            supervisor->midiOutput->Play();
        return 0;
    }
    else if (supervisor->musicMode == 1)
    {
        g_SoundPlayer.QueueCommand(10, 0, "dummy");
    }

    return 0;
}

int Supervisor::ReleaseMusic(int trackId)
{
    unsigned int index = 0;
    SupervisorMusicTrackRecord *track = g_SupervisorMusicTracks;
    SupervisorAudioLayout *supervisor =
        reinterpret_cast<SupervisorAudioLayout *>(&g_Supervisor);

    do
    {
        if (track->trackId == trackId)
            break;
        ++index;
        ++track;
    } while (index < 19);

    if (index >= 19)
        return 1;

    int resolvedTrackId = TH09_SUPERVISOR_MUSIC_TRACK(index).trackId;
    if (supervisor->musicMode == 2)
    {
        if (supervisor->midiOutput != NULL)
        {
            supervisor->midiOutput->ReleaseFileData(resolvedTrackId);
            return 0;
        }
    }
    else if (supervisor->musicMode == 1)
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
    SupervisorAudioLayout *supervisor =
        reinterpret_cast<SupervisorAudioLayout *>(&g_Supervisor);

    if (supervisor->musicMode == 2)
    {
        if (supervisor->midiOutput != NULL)
            supervisor->midiOutput->StopPlayback();
    }
    else if (supervisor->musicMode == 1)
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
    SupervisorAudioLayout *supervisor =
        reinterpret_cast<SupervisorAudioLayout *>(&g_Supervisor);
    SupervisorAudioLayout *self =
        reinterpret_cast<SupervisorAudioLayout *>(this);

    if (supervisor->musicMode == 2)
    {
        if (supervisor->midiOutput != NULL)
            supervisor->midiOutput->SetFadeOut(
                (unsigned int)(1000.0f * durationSeconds));
    }
    else if (supervisor->musicMode == 1)
    {
        if (self->framerateMultiplier == 0.0f)
            fadeTime = durationSeconds;
        else if (self->framerateMultiplier > 1.0f)
            fadeTime = durationSeconds;
        else
            fadeTime = durationSeconds / self->framerateMultiplier;

        g_SoundPlayer.QueueCommand(5, (int)fadeTime, "");
    }
    else
    {
        return -1;
    }

    return 0;
}

#undef TH09_SUPERVISOR_MUSIC_UNLOCK

#undef TH09_SUPERVISOR_MUSIC_TRACK
