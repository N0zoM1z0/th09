#include "GameManagerMode.hpp"

int GameManagerModeView::IsGameMode0()
{
    return gameMode == 0;
}

int GameManagerModeView::IsGameMode1()
{
    return gameMode == 1;
}

int GameManagerModeView::IsGameMode2()
{
    return gameMode == 2;
}

int GameManagerModeView::IsReplayNeutral()
{
    return (flags134 >> 3) & 1;
}
