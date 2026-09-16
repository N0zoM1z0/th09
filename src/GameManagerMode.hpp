#pragma once

#include <stddef.h>

// Minimal target-bound view shared by mode predicates and Ending progression.
// Names are reconstruction aliases; this does not claim the original class layout.
struct GameManagerModeView
{
    unsigned char unknown000[0x20];
    int side0Character;
    unsigned char unknown024[0xF4];
    int gameMode;
    int difficulty;

    int IsGameMode0();
    int IsGameMode1();
};

typedef char GameManagerModeSide0CharacterAt20[
    (offsetof(GameManagerModeView, side0Character) == 0x20) ? 1 : -1];
typedef char GameManagerModeGameModeAt118[
    (offsetof(GameManagerModeView, gameMode) == 0x118) ? 1 : -1];
typedef char GameManagerModeDifficultyAt11C[
    (offsetof(GameManagerModeView, difficulty) == 0x11C) ? 1 : -1];

extern GameManagerModeView g_GameManager;
