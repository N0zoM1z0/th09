#pragma once

#include "AnmVmLifecycle.hpp"
#include "ReplayDataOriginalView.hpp"

struct TitleMusicConstructView
{
    unsigned char storage[0x292];
    TitleMusicConstructView();
};

// Retail TitleScreen::TitleScreen does not invoke GameConfiguration's ctor
// here; it only calls GameConfiguration::Initialize() at 0x0041A8A2.
struct TitleConfigConstructView
{
    unsigned char storage[0xCC];
    void Initialize();
};

struct TitleFactoryConstructView
{
    unsigned char unknown00000[0x68E4];
    ReplayDataView replayEntries68E4[50];
    unsigned char unknownC8FC[0xC934 - 0xC8FC];
    TitleMusicConstructView musicEntriesC934[32];
    unsigned char unknown11B74[0x11BA0 - 0x11B74];
    AnmVm vmGroup11BA0[14];
    AnmVm vmGroup14098[21];
    AnmVm vmGroup1780C[14];
    AnmVm vmGroup19D04[8];
    unsigned char unknown1B224[0x1B2BC - 0x1B224];
    TitleConfigConstructView config1B2BC;

    TitleFactoryConstructView();
};

typedef char TitleFactoryConstructSizeIs1B388[
    (sizeof(TitleFactoryConstructView) == 0x1B388) ? 1 : -1];
