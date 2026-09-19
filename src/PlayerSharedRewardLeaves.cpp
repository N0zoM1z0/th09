#include "PlayerLifecycleView.hpp"

struct PlayerSharedTransitionView
{
    unsigned char unknown0000[0x11EA8];
    int transitionBlock11EA8;

    int GetTransitionBlockFlag();
};

int PlayerSharedTransitionView::GetTransitionBlockFlag()
{
    return transitionBlock11EA8;
}

int PlayerOwnerStateView::ApplyRewardWithStateAdvance(
    PlayerPositionView *position,
    int value0, int value1, int value2, int value3)
{
    ++state38;
    return ApplyReward(position, value0, value1, value2, value3);
}
