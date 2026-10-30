#pragma once

#include <stddef.h>

// Original TH09 v1.50a score record at g_TitleScoreTable (0x004A8380):
// 16 banks x 5 difficulties x 5 ranks; 0x2C bytes per record. The table
// insertion and result-drawing consumers share one physical record owner.
// The names below are source-facing roles, not claimed original identifiers.
struct TitleScoreRecordView
{
    unsigned int magic;
    unsigned short chapterSize;
    unsigned short chapterSizeCopy;
    unsigned char version;
    unsigned char runtimeMarker;
    unsigned char unknown0A[2];
    unsigned int score;
    float value10;
    unsigned char category0;
    unsigned char category1;
    unsigned char rank;
    unsigned char unknown17;
    char name[9];
    char date[6];
    unsigned char unknown27[4];
    char scoreDigit;

    int InsertIntoTable();
};

typedef char TitleScoreRecordSizeIs2C[
    (sizeof(TitleScoreRecordView) == 0x2C) ? 1 : -1];
typedef char TitleScoreRecordScoreAt0C[
    (offsetof(TitleScoreRecordView, score) == 0x0C) ? 1 : -1];
typedef char TitleScoreRecordCategory0At14[
    (offsetof(TitleScoreRecordView, category0) == 0x14) ? 1 : -1];
typedef char TitleScoreRecordCategory1At15[
    (offsetof(TitleScoreRecordView, category1) == 0x15) ? 1 : -1];
typedef char TitleScoreRecordRankAt16[
    (offsetof(TitleScoreRecordView, rank) == 0x16) ? 1 : -1];
typedef char TitleScoreRecordNameAt18[
    (offsetof(TitleScoreRecordView, name) == 0x18) ? 1 : -1];
typedef char TitleScoreRecordDigitAt2B[
    (offsetof(TitleScoreRecordView, scoreDigit) == 0x2B) ? 1 : -1];

typedef char SharedScoreMagicAt00[
    (offsetof(TitleScoreRecordView, magic) == 0x00) ? 1 : -1];
typedef char SharedScoreChapterSizeAt04[
    (offsetof(TitleScoreRecordView, chapterSize) == 0x04) ? 1 : -1];
typedef char SharedScoreChapterSizeCopyAt06[
    (offsetof(TitleScoreRecordView, chapterSizeCopy) == 0x06) ? 1 : -1];
typedef char SharedScoreVersionAt08[
    (offsetof(TitleScoreRecordView, version) == 0x08) ? 1 : -1];
typedef char SharedScoreRuntimeMarkerAt09[
    (offsetof(TitleScoreRecordView, runtimeMarker) == 0x09) ? 1 : -1];
typedef char SharedScoreValueAt10[
    (offsetof(TitleScoreRecordView, value10) == 0x10) ? 1 : -1];
typedef char SharedScoreDateAt21[
    (offsetof(TitleScoreRecordView, date) == 0x21) ? 1 : -1];

// Both producer and renderer resolve to these very same external names.
// Declaration-only; storage/constructors still require independent native
// product closure and are not manufactured in the source here.
extern TitleScoreRecordView g_TitleScoreRecord;
extern TitleScoreRecordView g_TitleScoreTable[16][5][5];
