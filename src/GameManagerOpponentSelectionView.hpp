#pragma once

#include <stddef.h>

// Target-facing fields used by the original opponent-selection routine.
// This prefix does not establish the complete original GameManager class.
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
