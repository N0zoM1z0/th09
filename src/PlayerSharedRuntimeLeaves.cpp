#include "FrontFactoryConstructView.hpp"

#include <stddef.h>

// The original signed block flag at Front +0xE94C is the native
// message-runtime index at Front +0xE944 + message +0x08.
typedef char NativeFrontBlockedRuntimeIndexAtE94C[
    (offsetof(FrontFactoryConstructView, messageRuntimeE944) +
     offsetof(FrontMessageRuntimeView, currentIndex08) == 0xE94C) ? 1 : -1];

int FrontFactoryConstructView::IsBlocked()
{
    int value = messageRuntimeE944.currentIndex08;
    if (value >= 0 || value == -2)
        return 1;
    return 0;
}
