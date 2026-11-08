#include "PlayerLifecycleView.hpp"

// The complete native 0x44-byte Player owner state has constructed timers
// at +0x14 and +0x20. VC7.1 naturally generates two real ZunTimer calls.
// Whether the original game authored an explicit empty body or the compiler
// generated the same body implicitly cannot be determined from original PE.
PlayerOwnerStateView::PlayerOwnerStateView()
{
}
