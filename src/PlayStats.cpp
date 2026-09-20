#include "PlayStats.hpp"

void PlayStatsRecordView::IncrementCharacterStat(char character, int statIndex)
{
    int *value = &characterStats[character][statIndex];
    if (*value < 999999)
        ++*value;
}

void PlayStatsRecordView::SetCharacterUnlocked0(char character)
{
    characterUnlocked[0][character] = 1;
}

void PlayStatsRecordView::SetCharacterUnlocked1(char character)
{
    characterUnlocked[1][character] = 1;
}

void PlayStatsRecordView::SetCharacterUnlocked2(char character)
{
    characterUnlocked[2][character] = 1;
}

int PlayStatsRecordView::HasAnyPrimaryCharacterStat(char character)
{
    return characterStats[character][0] || characterStats[character][1] ||
           characterStats[character][2] || characterStats[character][3] ||
           characterStats[character][4];
}

int PlayStatsRecordView::GetCharacterStat5(char character)
{
    return characterStats[character][5];
}

void PlayStatsRecordView::IncrementCharacterStat5(char character)
{
    int *value = &characterStats[character][5];
    if (*value < 999999)
        ++*value;
}
