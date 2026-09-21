#pragma once

struct TitleReplayConstructView
{
    unsigned char storage[0x1EC];
    TitleReplayConstructView();
};

struct TitleMusicConstructView
{
    unsigned char storage[0x292];
    TitleMusicConstructView();
};

struct TitleAnmVmConstructView
{
    unsigned char storage[0x2A4];
    TitleAnmVmConstructView();
};

struct TitleConfigConstructView
{
    unsigned char storage[0xCC];
    void Initialize();
};

struct TitleFactoryConstructView
{
    unsigned char unknown00000[0x68E4];
    TitleReplayConstructView replayEntries68E4[50];
    unsigned char unknownC8FC[0xC934 - 0xC8FC];
    TitleMusicConstructView musicEntriesC934[32];
    unsigned char unknown11B74[0x11BA0 - 0x11B74];
    TitleAnmVmConstructView vmGroup11BA0[14];
    TitleAnmVmConstructView vmGroup14098[21];
    TitleAnmVmConstructView vmGroup1780C[14];
    TitleAnmVmConstructView vmGroup19D04[8];
    unsigned char unknown1B224[0x1B2BC - 0x1B224];
    TitleConfigConstructView config1B2BC;

    TitleFactoryConstructView();
};

typedef char TitleFactoryConstructSizeIs1B388[
    (sizeof(TitleFactoryConstructView) == 0x1B388) ? 1 : -1];
