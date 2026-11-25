#include "EnemyManager.hpp"
#include "FileSystem.hpp"
#include "GameErrorContext.hpp"

extern GameErrorContext g_GameErrorContext;
extern const char g_EnemyEclLoadError[] = "\x93\x47\x83\x66\x81\x5B\x83\x5E\x82\xCC\x93\xC7\x82\xDD\x8D\x9E\x82\xDD\x82\xC9\x8E\xB8\x94\x73\x82\xB5\x82\xDC\x82\xB5\x82\xBD\x81\x41\x83\x66\x81\x5B\x83\x5E\x82\xAA\x89\xF3\x82\xEA\x82\xC4\x82\xE9\x82\xA9\x8E\xB8\x82\xED\x82\xEA\x82\xC4\x82\xA2\x82\xDC\x82\xB7\r\n"; // 敵データの読み込みに失敗しました、データが壊れてるか失われています\r\n
extern const char g_EnemyEclVersionError[] = "\x93\x47\x83\x66\x81\x5B\x83\x5E\x82\xCC\x83\x6F\x81\x5B\x83\x57\x83\x87\x83\x93\x82\xAA\x88\xE1\x82\xA2\x82\xDC\x82\xB7\r\n"; // 敵データのバージョンが違います\r\n

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
