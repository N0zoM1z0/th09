#include "PlayerLifecycleView.hpp"
#include "AnmVmRotationFieldView.hpp"

void PlayerAnmVmView::SetZRotation(float angle)
{
    AnmVmRotationFieldView *vm =
        reinterpret_cast<AnmVmRotationFieldView *>(this);
    vm->rotationZ08 = angle;
    vm->flags1F8 |= 4;
}
