#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "GameManagerMode.hpp"
#include "Supervisor.hpp"

#include <string.h>
#include "FrontMessageRuntimeView.hpp"

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

    if (g_FrontMessagePrimaryCache == 0)
    {
        // Preserve the nullable input for both the open and error path.
        if (primaryPath == NULL)
        {
            errorPath = NULL;
        }
        else
        {
            errorPath = primaryPath;
        }
        primary00 = reinterpret_cast<FrontMessageTableView *>(
            FileSystem::OpenFile(errorPath, 0, 0));
        g_FrontMessagePrimaryCache = primary00;
        if (primary00 == 0)
            goto load_error;

        for (int i = 0; i < primary00->count00; ++i)
        {
            unsigned int offset = primary00->entries04[i].offset00;
            if (offset != 0)
            {
                primary00->entries04[i].instructions00 =
                    reinterpret_cast<FrontMessageInstructionView *>(
                        reinterpret_cast<unsigned char *>(primary00) + offset);
            }
        }
    }
    else
    {
        primary00 = g_FrontMessagePrimaryCache;
    }

    if (!g_GameManager.IsGameMode2())
    {
        unsigned char loadedMusic[256];
        memset(loadedMusic, 0, sizeof(loadedMusic));

        // A bounded outer do/while preserves the original zero-trip guard
        // and the pre-body table-count exit from TH09's message worker.
        int endIndex = startIndex + 10;
        if (startIndex < endIndex)
        {
            do
            {
                if (startIndex >= primary00->count00)
                    break;
                current04 = primary00->entries04[startIndex].instructions00;
                if (current04 != 0)
                {
                    while (current04->opcode02 != 0)
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
                }

                ++startIndex;
            }
            while (startIndex < endIndex);
        }
        return 0;
    }

    if (g_FrontMessageSecondaryCache == 0)
    {
        // Preserve the nullable input for both the open and error path.
        if (secondaryPath == NULL)
        {
            errorPath = NULL;
        }
        else
        {
            errorPath = secondaryPath;
        }
        secondary0C = reinterpret_cast<FrontMessageTableView *>(
            FileSystem::OpenFile(errorPath, 0, 0));
        g_FrontMessageSecondaryCache = secondary0C;
        if (secondary0C == 0)
            goto load_error;

        for (int i = 0; i < secondary0C->count00; ++i)
        {
            secondary0C->entries04[i].offset00 +=
                reinterpret_cast<unsigned int>(secondary0C);
        }
    }
    else
    {
        secondary0C = g_FrontMessageSecondaryCache;
    }

    g_Supervisor.LoadMusic(g_FrontMessageSource->musicId0C);
    return 0;

load_error:
    g_GameErrorContext.Log(g_FrontMessageLoadError, errorPath);
    return -1;
}
