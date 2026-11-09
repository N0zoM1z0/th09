#include "FrontSide.hpp"

#include <stddef.h>

typedef char FrontSideNativeAuxTransitionFields[
    (offsetof(FrontSide, auxA678) +
     offsetof(FrontSideAuxView, unknown554) == 0xABCC &&
     offsetof(FrontSide, transitionTimerABD0) == 0xABD0) ? 1 : -1];

void FrontSide::BeginAuxTransition()
{
    auxA678.unknown554 = 1;
    transitionTimerABD0 = 0;
}
