#include "AsciiManager.hpp"
#include "PlayerLifecycleView.hpp"

#include <stddef.h>

typedef char Float3PointerStartsAtObject[(offsetof(Float3, x) == 0) ? 1 : -1];
typedef char PlayerPositionPointerStartsAtObject[
    (offsetof(PlayerPositionView, x) == 0) ? 1 : -1];

// These existing logical interfaces share a physical identity body in TH09.
// Complete byte equality does not identify the original class or the origin
// of that folded body, which remains unknown in the tracking ledger.
Float3::operator float *()
{
    return &x;
}

PlayerPositionView::operator float *()
{
    return &x;
}
