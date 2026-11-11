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

// Retail opponent selection reads native DWORD list/weight/mode tables,
// 0x38-byte gameplay side records and individual live used/unlock bytes.
// No range checks, candidate caches or secondary record allocation are added.
#define TH09_OPP_DWORD(table, idx) \
    (*reinterpret_cast<const int *>( \
        reinterpret_cast<unsigned int>(&(table)[0]) + \
        static_cast<unsigned int>(idx) * sizeof((table)[0])))
#define TH09_OPP_PTR(table, idx) \
    (*reinterpret_cast<OpponentSelectionEntry **>( \
        reinterpret_cast<unsigned int>(&(table)[0]) + \
        static_cast<unsigned int>(idx) * sizeof((table)[0])))
#define TH09_OPP_BYTE(table, idx) \
    (*reinterpret_cast<unsigned char *>( \
        reinterpret_cast<unsigned int>(&(table)[0]) + \
        static_cast<unsigned int>(idx)))
#define TH09_OPP_SIDE(idx) \
    (*reinterpret_cast<AsciiGameManagerSideView *>( \
        reinterpret_cast<unsigned int>(&sides[0]) + \
        static_cast<unsigned int>(idx) * sizeof(sides[0])))

void AsciiGameManagerView::SelectOpponentConfiguration()
{
    if (IsGameMode0() || IsGameMode1())
    {
        OpponentSelectionEntry *entry =
            TH09_OPP_PTR(g_OpponentLists, sides[0].value20);
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
                    TH09_OPP_PTR(candidates, candidateCount++) = entry;
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
                    TH09_OPP_PTR(candidates, selectedIndex);
                unsigned short weight =
                    static_cast<unsigned short>(candidate->weight0E);

                if (static_cast<short>(roll) <
                        static_cast<short>(weight) &&
                    TH09_OPP_BYTE(usedCharacters, candidate->character04) == 0)
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
            if (TH09_OPP_BYTE(usedCharacters, TH09_OPP_PTR(candidates, 0)->character04) != 0)
            {
                do
                {
                    ++selectedIndex;
                }
                while (TH09_OPP_BYTE(usedCharacters, TH09_OPP_PTR(candidates, selectedIndex)->character04) != 0);
            }
        }

        OpponentSelectionEntry *selected =
            TH09_OPP_PTR(candidates, selectedIndex);

        selectedValueF4 = selected->valueF4_02;
        sides[1].value20 = selected->character04;
        g_PlayStatsRecord.IncrementCharacterStat5(
            static_cast<char>(selected->character04));
        TH09_OPP_BYTE(usedCharacters, sides[1].value20) = 1;
        selectedEntryF0 = selected;

        AsciiGameSideRuntimeView *state = sides[0].state;
        // The target also takes this arm for an unordered float comparison.
        if (state->value14 != 0 || !(state->value00 > 0.0f))
        {
            int variant = g_FrontRuntimeValues90[0];
            if (variant > 3)
                variant = 3;
            int index = counterFC + 9 * setupMode11C;
            sides[1].selectedParameter2C =
                TH09_OPP_DWORD(g_OpponentParameterTable, 5 * index + 1 + variant);
        }
        else
        {
            int index = counterFC + 9 * setupMode11C;
            sides[1].selectedParameter2C =
                TH09_OPP_DWORD(g_OpponentParameterTable, 5 * index);
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
            int wantedCharacter = TH09_OPP_SIDE(randomSide).value20;
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
            if (TH09_OPP_BYTE(g_TitleCharacterUnlocked, countCharacter) ||
                g_SupervisorNetworkState->active)
            {
                ++availableCount;
            }

            if (TH09_OPP_BYTE(g_TitleCharacterUnlocked, countCharacter + 1) ||
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
            if (TH09_OPP_BYTE(g_TitleCharacterUnlocked, character) ||
                g_SupervisorNetworkState->active)
            {
                if (visibleIndex == static_cast<int>(selectedVisible))
                    break;
                ++visibleIndex;
            }

            if (TH09_OPP_BYTE(g_TitleCharacterUnlocked, character + 1) ||
                g_SupervisorNetworkState->active)
            {
                if (visibleIndex == static_cast<int>(selectedVisible))
                {
                    character += 1;
                    break;
                }
                ++visibleIndex;
            }

            if (TH09_OPP_BYTE(g_TitleCharacterUnlocked, character + 2) ||
                g_SupervisorNetworkState->active)
            {
                if (visibleIndex == static_cast<int>(selectedVisible))
                {
                    character += 2;
                    break;
                }
                ++visibleIndex;
            }

            if (TH09_OPP_BYTE(g_TitleCharacterUnlocked, character + 3) ||
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
    sides[0].selectedParameter2C = TH09_OPP_DWORD(g_OpponentModeValues, mode);
    sides[1].selectedParameter2C = TH09_OPP_DWORD(g_OpponentModeValues, mode);
    selectedValueF4 = selected->valueF4_02;
    selectedEntryF0 = selected;
}

#undef TH09_OPP_DWORD
#undef TH09_OPP_PTR
#undef TH09_OPP_BYTE
#undef TH09_OPP_SIDE
