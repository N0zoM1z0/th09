#include "PlayerLifecycleView.hpp"
#include "PlayerSharedTransitionView.hpp"

int PlayerSharedTransitionView::GetTransitionBlockFlag()
{
    return transitionBlock11EA8;
}

void PlayerOwnerStateView::ApplyRewardWithStateAdvance(
    PlayerPositionView *position,
    int value0, int value1, int value2, int value3)
{
    ++state38;
    ApplyReward(position, value0, value1, value2, value3);
}
