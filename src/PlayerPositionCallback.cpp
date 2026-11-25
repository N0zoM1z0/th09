#include "PlayerLifecycleView.hpp"
#include "PlayerPositionCallbackView.hpp"

int PlayerPositionCallbackView::InvokePositionCallback(
    PlayerPositionView *position)
{
    return callback30404(this, position);
}
