#include "Supervisor.hpp"

#include <stddef.h>
#include <string.h>

namespace
{
struct SupervisorAudioMidiOutputView
{
    void PlayTrack(int trackId);
    void PlayPath(const char *path);
};

struct SupervisorAudioSoundPlayerView
{
    void QueueCommand(int opcode, int argument, char *path);
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
    unsigned char unknown437[0x5BC - 0x437];
    SupervisorAudioMidiOutputView *midiOutput;
};

struct SupervisorMusicTrackRecord
{
    int trackId;
    int unknown04;
    int soundPlayerValue;
    int unlockIndex;
};

typedef char SupervisorAudioMusicModeAt436[
    (offsetof(SupervisorAudioLayout, musicMode) == 0x436) ? 1 : -1];
typedef char SupervisorAudioMidiAt5BC[
    (offsetof(SupervisorAudioLayout, midiOutput) == 0x5BC) ? 1 : -1];
typedef char SupervisorMusicTrackRecordSizeIs10[
    (sizeof(SupervisorMusicTrackRecord) == 0x10) ? 1 : -1];
}

extern SupervisorAudioSoundPlayerView g_SoundPlayer;
extern SupervisorAudioGameManagerView g_GameManager;
extern SupervisorMusicTrackRecord g_SupervisorMusicTracks[];
extern unsigned char g_MusicUnlocked[];
extern int g_SoundPlayerBgmValue;

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

    int unlockIndex = g_SupervisorMusicTracks[index].unlockIndex;
    int resolvedTrackId = g_SupervisorMusicTracks[index].trackId;

    if (supervisor->musicMode == 2)
    {
        if (supervisor->midiOutput != NULL)
            supervisor->midiOutput->PlayTrack(resolvedTrackId);

        if (!g_GameManager.IsReplayMode() && !g_GameManager.HasFlagBit1())
            g_MusicUnlocked[unlockIndex] = 1;
    }
    else
    {
        if (supervisor->musicMode == 1)
        {
            if (g_Supervisor.IsMusicPreloaded())
                g_SoundPlayer.QueueCommand(4, 0, "dummy");

            g_SoundPlayer.QueueCommand(2, resolvedTrackId, "dummy");

            if (!g_GameManager.IsReplayMode() && !g_GameManager.HasFlagBit1())
                g_MusicUnlocked[unlockIndex] = 1;
        }

        g_SoundPlayerBgmValue = g_SupervisorMusicTracks[index].soundPlayerValue;
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
            g_MusicUnlocked[bgmUnlockIndex] = 1;
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
            g_MusicUnlocked[bgmUnlockIndex] = 1;
    }
    else
    {
        return -1;
    }

    return 0;
}
