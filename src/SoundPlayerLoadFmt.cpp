#include "FileSystem.hpp"
#include "SoundPlayer.hpp"

#include <stddef.h>

typedef char SoundPlayerLoadFmtAt1F84[
    (offsetof(SoundPlayerLoadFmtView, bgmFmtData) == 0x1F84) ? 1 : -1];

int SoundPlayerLoadFmtView::LoadFmt(const char *path)
{
    bgmFmtData = FileSystem::OpenFile(path, NULL, 0);
    return bgmFmtData != NULL ? 0 : -1;
}
