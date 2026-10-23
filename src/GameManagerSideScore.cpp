#include "GameManagerSideScoreView.hpp"

void GameManagerSideStateView::AddScore(int score)
{
    this->state1C->score08 += score / 10;
}
