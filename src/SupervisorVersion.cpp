#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "Supervisor.hpp"
#include "SupervisorVersionView.hpp"

#include <stdio.h>
#include <string.h>
#include <stddef.h>

typedef unsigned char u8;
typedef unsigned int u32;
typedef int i32;

struct SupervisorVersionPbgArchiveView {
    bool Load(const char *path);
};

extern SupervisorVersionPbgArchiveView g_PbgArchive;
extern GameErrorContext g_GameErrorContext;
extern const char g_DatNotFoundError[];
extern const char g_DatVersionError[];

int SupervisorAddedLoadDat()
{
    SupervisorVersionView *versionState =
        static_cast<SupervisorVersionView *>(&g_Supervisor);
    if (g_PbgArchive.Load("th09.dat"))
    {
        int fileSize;
        char versionFileName[128];
        sprintf(versionFileName, "th09_%.4x%c.ver", 0x150, 'a');
        versionState->versionData7AC = reinterpret_cast<char *>(
            FileSystem::OpenFile(versionFileName, &fileSize, 0));
        versionState->versionDataSize7A8 = fileSize;
        if (versionState->versionData7AC == NULL)
        {
            g_GameErrorContext.Fatal(g_DatVersionError);
            return -1;
        }
    }
    else
    {
        g_GameErrorContext.Fatal(g_DatNotFoundError);
        return -1;
    }
    return 0;
}

int SupervisorVersionView::CheckVersion(const char *version, i32 exeSize, i32 exeChecksum)
{
    const char *versionData;
    u32 versionDataSize;
    i32 versionDataExeSize;
    i32 versionDataExeChecksum;

    if (this->versionData7AC == NULL)
        return 0;

    versionData = reinterpret_cast<const char *>(this->versionData7AC);
    versionDataSize = this->versionDataSize7A8;

    if (strncmp(version, "debug", 5) == 0)
        return 0;

    if (strcmp("0150a", "debug") == 0)
        return 0;

    // Version records share the real loaded .ver file buffer; retail
    // advances its cursor and remaining native DWORD count in i386
    // modulo-32 byte-address arithmetic, retaining the original scan.
    while (versionDataSize > 0)
    {
        if (strncmp(version, versionData, 5) == 0)
        {
            versionData = reinterpret_cast<const char *>(
                reinterpret_cast<u32>(versionData) + 6u);
            sscanf(versionData, "%d %d", &versionDataExeSize, &versionDataExeChecksum);
            if (versionDataExeSize == exeSize && versionDataExeChecksum == exeChecksum)
                return 0;
        }

        const char *oldPos = versionData;
        versionData = strchr(versionData, '\n');
        if (versionData == NULL)
            return -1;
        versionData = reinterpret_cast<const char *>(
            reinterpret_cast<u32>(versionData) + 1u);
        versionDataSize -= reinterpret_cast<u32>(versionData) -
            reinterpret_cast<u32>(oldPos);
    }
    return -1;
}
