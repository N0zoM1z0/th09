#pragma once

// Observed TH09 8-way player movement direction codes (1..8), shared by
// PlayerMovement and PlayerLifecycleView::ResolvePatternOffset.
enum PlayerMoveDirection
{
    PLAYER_DIR_NONE = 0,
    PLAYER_DIR_UP = 1,
    PLAYER_DIR_DOWN = 2,
    PLAYER_DIR_LEFT = 3,
    PLAYER_DIR_RIGHT = 4,
    PLAYER_DIR_UP_LEFT = 5,
    PLAYER_DIR_UP_RIGHT = 6,
    PLAYER_DIR_DOWN_LEFT = 7,
    PLAYER_DIR_DOWN_RIGHT = 8,
};
