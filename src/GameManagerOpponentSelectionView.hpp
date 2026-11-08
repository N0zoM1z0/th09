#pragma once

#include <stddef.h>

// Target-facing fields used by the original opponent-selection routine.
// This prefix does not establish the complete original GameManager class.
// One exact 0x10-byte stage/opponent-table record. All mutable manager
// and side fields belong to the native AsciiGameManagerView instead.
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
