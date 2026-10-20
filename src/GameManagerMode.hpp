#pragma once

#include <stddef.h>

// Minimal target-bound view shared by mode predicates and Ending progression.
// Names are reconstruction aliases; this does not claim the original class layout.
struct GameManagerModeView
{
    unsigned char unknown000[0x20];
    int side0Character;
    unsigned char unknown024[0x58 - 0x24];
    int side1Character;
    unsigned char unknown05C[0xFC - 0x5C];
    int field0FC;
    unsigned char unknown100[0x118 - 0x100];
    int gameMode;
    int difficulty;
    unsigned char unknown120[0x14];
    unsigned int flags134;

    int IsGameMode0();
    int IsGameMode1();
    int IsGameMode2();
    int IsReplayNeutral();
};

typedef char GameManagerModeSide0CharacterAt20[
    (offsetof(GameManagerModeView, side0Character) == 0x20) ? 1 : -1];
typedef char GameManagerModeSide1CharacterAt58[
    (offsetof(GameManagerModeView, side1Character) == 0x58) ? 1 : -1];
typedef char GameManagerModeField0FC[
    (offsetof(GameManagerModeView, field0FC) == 0xFC) ? 1 : -1];

typedef char GameManagerModeGameModeAt118[
    (offsetof(GameManagerModeView, gameMode) == 0x118) ? 1 : -1];
typedef char GameManagerModeDifficultyAt11C[
    (offsetof(GameManagerModeView, difficulty) == 0x11C) ? 1 : -1];
typedef char GameManagerModeFlagsAt134[
    (offsetof(GameManagerModeView, flags134) == 0x134) ? 1 : -1];

extern GameManagerModeView g_GameManager;
