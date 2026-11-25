#pragma once

// Observed TH09 difficulty / setup-mode selector values (0..4), used across
// GameManagerSetup, PlayerAddedState, and PlayerRuntime reward scaling.
enum GameDifficulty
{
    DIFFICULTY_EASY = 0,
    DIFFICULTY_NORMAL = 1,
    DIFFICULTY_HARD = 2,
    DIFFICULTY_LUNATIC = 3,
    DIFFICULTY_EXTRA = 4,
};
