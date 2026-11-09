#include "AsciiManager.hpp"
#include "AnmManager.hpp"
#include "FrontFactoryConstructView.hpp"

#include <stddef.h>

extern int g_FrontRuntimeValues90[4];

void FrontFactoryConstructView::SelectOpponentSide(int sideIndex)
{
    transitionState11EA8 = 1;
    ++g_FrontRuntimeValues90[0];

    int sideCount = ++g_FrontRuntimeValues90[2 + sideIndex];
    if (sideCount >= g_FrontRuntimeValues90[1])
    {
        frontAnm08->ExecuteAnmIdx(&resultVms10968[0], 60);
        frontAnm08->ExecuteAnmIdx(&resultVms10968[1], 61);
        frontAnm08->ExecuteAnmIdx(&resultVms10968[2], 62);

        resultVms10968[1].pos2.x = -999.0f;
        resultVms10968[2].pos2.x = -999.0f;
        resultVms10968[1].pos.x = -999.0f;
        resultVms10968[2].pos.x = -999.0f;

        frontAnm08->SetSprite(&resultVms10968[1 + sideIndex], 44);
        frontAnm08->SetSprite(&resultVms10968[2 - sideIndex], 45);
    }

    transitionSide10964 = sideIndex;
    transitionVmCount1095C = 1;
    transitionFrame10960 = 0;
    transitionStarted11EA4 = 0;
}

