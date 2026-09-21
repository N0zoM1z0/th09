#include "GameManagerMode.hpp"
#include "PlayStats.hpp"
#include "RngRuntimeLeaves.hpp"
#include "SupervisorNetworkState.hpp"

#include <stddef.h>

struct OpponentSelectionState
{
    float value00;
    unsigned char unknown04[0x10];
    unsigned char value14;
};

struct OpponentSelectionSide
{
    unsigned char unknown00[0x1C];
    OpponentSelectionState *state1C;
    int value20;
    int value24;
    int selector28;
    int selectedParameter2C;
    unsigned char unknown30[4];
    unsigned int flags34;
};
typedef char OpponentSelectionSideSizeIs38[
    (sizeof(OpponentSelectionSide) == 0x38) ? 1 : -1];

struct OpponentSelectionEntry
{
    short key00;
    short valueF4_02;
    short character04;
    short field06;
    short field08;
    short field0A;
    short field0C;
    short weight0E;
};
typedef char OpponentSelectionEntrySizeIs10[
    (sizeof(OpponentSelectionEntry) == 0x10) ? 1 : -1];

struct GameManagerOpponentSelectionView
{
    OpponentSelectionSide sides[2];
    unsigned char unknown070[0x68];
    unsigned char usedCharactersD8[16];
    unsigned char unknown0E8[8];
    OpponentSelectionEntry *selectedEntryF0;
    int selectedValueF4;
    int unknownF8;
    int counterFC;
    unsigned char unknown100[0x1C];
    int setupMode11C;
    unsigned char unknown120[0x18];
    int selector138;

    void SelectOpponentConfiguration();
};

typedef char OpponentSelectionUsedAtD8[
    (offsetof(GameManagerOpponentSelectionView, usedCharactersD8) == 0xD8) ? 1 : -1];
typedef char OpponentSelectionEntryAtF0[
    (offsetof(GameManagerOpponentSelectionView, selectedEntryF0) == 0xF0) ? 1 : -1];
typedef char OpponentSelectionCounterAtFC[
    (offsetof(GameManagerOpponentSelectionView, counterFC) == 0xFC) ? 1 : -1];
typedef char OpponentSelectionModeAt11C[
    (offsetof(GameManagerOpponentSelectionView, setupMode11C) == 0x11C) ? 1 : -1];
typedef char OpponentSelectionSelectorAt138[
    (offsetof(GameManagerOpponentSelectionView, selector138) == 0x138) ? 1 : -1];

extern RngRuntimeView g_Rng;
extern unsigned char g_TitleCharacterUnlocked[16];
extern int g_FrontRuntimeValues90[4];
extern OpponentSelectionEntry g_OpponentEntries[];
extern OpponentSelectionEntry *g_OpponentLists[];
extern int g_OpponentParameterTable[];
extern int g_OpponentModeValues[];

void GameManagerOpponentSelectionView::SelectOpponentConfiguration()
{
    GameManagerModeView *modeView =
        reinterpret_cast<GameManagerModeView *>(this);

    if (modeView->IsGameMode0() || modeView->IsGameMode1())
    {
        OpponentSelectionEntry *entry =
            g_OpponentLists[sides[0].value20];
        OpponentSelectionEntry *candidates[16];
        int candidateCount = 0;
        int totalWeight = 0;

        if (entry->key00 >= 0)
        {
            short key = entry->key00;
            int targetKey = counterFC;
            do
            {
                if (key == targetKey)
                {
                    totalWeight += entry->weight0E;
                    candidates[candidateCount++] = entry;
                }

                key = entry[1].key00;
                ++entry;
            }
            while (key >= 0);
        }

        unsigned int roll =
            g_Rng.GetRandomU32InRange(totalWeight);

        int selectedIndex = 0;
        if (candidateCount > 0)
        {
            for (;;)
            {
                OpponentSelectionEntry *candidate =
                    candidates[selectedIndex];
                unsigned short weight =
                    static_cast<unsigned short>(candidate->weight0E);

                if (static_cast<short>(roll) <
                        static_cast<short>(weight) &&
                    usedCharactersD8[candidate->character04] == 0)
                {
                    break;
                }

                roll -= weight;
                if (++selectedIndex >= candidateCount)
                    break;
            }
        }

        if (selectedIndex >= candidateCount)
        {
            selectedIndex = 0;
            if (usedCharactersD8[
                    candidates[0]->character04] != 0)
            {
                do
                {
                    ++selectedIndex;
                }
                while (usedCharactersD8[
                    candidates[selectedIndex]->character04] != 0);
            }
        }

        OpponentSelectionEntry *selected =
            candidates[selectedIndex];

        selectedValueF4 = selected->valueF4_02;
        sides[1].value20 = selected->character04;
        g_PlayStatsRecord.IncrementCharacterStat5(
            static_cast<char>(selected->character04));
        usedCharactersD8[sides[1].value20] = 1;
        selectedEntryF0 = selected;

        int index = counterFC + 9 * setupMode11C;
        OpponentSelectionState *state = sides[0].state1C;
        if (state->value14 != 0 || state->value00 <= 0.0f)
        {
            int variant = g_FrontRuntimeValues90[0];
            if (variant > 3)
                variant = 3;
            sides[1].selectedParameter2C =
                g_OpponentParameterTable[5 * index + 1 + variant];
        }
        else
        {
            sides[1].selectedParameter2C =
                g_OpponentParameterTable[5 * index];
        }

        return;
    }

    int selector = selector138;
    counterFC = 9;

    OpponentSelectionEntry *selected = 0;

    if (selector == -1)
    {
        int randomSide = g_Rng.GetRandomU32InRange(2);
        selected = g_OpponentEntries;
        if (selected->key00 >= 0)
        {
            int wantedCharacter = sides[randomSide].value20;
            while (selected->character04 != wantedCharacter)
            {
                ++selected;
                if (selected->key00 < 0)
                {
                    selected = g_OpponentEntries;
                    break;
                }
            }
        }
        else
        {
            selected = g_OpponentEntries;
        }
    }
    else if (selector == -2)
    {
        int availableCount = 0;
        for (int countCharacter = 0;
             countCharacter <= 13;
             countCharacter += 2)
        {
            if (g_TitleCharacterUnlocked[countCharacter] ||
                g_SupervisorNetworkState->active)
            {
                ++availableCount;
            }

            if (g_TitleCharacterUnlocked[countCharacter + 1] ||
                g_SupervisorNetworkState->active)
            {
                ++availableCount;
            }
        }

        unsigned int selectedVisible =
            g_Rng.GetRandomU32InRange(availableCount);
        int visibleIndex = 0;
        int character = 0;

        for (;;)
        {
            if (g_TitleCharacterUnlocked[character] ||
                g_SupervisorNetworkState->active)
            {
                if (visibleIndex == static_cast<int>(selectedVisible))
                    break;
                ++visibleIndex;
            }

            if (g_TitleCharacterUnlocked[character + 1] ||
                g_SupervisorNetworkState->active)
            {
                if (visibleIndex == static_cast<int>(selectedVisible))
                {
                    character += 1;
                    break;
                }
                ++visibleIndex;
            }

            if (g_TitleCharacterUnlocked[character + 2] ||
                g_SupervisorNetworkState->active)
            {
                if (visibleIndex == static_cast<int>(selectedVisible))
                {
                    character += 2;
                    break;
                }
                ++visibleIndex;
            }

            if (g_TitleCharacterUnlocked[character + 3] ||
                g_SupervisorNetworkState->active)
            {
                if (visibleIndex == static_cast<int>(selectedVisible))
                {
                    character += 3;
                    break;
                }
                ++visibleIndex;
            }

            character += 4;
            if (character >= 16)
                break;
        }

        selected = g_OpponentEntries;
        if (selected->key00 >= 0)
        {
            while (selected->character04 != character)
            {
                ++selected;
                if (selected->key00 < 0)
                {
                    selected = g_OpponentEntries;
                    break;
                }
            }
        }
        else
        {
            selected = g_OpponentEntries;
        }
    }
    else
    {
        selected = g_OpponentEntries;
        if (selected->key00 >= 0)
        {
            while (selected->character04 != selector)
            {
                ++selected;
                if (selected->key00 < 0)
                {
                    selected = g_OpponentEntries;
                    break;
                }
            }
        }
        else
        {
            selected = g_OpponentEntries;
        }
    }

    int modeValue = g_OpponentModeValues[setupMode11C];
    sides[0].selectedParameter2C = modeValue;
    sides[1].selectedParameter2C = modeValue;
    selectedValueF4 = selected->valueF4_02;
    selectedEntryF0 = selected;
}
