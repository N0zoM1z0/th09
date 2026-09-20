#pragma once

#include <stddef.h>

// Target-bound TH09 PLST record view. Field names describe observed behavior;
// original source spelling and physical data-definition ownership remain open.
struct PlayStatsRecordView
{
    unsigned int magic;
    unsigned short chapterSize;
    unsigned short chapterSizeCopy;
    unsigned char version;
    unsigned char runtimeMarker;
    unsigned char unknown0A[2];
    unsigned int totalHours;
    unsigned int totalMinutes;
    unsigned int totalSeconds;
    unsigned int totalMilliseconds;
    unsigned int gameHours;
    unsigned int gameMinutes;
    unsigned int gameSeconds;
    unsigned int gameMilliseconds;
    unsigned char bgmUnlocked[32];
    unsigned char characterUnlocked[3][16];
    int characterStats[16][6];

    void IncrementCharacterStat(char character, int statIndex);
    void SetCharacterUnlocked0(char character);
    void SetCharacterUnlocked1(char character);
    void SetCharacterUnlocked2(char character);
    int HasAnyPrimaryCharacterStat(char character);
    int GetCharacterStat5(char character);
    void IncrementCharacterStat5(char character);
};

typedef char PlayStatsRecordSizeIs1FC[(sizeof(PlayStatsRecordView) == 0x1FC) ? 1 : -1];
typedef char PlayStatsRecordTotalHoursAt0C[
    (offsetof(PlayStatsRecordView, totalHours) == 0x0C) ? 1 : -1];
typedef char PlayStatsRecordTotalMillisecondsAt18[
    (offsetof(PlayStatsRecordView, totalMilliseconds) == 0x18) ? 1 : -1];
typedef char PlayStatsRecordGameHoursAt1C[
    (offsetof(PlayStatsRecordView, gameHours) == 0x1C) ? 1 : -1];
typedef char PlayStatsRecordGameMillisecondsAt28[
    (offsetof(PlayStatsRecordView, gameMilliseconds) == 0x28) ? 1 : -1];
typedef char PlayStatsRecordBgmUnlockedAt2C[
    (offsetof(PlayStatsRecordView, bgmUnlocked) == 0x2C) ? 1 : -1];
typedef char PlayStatsRecordCharacterUnlockedAt4C[
    (offsetof(PlayStatsRecordView, characterUnlocked) == 0x4C) ? 1 : -1];
typedef char PlayStatsRecordCharacterStatsAt7C[
    (offsetof(PlayStatsRecordView, characterStats) == 0x7C) ? 1 : -1];

extern PlayStatsRecordView g_PlayStatsRecord;
