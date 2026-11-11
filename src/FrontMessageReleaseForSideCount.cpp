#include "GameManagerRuntimeView.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

#include <string.h>
#include "FrontMessageRuntimeView.hpp"

extern FrontMessageTableView *g_FrontMessagePrimaryCache;
extern FrontMessageTableView *g_FrontMessageSecondaryCache;
extern FrontMessageSourceView *g_FrontMessageSource;

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

void FrontMessageRuntimeView::ReleaseForSideCount(int startIndex)
{
    if (primary00 == 0)
        return;

    unsigned char releasedMusic[256];
    memset(releasedMusic, 0, sizeof(releasedMusic));

    if (!g_GameManager.IsGameMode2())
    {
        int tableIndex = startIndex;
        int endIndex = static_cast<int>(static_cast<unsigned int>(startIndex) + 10u);
        if (tableIndex < endIndex)
        {
            do
            {
                if (tableIndex >= primary00->count00)
                    break;

                current04 = NativeFrontEntry(primary00, tableIndex).instructions00;
                if (current04 != 0)
                {
                    // The pre-test loop preserves the original empty-entry
                    // behavior as well as the physical loop-entry schedule.
                    while (current04->opcode02 != 0)
                    {
                        if (current04->opcode02 == 7 &&
                            NativeFrontMusicFlag(releasedMusic, current04->argument04) == 0)
                        {
                            g_Supervisor.ReleaseMusic(current04->argument04);
                            NativeFrontMusicFlag(releasedMusic, current04->argument04) = 1;
                        }

                        current04 =
                            reinterpret_cast<FrontMessageInstructionView *>(
                                reinterpret_cast<unsigned char *>(current04) +
                                current04->size03 + 4);
                    }
                }

                tableIndex = static_cast<int>(static_cast<unsigned int>(tableIndex) + 1u);
            }
            while (tableIndex < endIndex);
        }
    }
    else
    {
        g_Supervisor.ReleaseMusic(g_FrontMessageSource->musicId0C);
    }

    if (secondary0C != 0)
        g_ZunMemory.Free(secondary0C);
    g_ZunMemory.Free(primary00);

    g_FrontMessagePrimaryCache = 0;
    g_FrontMessageSecondaryCache = 0;
    Reset();
}
