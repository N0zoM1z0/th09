#include "AnmManager.hpp"

struct PlayerAnmManagerRawView
{
    void *GetAnmRawData(int anmIdx);
};

struct PlayerPreloadSideTailView
{
    int shotType20;
    int previousShotType24;
    unsigned char unknown28[0x08];
    int mode30;
    unsigned char unknown34[0x24];
};

typedef char PlayerPreloadSideTailSizeIs38[
    (sizeof(PlayerPreloadSideTailView) == 0x38) ? 1 : -1];

extern PlayerPreloadSideTailView g_PlayerPreloadSides[2];
extern const char *g_PlayerResourcePaths[][6];

int PreloadPlayerAnmResources()
{
    AnmManager *anmManager = g_AnmManager;
    PlayerAnmManagerRawView *rawView =
        reinterpret_cast<PlayerAnmManagerRawView *>(anmManager);
    int slot = 5;
    PlayerPreloadSideTailView *side = g_PlayerPreloadSides;

    for (int i = 0; i < 2; ++i, ++side, ++slot)
    {
        if (rawView->GetAnmRawData(slot) != 0 &&
            side->shotType20 == side->previousShotType24)
            continue;

        int pathIndex = side->mode30 != 0 ? 5 : 1;
        if (anmManager->PreloadAnm(
                slot, g_PlayerResourcePaths[side->shotType20][pathIndex]) == 0)
            return -1;

        anmManager = g_AnmManager;
        rawView = reinterpret_cast<PlayerAnmManagerRawView *>(anmManager);
    }

    return 0;
}
