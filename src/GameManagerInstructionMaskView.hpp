#pragma once

#include <stddef.h>

struct AsciiGameManagerView;
extern AsciiGameManagerView g_GameManager;

// Observed storage at GameManager +0x120 (original VA 0x004A7EB0).
// Setup writes a DWORD; RunEcl reads it and the timeline reads its low byte.
// This prefix/union records those accesses, not an original C++ class layout.
struct GameManagerInstructionMaskView
{
    unsigned char unknown000[0x120];
    union {
        unsigned int instructionMask120;
        unsigned char instructionMaskLowByte120;
    };
};

typedef char GameManagerInstructionMaskWordAt120[
    (offsetof(GameManagerInstructionMaskView, instructionMask120) == 0x120) ? 1 : -1];
typedef char GameManagerInstructionMaskByteAt120[
    (offsetof(GameManagerInstructionMaskView, instructionMaskLowByte120) == 0x120) ? 1 : -1];
typedef char GameManagerInstructionMaskPrefixSize124[
    (sizeof(GameManagerInstructionMaskView) == 0x124) ? 1 : -1];
