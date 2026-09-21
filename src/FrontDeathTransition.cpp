#include "AsciiManager.hpp"
#include "AnmManager.hpp"

#include <stddef.h>

struct PlayerDeathSharedView
{
    unsigned char unknown00000[0x08];
    AnmLoaded *anm08;
    unsigned char unknown0000C[0x1095C - 0x0C];
    int updateBlock1095C;
    int drawCounter10960;
    int side10964;
    AnmVm transitionVms10968[3];
    unsigned char unknown11154[0x11EA4 - 0x11154];
    int runtime11EA4;
    int transitionBlock11EA8;

    void SelectOpponentSide(int sideIndex);
};

typedef char FrontSharedTransitionVmAt10968[
    (offsetof(PlayerDeathSharedView, transitionVms10968) == 0x10968) ? 1 : -1];
typedef char FrontSharedTransitionBlockAt11EA8[
    (offsetof(PlayerDeathSharedView, transitionBlock11EA8) == 0x11EA8) ? 1 : -1];

extern int g_FrontRuntimeValues90[4];

void PlayerDeathSharedView::SelectOpponentSide(int sideIndex)
{
    transitionBlock11EA8 = 1;
    ++g_FrontRuntimeValues90[0];

    int sideCount = ++g_FrontRuntimeValues90[2 + sideIndex];
    if (sideCount >= g_FrontRuntimeValues90[1])
    {
        anm08->ExecuteAnmIdx(&transitionVms10968[0], 60);
        anm08->ExecuteAnmIdx(&transitionVms10968[1], 61);
        anm08->ExecuteAnmIdx(&transitionVms10968[2], 62);

        transitionVms10968[1].pos2.x = -999.0f;
        transitionVms10968[2].pos2.x = -999.0f;
        transitionVms10968[1].pos.x = -999.0f;
        transitionVms10968[2].pos.x = -999.0f;

        anm08->SetSprite(&transitionVms10968[1 + sideIndex], 44);
        anm08->SetSprite(&transitionVms10968[2 - sideIndex], 45);
    }

    side10964 = sideIndex;
    updateBlock1095C = 1;
    drawCounter10960 = 0;
    runtime11EA4 = 0;
}

