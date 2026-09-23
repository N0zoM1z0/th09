// TH09 v1.50a RunEcl control/arithmetic family (wire opcodes 1..53).
//
// This file is intentionally a lexical switch fragment.  EclManager::RunEcl
// is one 14,792-byte owner whose handlers share locals, labels, and a stack
// frame; splitting this family into a callable replacement would invent a
// target boundary.  EclManager.cpp includes the declarations once and defines
// TH09_ECL_RUN_CONTROL_BODY inside the maintained outer switch.  This fragment
// alone does not establish exactness for the complete interpreter.
//
// Target evidence:
//   dispatcher/table: 0x0040889B / 0x0040C0A0
//   family entries:   opcodes 1..53 (52 active, opcode 3 inactive)
//   comparison helper: 0x00407700
//   call/pop helpers:   0x00407A80 / 0x00406680
//
// Committed TH08 supplied a source-family hypothesis only.  Handler roles,
// operand masks, raw branch operands, target destinations, and the ordering
// below were checked independently against TH09.

#ifndef TH09_ECL_RUN_CONTROL_DECLARATIONS
#define TH09_ECL_RUN_CONTROL_DECLARATIONS

#include "EclOpcodes.hpp"
#include "EnemyManager.hpp"

namespace Th09EclRunControl
{

struct RngView
{
    unsigned short NextU16();
    unsigned int GetRandomU32();
    unsigned int GetRandomU32InRange(unsigned int maximum);
    float GetRandomF32();
    float GetRandomF32Signed();
    float GetRandomF32InRange(float maximum);
    float GetRandomF32SignedInRange(float maximum);
};

extern RngView g_Rng;

// These maintained names describe target-observed roles.  Their helper
// definitions and final TU ABI are still needed to settle exact object
// partitioning.
int __fastcall ResolveInt(EnemyView *enemy, int rawValue);
int *__fastcall ResolveIntLValue(
    EnemyView *enemy,
    int *rawValue,
    unsigned short parameterMask,
    int operandIndex);
float *__fastcall ResolveFloatLValue(
    EnemyView *enemy,
    float *rawValue,
    unsigned short parameterMask,
    int operandIndex);

__forceinline float ResolveFloat(EnemyView *enemy, float rawValue)
{
    return enemy->ResolveFloat(rawValue);
}

float __stdcall Sin(float value);
float __stdcall Cos(float value);
float __stdcall SquareRoot(float value);
float __stdcall FloatModulo(float dividend, float divisor);
float __stdcall VectorAngle(float y, float x);
float __stdcall AddNormalizeAngle(float angle, float delta);

static void ApplyInterpolationOperation(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction);
static void InstallInterpolationSlot(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction);
Th09EclRawInstructionHeaderView *__fastcall CompareOperands(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction);
static void CallSubroutine(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    int rawSubroutineId);
static int PopContext(EnemyView *enemy);

inline int &RawInt(
    Th09EclRawInstructionHeaderView *instruction,
    int operandIndex)
{
    return *reinterpret_cast<int *>(
        reinterpret_cast<unsigned char *>(instruction) +
        0x0C + operandIndex * 4);
}

inline float &RawFloat(
    Th09EclRawInstructionHeaderView *instruction,
    int operandIndex)
{
    return *reinterpret_cast<float *>(
        reinterpret_cast<unsigned char *>(instruction) +
        0x0C + operandIndex * 4);
}

// Target RunEcl inlines these four operand wrappers at every call site while
// preserving the resolver boundaries.  Ordinary VC7.1 /Ob1 declines them, so
// the explicit inline contract records the observed TU surface.
__forceinline int ReadInt(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    int operandIndex)
{
    return (instruction->parameterMask0A & (1U << operandIndex))
               ? ResolveInt(enemy, RawInt(instruction, operandIndex))
               : RawInt(instruction, operandIndex);
}

__forceinline float ReadFloat(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    int operandIndex)
{
    return (instruction->parameterMask0A & (1U << operandIndex))
               ? enemy->ResolveFloat(RawFloat(instruction, operandIndex))
               : RawFloat(instruction, operandIndex);
}

__forceinline int *WriteInt(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    int operandIndex)
{
    return ResolveIntLValue(
        enemy,
        &RawInt(instruction, operandIndex),
        instruction->parameterMask0A,
        operandIndex);
}

__forceinline float *WriteFloat(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction,
    int operandIndex)
{
    return ResolveFloatLValue(
        enemy,
        &RawFloat(instruction, operandIndex),
        instruction->parameterMask0A,
        operandIndex);
}

} // namespace Th09EclRunControl

#endif // TH09_ECL_RUN_CONTROL_DECLARATIONS

#if defined(TH09_ECL_RUN_CONTROL_BODY)

#if !defined(TH09_ECL_RUN_SHARED_SWITCH)
#error EclRunControl.inl must be included lexically inside RunEcl's switch
#endif

// The outer function supplies EnemyView *enemy,
// Th09EclRawInstructionHeaderView *instruction, Th09EclContextView *context,
// and these four labels:
//   th09_ecl_advance_instruction
//   th09_ecl_redispatch_instruction
//   th09_ecl_restart_context
//   th09_ecl_select_next_context

    int rhsInt;
    float lhsFloat;
    float rhsFloat;
    float angle;
    float magnitude;

    case TH09_ECL_OPCODE_TERMINATE:
        return -1;

    case TH09_ECL_OPCODE_SET_SECONDARY_TIME:
        context->secondaryTime094 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_UNHANDLED_03:
        break;

    // Target places the decrement handler immediately before the shared jump
    // tail.  Operands 0/1 are raw time/displacement; operand 2 is resolved.
    case TH09_ECL_OPCODE_JUMP_DEC:
        --*Th09EclRunControl::WriteInt(enemy, instruction, 2);
        if (Th09EclRunControl::ReadInt(enemy, instruction, 2) <= 0)
            goto th09_ecl_advance_instruction;

    case TH09_ECL_OPCODE_JUMP:
        context->time008 =
            Th09EclRunControl::RawInt(instruction, 0);
        instruction = reinterpret_cast<Th09EclRawInstructionHeaderView *>(
            reinterpret_cast<unsigned char *>(instruction) +
            Th09EclRunControl::RawInt(instruction, 1));
        goto th09_ecl_redispatch_instruction;

    case TH09_ECL_OPCODE_SET_INT:
        lhsInt = Th09EclRunControl::ReadInt(enemy, instruction, 1);
        goto th09_ecl_store_int_result;

    case TH09_ECL_OPCODE_SET_FLOAT:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        break;

    case TH09_ECL_OPCODE_SET_INT_RANDOM_SIGN:
        lhsInt = (Th09EclRunControl::g_Rng.NextU16() & 1U) ? 1 : -1;
        rhsInt = Th09EclRunControl::ReadInt(enemy, instruction, 1);
        *Th09EclRunControl::WriteInt(enemy, instruction, 0) =
            rhsInt * lhsInt;
        break;

    case TH09_ECL_OPCODE_SET_FLOAT_RANDOM_SIGN:
        lhsFloat = (Th09EclRunControl::g_Rng.NextU16() & 1U)
                       ? 1.0f
                       : -1.0f;
        rhsFloat = Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            rhsFloat * lhsFloat;
        break;

    // Preserve the target's interleaved integer/float handler order.
    case TH09_ECL_OPCODE_INT_ADD_ASSIGN:
        *Th09EclRunControl::WriteInt(enemy, instruction, 0) +=
            Th09EclRunControl::ReadInt(enemy, instruction, 1);
        break;
    case TH09_ECL_OPCODE_FLOAT_ADD_ASSIGN:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) +=
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        break;
    case TH09_ECL_OPCODE_INT_SUBTRACT_ASSIGN:
        *Th09EclRunControl::WriteInt(enemy, instruction, 0) -=
            Th09EclRunControl::ReadInt(enemy, instruction, 1);
        break;
    case TH09_ECL_OPCODE_FLOAT_SUBTRACT_ASSIGN:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) -=
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        break;
    case TH09_ECL_OPCODE_INT_MULTIPLY_ASSIGN:
        *Th09EclRunControl::WriteInt(enemy, instruction, 0) *=
            Th09EclRunControl::ReadInt(enemy, instruction, 1);
        break;
    case TH09_ECL_OPCODE_FLOAT_MULTIPLY_ASSIGN:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) *=
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        break;
    case TH09_ECL_OPCODE_INT_DIVIDE_ASSIGN:
        *Th09EclRunControl::WriteInt(enemy, instruction, 0) /=
            Th09EclRunControl::ReadInt(enemy, instruction, 1);
        break;
    case TH09_ECL_OPCODE_FLOAT_DIVIDE_ASSIGN:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) /=
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        break;
    case TH09_ECL_OPCODE_INT_MODULO_ASSIGN:
        *Th09EclRunControl::WriteInt(enemy, instruction, 0) %=
            Th09EclRunControl::ReadInt(enemy, instruction, 1);
        break;
    case TH09_ECL_OPCODE_FLOAT_MODULO_ASSIGN:
        lhsFloat = Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        rhsFloat = Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            Th09EclRunControl::FloatModulo(lhsFloat, rhsFloat);
        break;

    case TH09_ECL_OPCODE_INT_ADD:
        *Th09EclRunControl::WriteInt(enemy, instruction, 0) =
            Th09EclRunControl::ReadInt(enemy, instruction, 1) +
            Th09EclRunControl::ReadInt(enemy, instruction, 2);
        break;
    case TH09_ECL_OPCODE_FLOAT_ADD:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1) +
            Th09EclRunControl::ReadFloat(enemy, instruction, 2);
        break;
    case TH09_ECL_OPCODE_INT_SUBTRACT:
        *Th09EclRunControl::WriteInt(enemy, instruction, 0) =
            Th09EclRunControl::ReadInt(enemy, instruction, 1) -
            Th09EclRunControl::ReadInt(enemy, instruction, 2);
        break;
    case TH09_ECL_OPCODE_FLOAT_SUBTRACT:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1) -
            Th09EclRunControl::ReadFloat(enemy, instruction, 2);
        break;
    case TH09_ECL_OPCODE_INT_MULTIPLY:
        *Th09EclRunControl::WriteInt(enemy, instruction, 0) =
            Th09EclRunControl::ReadInt(enemy, instruction, 1) *
            Th09EclRunControl::ReadInt(enemy, instruction, 2);
        break;
    case TH09_ECL_OPCODE_FLOAT_MULTIPLY:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1) *
            Th09EclRunControl::ReadFloat(enemy, instruction, 2);
        break;
    case TH09_ECL_OPCODE_INT_DIVIDE:
        *Th09EclRunControl::WriteInt(enemy, instruction, 0) =
            Th09EclRunControl::ReadInt(enemy, instruction, 1) /
            Th09EclRunControl::ReadInt(enemy, instruction, 2);
        break;
    case TH09_ECL_OPCODE_FLOAT_DIVIDE:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1) /
            Th09EclRunControl::ReadFloat(enemy, instruction, 2);
        break;
    case TH09_ECL_OPCODE_INT_MODULO:
        *Th09EclRunControl::WriteInt(enemy, instruction, 0) =
            Th09EclRunControl::ReadInt(enemy, instruction, 1) %
            Th09EclRunControl::ReadInt(enemy, instruction, 2);
        break;
    case TH09_ECL_OPCODE_FLOAT_MODULO:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            Th09EclRunControl::FloatModulo(
                Th09EclRunControl::ReadFloat(enemy, instruction, 1),
                Th09EclRunControl::ReadFloat(enemy, instruction, 2));
        break;

    case TH09_ECL_OPCODE_INT_INCREMENT:
        ++*Th09EclRunControl::WriteInt(enemy, instruction, 0);
        break;
    case TH09_ECL_OPCODE_INT_DECREMENT:
        --*Th09EclRunControl::WriteInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_FLOAT_SINE:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            Th09EclRunControl::Sin(
                Th09EclRunControl::ReadFloat(enemy, instruction, 1));
        break;
    case TH09_ECL_OPCODE_FLOAT_COSINE:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            Th09EclRunControl::Cos(
                Th09EclRunControl::ReadFloat(enemy, instruction, 1));
        break;
    case TH09_ECL_OPCODE_POINT_ANGLE:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            Th09EclRunControl::VectorAngle(
                Th09EclRunControl::ReadFloat(enemy, instruction, 4) -
                    Th09EclRunControl::ReadFloat(enemy, instruction, 2),
                Th09EclRunControl::ReadFloat(enemy, instruction, 3) -
                    Th09EclRunControl::ReadFloat(enemy, instruction, 1));
        break;

    // The target's physical handler order is 37, 35, 36, 38, 39.
    case TH09_ECL_OPCODE_NORMALIZE_ANGLE:
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            Th09EclRunControl::AddNormalizeAngle(
                Th09EclRunControl::ReadFloat(enemy, instruction, 0),
                0.0f);
        break;
    case TH09_ECL_OPCODE_INTERPOLATE_VALUE:
        Th09EclRunControl::ApplyInterpolationOperation(enemy, instruction);
        break;
    case TH09_ECL_OPCODE_INSTALL_INTERPOLATION:
        Th09EclRunControl::InstallInterpolationSlot(enemy, instruction);
        break;
    case TH09_ECL_OPCODE_POLAR_TO_CARTESIAN:
        angle = Th09EclRunControl::AddNormalizeAngle(
            Th09EclRunControl::ReadFloat(enemy, instruction, 2), 0.0f);
        magnitude = Th09EclRunControl::ReadFloat(enemy, instruction, 3);
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            Th09EclRunControl::Cos(angle) * magnitude;
        *Th09EclRunControl::WriteFloat(enemy, instruction, 1) =
            Th09EclRunControl::Sin(angle) * magnitude;
        break;
    case TH09_ECL_OPCODE_POINT_DISTANCE:
        lhsFloat =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1) -
            Th09EclRunControl::ReadFloat(enemy, instruction, 3);
        rhsFloat =
            Th09EclRunControl::ReadFloat(enemy, instruction, 2) -
            Th09EclRunControl::ReadFloat(enemy, instruction, 4);
        *Th09EclRunControl::WriteFloat(enemy, instruction, 0) =
            Th09EclRunControl::SquareRoot(
                lhsFloat * lhsFloat + rhsFloat * rhsFloat);
        break;

    // Successful comparisons use raw operands 2/3 as replacement time and
    // signed bytecode displacement.  The helper owns the twelve typed tests.
    case TH09_ECL_OPCODE_JUMP_IF_INT_EQUAL:
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_EQUAL:
    case TH09_ECL_OPCODE_JUMP_IF_INT_NOT_EQUAL:
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_NOT_EQUAL:
    case TH09_ECL_OPCODE_JUMP_IF_INT_LESS:
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_LESS:
    case TH09_ECL_OPCODE_JUMP_IF_INT_LESS_EQUAL:
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_LESS_EQUAL:
    case TH09_ECL_OPCODE_JUMP_IF_INT_GREATER:
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_GREATER:
    case TH09_ECL_OPCODE_JUMP_IF_INT_GREATER_EQUAL:
    case TH09_ECL_OPCODE_JUMP_IF_FLOAT_GREATER_EQUAL:
    {
        Th09EclRawInstructionHeaderView *branch =
            Th09EclRunControl::CompareOperands(enemy, instruction);
        if (branch != 0)
        {
            instruction = branch;
            goto th09_ecl_redispatch_instruction;
        }
        break;
    }

    case TH09_ECL_OPCODE_CALL:
        Th09EclRunControl::CallSubroutine(
            enemy,
            instruction,
            Th09EclRunControl::RawInt(instruction, 0));
        goto th09_ecl_restart_context;

    case TH09_ECL_OPCODE_RETURN:
        if (!Th09EclRunControl::PopContext(enemy))
            goto th09_ecl_restart_context;
        goto th09_ecl_select_next_context;

#endif // TH09_ECL_RUN_CONTROL_BODY
