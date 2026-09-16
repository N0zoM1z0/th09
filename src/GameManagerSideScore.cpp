#include <stddef.h>

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

void GameManagerSideStateView::AddScore(int score)
{
    this->state1C->score08 += score / 10;
}
