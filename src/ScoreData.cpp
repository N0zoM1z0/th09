// Target-bound TH09 score.dat reconstruction views. These names describe
// observed behavior; original identifier spelling and TU partition remain unknown.

#include "FileSystem.hpp"
#include "GameErrorContext.hpp"
#include "Lzss.hpp"
#include "PlayStats.hpp"
#include "ScoreFileView.hpp"
#include "ZunMemory.hpp"
#include "Supervisor.hpp"

#include <windows.h>

#include <stddef.h>

// Keep these local while score-data ownership is still being reconciled.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

extern "C" char *__cdecl strcpy(char *, const char *);
extern "C" void *__cdecl memset(void *, int, size_t);
extern "C" void *__cdecl memcpy(void *, const void *, size_t);

extern GameErrorContext g_GameErrorContext;

struct ScoreRecordView {
    u32 magic;
    u16 chapterSize;
    u16 chapterSizeCopy;
    u8 version;
    u8 runtimeMarker;
    u8 unknown0A[2];
    u32 score;
    float value10;
    u8 category0;
    u8 category1;
    u8 rank;
    u8 zero17;
    char name[9];
    char date[6];
    u8 unknown27[4];
    u8 value2B;
};

typedef char ScoreRecordSizeIs2C[(sizeof(ScoreRecordView) == 0x2C) ? 1 : -1];
typedef char ScoreRecordChapterSizeAt04[(offsetof(ScoreRecordView, chapterSize) == 0x04) ? 1 : -1];
typedef char ScoreRecordVersionAt08[(offsetof(ScoreRecordView, version) == 0x08) ? 1 : -1];
typedef char ScoreRecordScoreAt0C[(offsetof(ScoreRecordView, score) == 0x0C) ? 1 : -1];
typedef char ScoreRecordCategory0At14[(offsetof(ScoreRecordView, category0) == 0x14) ? 1 : -1];
typedef char ScoreRecordCategory1At15[(offsetof(ScoreRecordView, category1) == 0x15) ? 1 : -1];
typedef char ScoreRecordRankAt16[(offsetof(ScoreRecordView, rank) == 0x16) ? 1 : -1];
typedef char ScoreRecordNameAt18[(offsetof(ScoreRecordView, name) == 0x18) ? 1 : -1];
typedef char ScoreRecordDateAt21[(offsetof(ScoreRecordView, date) == 0x21) ? 1 : -1];
typedef char ScoreRecordValue2BAt2B[(offsetof(ScoreRecordView, value2B) == 0x2B) ? 1 : -1];

struct LastNameRecordView {
    u32 magic;
    u16 chapterSize;
    u16 chapterSizeCopy;
    u8 version;
    u8 runtimeMarker;
    u8 unknown0A[2];
    char name[9];
};

typedef char LastNameRecordSizeIs18[(sizeof(LastNameRecordView) == 0x18) ? 1 : -1];
typedef char LastNameRecordNameAt0C[(offsetof(LastNameRecordView, name) == 0x0C) ? 1 : -1];

struct ScoreChapterView {
    u32 magic;
    u16 chapterSize;
    u16 chapterSizeCopy;
    u8 version;
    u8 runtimeMarker;
    u8 unknown0A[2];
};

typedef char ScoreChapterViewSizeIs0C[(sizeof(ScoreChapterView) == 0x0C) ? 1 : -1];

struct ScoreVersionRecordView {
    ScoreChapterView base;
    char version[8];
    u32 exeSize;
    u32 exeChecksum;
};

typedef char ScoreVersionRecordSizeIs1C[(sizeof(ScoreVersionRecordView) == 0x1C) ? 1 : -1];

struct ReplayRngView {
    u16 seed;
    u16 unknown02;
    i32 generationCount;
    int GetRandomU16InRange(u16 max);
    u32 GetRandomU32InRange(u32 max);
};

struct ScoreSupervisorView {
    u8 unknown000[0x798];
    u32 totalPlayTime;
    u32 systemTime;
    i32 exeChecksum;
    i32 exeSize;
    void UpdatePlayTime();
    void UpdateGameTime();
};

typedef char ScoreSupervisorTotalPlayTimeAt798[(offsetof(ScoreSupervisorView, totalPlayTime) == 0x798) ? 1 : -1];
typedef char ScoreSupervisorSystemTimeAt79C[(offsetof(ScoreSupervisorView, systemTime) == 0x79C) ? 1 : -1];
typedef char ScoreSupervisorExeChecksumAt7A0[(offsetof(ScoreSupervisorView, exeChecksum) == 0x7A0) ? 1 : -1];

extern ReplayRngView g_ReplayRng;

extern ScoreRecordView g_ScoreTable[16][5][5];
extern ScoreRecordView g_CurrentScoreRecord;
extern LastNameRecordView g_LastNameRecord;

void ScoreSupervisorView::UpdatePlayTime()
{
    DWORD playTime = timeGetTime();
    if (playTime < totalPlayTime)
        totalPlayTime = playTime;

    DWORD difference = playTime - totalPlayTime;
    g_PlayStatsRecord.totalHours += difference / 3600000;
    difference %= 3600000;
    g_PlayStatsRecord.totalMinutes += difference / 60000;
    difference %= 60000;
    g_PlayStatsRecord.totalSeconds += difference / 1000;
    difference %= 1000;
    g_PlayStatsRecord.totalMilliseconds += difference;

    if (g_PlayStatsRecord.totalMilliseconds >= 1000) {
        g_PlayStatsRecord.totalSeconds += g_PlayStatsRecord.totalMilliseconds / 1000;
        g_PlayStatsRecord.totalMilliseconds %= 1000;
    }
    if (g_PlayStatsRecord.totalSeconds >= 60) {
        g_PlayStatsRecord.totalMinutes += g_PlayStatsRecord.totalSeconds / 60;
        g_PlayStatsRecord.totalSeconds %= 60;
    }
    if (g_PlayStatsRecord.totalMinutes >= 60) {
        g_PlayStatsRecord.totalHours += g_PlayStatsRecord.totalMinutes / 60;
        g_PlayStatsRecord.totalMinutes %= 60;
    }
    totalPlayTime = playTime;
}

void ScoreSupervisorView::UpdateGameTime()
{
    DWORD gameTime = timeGetTime();
    if (gameTime < systemTime)
        systemTime = 0;

    DWORD difference = gameTime - systemTime;
    g_PlayStatsRecord.gameHours += difference / 3600000;
    difference %= 3600000;
    g_PlayStatsRecord.gameMinutes += difference / 60000;
    difference %= 60000;
    g_PlayStatsRecord.gameSeconds += difference / 1000;
    difference %= 1000;
    g_PlayStatsRecord.gameMilliseconds += difference;

    if (g_PlayStatsRecord.gameMilliseconds >= 1000) {
        g_PlayStatsRecord.gameSeconds += g_PlayStatsRecord.gameMilliseconds / 1000;
        g_PlayStatsRecord.gameMilliseconds %= 1000;
    }
    if (g_PlayStatsRecord.gameSeconds >= 60) {
        g_PlayStatsRecord.gameMinutes += g_PlayStatsRecord.gameSeconds / 60;
        g_PlayStatsRecord.gameSeconds %= 60;
    }
    if (g_PlayStatsRecord.gameMinutes >= 60) {
        g_PlayStatsRecord.gameHours += g_PlayStatsRecord.gameMinutes / 60;
        g_PlayStatsRecord.gameMinutes %= 60;
    }
    systemTime = gameTime;
}

int SaveTitleScoreData()
{
    int pendingMask = 7;
    if (g_ScoreFileHeader.version == 0)
        return 1;

    int currentOffset = 0;
    u8 *scoreData = static_cast<u8 *>(g_ZunMemory.Alloc(0x640000, "scoretmp"));
    memcpy(scoreData + currentOffset, &g_ScoreFileHeader, sizeof(g_ScoreFileHeader));
    currentOffset += sizeof(g_ScoreFileHeader);

    {
        ScoreChapterView th9k;
        th9k.magic = 0x4B394854u;
        th9k.chapterSizeCopy = sizeof(th9k);
        th9k.chapterSize = sizeof(th9k);
        th9k.version = 1;
        memcpy(scoreData + currentOffset, &th9k, sizeof(th9k));
        currentOffset += sizeof(th9k);
    }

    do
    {
        u32 recordType = g_ReplayRng.GetRandomU32InRange(3);
        if ((pendingMask & 1) && recordType == 0)
        {
            memcpy(scoreData + currentOffset, g_ScoreTable, sizeof(g_ScoreTable));
            currentOffset += sizeof(g_ScoreTable);
            pendingMask ^= 1;
        }
        else if ((pendingMask & 2) && recordType == 1)
        {
            memcpy(scoreData + currentOffset, &g_LastNameRecord, sizeof(g_LastNameRecord));
            currentOffset += sizeof(g_LastNameRecord);
            pendingMask ^= 2;
        }
        else if ((pendingMask & 4) && recordType == 2)
        {
            reinterpret_cast<ScoreSupervisorView *>(&g_Supervisor)->UpdatePlayTime();
            if (g_PlayStatsRecord.version != 0)
            {
                memcpy(scoreData + currentOffset, &g_PlayStatsRecord, sizeof(g_PlayStatsRecord));
                currentOffset += sizeof(g_PlayStatsRecord);
                pendingMask ^= 4;
            }
        }
    } while (pendingMask != 0);

    ScoreVersionRecordView vrsm;
    vrsm.base.magic = 0x4D535256u;
    vrsm.base.chapterSizeCopy = sizeof(vrsm);
    vrsm.base.chapterSize = sizeof(vrsm);
    vrsm.base.version = 1;
    vrsm.base.runtimeMarker = 0;
    strcpy(vrsm.version, "0150a");
    ScoreSupervisorView *supervisor = reinterpret_cast<ScoreSupervisorView *>(&g_Supervisor);
    vrsm.exeSize = supervisor->exeSize;
    vrsm.exeChecksum = supervisor->exeChecksum;
    memcpy(scoreData + currentOffset, &vrsm, sizeof(vrsm));
    currentOffset += sizeof(vrsm);

    ScoreFileView *header = reinterpret_cast<ScoreFileView *>(scoreData);
    header->decompressedPayloadSize = currentOffset - sizeof(ScoreFileView);
    header->totalSize = currentOffset;
    u8 *compressedData = Lzss::Encode(
        scoreData + sizeof(ScoreFileView), header->decompressedPayloadSize,
        reinterpret_cast<int *>(&header->compressedSize));
    memcpy(scoreData + sizeof(ScoreFileView), compressedData, header->compressedSize);
    GlobalFree(compressedData);

    currentOffset = header->compressedSize + sizeof(ScoreFileView);
    header->headerSize = sizeof(ScoreFileView);
    header->checksum = 0;
    header->rngValue1 = static_cast<u8>(g_ReplayRng.GetRandomU16InRange(0x100));
    header->rngValue2 = static_cast<u8>(g_ReplayRng.GetRandomU16InRange(0x100));
    header->version = 4;

    int byteIdx;
    for (byteIdx = offsetof(ScoreFileView, version); byteIdx < currentOffset; byteIdx++)
        header->checksum += scoreData[byteIdx];

    u8 *bytes = scoreData + 1;
    byteIdx = currentOffset - offsetof(ScoreFileView, checksum);
    u8 xorValue = *bytes;
    u8 byteValue;
    while (byteIdx > 0)
    {
        byteValue = bytes[1];
        xorValue = static_cast<u8>((xorValue & 0xE0) >> 5 | (xorValue & 0x1F) << 3);
        bytes[1] ^= xorValue;
        xorValue += byteValue;
        bytes++;
        byteIdx--;
    }

    u8 *encryptedData = FileSystem::Encrypt(
        scoreData, currentOffset, 0x3A, 0xCD, 0x100, 0xC00);
    FileSystem::WriteDataToFile("score.dat", encryptedData, currentOffset);
    g_ZunMemory.Free(scoreData);
    g_ZunMemory.Free(encryptedData);
    return 0;
}

ScoreFileView *ScoreFileView::OpenScore(const char *filename)
{
    ScoreFileView *expandedScoreFile;
    ScoreFileView *scoreFile;
    u32 fileSize;
    ScoreFileView *decryptedScoreFile;
    u16 checksum;
    u8 xorValue;
    i32 bytesShifted;
    i32 bytesToShift;
    u8 *bytes;
    i32 bytesToRead;
    ScoreChapterView *chapter;
    ScoreChapterView *th9kChapter;
    int hasFoundTH9K;

    g_GameErrorContext.Log("info : score load\r\n");

    scoreFile = reinterpret_cast<ScoreFileView *>(
        FileSystem::OpenFile(filename, reinterpret_cast<int *>(&fileSize), 1));

    if (scoreFile == NULL)
    {
    recreateScoreFile:
        g_GameErrorContext.Log("info : score recreate\r\n");
        if (scoreFile != NULL)
            g_ZunMemory.Free(scoreFile);

        scoreFile = reinterpret_cast<ScoreFileView *>(
            g_ZunMemory.Alloc(sizeof(ScoreFileView), "scorefile"));
        scoreFile->headerSize = sizeof(ScoreFileView);
        scoreFile->totalSize = sizeof(ScoreFileView);
        scoreFile->version = 4;
        return scoreFile;
    }

    if (fileSize < sizeof(ScoreFileView))
    {
        g_GameErrorContext.Log("warning : score.dat size is short\r\n");
        g_ZunMemory.Free(scoreFile);
        goto recreateScoreFile;
    }

    decryptedScoreFile = reinterpret_cast<ScoreFileView *>(
        FileSystem::Decrypt(reinterpret_cast<u8 *>(scoreFile), fileSize,
                            0x3A, 0xCD, 0x100, 0xC00));
    g_ZunMemory.Free(scoreFile);
    scoreFile = decryptedScoreFile;

    bytesToShift = fileSize - 2;
    checksum = 0;
    xorValue = 0;
    bytesShifted = 0;
    bytes = reinterpret_cast<u8 *>(scoreFile) + 1;

    while (bytesToShift > 0)
    {
        xorValue += *bytes;
        xorValue = (u8)((xorValue & 0xE0) >> 5 | (xorValue & 0x1F) << 3);
        bytes[1] ^= xorValue;

        if (bytesShifted >= 2)
            checksum += bytes[1];

        bytes++;
        bytesToShift--;
        bytesShifted++;
    }

    if (scoreFile->checksum != checksum)
    {
        g_GameErrorContext.Log("warning : score.dat chksum error\r\n");
        goto recreateScoreFile;
    }

    if (scoreFile->headerSize != sizeof(ScoreFileView))
    {
        g_GameErrorContext.Log("warning : header size is mismatch\r\n");
        goto recreateScoreFile;
    }

    if (scoreFile->version != 4)
    {
        g_GameErrorContext.Log("warning : score.dat version mismatch\r\n");
        goto recreateScoreFile;
    }

    expandedScoreFile = reinterpret_cast<ScoreFileView *>(
        g_ZunMemory.Alloc(sizeof(ScoreFileView) + 0xA0000, "scorefile2"));
    memcpy(expandedScoreFile, scoreFile, sizeof(ScoreFileView));
    Lzss::Decode(reinterpret_cast<u8 *>(scoreFile) + sizeof(ScoreFileView),
                 scoreFile->compressedSize,
                 reinterpret_cast<u8 *>(expandedScoreFile) + sizeof(ScoreFileView),
                 scoreFile->decompressedPayloadSize);
    g_ZunMemory.Free(scoreFile);
    scoreFile = expandedScoreFile;

    bytesToRead = (i32)(scoreFile->totalSize - scoreFile->headerSize);
    chapter = reinterpret_cast<ScoreChapterView *>(
        reinterpret_cast<u8 *>(scoreFile) + scoreFile->headerSize);
    hasFoundTH9K = 0;

    while (bytesToRead > 0)
    {
        if (chapter->magic == 0x4B394854u)
        {
            hasFoundTH9K = 1;
            th9kChapter = chapter;
        }

        if (chapter->chapterSize == 0)
        {
            g_GameErrorContext.Log("warning : score.dat chapter size is ZERO\r\n");
            goto recreateScoreFile;
        }

        bytesToRead -= chapter->chapterSize;
        chapter = reinterpret_cast<ScoreChapterView *>(
            reinterpret_cast<u8 *>(chapter) + chapter->chapterSize);
    }

    if (!hasFoundTH9K || th9kChapter->version != 1)
    {
        g_GameErrorContext.Log("warning : score.dat version mismatch\r\n");
        goto recreateScoreFile;
    }

    return scoreFile;
}

int ScoreFileView::LoadScoreRecords()
{
    i32 category0;
    i32 category1;
    i32 scoreOffset;
    i32 rank;

    category0 = 0;
    do
    {
        for (category1 = 0; category1 < 5; category1++)
        {
            scoreOffset = 0;
            rank = 0;
            do
            {
                g_ScoreTable[category0][category1][rank].score = 100000 - scoreOffset;
                g_ScoreTable[category0][category1][rank].value10 = 0.0f;
                g_ScoreTable[category0][category1][rank].category0 = (u8)category0;
                g_ScoreTable[category0][category1][rank].chapterSizeCopy = sizeof(ScoreRecordView);
                g_ScoreTable[category0][category1][rank].chapterSize = sizeof(ScoreRecordView);
                strcpy(g_ScoreTable[category0][category1][rank].name, "No Name ");
                g_ScoreTable[category0][category1][rank].category1 = (u8)category1;
                g_ScoreTable[category0][category1][rank].zero17 = 0;
                g_ScoreTable[category0][category1][rank].runtimeMarker = 0;
                g_ScoreTable[category0][category1][rank].rank = (u8)rank;
                strcpy(g_ScoreTable[category0][category1][rank].date, "--/--");
                g_ScoreTable[category0][category1][rank].magic = 0x52435348u;
                g_ScoreTable[category0][category1][rank].version = 2;
                g_ScoreTable[category0][category1][rank].value2B = 0;

                scoreOffset += 20000;
                rank++;
            } while (scoreOffset < 100000);
        }
        category0++;
    } while (category0 < 16);

    i32 bytesToRead = (i32)(totalSize - headerSize);
    ScoreRecordView *inputRecord =
        reinterpret_cast<ScoreRecordView *>(reinterpret_cast<u8 *>(this) + headerSize);
    while (bytesToRead > 0)
    {
        if (inputRecord->magic == 0x52435348u && inputRecord->version == 2)
        {
            g_ScoreTable[inputRecord->category0][inputRecord->category1][inputRecord->rank] =
                *inputRecord;
        }

        u16 chapterSize = inputRecord->chapterSize;
        bytesToRead -= chapterSize;
        inputRecord = reinterpret_cast<ScoreRecordView *>(
            reinterpret_cast<u8 *>(inputRecord) + chapterSize);
    }

    g_CurrentScoreRecord.chapterSizeCopy = sizeof(ScoreRecordView);
    g_CurrentScoreRecord.chapterSize = sizeof(ScoreRecordView);
    strcpy(g_CurrentScoreRecord.name, "No Name ");
    g_CurrentScoreRecord.value10 = 0.0f;
    g_CurrentScoreRecord.category0 = 0;
    g_CurrentScoreRecord.category1 = 0;
    g_CurrentScoreRecord.magic = 0;
    g_CurrentScoreRecord.zero17 = 0;
    g_CurrentScoreRecord.version = 2;
    g_CurrentScoreRecord.runtimeMarker = 0;
    strcpy(g_CurrentScoreRecord.date, "--/--");

    return 0;
}

int ScoreFileView::LoadLastName()
{
    LastNameRecordView *record = reinterpret_cast<LastNameRecordView *>(
        reinterpret_cast<u8 *>(this) + headerSize);

    memset(&g_LastNameRecord, 0, sizeof(g_LastNameRecord));
    g_LastNameRecord.chapterSize = sizeof(LastNameRecordView);
    g_LastNameRecord.chapterSizeCopy = sizeof(LastNameRecordView);
    g_LastNameRecord.magic = 0x4D4E534Cu;
    g_LastNameRecord.version = 1;
    g_LastNameRecord.runtimeMarker = 0;
    strcpy(g_LastNameRecord.name, "        ");

    i32 bytesToRead = (i32)(totalSize - headerSize);
    while (bytesToRead > 0)
    {
        if (record->magic == 0x4D4E534Cu && record->version == 1)
        {
            g_LastNameRecord = *record;
            return 1;
        }

        bytesToRead -= record->chapterSize;
        record = reinterpret_cast<LastNameRecordView *>(
            reinterpret_cast<u8 *>(record) + record->chapterSize);
    }

    return 0;
}

int ScoreFileView::LoadPlayStats()
{
    memset(&g_PlayStatsRecord, 0, sizeof(g_PlayStatsRecord));
    g_PlayStatsRecord.chapterSize = sizeof(PlayStatsRecordView);
    g_PlayStatsRecord.chapterSizeCopy = sizeof(PlayStatsRecordView);

    g_PlayStatsRecord.bgmUnlocked[0] = 1;
    g_PlayStatsRecord.characterUnlocked[0][0] = 1;
    g_PlayStatsRecord.characterUnlocked[0][1] = 1;
    g_PlayStatsRecord.characterUnlocked[0][2] = 1;
    g_PlayStatsRecord.characterUnlocked[0][3] = 1;
    g_PlayStatsRecord.characterUnlocked[0][4] = 1;
    g_PlayStatsRecord.characterUnlocked[1][0] = 1;
    g_PlayStatsRecord.characterUnlocked[1][1] = 1;
    g_PlayStatsRecord.characterUnlocked[1][2] = 1;
    g_PlayStatsRecord.characterUnlocked[1][3] = 1;
    g_PlayStatsRecord.characterUnlocked[1][4] = 1;

    g_PlayStatsRecord.magic = 0x54534C50u;
    g_PlayStatsRecord.version = 3;
    g_PlayStatsRecord.runtimeMarker = 0;

    PlayStatsRecordView *record = reinterpret_cast<PlayStatsRecordView *>(
        reinterpret_cast<u8 *>(this) + headerSize);
    i32 bytesToRead = (i32)(totalSize - headerSize);
    while (bytesToRead > 0)
    {
        if (record->magic == 0x54534C50u && record->version == 3)
            g_PlayStatsRecord = *record;

        bytesToRead -= record->chapterSize;
        record = reinterpret_cast<PlayStatsRecordView *>(
            reinterpret_cast<u8 *>(record) + record->chapterSize);
    }

    return 0;
}
