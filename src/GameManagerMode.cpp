#include "GameManagerRuntimeView.hpp"

int AsciiGameManagerView::IsGameMode0()
{
    return gameMode == 0;
}

int AsciiGameManagerView::IsGameMode1()
{
    return gameMode == 1;
}

int AsciiGameManagerView::IsGameMode2()
{
    return gameMode == 2;
}

int AsciiGameManagerView::IsReplayNeutral()
{
    return (flags >> 3) & 1;
}
