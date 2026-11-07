#pragma once

#include <stddef.h>

// The physical TH09 input window at +0x00..+0x07 is shared by Title
// screen/key settings and the independently exact 42-byte scrolling helper.
// Keep its source-facing original InputView ABI, without asserting that this
// 8-byte prefix is the entire ReplayInputState allocation.
struct InputView
{
    union { unsigned short held00; unsigned short currentInput; };
    unsigned short previous02;
    unsigned short repeatOutput04;
    union { unsigned short frontState06; unsigned short pressed06; };

    unsigned short IsPressedScrolling(int mask);
};

typedef char TitleInputNativeWindowSizeIs08[
    (sizeof(InputView) == 0x08 &&
     offsetof(InputView, held00) == 0 &&
     offsetof(InputView, currentInput) == 0 &&
     offsetof(InputView, previous02) == 0x02 &&
     offsetof(InputView, repeatOutput04) == 0x04 &&
     offsetof(InputView, frontState06) == 0x06 &&
     offsetof(InputView, pressed06) == 0x06) ? 1 : -1];

// Key configuration's source-facing slot name borrows this exact member
// method and its physical input words without adding another allocation.
struct TitleKeyInputView : InputView {};
typedef char TitleKeyInputNativeWindow[
    (sizeof(TitleKeyInputView) == sizeof(InputView)) ? 1 : -1];
