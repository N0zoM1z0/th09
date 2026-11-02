#pragma once

#include <stddef.h>

// The ECL loader first interprets the variable-length table as image-relative
// u32 offsets.  Load() relocates each entry in place, so the small exact
// lookup leaves subsequently observe pointers in the same physical dwords.
struct EnemyEclRawFileView
{
    int version;
    short secondaryTableCount;
    short subroutineCount;
    union
    {
        unsigned int subroutineOffsets[1];
        void *resolvedSubroutines[1];
    };
};

typedef char EnemyEclRawFileHeaderSizeIs0C[
    (sizeof(EnemyEclRawFileView) == 0x0C) ? 1 : -1];
typedef char EnemyEclRawFileVersionAt00[
    (offsetof(EnemyEclRawFileView, version) == 0x00) ? 1 : -1];
typedef char EnemyEclRawFileSubroutineCountAt06[
    (offsetof(EnemyEclRawFileView, subroutineCount) == 0x06) ? 1 : -1];
typedef char EnemyEclRawFileSubroutineTableAt08[
    (offsetof(EnemyEclRawFileView, subroutineOffsets) == 0x08 &&
     offsetof(EnemyEclRawFileView, resolvedSubroutines) == 0x08) ? 1 : -1];
