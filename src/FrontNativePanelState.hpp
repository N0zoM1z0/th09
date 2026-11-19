#pragma once

// FrontFactory owns the two original per-side panel color and timer DWORDs
// at +0xE934 and +0xE93C. Access those native arrays directly, preserving
// the live signed side selector and independent reads/writes.
#define FRONT_NATIVE_TIMER(front, side) ((front)->panelTimersE93C[(side)])
#define FRONT_NATIVE_COLOR(front, side) ((front)->panelColorsE934[(side)])
