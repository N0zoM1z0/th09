#pragma once

#include <stddef.h>

// Observed global Player shared-runtime allocation; these are native words
// read independently by exact Player/Title/Replay callers. The ordinary
// state at +0xE94C belongs to this allocation, not a second global.
struct PlayerSharedRuntimeView
{
    int IsBlocked();
    unsigned char unknown0000[0xE94C];
    int blockStateE94C;
    unsigned char unknownE950[0x1095C - 0xE950];
    int updateBlock1095C;
    int drawCounter10960;
};

typedef char PlayerSharedNativeRuntimeFields[
    (offsetof(PlayerSharedRuntimeView, blockStateE94C) == 0xE94C &&
     offsetof(PlayerSharedRuntimeView, updateBlock1095C) == 0x1095C &&
     offsetof(PlayerSharedRuntimeView, drawCounter10960) == 0x10960 &&
     sizeof(PlayerSharedRuntimeView) == 0x10964) ? 1 : -1];
