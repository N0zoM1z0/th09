#include "PlayerLifecycleView.hpp"

typedef char PlayerUpdateStateAtObjectStart[
    (offsetof(PlayerLifecycleView, updateState00) == 0) ? 1 : -1];

// This logical interface shares a three-byte physical getter with unrelated
// TH09 aliases. Byte equality does not identify its original owner or origin.
int PlayerLifecycleView::GetUpdateState()
{
    return updateState00;
}
