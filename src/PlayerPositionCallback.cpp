#include "PlayerLifecycleView.hpp"

struct PlayerPositionCallbackView;

typedef int (__fastcall *PlayerPositionCallbackFn)(
    PlayerPositionCallbackView *player,
    PlayerPositionView *position);

struct PlayerPositionCallbackView
{
    unsigned char unknown00000[0x30404];
    PlayerPositionCallbackFn callback30404;

    int InvokePositionCallback(PlayerPositionView *position);
};

int PlayerPositionCallbackView::InvokePositionCallback(
    PlayerPositionView *position)
{
    return callback30404(this, position);
}
