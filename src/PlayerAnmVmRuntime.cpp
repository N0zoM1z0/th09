#include "PlayerLifecycleView.hpp"

#include <stddef.h>

struct PlayerAnmVmRotationView
{
    unsigned char unknown000[0x08];
    float rotationZ08;
    unsigned char unknown00C[0x1F8 - 0x0C];
    unsigned int flags1F8;
};

typedef char PlayerAnmVmRotationZAt08[
    (offsetof(PlayerAnmVmRotationView, rotationZ08) == 0x08) ? 1 : -1];
typedef char PlayerAnmVmRotationFlagsAt1F8[
    (offsetof(PlayerAnmVmRotationView, flags1F8) == 0x1F8) ? 1 : -1];

void PlayerAnmVmView::SetZRotation(float angle)
{
    PlayerAnmVmRotationView *vm =
        reinterpret_cast<PlayerAnmVmRotationView *>(this);
    vm->rotationZ08 = angle;
    vm->flags1F8 |= 4;
}
