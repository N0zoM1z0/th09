#include "ZunTimer.hpp"

#include <stddef.h>

struct FrontSideAuxTransitionView
{
    unsigned char unknown000[0xABCC];
    int auxStateABCC;
    ZunTimer transitionTimerABD0;

    void BeginAuxTransition();
};

typedef char FrontSideAuxTransitionStateAtABCC[
    (offsetof(FrontSideAuxTransitionView, auxStateABCC) == 0xABCC) ? 1 : -1];
typedef char FrontSideAuxTransitionTimerAtABD0[
    (offsetof(FrontSideAuxTransitionView, transitionTimerABD0) == 0xABD0) ? 1 : -1];

void FrontSideAuxTransitionView::BeginAuxTransition()
{
    auxStateABCC = 1;
    transitionTimerABD0 = 0;
}
