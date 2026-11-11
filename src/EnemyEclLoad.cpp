#include "EnemyManager.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"

extern GameErrorContext g_GameErrorContext;
extern const char g_EnemyEclLoadError[];
extern const char g_EnemyEclVersionError[];

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

    // The on-disk ECL header declares only the first offset, but retail
    // relocates a variable-length table through native 32-bit byte addresses.
    for (int i = 0;
         i < this->rawFile00->subroutineCount;
         ++i)
    {
        unsigned int *offset = reinterpret_cast<unsigned int *>(
            reinterpret_cast<unsigned int>(&this->rawFile00->subroutineOffsets[0]) +
            static_cast<unsigned int>(i) * sizeof(this->rawFile00->subroutineOffsets[0]));
        *offset += reinterpret_cast<unsigned int>(this->rawFile00);
    }

    // The secondary table begins immediately after the signed-count primary
    // table. Keep signed loop gates and live file/manager field reloads.
    EnemyEclRawFileView *raw = this->rawFile00;
    this->subroutineTable04 = reinterpret_cast<Th09EclRawInstructionHeaderView **>(
        reinterpret_cast<unsigned int>(
            &raw->subroutineOffsets[0]) +
            static_cast<unsigned int>(raw->subroutineCount) *
            sizeof(raw->subroutineOffsets[0]));

    for (int i = 0;
         i < this->rawFile00->secondaryTableCount;
         ++i)
    {
        unsigned int *offset =
            reinterpret_cast<unsigned int *>(
                reinterpret_cast<unsigned int>(this->subroutineTable04) +
                static_cast<unsigned int>(i) * sizeof(unsigned int));
        *offset += reinterpret_cast<unsigned int>(this->rawFile00);
    }

    return 0;
}
