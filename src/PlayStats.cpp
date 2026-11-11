#include "PlayStats.hpp"

// Retail PLST uses signed-byte character ids as indices into one physical
// 0x7C-offset statistics allocation. Its i386 row offsets multiply/add
// modulo 2^32; do not use out-of-bounds C++ array subscripting here.

void PlayStatsRecordView::IncrementCharacterStat(char character, int statIndex)
{
    int *value = reinterpret_cast<int *>(
        reinterpret_cast<unsigned int>(&characterStats[0][0]) +
        (static_cast<unsigned int>(static_cast<int>(character)) * 6u +
         static_cast<unsigned int>(statIndex)) * sizeof(characterStats[0][0]));
    if (*value < 999999)
        ++*value;
}

void PlayStatsRecordView::SetCharacterUnlocked0(char character)
{
    *reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(&characterUnlocked[0][0]) +
        static_cast<unsigned int>(static_cast<int>(character))) = 1;
}

void PlayStatsRecordView::SetCharacterUnlocked1(char character)
{
    *reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(&characterUnlocked[1][0]) +
        static_cast<unsigned int>(static_cast<int>(character))) = 1;
}

void PlayStatsRecordView::SetCharacterUnlocked2(char character)
{
    *reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(&characterUnlocked[2][0]) +
        static_cast<unsigned int>(static_cast<int>(character))) = 1;
}

int PlayStatsRecordView::HasAnyPrimaryCharacterStat(char character)
{
    // Preserve five individual short-circuit reads from the same native row.
    return *reinterpret_cast<const int *>(
        reinterpret_cast<unsigned int>(&characterStats[0][0]) +
        static_cast<unsigned int>(static_cast<int>(character)) *
            sizeof(characterStats[0]) + 0u) ||
           *reinterpret_cast<const int *>(
        reinterpret_cast<unsigned int>(&characterStats[0][0]) +
        static_cast<unsigned int>(static_cast<int>(character)) *
            sizeof(characterStats[0]) + 4u) ||
           *reinterpret_cast<const int *>(
        reinterpret_cast<unsigned int>(&characterStats[0][0]) +
        static_cast<unsigned int>(static_cast<int>(character)) *
            sizeof(characterStats[0]) + 8u) ||
           *reinterpret_cast<const int *>(
        reinterpret_cast<unsigned int>(&characterStats[0][0]) +
        static_cast<unsigned int>(static_cast<int>(character)) *
            sizeof(characterStats[0]) + 12u) ||
           *reinterpret_cast<const int *>(
        reinterpret_cast<unsigned int>(&characterStats[0][0]) +
        static_cast<unsigned int>(static_cast<int>(character)) *
            sizeof(characterStats[0]) + 16u);
}

int PlayStatsRecordView::GetCharacterStat5(char character)
{
    return *reinterpret_cast<int *>(
        reinterpret_cast<unsigned int>(&characterStats[0][0]) +
        (static_cast<unsigned int>(static_cast<int>(character)) * 6u + 5u) *
            sizeof(characterStats[0][0]));
}

void PlayStatsRecordView::IncrementCharacterStat5(char character)
{
    int *value = reinterpret_cast<int *>(
        reinterpret_cast<unsigned int>(&characterStats[0][0]) +
        (static_cast<unsigned int>(static_cast<int>(character)) * 6u + 5u) *
            sizeof(characterStats[0][0]));
    if (*value < 999999)
        ++*value;
}
