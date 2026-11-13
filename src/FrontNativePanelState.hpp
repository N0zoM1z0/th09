#pragma once

// Both FrontFactory OnCalc and Player death transitions access the same
// two real panel color/timer DWORD arrays at Front+0xE934 and Front+0xE93C.
// Keep independent Win32 unsigned index byte-strides, not shadow state.
#define FRONT_NATIVE_TIMER(front, side) \
    (*reinterpret_cast<int *>(reinterpret_cast<unsigned int>(&(front)->panelTimersE93C[0]) + static_cast<unsigned int>(side) * sizeof((front)->panelTimersE93C[0])))
#define FRONT_NATIVE_COLOR(front, side) \
    (*reinterpret_cast<int *>(reinterpret_cast<unsigned int>(&(front)->panelColorsE934[0]) + static_cast<unsigned int>(side) * sizeof((front)->panelColorsE934[0])))
