#include "PlayerLifecycleView.hpp"

// The original Player +0x3031C embedded 0x1C-byte state constructs the
// ZunTimer at offset zero (the shot pointer is at +0x0C). VC7.1 naturally
// emits the complete 12-byte original constructor for this ordinary source.
// Explicit versus implicit historic special-member authorship is unknown.
PlayerState3031CView::PlayerState3031CView()
{
}
