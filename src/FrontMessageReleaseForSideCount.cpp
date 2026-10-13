#include "GameManagerMode.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

#include <string.h>
#include "FrontMessageTableView.hpp"

struct FrontMessageRuntimeView
{
    FrontMessageTableView *primary00;
    FrontMessageInstructionView *current04;
    int currentIndex08;
    FrontMessageTableView *secondary0C;
    unsigned char unknown10[0x1D70 - 0x10];

    void ReleaseForSideCount(int startIndex);
};

extern FrontMessageTableView *g_FrontMessagePrimaryCache;
extern FrontMessageTableView *g_FrontMessageSecondaryCache;
extern FrontMessageSourceView *g_FrontMessageSource;

void FrontMessageRuntimeView::ReleaseForSideCount(int startIndex)
{
    if (primary00 == 0)
        return;

    unsigned char releasedMusic[256];
    memset(releasedMusic, 0, sizeof(releasedMusic));

    if (!g_GameManager.IsGameMode2())
    {
        int tableIndex = startIndex;
        int endIndex = startIndex + 10;
        if (tableIndex < endIndex)
        {
            do
            {
                if (tableIndex >= primary00->count00)
                    break;

                current04 = primary00->entries04[tableIndex].instructions00;
                if (current04 != 0)
                {
                    // The pre-test loop preserves the original empty-entry
                    // behavior as well as the physical loop-entry schedule.
                    while (current04->opcode02 != 0)
                    {
                        if (current04->opcode02 == 7 &&
                            releasedMusic[current04->argument04] == 0)
                        {
                            g_Supervisor.ReleaseMusic(current04->argument04);
                            releasedMusic[current04->argument04] = 1;
                        }

                        current04 =
                            reinterpret_cast<FrontMessageInstructionView *>(
                                reinterpret_cast<unsigned char *>(current04) +
                                current04->size03 + 4);
                    }
                }

                ++tableIndex;
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
    reinterpret_cast<FrontMessageRuntimeResetView *>(this)->Reset();
}
