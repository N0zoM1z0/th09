#pragma once

#include <stddef.h>

// Target-observed score prefix of the 0x38-byte GameManager side record.
// The original nominal class name and complete side layout remain provisional.
struct GameManagerSideScoreStorageView
{
    unsigned char unknown00[0x08];
    int score08;
};

struct GameManagerSideStateView
{
    unsigned char unknown00[0x1C];
    GameManagerSideScoreStorageView *state1C;

    void AddScore(int score);
};

typedef char GameManagerSideStateAt1C[
    (offsetof(GameManagerSideStateView, state1C) == 0x1C) ? 1 : -1];
typedef char GameManagerSideScoreAt08[
    (offsetof(GameManagerSideScoreStorageView, score08) == 0x08) ? 1 : -1];
