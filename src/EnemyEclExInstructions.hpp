#pragma once

#include <stddef.h>

struct EnemyView;
// Only the +0x10 integer operand is observed by slot1; this is a bounded
// instruction prefix, not the complete variable-length ECL instruction type.
struct EnemyEclExInstructionView
{
    unsigned char unknown00[0x10];
    int value10;
};
typedef char EnemyEclExInstructionValueAt10[
    (offsetof(EnemyEclExInstructionView, value10) == 0x10) ? 1 : -1];
typedef char EnemyEclExInstructionPrefixSize14[
    (sizeof(EnemyEclExInstructionView) == 0x14) ? 1 : -1];

// RunEcl passes Enemy in ECX and the instruction in EDX at every table call.
// Unused instruction arguments do not establish a different member ABI.
// These are target-facing callback contracts, not original class/name claims.
typedef void (__fastcall *EnemyEclExInstruction)(
    EnemyView *enemy, EnemyEclExInstructionView *instruction);

void __fastcall EnemyEclExInstruction0(
    EnemyView *enemy, EnemyEclExInstructionView *instruction);
void __fastcall EnemyEclExInstruction1(
    EnemyView *enemy, EnemyEclExInstructionView *instruction);
void __fastcall EnemyEclExInstruction2(
    EnemyView *enemy, EnemyEclExInstructionView *instruction);
void __fastcall EnemyEclExInstruction3(
    EnemyView *enemy, EnemyEclExInstructionView *instruction);
void __fastcall EnemyEclExInstruction4(
    EnemyView *enemy, EnemyEclExInstructionView *instruction);
void __fastcall EnemyEclExInstruction5(
    EnemyView *enemy, EnemyEclExInstructionView *instruction);
void __fastcall EnemyEclExInstruction6(
    EnemyView *enemy, EnemyEclExInstructionView *instruction);
void __fastcall EnemyEclExInstruction7(
    EnemyView *enemy, EnemyEclExInstructionView *instruction);
void __fastcall EnemyEclExInstruction8(
    EnemyView *enemy, EnemyEclExInstructionView *instruction);
void __fastcall EnemyEclExInstruction9(
    EnemyView *enemy, EnemyEclExInstructionView *instruction);
void __fastcall EnemyEclExInstruction10(
    EnemyView *enemy, EnemyEclExInstructionView *instruction);
void __fastcall EnemyEclExInstruction11(
    EnemyView *enemy, EnemyEclExInstructionView *instruction);

// Twelve reviewed callable slots at 0x004A0B14. The following zero DWORD's
// ownership (sentinel, padding or adjacent object) remains unknown.
extern EnemyEclExInstruction g_EnemyEclExInstructions[];

// Bounded callback storage in the copied context. The complete context keeps
// its existing opaque storage; this view supplies the observed callable type.
struct EnemyEclCallbackContextView
{
    unsigned char unknown000[0x14];
    EnemyEclExInstruction callback014;
    EnemyEclExInstructionView *instruction018;
};

typedef char EnemyEclCallbackAt014[
    (offsetof(EnemyEclCallbackContextView, callback014) == 0x14) ? 1 : -1];
typedef char EnemyEclCallbackInstructionAt018[
    (offsetof(EnemyEclCallbackContextView, instruction018) == 0x18) ? 1 : -1];
typedef char EnemyEclCallbackPrefixSize01C[
    (sizeof(EnemyEclCallbackContextView) == 0x1C) ? 1 : -1];
