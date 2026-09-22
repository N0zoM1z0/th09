#include "GameManagerMode.hpp"
#include "Supervisor.hpp"
#include "ZunMemory.hpp"

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
        int endIndex = startIndex + 10;
        int tableIndex = startIndex;
        if (tableIndex < endIndex)
        {
            do
            {
                if (tableIndex >= primary00->count00)
                    break;

                current04 = primary00->entries04[tableIndex].instructions00;
                if (current04 != 0 && current04->opcode02 != 0)
                {
                    do
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
                    while (current04->opcode02 != 0);
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
