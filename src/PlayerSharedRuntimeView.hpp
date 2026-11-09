#pragma once

#include <stddef.h>

// Partial compatibility projection of the actual constructed Front allocation,
// not an independently allocated Player runtime. Unmigrated Player owners still
// read native words at +0x1095C/+0x10960 through this older source-facing view.
// The +0xE94C signed state is the real Front message current index at
// Front +0xE944 + message +0x08; its getter belongs to FrontFactoryConstructView.
struct PlayerSharedRuntimeView
{
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
