#pragma once

#include <stddef.h>

struct PlayerResourceSubtractView
{
    unsigned char unknown00000[0x30388];
    float resource30388;

    void SubtractResourceClamped(float amount);
};

typedef char PlayerResourceAt30388[
    (offsetof(PlayerResourceSubtractView, resource30388) == 0x30388) ? 1 : -1];
