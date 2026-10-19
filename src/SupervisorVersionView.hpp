#pragma once

#include <stddef.h>

// Reviewed version-data suffix of the Supervisor object; other fields remain
// opaque here. This is a storage view, not a second global object.
struct SupervisorVersionView
{
    unsigned char unknown000[0x7A8];
    unsigned int versionDataSize;
    char *versionData;

    int CheckVersion(const char *version, int exeSize, int exeChecksum);
};

typedef char VersionDataSizeAt7A8[
    (offsetof(SupervisorVersionView, versionDataSize) == 0x7A8) ? 1 : -1];
typedef char VersionDataAt7AC[
    (offsetof(SupervisorVersionView, versionData) == 0x7AC) ? 1 : -1];
typedef char SupervisorVersionViewSizeIs7B0[
    (sizeof(SupervisorVersionView) == 0x7B0) ? 1 : -1];
