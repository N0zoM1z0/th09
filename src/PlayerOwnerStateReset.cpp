#include "PlayerLifecycleView.hpp"
#include <stddef.h>

// The original 29-byte reset is a member of the actual Player+0x30410
// owner state, not an additional independently allocated 0x44-byte view.
typedef char PlayerOwnerPrepareResetNativeLayout[
    (sizeof(PlayerOwnerStateView) == 0x44 &&
     offsetof(PlayerOwnerStateView, timer20) == 0x20 &&
     offsetof(PlayerOwnerStateView, state04) == 0x04 &&
     offsetof(PlayerOwnerStateView, state38) == 0x38) ? 1 : -1];

void PlayerOwnerStateView::PrepareReset()
{
    this->timer20 = 0;
    this->state04 = 0;
    this->state38 = 0;
}
