#include "EnemyManager.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"

extern GameErrorContext g_GameErrorContext;
extern const char g_EnemyEclLoadError[];
extern const char g_EnemyEclVersionError[];

namespace
{
struct EnemyEclRawFileView
{
    int version;
    short secondaryTableCount;
    short subroutineCount;
    unsigned int subroutineOffsets[1];
};
}

int EnemyEclManagerView::Load(char *path)
{
    EnemyEclRawFileView *file =
        reinterpret_cast<EnemyEclRawFileView *>(
            FileSystem::OpenFile(path, 0, 0));
    this->rawFile00 = file;
    if (file == 0)
    {
        g_GameErrorContext.Log(g_EnemyEclLoadError);
        return -1;
    }

    if (file->version != 0x900)
    {
        g_GameErrorContext.Log(g_EnemyEclVersionError);
        return -1;
    }

    for (int i = 0;
         i < reinterpret_cast<EnemyEclRawFileView *>(this->rawFile00)
                 ->subroutineCount;
         ++i)
    {
        unsigned int *offset =
            reinterpret_cast<unsigned int *>(
                reinterpret_cast<unsigned char *>(this->rawFile00) + 0x08) +
            i;
        *offset += reinterpret_cast<unsigned int>(this->rawFile00);
    }

    EnemyEclRawFileView *raw =
        reinterpret_cast<EnemyEclRawFileView *>(this->rawFile00);
    this->subroutineTable04 =
        reinterpret_cast<unsigned char *>(this->rawFile00) +
        raw->subroutineCount * 4 + 0x08;

    for (int i = 0;
         i < reinterpret_cast<EnemyEclRawFileView *>(this->rawFile00)
                 ->secondaryTableCount;
         ++i)
    {
        unsigned int *offset =
            reinterpret_cast<unsigned int *>(this->subroutineTable04) + i;
        *offset += reinterpret_cast<unsigned int>(this->rawFile00);
    }

    return 0;
}
