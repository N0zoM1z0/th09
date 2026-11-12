#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "GameManagerRuntimeView.hpp"
#include "Supervisor.hpp"

#include <string.h>
#include "FrontMessageRuntimeView.hpp"

extern FrontMessageTableView *g_FrontMessagePrimaryCache;
extern FrontMessageTableView *g_FrontMessageSecondaryCache;
extern FrontMessageSourceView *g_FrontMessageSource;
extern GameErrorContext g_GameErrorContext;
extern const char g_FrontMessageLoadError[];

// The file-backed table contains count00 native 0x08-byte entries, although
// its C++ wire projection declares only the first entry.
static inline FrontMessageTableEntryView &NativeFrontEntry(
    FrontMessageTableView *file, int index)
{
    return *reinterpret_cast<FrontMessageTableEntryView *>(
        reinterpret_cast<unsigned int>(&file->entries04[0]) +
        static_cast<unsigned int>(index) * sizeof(file->entries04[0]));
}

// The wire music identifier is an i386 DWORD offset into this stack
// byte map. Retail does not insert a C++ array-bounds check.
static inline unsigned char &NativeFrontMusicFlag(
    unsigned char *flags, int index)
{
    return *reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(&flags[0]) +
        static_cast<unsigned int>(index));
}

int FrontMessageRuntimeView::LoadForSideCount(
    char *primaryPath,
    char *secondaryPath,
    int startIndex)
{
    char *errorPath;

    ReleaseForSideCount(startIndex);
    Reset();

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

        // The primary table offsets and variable-length dialogue script
        // records are packed in the real file-backed allocation. Form their
        // native Win32 DWORD byte addresses without bounded C++ pointer
        // arithmetic beyond the fixed wire-header projection.
        for (int i = 0; i < primary00->count00; ++i)
        {
            unsigned int offset = NativeFrontEntry(primary00, i).offset00;
            if (offset != 0)
            {
                NativeFrontEntry(primary00, i).instructions00 =
                    reinterpret_cast<FrontMessageInstructionView *>(
                        reinterpret_cast<unsigned int>(primary00) +
                        static_cast<unsigned int>(offset));
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
        int endIndex = static_cast<int>(static_cast<unsigned int>(startIndex) + 10u);
        if (startIndex < endIndex)
        {
            do
            {
                if (startIndex >= primary00->count00)
                    break;
                current04 = NativeFrontEntry(primary00, startIndex).instructions00;
                if (current04 != 0)
                {
                    while (current04->opcode02 != 0)
                    {
                        if (current04->opcode02 == 7 &&
                            NativeFrontMusicFlag(loadedMusic, current04->argument04) == 0)
                        {
                            g_Supervisor.LoadMusic(current04->argument04);
                            NativeFrontMusicFlag(loadedMusic, current04->argument04) = 1;
                        }

                        FrontMessageInstructionView *next =
                            reinterpret_cast<FrontMessageInstructionView *>(
                                reinterpret_cast<unsigned int>(current04) +
                                static_cast<unsigned int>(current04->size03) + 4u);
                        current04 = next;
                    }
                }

                startIndex = static_cast<int>(static_cast<unsigned int>(startIndex) + 1u);
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
            NativeFrontEntry(secondary0C, i).offset00 +=
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
