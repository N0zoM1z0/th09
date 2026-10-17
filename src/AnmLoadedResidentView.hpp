#pragma once

#include <stddef.h>

// Original-PE-backed ANM loaded-slot state shared by the bullet and effect
// resource-added callbacks. The original game reads the pointer at +0x04
// after GetAnm(8) before deciding whether to PreloadAnm(8, "etama.anm").
// The retail class name and other internal members remain unknown; this is
// only the reviewed, independently consumed eight-byte prefix.
struct AnmLoadedResidentView
{
    unsigned char unknown00[4];
    void *resident04;
};

typedef char AnmLoadedResidentPrefixSizeIs08[
    (sizeof(AnmLoadedResidentView) == 0x08) ? 1 : -1];
typedef char AnmLoadedResidentSlotAt04[
    (offsetof(AnmLoadedResidentView, resident04) == 0x04) ? 1 : -1];
