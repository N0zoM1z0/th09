#pragma once

#include <stddef.h>

// The message container is a file-backed, mutable Win32 i386 wire layout.
// The same four-byte entry slot contains an on-disk relative offset until
// FrontMessageRuntimeView::LoadForSideCount converts it to an instruction
// pointer. ReleaseForSideCount consumes only already-converted entries.
struct FrontMessageInstructionView
{
    unsigned short time00;
    unsigned char opcode02;
    unsigned char size03;
    int argument04;
};
typedef char FrontMessageInstructionSizeIs08[
    (sizeof(FrontMessageInstructionView) == 0x08) ? 1 : -1];

// TH09 v1.50a dialogue/message script opcodes recovered from
// FrontMessageRuntimeView::Update in src/FrontMessageUpdate.cpp.
enum FrontMessageOpcode
{
    FRONT_MESSAGE_OPCODE_END = 0x00,
    FRONT_MESSAGE_OPCODE_LOAD_PORTRAIT_SCRIPTS = 0x01,
    FRONT_MESSAGE_OPCODE_SET_PORTRAIT_SPRITE = 0x02,
    FRONT_MESSAGE_OPCODE_DRAW_TEXT_LINE = 0x03,
    FRONT_MESSAGE_OPCODE_WAIT = 0x04,
    FRONT_MESSAGE_OPCODE_INTERRUPT_PORTRAIT = 0x05,
    FRONT_MESSAGE_OPCODE_INCREMENT_IGNORE_WAIT = 0x06,
    FRONT_MESSAGE_OPCODE_SET_MUSIC = 0x07,
    FRONT_MESSAGE_OPCODE_SHOW_INTRO_GRAPHIC = 0x08,
    FRONT_MESSAGE_OPCODE_INIT_RESULT_BONUS_AND_FADE_MUSIC = 0x09,
    FRONT_MESSAGE_OPCODE_PAUSE_SCRIPT = 0x0A,
    FRONT_MESSAGE_OPCODE_TRIGGER_STAGE_TRANSITION = 0x0B,
    FRONT_MESSAGE_OPCODE_FADE_OUT_MUSIC = 0x0C,
    FRONT_MESSAGE_OPCODE_SET_SKIPPABLE = 0x0D,
    FRONT_MESSAGE_OPCODE_START_SCREEN_FADE = 0x0E,
    FRONT_MESSAGE_OPCODE_FOCUS_SIDE_AND_SET_BOTH_PORTRAITS = 0x0F,
    FRONT_MESSAGE_OPCODE_DRAW_NEXT_TEXT_LINE = 0x10,
    FRONT_MESSAGE_OPCODE_FOCUS_PORTRAIT_AND_SET_SPRITE = 0x11,
    FRONT_MESSAGE_OPCODE_SET_TEXT_BOX_VISIBLE = 0x12,
    FRONT_MESSAGE_OPCODE_SET_PLAYER_UPDATE_STATE_1 = 0x17,
    FRONT_MESSAGE_OPCODE_START_SECONDARY_TIMER = 0x18,
    FRONT_MESSAGE_OPCODE_LOAD_BOTH_FRONT_SIDE_SCRIPT_67 = 0x19,
    FRONT_MESSAGE_OPCODE_SET_TEXT_COLOR_AND_RESET = 0x1A,
    FRONT_MESSAGE_OPCODE_SET_PENDING_STAGE_LABEL = 0x1C
};

struct FrontMessageTableEntryView
{
    union
    {
        unsigned int offset00;
        FrontMessageInstructionView *instructions00;
    };
    // Weighted random message selection consumes this signed weight.
    int weight04;
};
typedef char FrontMessageTableEntrySizeIs08[
    (sizeof(FrontMessageTableEntryView) == 0x08) ? 1 : -1];
typedef char FrontMessageTableEntryOffsetAt00[
    (offsetof(FrontMessageTableEntryView, offset00) == 0) ? 1 : -1];
typedef char FrontMessageTableEntryWeightAt04[
    (offsetof(FrontMessageTableEntryView, weight04) == 0x04) ? 1 : -1];

struct FrontMessageTableView
{
    int count00;
    FrontMessageTableEntryView entries04[1];
};
typedef char FrontMessageTableEntriesAt04[
    (offsetof(FrontMessageTableView, entries04) == 0x04) ? 1 : -1];

struct FrontMessageSourceView
{
    unsigned char unknown00[0x0C];
    short musicId0C;
};
typedef char FrontMessageSourceMusicAt0C[
    (offsetof(FrontMessageSourceView, musicId0C) == 0x0C) ? 1 : -1];
