#pragma once

#include <stddef.h>

struct AsciiGameManagerView;
extern AsciiGameManagerView g_GameManager;

// Shared observed GameManager storage, not an original nominal class layout.
// +120: DWORD setup/RunEcl mask, also read as a BYTE by the timeline.
// +380: opcode148 adds1800; ResetGameManager clears the same DWORD.
// The latter's gameplay meaning and original declared type remain unknown.
struct GameManagerEclStateView
{
    unsigned char unknown000[0x120];
    union {
        unsigned int instructionMask120;
        unsigned char instructionMaskLowByte120;
    };
    unsigned char unknown124[0x380 - 0x124];
    int value380;
};

typedef char GameManagerEclMaskWordAt120[
    (offsetof(GameManagerEclStateView, instructionMask120) == 0x120) ? 1 : -1];
typedef char GameManagerEclMaskByteAt120[
    (offsetof(GameManagerEclStateView, instructionMaskLowByte120) == 0x120) ? 1 : -1];
typedef char GameManagerEclValueAt380[
    (offsetof(GameManagerEclStateView, value380) == 0x380) ? 1 : -1];
typedef char GameManagerEclStatePrefixSize384[
    (sizeof(GameManagerEclStateView) == 0x384) ? 1 : -1];
