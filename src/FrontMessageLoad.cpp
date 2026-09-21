#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "GameManagerMode.hpp"
#include "Supervisor.hpp"

#include <string.h>

struct FrontMessageInstructionView
{
    short time00;
    unsigned char opcode02;
    unsigned char size03;
    int argument04;
};

struct FrontMessageTableEntryView
{
    FrontMessageInstructionView *instructions00;
    int unknown04;
};

struct FrontMessageTableView
{
    int count00;
    FrontMessageTableEntryView entries04[1];
};

struct FrontMessageSourceView
{
    unsigned char unknown00[0x0C];
    short musicId0C;
};

struct FrontMessageRuntimeResetView
{
    unsigned char storage[0x1D70];
    int Reset();
};

struct FrontMessageRuntimeView
{
    FrontMessageTableView *primary00;
    FrontMessageInstructionView *current04;
    int currentIndex08;
    FrontMessageTableView *secondary0C;
    unsigned char unknown10[0x1D70 - 0x10];

    void ReleaseForSideCount(int startIndex);
    int LoadForSideCount(
        char *primaryPath,
        char *secondaryPath,
        int startIndex);
};

extern FrontMessageTableView *g_FrontMessagePrimaryCache;
extern FrontMessageTableView *g_FrontMessageSecondaryCache;
extern FrontMessageSourceView *g_FrontMessageSource;
extern GameErrorContext g_GameErrorContext;
extern const char g_FrontMessageLoadError[];

int FrontMessageRuntimeView::LoadForSideCount(
    char *primaryPath,
    char *secondaryPath,
    int startIndex)
{
    char *errorPath;

    ReleaseForSideCount(startIndex);
    reinterpret_cast<FrontMessageRuntimeResetView *>(this)->Reset();

    if (g_FrontMessagePrimaryCache != 0)
    {
        primary00 = g_FrontMessagePrimaryCache;
    }
    else
    {
        errorPath = primaryPath;
        primary00 = reinterpret_cast<FrontMessageTableView *>(
            FileSystem::OpenFile(primaryPath, 0, 0));
        g_FrontMessagePrimaryCache = primary00;
        if (primary00 == 0)
            goto load_error;

        for (int i = 0; i < primary00->count00; ++i)
        {
            unsigned int offset =
                reinterpret_cast<unsigned int>(
                    primary00->entries04[i].instructions00);
            if (offset != 0)
            {
                primary00->entries04[i].instructions00 =
                    reinterpret_cast<FrontMessageInstructionView *>(
                        reinterpret_cast<unsigned char *>(primary00) + offset);
            }
        }
    }

    if (!g_GameManager.IsGameMode2())
    {
        unsigned char loadedMusic[256];
        memset(loadedMusic, 0, sizeof(loadedMusic));

        int endIndex = startIndex + 10;
        if (startIndex < endIndex)
        {
            while (startIndex < primary00->count00)
            {
                current04 = primary00->entries04[startIndex].instructions00;
                if (current04 != 0 && current04->opcode02 != 0)
                {
                    do
                    {
                        if (current04->opcode02 == 7 &&
                            loadedMusic[current04->argument04] == 0)
                        {
                            g_Supervisor.LoadMusic(current04->argument04);
                            loadedMusic[current04->argument04] = 1;
                        }

                        FrontMessageInstructionView *next =
                            reinterpret_cast<FrontMessageInstructionView *>(
                                reinterpret_cast<unsigned char *>(current04) +
                                current04->size03 + 4);
                        current04 = next;
                    }
                    while (current04->opcode02 != 0);
                }

                ++startIndex;
                if (startIndex >= endIndex)
                    return 0;
            }
        }
        return 0;
    }

    if (g_FrontMessageSecondaryCache != 0)
    {
        secondary0C = g_FrontMessageSecondaryCache;
    }
    else
    {
        errorPath = secondaryPath;
        secondary0C = reinterpret_cast<FrontMessageTableView *>(
            FileSystem::OpenFile(secondaryPath, 0, 0));
        g_FrontMessageSecondaryCache = secondary0C;
        if (secondary0C == 0)
            goto load_error;

        for (int i = 0; i < secondary0C->count00; ++i)
        {
            secondary0C->entries04[i].instructions00 =
                reinterpret_cast<FrontMessageInstructionView *>(
                    reinterpret_cast<unsigned char *>(secondary0C) +
                    reinterpret_cast<unsigned int>(
                        secondary0C->entries04[i].instructions00));
        }
    }

    g_Supervisor.LoadMusic(g_FrontMessageSource->musicId0C);
    return 0;

load_error:
    g_GameErrorContext.Log(g_FrontMessageLoadError, errorPath);
    return -1;
}
