#include "FileSystem.hpp"
#include "ZunMemory.hpp"

#include <windows.h>
#include <string.h>

struct SupervisorExecutableIdentityView
{
    unsigned char unknown000[0x7A0];
    int exeChecksum7A0;
    int exeSize7A4;
};

extern SupervisorExecutableIdentityView g_Supervisor;

int ComputeExecutableChecksum()
{
    char filename[264];
    int fileSize;

    if (GetModuleFileNameA(NULL, filename, 0x105))
    {
        int checksum = 0;
        char *basename = strrchr(filename, '\\');
        if (basename == NULL)
            basename = strrchr(filename, '/');
        if (basename == NULL)
            basename = filename;
        else
            ++basename;

        unsigned char *cursor =
            FileSystem::OpenFile(basename, &fileSize, 1);
        unsigned char *allocation = cursor;
        if (cursor == NULL)
            return -1;

        int dwordCount = fileSize / 4 - 1;
        if (dwordCount > 0)
        {
            do
            {
                checksum += *reinterpret_cast<int *>(cursor);
                cursor += 4;
                --dwordCount;
            }
            while (dwordCount != 0);
        }

        g_ZunMemory.Free(allocation);
        g_Supervisor.exeSize7A4 = fileSize;
        g_Supervisor.exeChecksum7A0 = checksum;
        return checksum;
    }

    return -1;
}
