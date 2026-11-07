#pragma once
// Two native TH09 title name-entry owners use the same 6 x 16 UI grid;
// the final row provides one skipped key and two special action keys.
struct TitleNativeNameKeyboard
{
    enum
    {
        Columns = 16,
        Rows = 6,
        KeyCount = Columns * Rows,
        LastRow = Rows - 1,
        GapKey = KeyCount - 3,
        SpaceKey = KeyCount - 2,
        ConfirmKey = KeyCount - 1,
        SpaceColumn = Columns - 2,
        ConfirmColumn = Columns - 1
    };
};
typedef char TitleNativeNameKeyboardLayout[
    (TitleNativeNameKeyboard::Columns == 16 &&
     TitleNativeNameKeyboard::Rows == 6 &&
     TitleNativeNameKeyboard::KeyCount == 96 &&
     TitleNativeNameKeyboard::GapKey == 93 &&
     TitleNativeNameKeyboard::SpaceKey == 94 &&
     TitleNativeNameKeyboard::ConfirmKey == 95 &&
     TitleNativeNameKeyboard::SpaceColumn == 14 &&
     TitleNativeNameKeyboard::ConfirmColumn == 15) ? 1 : -1];
