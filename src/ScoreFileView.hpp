#pragma once

#include <stddef.h>

struct ScoreFileView
{
    unsigned char unknown00;
    unsigned char rngValue1;
    unsigned short checksum;
    unsigned short version;
    unsigned char rngValue2;
    unsigned char unknown07;
    unsigned int headerSize;
    unsigned int totalSize;
    unsigned int decompressedPayloadSize;
    unsigned int compressedSize;

    static ScoreFileView *OpenScore(const char *filename);
    int LoadScoreRecords();
    int LoadLastName();
    int LoadPlayStats();
};

typedef char ScoreFileViewHeaderSizeAt08[
    (offsetof(ScoreFileView, headerSize) == 0x08) ? 1 : -1];
typedef char ScoreFileViewTotalSizeAt0C[
    (offsetof(ScoreFileView, totalSize) == 0x0C) ? 1 : -1];
typedef char ScoreFileViewRngValue1At01[
    (offsetof(ScoreFileView, rngValue1) == 0x01) ? 1 : -1];
typedef char ScoreFileViewChecksumAt02[
    (offsetof(ScoreFileView, checksum) == 0x02) ? 1 : -1];
typedef char ScoreFileViewVersionAt04[
    (offsetof(ScoreFileView, version) == 0x04) ? 1 : -1];
typedef char ScoreFileViewRngValue2At06[
    (offsetof(ScoreFileView, rngValue2) == 0x06) ? 1 : -1];
typedef char ScoreFileViewSizeIs18[(sizeof(ScoreFileView) == 0x18) ? 1 : -1];

extern ScoreFileView g_ScoreFileHeader;
