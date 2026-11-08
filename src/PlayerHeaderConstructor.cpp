#include "PlayerLifecycleView.hpp"

// The original 0x41EE00 constructor calls ZunTimer::ZunTimer on the three
// constructed timer members at +0x00, +0x5C, and +0x68. An ordinary empty
// out-of-line VC7.1 special member naturally emits that 28-byte sequence.
// The target cannot reveal whether retail C++ authored this body explicitly
// or received the identical body from implicit special-member generation.
PlayerHeaderStateView::PlayerHeaderStateView()
{
}
