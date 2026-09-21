#pragma once

#include <windows.h>
#include <stddef.h>

#pragma pack(push, 4)
struct GameWindow
{
    HWND window00;
    unsigned char unknown04[0x0C];
    signed char framesSinceRedraw10;
    unsigned char unknown11[3];
    LARGE_INTEGER performanceFrequency14;
    unsigned char startupPathDiffers1C;
    unsigned char unknown1D[0x0F];
    double currentTimestamp2C;
    double lastTimestamp34;
    double lastFrameTime3C;

    int Render();
    static void Present();
    double GetTimestamp();
};
#pragma pack(pop)

typedef char GameWindowSizeIs44[(sizeof(GameWindow) == 0x44) ? 1 : -1];
typedef char GameWindowFramesSinceRedrawAt10[
    (offsetof(GameWindow, framesSinceRedraw10) == 0x10) ? 1 : -1];
typedef char GameWindowPerformanceFrequencyAt14[
    (offsetof(GameWindow, performanceFrequency14) == 0x14) ? 1 : -1];
typedef char GameWindowStartupPathDiffersAt1C[
    (offsetof(GameWindow, startupPathDiffers1C) == 0x1C) ? 1 : -1];
typedef char GameWindowCurrentTimestampAt2C[
    (offsetof(GameWindow, currentTimestamp2C) == 0x2C) ? 1 : -1];
typedef char GameWindowLastTimestampAt34[
    (offsetof(GameWindow, lastTimestamp34) == 0x34) ? 1 : -1];
typedef char GameWindowLastFrameTimeAt3C[
    (offsetof(GameWindow, lastFrameTime3C) == 0x3C) ? 1 : -1];

extern GameWindow g_GameWindow;
