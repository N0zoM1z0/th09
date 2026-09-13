#include <windows.h>
#include <dsound.h>
#include <string.h>

struct ThBgmFormatProcessView
{
    unsigned char unknown000[0x1C];
    unsigned int totalLength;
    unsigned char unknown020[0x34 - 0x20];
};

struct WaveFileProcessView
{
    ThBgmFormatProcessView *GetFormat();
    int Reopen(ThBgmFormatProcessView *format);
};

struct StreamingSoundProcessView
{
    virtual ~StreamingSoundProcessView();

    unsigned char unknown004[0x78 - 0x4];
    int isLocked;

    LPDIRECTSOUNDBUFFER GetBuffer(unsigned int index);
    WaveFileProcessView *GetWaveFile();
    int Reset();
    int FillBufferWithSound(LPDIRECTSOUNDBUFFER buffer, int looped);
    void InitSoundBuffers();
    void Play(unsigned int priority, unsigned int flags);
    void Stop();
    void Pause();
    void Unpause();
    void SetVolume(int volume);
};

struct SoundPlayerCommand
{
    int opcode;
    int argument;
    int step;
    char path[256];
};

struct SoundBufferIdxVolume
{
    int bufferIdx;
    short volume;
    short unconsumedMetadata;
};

struct SupervisorProcessQueuesView
{
    unsigned char unknown000[0x436];
    unsigned char musicMode;
    unsigned char playSounds;

    int IsMusicPreloaded();
};

extern SupervisorProcessQueuesView g_Supervisor;
extern SoundBufferIdxVolume g_SoundBufferIdxVol[];

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

class SoundPlayer
{
  public:
    LPDIRECTSOUND dsoundHdl;
    int unknown004;
    LPDIRECTSOUNDBUFFER soundBuffers[128];
    LPDIRECTSOUNDBUFFER duplicateSoundBuffers[128];
    int unconsumedMetadataBySound[128];
    LPDIRECTSOUNDBUFFER initSoundBuffer;
    HWND gameWindow;
    void *manager;
    DWORD bgmThreadId;
    HANDLE bgmThreadHandle;
    int unknown61C;
    int soundQueue[12];
    int soundQueueRequestCounts[12];
    unsigned int soundQueuePanData[12][128];
    ThBgmFormatProcessView *bgmPreloadFmtData[16];
    unsigned char *bgmPreloadAllocations[16];
    unsigned char *bgmPreloadData[16];
    DWORD bgmPreloadAllocSizes[16];
    unsigned int loadedBgmSlot;
    ThBgmFormatProcessView *bgmFmtData;
    SoundPlayerCommand commandQueue[32];
    char bgmFileNames[32][256];
    char currentBgmFileName[256];
    StreamingSoundProcessView *bgm;
    HANDLE bgmUpdateEvent;
    int unknown6210;
    unsigned int bgmFileBaseOffset;
    int bgmVolume;
    int sfxVolume;
    int unconsumedBgmAttenuation;

    int ProcessQueues();
    int PreloadBGM(int index, char *path);
    int LoadBGM(int index);
    void FreePreloadedBGM(int index);
    void StopBGM();
    void FadeOut(float seconds);
    int GetFmtIndexByName(char *name);
};

extern SoundPlayer g_SoundPlayer;

int SoundPlayer::ProcessQueues()
{
    SoundPlayerCommand *commandCursor;
    BOOL restartCommandProcessing;
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
    restartCommandProcessing = FALSE;

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
                commandCursor->step++;
                break;
            }
            this->StopBGM();
            this->PreloadBGM(commandCursor->argument, commandCursor->path);
        }
        else
        {
            this->PreloadBGM(commandCursor->argument, commandCursor->path);
        }
        restartCommandProcessing = TRUE;
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
        commandCursor->step++;
        break;

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
                commandCursor->step--;
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
        commandCursor->step++;
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
        commandCursor->step++;
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
                commandCursor->step++;
                break;
            }
            this->StopBGM();
        }
        this->FreePreloadedBGM(commandCursor->argument);
        restartCommandProcessing = TRUE;
        goto next_command;

    case SOUNDPLAYER_COMMAND_START_LOADED_BGM:
        if (this->bgm != NULL)
            this->bgm->Play(0, DSBPLAY_LOOPING);
        goto next_command;

    default:
        break;

    next_command:
        for (i = 0; i < 31; i++, commandCursor++)
        {
            if (commandCursor->opcode == SOUNDPLAYER_COMMAND_NONE)
                break;
            memcpy(commandCursor, commandCursor + 1, sizeof(*commandCursor));
        }

        if (restartCommandProcessing)
            goto loop;
    }

    if (!g_Supervisor.playSounds)
        return this->commandQueue[0].opcode;

    for (i = 0; i < 12; i++)
    {
        if (this->soundQueue[i] < 0)
            break;

        soundIndex = this->soundQueue[i];
        this->soundQueue[i] = -1;

        averagedPan = 0;
        for (j = 0; j < this->soundQueueRequestCounts[i]; j++)
            averagedPan += this->soundQueuePanData[i][j];
        averagedPan /= this->soundQueueRequestCounts[i];
        this->soundQueueRequestCounts[i] = 0;

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
