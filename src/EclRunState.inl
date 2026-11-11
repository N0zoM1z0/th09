// TH09 v1.50a RunEcl boss/state/effects family (wire opcodes 124..157).
//
// This lexical switch fragment covers 32 active handlers and the two target-
// default slots 141/142. It remains part of the unsplit physical
// EclManager::RunEcl owner; declarations below name target-observed roles but
// do not invent separately callable opcode handlers.
//
// Target evidence:
//   boss/death/life: 0x0040AB5D..0x0040AF9C
//   effects/items:   0x0040AFA1..0x0040B19E
//   callbacks/trail: 0x0040B1A6..0x0040B86C
//
// Committed TH08 supplied source-shape hypotheses only.  TH09 independently
// fixes the offsets, masks, repeated resolver calls, helper calls, raw operands
// and default entries represented here.

#ifndef TH09_ECL_RUN_STATE_DECLARATIONS
#define TH09_ECL_RUN_STATE_DECLARATIONS

#include "EclRunBullet.inl"
#include "EnemyEclExInstructions.hpp"
#include "EffectManager.hpp"
#include "EnemyAttachedEffectStateView.hpp"
#include "AsciiManager.hpp"
#include "SoundPlayer.hpp"
#include "Background.hpp"
#include "CardAttack.hpp"
#include "EnemyCorePlayerView.hpp"
#include "ZunMemory.hpp"

#include "EclEnemyPrimaryFlags.hpp"
#include <stddef.h>
#include <string.h>

extern AsciiManager g_AsciiManager;

namespace Th09EclRunState
{

enum EnemyStateFlag
{
    ENEMY_STATE_BOSS = 0x00000002,
    ENEMY_STATE_SPECIAL_INTERACTION = 0x00000080,
    ENEMY_STATE_DEATH_MODE_MASK = 0x000E0000,
    ENEMY_STATE_ANM_ROTATION_ENABLED = 0x00400000,
    ENEMY_STATE_CALL_STACK_DISABLED = 0x00800000,
    ENEMY_STATE_TIMEOUT_SPELL = 0x01000000
};

enum TrailFlag
{
    TRAIL_BUILD_GEOMETRY = 0x08
};

typedef Th09EclEnemyPrimaryFlagBits EnemyStateFlagBits;
typedef char Th09EclStateFlagBitsSizeIs04[
    (sizeof(EnemyStateFlagBits) == 0x04) ? 1 : -1];

} // namespace Th09EclRunState

// Shared observed storage used by ECL state handlers and Enemy update.
// Roles and offsets are target-bound; original nominal C++ classes remain unknown.
struct EnemyManagerCoreView;
struct EnemyCoreView;

// The owner in Side slot +0x14 is the native CardAttack created by
// GameManagerSetup. These opcode state writes hit its exact counters at
// +0xA4/+0xA8, shared with the original-bound CardAttack::OnUpdate.
typedef CardAttack EnemyCoreSideOwnerView;
typedef char EnemyCoreSideOwnerPrimaryAtA4[
    (offsetof(EnemyCoreSideOwnerView, primaryPresentationCounterA4) == 0xA4) ? 1 : -1];
typedef char EnemyCoreSideOwnerAlternateAtA8[
    (offsetof(EnemyCoreSideOwnerView, secondaryPresentationCounterA8) == 0xA8) ? 1 : -1];

// The ECL interpreter and Enemy calc traverse the same native side object.
// Keep one source layout and one field authority rather than a copy of 0x38.
typedef EnemySideStateView EnemyCoreSideStateView;
// Both lexical ECL and Enemy update use native Enemy's actual samples and
// vertices, rather than private lookalikes at the same constant offsets.
typedef EnemyTrailSampleView EnemyCoreTrailSampleView;
typedef EnemyTrailVertexView EnemyCoreTrailVertexView;
// The ECL state family uses the native EnemyView fields directly. The opaque
// EnemyCoreView tag above survives only in independently bound helper APIs;
// it does not describe a second allocation or duplicate the Enemy layout.

// The exact mass-clear leaf owns this distinct C++ receiver name.  Its
// physical state, however, is the same native EnemyManagerView allocation
// used by the ECL interpreter and 3,900-byte Enemy calc.  Preserve the
// original target-linked member name without duplicating 0x2AC450 bytes.
struct EnemyManagerCoreView : EnemyManagerView
{
    int KillAllNonBossEnemies(int transitionValue, int startingValue);
};
typedef char EnemyCoreNativeManagerIdentity[
    (sizeof(EnemyManagerCoreView) == sizeof(EnemyManagerView) &&
     offsetof(EnemyManagerCoreView, primaryEclManager000) == 0 &&
     offsetof(EnemyManagerCoreView, sideState320) == 0x320 &&
     offsetof(EnemyManagerCoreView, spawnTemplate328) == 0x328 &&
     offsetof(EnemyManagerCoreView, enemies5758) == 0x5758 &&
     offsetof(EnemyManagerCoreView, remoteSlots2AC388) == 0x2AC388 &&
     offsetof(EnemyManagerCoreView, scheduleRuntime2AC3E0) == 0x2AC3E0 &&
     offsetof(EnemyManagerCoreView, drawGroupHeads2AC410) == 0x2AC410 &&
     offsetof(EnemyManagerCoreView, priorityEnemy2AC444) == 0x2AC444) ? 1 : -1];

namespace Th09EclRunState
{
typedef ::EnemyCoreSideStateView SideStateView;
typedef ::EnemyManagerCoreView ManagerStateView;
typedef ::EnemyCoreTrailSampleView TrailSampleView;
typedef ::EnemyCoreTrailVertexView TrailVertexView;
typedef ::EnemyView EnemyStateView;

// These five indices are signed native ECL wire words into the one genuine
// EnemyView allocation. Retail i386 calculates base + index * element-size
// modulo 2^32, without a C++ array-bound guard. Keep every original lookup
// at its execution point; the child block source deliberately rereads it.
inline short &SubroutineSlot(EnemyView *enemy, int index)
{
    return *reinterpret_cast<short *>(
        reinterpret_cast<unsigned int>(&enemy->subroutineSlots2D30[0]) +
        static_cast<unsigned int>(index) * sizeof(enemy->subroutineSlots2D30[0]));
}

inline int &LifeCallbackThreshold(EnemyView *enemy, int index)
{
    return *reinterpret_cast<int *>(
        reinterpret_cast<unsigned int>(&enemy->lifeCallbackThresholds33B0[0]) +
        static_cast<unsigned int>(index) * sizeof(enemy->lifeCallbackThresholds33B0[0]));
}

inline int &LifeCallbackSubId(EnemyView *enemy, int index)
{
    return *reinterpret_cast<int *>(
        reinterpret_cast<unsigned int>(&enemy->lifeCallbackSubIds33C0[0]) +
        static_cast<unsigned int>(index) * sizeof(enemy->lifeCallbackSubIds33C0[0]));
}

inline void * &ChildSlotRef(EnemyView *enemy, int index)
{
    return *reinterpret_cast<void * *>(
        reinterpret_cast<unsigned int>(&enemy->childEclBlocks33D8[0]) +
        static_cast<unsigned int>(index) * sizeof(enemy->childEclBlocks33D8[0]));
}

inline AnmVm &SecondaryVm(EnemyView *enemy, int index)
{
    return *reinterpret_cast<AnmVm *>(
        reinterpret_cast<unsigned int>(&enemy->secondaryVms2AC[0]) +
        static_cast<unsigned int>(index) * sizeof(enemy->secondaryVms2AC[0]));
}
typedef char Th09EclStateSideEffectAt0C[(offsetof(SideStateView, effectManager0C) == 0x0C) ? 1 : -1];
typedef char Th09EclStateManagerSideAt320[(offsetof(ManagerStateView, sideState320) == 0x320) ? 1 : -1];
typedef char Th09EclStateTrailSampleSizeIs1C[(sizeof(TrailSampleView) == 0x1C) ? 1 : -1];
typedef char Th09EclStateTrailVertexSizeIs1C[(sizeof(TrailVertexView) == 0x1C) ? 1 : -1];
typedef char Th09EclStateEnemySizeIs5430[
    (sizeof(EnemyStateView) == 0x5430) ? 1 : -1];
typedef char Th09EclStatePrimaryVmAt008[
    (offsetof(EnemyStateView, primaryAnmVm0008) == 0x0008) ? 1 : -1];
typedef char Th09EclStateMainContextAt07F4[
    (offsetof(EnemyStateView, mainContext07F4) == 0x07F4) ? 1 : -1];
typedef char Th09EclStateMainCallStackAt0A20[
    (offsetof(EnemyStateView, mainCallStack0A20) == 0x0A20) ? 1 : -1];
typedef char Th09EclStateActiveContextAt2CE0[
    (offsetof(EnemyStateView, activeContext2CE0) == 0x2CE0) ? 1 : -1];
typedef char Th09EclStateActiveCallStackAt2CE4[
    (offsetof(EnemyStateView, activeCallStack2CE4) == 0x2CE4) ? 1 : -1];
typedef char Th09EclStateCallDepthsAt2D28[
    (offsetof(EnemyStateView, mainCallDepth2D28) == 0x2D28 &&
     offsetof(EnemyStateView, activeCallDepth2D2A) == 0x2D2A) ? 1 : -1];
typedef char Th09EclStateSubroutineSlotsAt2D30[
    (offsetof(EnemyStateView, subroutineSlots2D30) == 0x2D30) ? 1 : -1];
typedef char Th09EclStatePendingSubAt2D70[
    (offsetof(EnemyStateView, pendingSubroutineId2D70) == 0x2D70) ? 1 : -1];
typedef char Th09EclStateWorldPositionAt2DD4[
    (offsetof(EnemyStateView, worldPosition2DD4) == 0x2DD4) ? 1 : -1];
typedef char Th09EclStateMovementAngleAt2DE0[
    (offsetof(EnemyStateView, movementAngle2DE0) == 0x2DE0) ? 1 : -1];
typedef char Th09EclStateBulletRankAt2E38[
    (offsetof(EnemyStateView, bulletRankSpeedLow2E38) == 0x2E38) ? 1 : -1];
typedef char Th09EclStateBossTimerAt2E64[
    (offsetof(EnemyStateView, bossTimer2E64) == 0x2E64) ? 1 : -1];
typedef char Th09EclStateLaserSlotsAt32D8[
    (offsetof(EnemyStateView, laserSlots32D8) == 0x32D8) ? 1 : -1];
typedef char Th09EclStateBossSlotAt336B[
    (offsetof(EnemyStateView, bossSlot336B) == 0x336B) ? 1 : -1];
typedef char Th09EclStateDifficultyOverrideAt3388[
    (offsetof(EnemyStateView, difficultyOverride3388) == 0x3388) ? 1 : -1];
typedef char Th09EclStateLifeCallbacksAt33B0[
    (offsetof(EnemyStateView, lifeCallbackThresholds33B0) == 0x33B0) ? 1 : -1];
typedef char Th09EclStateChildBlocksAt33D8[
    (offsetof(EnemyStateView, childEclBlocks33D8) == 0x33D8) ? 1 : -1];
typedef char Th09EclStateTrailAt33E8[
    (offsetof(EnemyStateView, trailSamples33E8) == 0x33E8) ? 1 : -1];
typedef char Th09EclStateTrailVerticesAt3E68[
    (offsetof(EnemyStateView, trailVertices3E68) == 0x3E68) ? 1 : -1];
typedef char Th09EclStateTrailFlagsAt53A0[
    (offsetof(EnemyStateView, trailFlags53A0) == 0x53A0) ? 1 : -1];
typedef char Th09EclStateEffectsAt53B4[
    (offsetof(EnemyStateView, attachedEffectState53B4) == 0x53B4) ? 1 : -1];

struct TrailInstructionArgs
{
    unsigned char flags00;
    unsigned char padding01[3];
    int historyLength04;
    int collisionLength08;
    int sampleStride0C;
};

typedef char Th09EclTrailInstructionArgsHistoryAt04[
    (offsetof(TrailInstructionArgs, historyLength04) == 0x04) ? 1 : -1];
typedef char Th09EclTrailInstructionArgsSampleAt0C[
    (offsetof(TrailInstructionArgs, sampleStride0C) == 0x0C) ? 1 : -1];

struct ChildEclBlock
{
    int subroutineId00;
    short unknown04;
    short callStackDepth06;
    Th09EclContextView context08;
    Th09EclContextView callStack0234[TH09_ECL_MAIN_CALL_STACK_CAPACITY];
};
typedef char Th09EclStateChildBlockSizeIs24F4[
    (sizeof(ChildEclBlock) == 0x24F4) ? 1 : -1];
typedef char Th09EclStateChildVariablesAt24[
    (offsetof(ChildEclBlock, context08) +
         offsetof(Th09EclContextView, intVariables1C) == 0x24) ? 1 : -1];
typedef char Th09EclStateChildDepthAt06[
    (offsetof(ChildEclBlock, callStackDepth06) == 0x06) ? 1 : -1];
typedef char Th09EclStateChildCallStackAt234[
    (offsetof(ChildEclBlock, callStack0234) == 0x0234) ? 1 : -1];

void __fastcall ReleaseAttachedEffects(EnemyView *enemy);

inline EnemyStateView *View(EnemyView *enemy)
{
    return enemy;
}

inline int &BackgroundScriptLabel(EnemyView *enemy)
{
    return enemy->manager00->sideState320->background00->pendingStageLabel18;
}

// Byte and halfword views address the same counted wire payload; these
// native 32-bit byte offsets do not subscript the zero-length C++ member.
inline unsigned char &RawByte(
    Th09EclRawInstructionHeaderView *instruction,
    int byteOffset)
{
    // Byte offsets traverse the packed four-byte native wire arguments,
    // not the instruction prefix or a second synthetic payload layout.
    return *reinterpret_cast<unsigned char *>(
        reinterpret_cast<unsigned int>(instruction) +
        static_cast<unsigned int>(offsetof(Th09EclRawInstructionHeaderView, arguments)) +
        static_cast<unsigned int>(byteOffset));
}

inline short &RawShort(
    Th09EclRawInstructionHeaderView *instruction,
    int byteOffset)
{
    // The original short operand is an element of the same packed 4-byte
    // union word used by RawByte, RawInt, and RawFloat. The observed callers
    // use aligned byte offsets 0 and 4; no shadow short-buffer allocation.
    return *reinterpret_cast<short *>(
        reinterpret_cast<unsigned int>(instruction) +
        static_cast<unsigned int>(offsetof(Th09EclRawInstructionHeaderView, arguments)) +
        static_cast<unsigned int>(byteOffset));
}

__forceinline void AssignFlagField(
    unsigned int &flags,
    unsigned int mask,
    unsigned int shiftedValue)
{
    flags = (flags & ~mask) | (shiftedValue & mask);
}

// Opcode 155 updates the one true timeout bit of the shared flags word.
// TH09's physical target uses a masked XOR of the previous word with the
// shifted wire byte. Only bit 24 changes, for every possible wire byte.
// This is a TU-inline implementation detail, not a separate target boundary.
inline unsigned int AssignTimeoutSpellBit(unsigned int old,
                                                 unsigned char value)
{
    return old ^ ((old ^ (static_cast<unsigned int>(value) << 24)) &
                   ENEMY_STATE_TIMEOUT_SPELL);
}

} // namespace Th09EclRunState

#endif // TH09_ECL_RUN_STATE_DECLARATIONS

#if defined(TH09_ECL_RUN_STATE_BODY)

#if !defined(TH09_ECL_RUN_SHARED_SWITCH)
#error EclRunState.inl must be included lexically inside RunEcl's switch
#endif

    int stateInt;
    int stateIndex;
    int stateCallback;
    Th09EclRunState::ChildEclBlock *childEcl;

    case TH09_ECL_OPCODE_SET_BOSS:
        stateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (stateInt >= 0)
        {
            enemy->manager00->remoteSlots2AC388[
                Th09EclRunControl::ReadInt(enemy, instruction, 0)] = enemy;
            enemy->primaryFlagBits337C.boss01 = 1;
            enemy->bossSlot336B =
                static_cast<unsigned char>(
                    Th09EclRunControl::ReadInt(enemy, instruction, 0));
            // Retail calls exact SetBossMarkerInterrupt @0x4067D0 here,
            // not the distinct AsciiManager::SetBossMarkerState @0x40F890.
            ::g_AsciiManager.SetBossMarkerInterrupt(
                enemy->bossSlot336B,
                1);
            enemy->minimumPlayerDistanceSquared33A8 =
                0.0f;
        }
        else
        {
            Th09EclRunRemote::RemoteEnemy(enemy, enemy->bossSlot336B) = 0;
            enemy->primaryFlagBits337C.boss01 = 0;
            ::g_AsciiManager.SetBossMarkerInterrupt(
                enemy->bossSlot336B,
                2);
            Th09EclRunState::ReleaseAttachedEffects(enemy);
            ::g_AsciiManager.SetBossMarkerPosition(
                enemy->bossSlot336B,
                &Float3(-999.0f, -999.0f, 0.0f));
        }
        break;

    case TH09_ECL_OPCODE_ATTACH_SPELL_EFFECT:
        enemy->attachedEffectState53B4.distance64 =
            Th09EclRunControl::RawFloat(instruction, 4);
        enemy->attachedEffectState53B4.count60 = static_cast<int>(
            static_cast<unsigned int>(enemy->attachedEffectState53B4.count60) + 1u);
        break;

    case TH09_ECL_OPCODE_SET_DRAW_GROUP:
        enemy->drawGroup3387 =
            static_cast<unsigned char>(
                Th09EclRunControl::ReadInt(enemy, instruction, 0));
        break;

    case TH09_ECL_OPCODE_PLAY_POSITIONED_SOUND:
        stateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        ::g_SoundPlayer.PlaySoundPositionedByIdx(
            stateInt,
            enemy->position2D74.x);
        break;

    case TH09_ECL_OPCODE_SET_DEATH_MODE:
        enemy->primaryFlagBits337C.deathMode17 =
            Th09EclRunState::RawByte(instruction, 0);
        break;

    case TH09_ECL_OPCODE_SET_DEATH_CALLBACK:
        enemy->deathCallbackSubId2D2E =
            Th09EclRunState::RawShort(instruction, 0);
        break;

    case TH09_ECL_OPCODE_SET_SUBROUTINE_SLOT:
        stateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        stateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 1);
        Th09EclRunState::SubroutineSlot(enemy, stateIndex) =
            static_cast<short>(stateInt);
        break;

    case TH09_ECL_OPCODE_CALL_SUBROUTINE_SLOT:
        enemy->pendingSubroutineId2D70 =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 0));
        goto th09_ecl_enter_pending_subroutine;

    case TH09_ECL_OPCODE_SET_LIFE:
        stateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        enemy->maxLife2E4C = stateInt;
        enemy->life2E48 = stateInt;
        enemy->phaseStartingLife2E50 = stateInt;
        break;

    case TH09_ECL_OPCODE_SET_BOSS_TIMER:
        enemy->bossTimer2E64 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_SET_LIFE_CALLBACK:
        stateInt = Th09EclRunControl::ReadInt(enemy, instruction, 1);
        stateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::LifeCallbackThreshold(enemy, stateIndex) =
            stateInt;
        stateCallback = Th09EclRunControl::ReadInt(enemy, instruction, 2);
        stateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::LifeCallbackSubId(enemy, stateIndex) =
            stateCallback;
        break;

    case TH09_ECL_OPCODE_SET_TIMER_CALLBACK:
        enemy->timerCallbackThreshold33D0 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        stateCallback = Th09EclRunControl::ReadInt(enemy, instruction, 1);
        goto th09_ecl_install_timer_callback;

    case TH09_ECL_OPCODE_SET_CHILD_ECL:
        stateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunState::ChildSlotRef(enemy, stateIndex))
        {
            g_ZunMemory.Free(
                Th09EclRunState::ChildSlotRef(enemy, stateIndex));
        }
        Th09EclRunState::ChildSlotRef(enemy, stateIndex) = 0;
        stateInt = Th09EclRunControl::ReadInt(enemy, instruction, 1);
        if (stateInt < 0)
            break;
        childEcl = static_cast<Th09EclRunState::ChildEclBlock *>(
            g_ZunMemory.Alloc(sizeof(*childEcl), "ECLInt"));
        Th09EclRunState::ChildSlotRef(enemy, stateIndex) =
            childEcl;
        if (!childEcl)
            break;
        memset(childEcl, 0, sizeof(*childEcl));
        reinterpret_cast<Th09EclRunState::ChildEclBlock *>(
            Th09EclRunState::ChildSlotRef(enemy, stateIndex))->subroutineId00 =
                    Th09EclRunControl::ReadInt(enemy, instruction, 1);
        enemy->manager00->primaryEclManager000.InitializeSubroutine(
            &reinterpret_cast<Th09EclRunState::ChildEclBlock *>(
                Th09EclRunState::ChildSlotRef(enemy, stateIndex))->
                    context08,
            static_cast<short>(
                reinterpret_cast<Th09EclRunState::ChildEclBlock *>(
                    Th09EclRunState::ChildSlotRef(enemy, stateIndex))->subroutineId00));
        memcpy(
            reinterpret_cast<unsigned char *>(
                &reinterpret_cast<Th09EclRunState::ChildEclBlock *>(
                    Th09EclRunState::ChildSlotRef(enemy, stateIndex))->context08) +
                0x1C,
            reinterpret_cast<unsigned char *>(
                enemy->activeContext2CE0) + 0x1C,
            0x78);
        break;

    case TH09_ECL_OPCODE_SPAWN_EFFECT:
        enemy->manager00->sideState320->
            effectManager0C->SpawnEffect(
                Th09EclRunControl::ReadInt(enemy, instruction, 0),
                reinterpret_cast<const EffectFloat3 *>(
                    &enemy->position2D74),
                Th09EclRunControl::ReadInt(enemy, instruction, 1),
                static_cast<unsigned int>(
                    *Th09EclRunControl::WriteInt(enemy, instruction, 2)));
        break;

    case TH09_ECL_OPCODE_SPAWN_EFFECT_WITH_VELOCITY:
    {
        EffectFloat3 effectVelocity;
        effectVelocity.x = Th09EclRunControl::ReadFloat(enemy, instruction, 3);
        effectVelocity.y = Th09EclRunControl::ReadFloat(enemy, instruction, 4);
        effectVelocity.z = Th09EclRunControl::ReadFloat(enemy, instruction, 5);
        enemy->manager00->sideState320->
            effectManager0C->SpawnEffectWithVelocity(
                Th09EclRunControl::ReadInt(enemy, instruction, 0),
                reinterpret_cast<const EffectFloat3 *>(
                    &enemy->position2D74),
                &effectVelocity,
                Th09EclRunControl::ReadInt(enemy, instruction, 1),
                static_cast<unsigned int>(
                    *Th09EclRunControl::WriteInt(enemy, instruction, 2)));
        break;
    }

    case TH09_ECL_OPCODE_SET_ITEM_DROP_TYPE:
        enemy->itemDropType335C =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_SET_ITEM_DROP_COUNTS:
        enemy->itemDropCount3360 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        enemy->itemDropExtraCount3364 =
            Th09EclRunControl::ReadInt(enemy, instruction, 1);
        break;

    case TH09_ECL_OPCODE_SET_ANM_ROTATION_ENABLED:
        enemy->primaryFlagBits337C.anmRotation22 =
            Th09EclRunState::RawByte(instruction, 0);
        break;

    case TH09_ECL_OPCODE_CALL_EX_INSTRUCTION:
        g_EnemyEclExInstructions[
            Th09EclRunControl::ReadInt(enemy, instruction, 0)](
                enemy,
                reinterpret_cast<EnemyEclExInstructionView *>(instruction));
        break;

    case TH09_ECL_OPCODE_SET_REPEATING_EX_INSTRUCTION:
        stateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (stateIndex >= 0)
        {
            stateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
            reinterpret_cast<EnemyEclCallbackContextView *>(
                enemy->activeContext2CE0)->callback014 =
                    g_EnemyEclExInstructions[stateIndex];
            reinterpret_cast<EnemyEclCallbackContextView *>(
                enemy->activeContext2CE0)->instruction018 =
                    reinterpret_cast<EnemyEclExInstructionView *>(instruction);
        }
        else
        {
            reinterpret_cast<EnemyEclCallbackContextView *>(
                enemy->activeContext2CE0)->callback014 = 0;
        }
        break;

    case TH09_ECL_OPCODE_ADD_TIME:
        enemy->activeContext2CE0->time008 +=
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_SET_BACKGROUND_SCRIPT_LABEL:
        Th09EclRunState::BackgroundScriptLabel(enemy) =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_ADD_GAME_MANAGER_VALUE380:
        // Retail uses ADD DWORD [GameManager+0x380], 0x708: retain its
        // modulo-32 result even when the signed counter crosses INT_MAX.
        g_GameManager.value380 = static_cast<int>(
            static_cast<unsigned int>(g_GameManager.value380) + 1800u);
        break;

    {
#define TH09_ECL_RUN_REMOTE_SPAWN_BODY
#include "EclRunRemote.inl"
#undef TH09_ECL_RUN_REMOTE_SPAWN_BODY
    }

    case TH09_ECL_OPCODE_INTERRUPT_MAIN_ANM:
        enemy->primaryAnmVm0008.pendingInterrupt =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 0));
        break;

    case TH09_ECL_OPCODE_INTERRUPT_SECONDARY_ANM:
        stateIndex = Th09EclRunControl::RawInt(instruction, 0);
        Th09EclRunState::SecondaryVm(enemy, stateIndex).
            pendingInterrupt = Th09EclRunState::RawShort(instruction, 4);
        break;

    {
#define TH09_ECL_RUN_BULLET_TRANSITION_BODY
#include "EclRunBullet.inl"
#include "EnemyEclExInstructions.hpp"
#undef TH09_ECL_RUN_BULLET_TRANSITION_BODY
    }

    case TH09_ECL_OPCODE_SET_CALL_STACK_DISABLED:
        enemy->primaryFlagBits337C.callStackDisabled23 =
            Th09EclRunState::RawByte(instruction, 0);
        break;

    case TH09_ECL_OPCODE_SET_BULLET_RANK_INFLUENCE:
        enemy->bulletRankSpeedLow2E38 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        enemy->bulletRankSpeedHigh2E3C =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        enemy->bulletRankCount1Low2E40 =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 2));
        enemy->bulletRankCount1High2E42 =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 3));
        enemy->bulletRankCount2Low2E44 =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 4));
        enemy->bulletRankCount2High2E46 =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 5));
        break;

    case TH09_ECL_OPCODE_RESET_BOSS_TIMER_CALLBACK:
        stateCallback =
            enemy->deathCallbackSubId2D2E;
    th09_ecl_install_timer_callback:
        enemy->timerCallbackSubId33D4 = stateCallback;
        enemy->bossTimer2E64 = 0;
        break;

    case TH09_ECL_OPCODE_SET_TIMEOUT_SPELL:
    {
        // The timeout bit is the native Enemy's primary word at +0x337C.
        // Read exactly byte zero of the four-byte wire argument. The masked
        // XOR changes bit 24 without disturbing the other primary flags.
        unsigned char value;
        memcpy(&value, &Th09EclRunState::RawByte(instruction, 0), sizeof(value));
        enemy->primaryFlags337C =
            Th09EclRunState::AssignTimeoutSpellBit(enemy->primaryFlags337C, value);
        break;
    }

    case TH09_ECL_OPCODE_SET_SPECIAL_INTERACTION:
    {
        enemy->primaryFlagBits337C.specialInteraction07 =
            Th09EclRunState::RawByte(instruction, 0);
        enemy->drawGroup3387 = 2;
        break;
    }

    case TH09_ECL_OPCODE_SET_TRAIL:
    {
        Th09EclRunState::TrailInstructionArgs *trailArgs =
            reinterpret_cast<Th09EclRunState::TrailInstructionArgs *>(
                reinterpret_cast<unsigned int>(instruction) +
                static_cast<unsigned int>(offsetof(Th09EclRawInstructionHeaderView, arguments)));

        enemy->trailFlags53A0 = trailArgs->flags00;

        int historyLength =
            (instruction->parameterMask0A & (1U << 1))
                ? Th09EclRunControl::ResolveInt(enemy, trailArgs->historyLength04)
                : trailArgs->historyLength04;
        enemy->trailHistoryLength53A2 =
            static_cast<short>(historyLength);

        int collisionLength =
            (instruction->parameterMask0A & (1U << 2))
                ? Th09EclRunControl::ResolveInt(enemy, trailArgs->collisionLength08)
                : trailArgs->collisionLength08;
        enemy->trailCollisionLength53A4 =
            static_cast<short>(collisionLength);

        int sampleStride =
            (instruction->parameterMask0A & (1U << 3))
                ? Th09EclRunControl::ResolveInt(enemy, trailArgs->sampleStride0C)
                : trailArgs->sampleStride0C;
        enemy->trailSampleStride53A6 =
            static_cast<short>(sampleStride);

        if ((enemy->trailFlags53A0 &
             Th09EclRunState::TRAIL_BUILD_GEOMETRY) != 0)
        {
            g_AnmManager->InitializeHorizontalTextureStrip(
                &enemy->primaryAnmVm0008,
                reinterpret_cast<VertexTex1DiffuseXyzrhw *>(
                    enemy->trailVertices3E68),
                2 * (enemy->trailHistoryLength53A2 /
                     static_cast<short>(sampleStride)));
        }
        break;
    }

#endif // TH09_ECL_RUN_STATE_BODY
