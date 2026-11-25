#include "AnmManager.hpp"

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
const char *g_PlayerResourcePaths[16][6] = {
    { "pl00.sht", "pl00.anm", "pl00.ecl", "pl00.msg", "pl00_match.msg", "pl00b.anm" },
    { "pl01.sht", "pl01.anm", "pl01.ecl", "pl01.msg", "pl01_match.msg", "pl01b.anm" },
    { "pl02.sht", "pl02.anm", "pl02.ecl", "pl02.msg", "pl02_match.msg", "pl02b.anm" },
    { "pl03.sht", "pl03.anm", "pl03.ecl", "pl03.msg", "pl03_match.msg", "pl03b.anm" },
    { "pl04.sht", "pl04.anm", "pl04.ecl", "pl04.msg", "pl04_match.msg", "pl04b.anm" },
    { "pl05.sht", "pl05.anm", "pl05.ecl", "pl05.msg", "pl05_match.msg", "pl05b.anm" },
    { "pl06.sht", "pl06.anm", "pl06.ecl", "pl06.msg", "pl06_match.msg", "pl06b.anm" },
    { "pl07.sht", "pl07.anm", "pl07.ecl", "pl07.msg", "pl07_match.msg", "pl07b.anm" },
    { "pl08.sht", "pl08.anm", "pl08.ecl", "pl08.msg", "pl08_match.msg", "pl08b.anm" },
    { "pl09.sht", "pl09.anm", "pl09.ecl", "pl09.msg", "pl09_match.msg", "pl09b.anm" },
    { "pl10.sht", "pl10.anm", "pl10.ecl", "pl10.msg", "pl10_match.msg", "pl10b.anm" },
    { "pl11.sht", "pl11.anm", "pl11.ecl", "pl11.msg", "pl11_match.msg", "pl11b.anm" },
    { "pl12.sht", "pl12.anm", "pl12.ecl", "pl12.msg", "pl12_match.msg", "pl12b.anm" },
    { "pl13.sht", "pl13.anm", "pl13.ecl", "pl13.msg", "pl13_match.msg", "pl13b.anm" },
    { "pl14.sht", "pl14.anm", "pl14.ecl", "pl14.msg", "pl14_match.msg", "pl14b.anm" },
    { "pl15.sht", "pl15.anm", "pl15.ecl", "pl15.msg", "pl15_match.msg", "pl15b.anm" },
};

int PreloadPlayerAnmResources()
{
    AnmManager *anmManager = g_AnmManager;
    PlayerAnmManagerRawView *rawView =
        reinterpret_cast<PlayerAnmManagerRawView *>(anmManager);
    int slot = 5;
    PlayerPreloadSideTailView *side = g_PlayerPreloadSides;

    // Two genuine 0x38-byte GameManager side records drive resource slots
    // five and six. Retail advances the live side pointer by its native
    // record stride and increments the signed ANM slot as an i386 DWORD.
    for (int i = 0; i < 2;
         ++i,
         side = reinterpret_cast<PlayerPreloadSideTailView *>(
             reinterpret_cast<unsigned int>(side) + sizeof(*side)),
         ++*reinterpret_cast<unsigned int *>(&slot))
    {
        if (rawView->GetAnmRawData(slot) != 0 &&
            side->shotType20 == side->previousShotType24)
            continue;

        int pathIndex = side->mode30 != 0 ? 5 : 1;
        // The native source selector scales by six 32-bit path pointers;
        // retail does not impose a C++ bound on the shot-type word.
        if (anmManager->PreloadAnm(
                slot, (*reinterpret_cast<const char *const *>(
        reinterpret_cast<unsigned int>(&g_PlayerResourcePaths[0][0]) +
        static_cast<unsigned int>(side->shotType20) * sizeof(g_PlayerResourcePaths[0]) +
        static_cast<unsigned int>(pathIndex) * sizeof(g_PlayerResourcePaths[0][0])))) == 0)
            return -1;

        anmManager = g_AnmManager;
        rawView = reinterpret_cast<PlayerAnmManagerRawView *>(anmManager);
    }

    return 0;
}
