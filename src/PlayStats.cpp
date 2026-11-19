#include "PlayStats.hpp"

// The native PLST record owns three sixteen-byte unlock rows and sixteen
// six-DWORD character-statistic rows. Preserve the original signed character
// index and each independent read; do not insert a new bounds gate.

void PlayStatsRecordView::IncrementCharacterStat(char character, int statIndex)
{
    int &value = characterStats[character][statIndex];
    if (value < 999999)
        ++value;
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
    // Do not cache the row: the five individual short-circuit member reads
    // are observed independently in the native original.
    return characterStats[character][0] ||
           characterStats[character][1] ||
           characterStats[character][2] ||
           characterStats[character][3] ||
           characterStats[character][4];
}

int PlayStatsRecordView::GetCharacterStat5(char character)
{
    return characterStats[character][5];
}

void PlayStatsRecordView::IncrementCharacterStat5(char character)
{
    int &value = characterStats[character][5];
    if (value < 999999)
        ++value;
}
