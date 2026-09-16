#include "FileSystem.hpp"

#include <stddef.h>

struct SoundPlayerLoadFmtView
{
    unsigned char unknown000[0x1F84];
    unsigned char *bgmFmtData;

    int LoadFmt(const char *path);
};

typedef char SoundPlayerLoadFmtAt1F84[
    (offsetof(SoundPlayerLoadFmtView, bgmFmtData) == 0x1F84) ? 1 : -1];

int SoundPlayerLoadFmtView::LoadFmt(const char *path)
{
    bgmFmtData = FileSystem::OpenFile(path, NULL, 0);
    return bgmFmtData != NULL ? 0 : -1;
}
