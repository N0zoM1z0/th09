#include <stdio.h>
#include <string.h>
#include <stddef.h>

typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;

struct SupervisorVersionView {
    u8 unknown000[0x7A8];
    u32 versionDataSize;
    char *versionData;
    int CheckVersion(const char *version, i32 exeSize, i32 exeChecksum);
};

typedef char VersionDataSizeAt7A8[(offsetof(SupervisorVersionView, versionDataSize) == 0x7A8) ? 1 : -1];
typedef char VersionDataAt7AC[(offsetof(SupervisorVersionView, versionData) == 0x7AC) ? 1 : -1];

int SupervisorVersionView::CheckVersion(const char *version, i32 exeSize, i32 exeChecksum)
{
    const char *versionData;
    u32 versionDataSize;
    i32 versionDataExeSize;
    i32 versionDataExeChecksum;

    if (this->versionData == NULL)
        return 0;

    versionData = this->versionData;
    versionDataSize = this->versionDataSize;

    if (strncmp(version, "debug", 5) == 0)
        return 0;

    if (strcmp("0150a", "debug") == 0)
        return 0;

    while (versionDataSize > 0)
    {
        if (strncmp(version, versionData, 5) == 0)
        {
            versionData += 6;
            sscanf(versionData, "%d %d", &versionDataExeSize, &versionDataExeChecksum);
            if (versionDataExeSize == exeSize && versionDataExeChecksum == exeChecksum)
                return 0;
        }

        const char *oldPos = versionData;
        versionData = strchr(versionData, '\n');
        if (versionData == NULL)
            return -1;
        versionData++;
        versionDataSize -= (u32)(versionData - oldPos);
    }
    return -1;
}
