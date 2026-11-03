#include "PlayerLifecycleView.hpp"
#include <stddef.h>

// The independently exact 20-byte player rotation method receives the
// same native AnmVm prefix as the original ANM interpreter. No second
// rotation/flags byte overlay is needed for this source method identity.
typedef char PlayerNativeAnmRotationAndFlags[
    (offsetof(AnmVm, rotation) + offsetof(Float3, z) == 0x08 &&
     offsetof(AnmVm, flagsWord) == 0x1F8 &&
     sizeof(PlayerAnmVmView) == sizeof(AnmVm)) ? 1 : -1];

void PlayerAnmVmView::SetZRotation(float angle)
{
    rotation.z = angle;
    flagsWord |= 4;
}
