#include "GameManagerRuntimeView.hpp"
#include "PlayStats.hpp"
#include "RngRuntimeLeaves.hpp"
#include "SupervisorNetworkState.hpp"

#include "GameManagerOpponentSelectionView.hpp"

extern RngRuntimeView g_Rng;
extern unsigned char g_TitleCharacterUnlocked[16];
extern int g_FrontRuntimeValues90[4];
extern OpponentSelectionEntry g_OpponentEntries[];
extern OpponentSelectionEntry *g_OpponentLists[];
extern int g_OpponentParameterTable[];
extern int g_OpponentModeValues[];

void GameManagerOpponentSelectionView::SelectOpponentConfiguration()
{
    AsciiGameManagerView *modeView =
        reinterpret_cast<AsciiGameManagerView *>(this);

    if (modeView->IsGameMode0() || modeView->IsGameMode1())
    {
        OpponentSelectionEntry *entry =
            g_OpponentLists[sides[0].value20];
        OpponentSelectionEntry *candidates[16];
        int candidateCount = 0;
        int totalWeight = 0;

        // Reuse the inline table address throughout selection. Its contents
        // and the final side index remain live reads across RNG/stat calls.
        unsigned char *usedCharacters = usedCharactersD8;
        short key = entry->key00;
        if (key >= 0)
        {
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

        // Every subsequent decision observes the signed low word. Unsigned
        // storage preserves the target subtraction's modulo-65536 behavior.
        unsigned short roll =
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
                    usedCharacters[candidate->character04] == 0)
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
            if (usedCharacters[
                    candidates[0]->character04] != 0)
            {
                do
                {
                    ++selectedIndex;
                }
                while (usedCharacters[
                    candidates[selectedIndex]->character04] != 0);
            }
        }

        OpponentSelectionEntry *selected =
            candidates[selectedIndex];

        selectedValueF4 = selected->valueF4_02;
        sides[1].value20 = selected->character04;
        g_PlayStatsRecord.IncrementCharacterStat5(
            static_cast<char>(selected->character04));
        usedCharacters[sides[1].value20] = 1;
        selectedEntryF0 = selected;

        OpponentSelectionState *state = sides[0].state1C;
        // The target also takes this arm for an unordered float comparison.
        if (state->value14 != 0 || !(state->value00 > 0.0f))
        {
            int variant = g_FrontRuntimeValues90[0];
            if (variant > 3)
                variant = 3;
            int index = counterFC + 9 * setupMode11C;
            sides[1].selectedParameter2C =
                g_OpponentParameterTable[5 * index + 1 + variant];
        }
        else
        {
            int index = counterFC + 9 * setupMode11C;
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
            do
            {
                if (selected->character04 == wantedCharacter)
                    break;
                ++selected;
            }
            while (selected->key00 >= 0);
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
        while (selected->key00 >= 0)
        {
            if (selected->character04 == character)
                break;
            ++selected;
        }
    }
    else
    {
        selected = g_OpponentEntries;
        while (selected->key00 >= 0)
        {
            if (selected->character04 == selector)
                break;
            ++selected;
        }
    }
    if (selected->key00 < 0)
        selected = g_OpponentEntries;

    // Keep the mode snapshot, but reread the table after the first side store.
    int mode = setupMode11C;
    sides[0].selectedParameter2C = g_OpponentModeValues[mode];
    sides[1].selectedParameter2C = g_OpponentModeValues[mode];
    selectedValueF4 = selected->valueF4_02;
    selectedEntryF0 = selected;
}
