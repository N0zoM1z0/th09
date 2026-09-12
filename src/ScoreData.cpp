// Target-bound TH09 score.dat reconstruction views. These names describe
// observed behavior; original identifier spelling and TU partition remain unknown.

#include <stddef.h>

// Keep these local while score-data ownership is still being reconciled.
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;

extern "C" char *__cdecl strcpy(char *, const char *);
extern "C" void *__cdecl memset(void *, int, size_t);

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

struct PlayStatsRecordView {
    u32 magic;
    u16 chapterSize;
    u16 chapterSizeCopy;
    u8 version;
    u8 runtimeMarker;
    u8 unknown0A[2];
    u8 unknown0C[0x20];
    u8 bgmUnlocked[32];
    u8 characterUnlocked[2][16];
    u8 unknown6C[0x190];
};

typedef char PlayStatsRecordSizeIs1FC[(sizeof(PlayStatsRecordView) == 0x1FC) ? 1 : -1];
typedef char PlayStatsRecordBgmUnlockedAt2C[(offsetof(PlayStatsRecordView, bgmUnlocked) == 0x2C) ? 1 : -1];
typedef char PlayStatsRecordCharacterUnlockedAt4C[(offsetof(PlayStatsRecordView, characterUnlocked) == 0x4C) ? 1 : -1];

struct ScoreFileView {
    u8 unknown00[8];
    u32 headerSize;
    u32 totalSize;
    u8 unknown10[8];

    int LoadScoreRecords();
    int LoadLastName();
    int LoadPlayStats();
};

typedef char ScoreFileViewHeaderSizeAt08[(offsetof(ScoreFileView, headerSize) == 0x08) ? 1 : -1];
typedef char ScoreFileViewTotalSizeAt0C[(offsetof(ScoreFileView, totalSize) == 0x0C) ? 1 : -1];

extern ScoreRecordView g_ScoreTable[16][5][5];
extern ScoreRecordView g_CurrentScoreRecord;
extern LastNameRecordView g_LastNameRecord;
extern PlayStatsRecordView g_PlayStatsRecord;

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
    g_LastNameRecord.chapterSizeCopy = sizeof(LastNameRecordView);
    g_LastNameRecord.chapterSize = sizeof(LastNameRecordView);
    strcpy(g_LastNameRecord.name, "        ");
    g_LastNameRecord.magic = 0x4D4E534Cu;
    g_LastNameRecord.version = 1;
    g_LastNameRecord.runtimeMarker = 0;

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
