// TH09 v1.50a RunEcl boss/state/effects family (wire opcodes 124..157).
//
// This lexical switch fragment covers 32 active handlers and the two target-
// default slots 141/142.  It remains part of the unsplit 14,792-byte
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
#include "EffectManager.hpp"
#include "ZunMemory.hpp"

#include <stddef.h>
#include <string.h>

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

struct SideStateView
{
    unsigned char *background00;
    unsigned char unknown04[0x08];
    EffectManager *effectManager0C;
};
typedef char Th09EclStateSideEffectAt0C[
    (offsetof(SideStateView, effectManager0C) == 0x0C) ? 1 : -1];

struct ManagerStateView
{
    unsigned char unknown000[0x0320];
    SideStateView *sideState320;

    int InitializeSubroutine(Th09EclContextView *context, short subroutineId);
};
typedef char Th09EclStateManagerSideAt320[
    (offsetof(ManagerStateView, sideState320) == 0x0320) ? 1 : -1];

struct TrailSampleView
{
    EnemyFloat3 position00;
    EnemyFloat3 velocity0C;
    float angle18;
};
typedef char Th09EclStateTrailSampleSizeIs1C[
    (sizeof(TrailSampleView) == 0x1C) ? 1 : -1];

struct TrailVertexView
{
    float x;
    float y;
    float z;
    float rhw;
    unsigned long diffuse;
    float u;
    float v;
};
typedef char Th09EclStateTrailVertexSizeIs1C[
    (sizeof(TrailVertexView) == 0x1C) ? 1 : -1];

struct EnemyStateView
{
    ManagerStateView *manager0000;
    unsigned char unknown0004[0x0008 - 0x0004];
    AnmVm primaryVm0008;
    AnmVm secondaryVms02AC[2];
    Th09EclContextView mainContext07F4;
    Th09EclContextView mainCallStack0A20[TH09_ECL_MAIN_CALL_STACK_CAPACITY];
    Th09EclContextView *activeContext2CE0;
    Th09EclContextView *activeCallStack2CE4;
    unsigned char unknown2CE8[0x2D28 - 0x2CE8];
    short mainCallDepth2D28;
    short activeCallDepth2D2A;
    unsigned char unknown2D2C[0x02];
    short deathCallbackSubId2D2E;
    short subroutineSlots2D30[32];
    short pendingSubroutineId2D70;
    unsigned char unknown2D72[0x02];
    Float3 position2D74;
    Float3 positionOffset2D80;
    Float3 velocity2D8C;
    unsigned char unknown2D98[0x2DD4 - 0x2D98];
    Float3 worldPosition2DD4;
    float movementAngle2DE0;
    unsigned char unknown2DE4[0x2E38 - 0x2DE4];
    float bulletRankSpeedLow2E38;
    float bulletRankSpeedHigh2E3C;
    short bulletRankCount1Low2E40;
    short bulletRankCount1High2E42;
    short bulletRankCount2Low2E44;
    short bulletRankCount2High2E46;
    int life2E48;
    int maxLife2E4C;
    int phaseStartingLife2E50;
    unsigned char unknown2E54[0x2E64 - 0x2E54];
    Th09EclTimerStorageView bossTimer2E64;
    unsigned char unknown2E70[0x32D8 - 0x2E70];
    Laser *laserSlots32D8[32];
    int selectedLaserSlot3358;
    int itemDropType335C;
    int itemDropCount3360;
    int itemDropExtraCount3364;
    unsigned char deathAnmScripts3368[3];
    unsigned char bossSlot336B;
    unsigned char unknown336C[0x337C - 0x336C];
    unsigned int primaryFlags337C;
    unsigned int secondaryFlags3380;
    unsigned char unknown3384[0x03];
    unsigned char drawGroup3387;
    unsigned char difficultyOverride3388;
    unsigned char unknown3389[0x33A8 - 0x3389];
    float minimumPlayerDistanceSquared33A8;
    int lastDamage33AC;
    int lifeCallbackThresholds33B0[4];
    int lifeCallbackSubIds33C0[4];
    int timerCallbackThreshold33D0;
    int timerCallbackSubId33D4;
    void *childEclBlocks33D8[4];
    TrailSampleView trailSamples33E8[96];
    TrailVertexView trailVertices3E68[192];
    unsigned char unknown5368[0x53A0 - 0x5368];
    unsigned char trailFlags53A0;
    unsigned char unknown53A1;
    short trailHistoryLength53A2;
    short trailCollisionLength53A4;
    short trailSampleStride53A6;
    Th09EclTimerStorageView damageReductionTimer53A8;
    void *attachedEffects53B4[24];
    int attachedEffectCount5414;
    float attachedEffectDistance5418;
    unsigned char unknown541C[0x5430 - 0x541C];
};
typedef char Th09EclStateEnemySizeIs5430[
    (sizeof(EnemyStateView) == 0x5430) ? 1 : -1];
typedef char Th09EclStatePrimaryVmAt008[
    (offsetof(EnemyStateView, primaryVm0008) == 0x0008) ? 1 : -1];
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
    (offsetof(EnemyStateView, attachedEffects53B4) == 0x53B4) ? 1 : -1];

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
         offsetof(Th09EclContextView, unknown01C) == 0x24) ? 1 : -1];
typedef char Th09EclStateChildDepthAt06[
    (offsetof(ChildEclBlock, callStackDepth06) == 0x06) ? 1 : -1];
typedef char Th09EclStateChildCallStackAt234[
    (offsetof(ChildEclBlock, callStack0234) == 0x0234) ? 1 : -1];

struct BossUiView
{
    void SetBossMarkerState(int slot, int state);
    void SetBossMarkerPosition(int slot, Float3 *position);
};

struct SoundPlayerView
{
    void PlaySoundPositionedByIdx(int soundIndex, float x);
};

typedef void (__fastcall *ExInstructionCallback)(
    EnemyView *enemy,
    Th09EclRawInstructionHeaderView *instruction);

extern BossUiView g_BossUi;
extern SoundPlayerView g_SoundPlayer;
extern ExInstructionCallback g_ExInstructionCallbacks[];
extern int g_BossLifeMarkerProtocolValue;

void __fastcall ReleaseAttachedEffects(EnemyView *enemy);

inline EnemyStateView *View(EnemyView *enemy)
{
    return reinterpret_cast<EnemyStateView *>(enemy);
}

inline EnemyView **BossSlots(EnemyView *enemy)
{
    return reinterpret_cast<EnemyView **>(
        reinterpret_cast<unsigned char *>(View(enemy)->manager0000) +
        0x2AC388);
}

inline int &BackgroundScriptLabel(EnemyView *enemy)
{
    return *reinterpret_cast<int *>(
        View(enemy)->manager0000->sideState320->background00 + 0x18);
}

inline unsigned char &RawByte(
    Th09EclRawInstructionHeaderView *instruction,
    int byteOffset)
{
    return *(reinterpret_cast<unsigned char *>(instruction) +
             0x0C + byteOffset);
}

inline short &RawShort(
    Th09EclRawInstructionHeaderView *instruction,
    int byteOffset)
{
    return *reinterpret_cast<short *>(
        reinterpret_cast<unsigned char *>(instruction) +
        0x0C + byteOffset);
}

__forceinline void AssignFlagField(
    unsigned int &flags,
    unsigned int mask,
    unsigned int shiftedValue)
{
    flags = (flags & ~mask) | (shiftedValue & mask);
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
    int effectId;
    int effectCount;
    unsigned int effectColor;
    Th09EclRunState::ChildEclBlock *childEcl;
    EffectFloat3 effectVelocity;

    case TH09_ECL_OPCODE_PLAY_POSITIONED_SOUND:
        stateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::g_SoundPlayer.PlaySoundPositionedByIdx(
            stateInt,
            Th09EclRunState::View(enemy)->position2D74.x);
        break;

    case TH09_ECL_OPCODE_CALL_SUBROUTINE_SLOT:
        Th09EclRunState::View(enemy)->pendingSubroutineId2D70 =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 0));
        goto th09_ecl_enter_pending_subroutine;

    case TH09_ECL_OPCODE_SET_SUBROUTINE_SLOT:
        stateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        stateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 1);
        Th09EclRunState::View(enemy)->subroutineSlots2D30[stateIndex] =
            static_cast<short>(stateInt);
        break;

    case TH09_ECL_OPCODE_SET_BOSS:
        stateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (stateInt < 0)
        {
            stateIndex = Th09EclRunState::View(enemy)->bossSlot336B;
            Th09EclRunState::BossSlots(enemy)[stateIndex] = 0;
            Th09EclRunState::View(enemy)->primaryFlags337C &=
                ~Th09EclRunState::ENEMY_STATE_BOSS;
            Th09EclRunState::g_BossUi.SetBossMarkerState(stateIndex, 2);
            Th09EclRunState::ReleaseAttachedEffects(enemy);
            Float3 statePosition(-999.0f, -999.0f, 0.0f);
            Th09EclRunState::g_BossUi.SetBossMarkerPosition(
                stateIndex,
                &statePosition);
        }
        else
        {
            stateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
            Th09EclRunState::BossSlots(enemy)[stateIndex] = enemy;
            Th09EclRunState::View(enemy)->primaryFlags337C |=
                Th09EclRunState::ENEMY_STATE_BOSS;
            stateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
            Th09EclRunState::View(enemy)->bossSlot336B =
                static_cast<unsigned char>(stateIndex);
            stateIndex = Th09EclRunState::View(enemy)->bossSlot336B;
            Th09EclRunState::g_BossUi.SetBossMarkerState(stateIndex, 1);
            Th09EclRunState::View(enemy)->minimumPlayerDistanceSquared33A8 =
                0.0f;
        }
        break;

    case TH09_ECL_OPCODE_ATTACH_SPELL_EFFECT:
        Th09EclRunState::View(enemy)->attachedEffectDistance5418 =
            Th09EclRunControl::RawFloat(instruction, 4);
        ++Th09EclRunState::View(enemy)->attachedEffectCount5414;
        break;

    case TH09_ECL_OPCODE_SET_DEATH_MODE:
        Th09EclRunState::AssignFlagField(
            Th09EclRunState::View(enemy)->primaryFlags337C,
            Th09EclRunState::ENEMY_STATE_DEATH_MODE_MASK,
            static_cast<unsigned int>(
                Th09EclRunState::RawByte(instruction, 0)) << 17);
        break;

    case TH09_ECL_OPCODE_SET_DEATH_CALLBACK:
        Th09EclRunState::View(enemy)->deathCallbackSubId2D2E =
            Th09EclRunState::RawShort(instruction, 0);
        break;

    case TH09_ECL_OPCODE_SET_LIFE:
        stateInt = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::View(enemy)->maxLife2E4C = stateInt;
        Th09EclRunState::View(enemy)->life2E48 = stateInt;
        Th09EclRunState::View(enemy)->phaseStartingLife2E50 = stateInt;
        break;

    case TH09_ECL_OPCODE_SET_BOSS_TIMER:
        Th09EclRunState::View(enemy)->bossTimer2E64 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_SET_LIFE_CALLBACK:
        stateInt = Th09EclRunControl::ReadInt(enemy, instruction, 1);
        stateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::View(enemy)->lifeCallbackThresholds33B0[stateIndex] =
            stateInt;
        stateCallback = Th09EclRunControl::ReadInt(enemy, instruction, 2);
        stateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::View(enemy)->lifeCallbackSubIds33C0[stateIndex] =
            stateCallback;
        break;

    case TH09_ECL_OPCODE_SET_TIMER_CALLBACK:
        Th09EclRunState::View(enemy)->timerCallbackThreshold33D0 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        stateCallback = Th09EclRunControl::ReadInt(enemy, instruction, 1);
        goto th09_ecl_install_timer_callback;

    case TH09_ECL_OPCODE_SET_CHILD_ECL:
        stateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (Th09EclRunState::View(enemy)->childEclBlocks33D8[stateIndex])
        {
            g_ZunMemory.Free(
                Th09EclRunState::View(enemy)->childEclBlocks33D8[stateIndex]);
        }
        Th09EclRunState::View(enemy)->childEclBlocks33D8[stateIndex] = 0;
        stateInt = Th09EclRunControl::ReadInt(enemy, instruction, 1);
        if (stateInt < 0)
            break;
        childEcl = static_cast<Th09EclRunState::ChildEclBlock *>(
            g_ZunMemory.Alloc(sizeof(*childEcl), "ECLInt"));
        Th09EclRunState::View(enemy)->childEclBlocks33D8[stateIndex] =
            childEcl;
        if (!childEcl)
            break;
        memset(childEcl, 0, sizeof(*childEcl));
        reinterpret_cast<Th09EclRunState::ChildEclBlock *>(
            Th09EclRunState::View(enemy)->
                childEclBlocks33D8[stateIndex])->subroutineId00 =
                    Th09EclRunControl::ReadInt(enemy, instruction, 1);
        Th09EclRunState::View(enemy)->manager0000->InitializeSubroutine(
            &reinterpret_cast<Th09EclRunState::ChildEclBlock *>(
                Th09EclRunState::View(enemy)->childEclBlocks33D8[stateIndex])->
                    context08,
            static_cast<short>(
                reinterpret_cast<Th09EclRunState::ChildEclBlock *>(
                    Th09EclRunState::View(enemy)->
                        childEclBlocks33D8[stateIndex])->subroutineId00));
        memcpy(
            reinterpret_cast<unsigned char *>(
                &reinterpret_cast<Th09EclRunState::ChildEclBlock *>(
                    Th09EclRunState::View(enemy)->
                        childEclBlocks33D8[stateIndex])->context08) +
                0x1C,
            reinterpret_cast<unsigned char *>(
                Th09EclRunState::View(enemy)->activeContext2CE0) + 0x1C,
            0x78);
        break;

    case TH09_ECL_OPCODE_CALL_EX_INSTRUCTION:
        if ((instruction->parameterMask0A & 1) != 0)
        {
            stateIndex = Th09EclRunControl::ResolveInt(
                enemy,
                *reinterpret_cast<int *>(
                    reinterpret_cast<unsigned char *>(instruction) + 0x0C));
            Th09EclRunState::g_ExInstructionCallbacks[stateIndex](
                enemy,
                instruction);
        }
        else
        {
            stateIndex = *reinterpret_cast<int *>(
                reinterpret_cast<unsigned char *>(instruction) + 0x0C);
            Th09EclRunState::g_ExInstructionCallbacks[stateIndex](
                enemy,
                instruction);
        }
        break;

    case TH09_ECL_OPCODE_SET_REPEATING_EX_INSTRUCTION:
        stateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        if (stateIndex < 0)
        {
            Th09EclRunState::View(enemy)->activeContext2CE0->
                perFrameCallback014 = 0;
        }
        else
        {
            stateIndex = Th09EclRunControl::ReadInt(enemy, instruction, 0);
            Th09EclRunState::View(enemy)->activeContext2CE0->
                perFrameCallback014 =
                    Th09EclRunState::g_ExInstructionCallbacks[stateIndex];
            Th09EclRunState::View(enemy)->activeContext2CE0->
                perFrameInstruction018 = instruction;
        }
        break;

    case TH09_ECL_OPCODE_SET_DEATH_ANM_SCRIPTS:
        Th09EclRunState::View(enemy)->deathAnmScripts3368[0] =
            Th09EclRunState::RawByte(instruction, 0);
        Th09EclRunState::View(enemy)->deathAnmScripts3368[1] =
            Th09EclRunState::RawByte(instruction, 1);
        Th09EclRunState::View(enemy)->deathAnmScripts3368[2] =
            Th09EclRunState::RawByte(instruction, 2);
        break;

    case TH09_ECL_OPCODE_SPAWN_EFFECT:
        effectCount = Th09EclRunControl::ReadInt(enemy, instruction, 1);
        effectId = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        effectColor = static_cast<unsigned int>(
            *Th09EclRunControl::WriteInt(enemy, instruction, 2));
        Th09EclRunState::View(enemy)->manager0000->sideState320->
            effectManager0C->SpawnEffect(
                effectId,
                reinterpret_cast<const EffectFloat3 *>(
                    &Th09EclRunState::View(enemy)->position2D74),
                effectCount,
                effectColor);
        break;

    case TH09_ECL_OPCODE_SPAWN_EFFECT_WITH_VELOCITY:
        effectVelocity.x = Th09EclRunControl::ReadFloat(enemy, instruction, 3);
        effectVelocity.y = Th09EclRunControl::ReadFloat(enemy, instruction, 4);
        effectVelocity.z = Th09EclRunControl::ReadFloat(enemy, instruction, 5);
        effectCount = Th09EclRunControl::ReadInt(enemy, instruction, 1);
        effectId = Th09EclRunControl::ReadInt(enemy, instruction, 0);
        effectColor = static_cast<unsigned int>(
            *Th09EclRunControl::WriteInt(enemy, instruction, 2));
        Th09EclRunState::View(enemy)->manager0000->sideState320->
            effectManager0C->SpawnEffectWithVelocity(
                effectId,
                reinterpret_cast<const EffectFloat3 *>(
                    &Th09EclRunState::View(enemy)->position2D74),
                &effectVelocity,
                effectCount,
                effectColor);
        break;

    case TH09_ECL_OPCODE_UNHANDLED_8D:
    case TH09_ECL_OPCODE_UNHANDLED_8E:
        break;

    case TH09_ECL_OPCODE_SET_ITEM_DROP_TYPE:
        Th09EclRunState::View(enemy)->itemDropType335C =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_SET_ITEM_DROP_COUNTS:
        Th09EclRunState::View(enemy)->itemDropCount3360 =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        Th09EclRunState::View(enemy)->itemDropExtraCount3364 =
            Th09EclRunControl::ReadInt(enemy, instruction, 1);
        break;

    case TH09_ECL_OPCODE_SET_ANM_ROTATION_ENABLED:
        Th09EclRunState::AssignFlagField(
            Th09EclRunState::View(enemy)->primaryFlags337C,
            Th09EclRunState::ENEMY_STATE_ANM_ROTATION_ENABLED,
            static_cast<unsigned int>(
                Th09EclRunState::RawByte(instruction, 0)) << 22);
        break;

    case TH09_ECL_OPCODE_ADD_TIME:
        if ((instruction->parameterMask0A & 1U) != 0)
        {
            Th09EclRunState::View(enemy)->activeContext2CE0->time008.AddCurrent(
                Th09EclRunControl::ResolveInt(
                    enemy,
                    Th09EclRunControl::RawInt(instruction, 0)));
        }
        else
        {
            Th09EclRunState::View(enemy)->activeContext2CE0->time008.AddCurrent(
                Th09EclRunControl::RawInt(instruction, 0));
        }
        break;

    case TH09_ECL_OPCODE_SET_BACKGROUND_SCRIPT_LABEL:
        Th09EclRunState::BackgroundScriptLabel(enemy) =
            Th09EclRunControl::ReadInt(enemy, instruction, 0);
        break;

    case TH09_ECL_OPCODE_SET_BOSS_LIFE_MARKER_COUNT:
        Th09EclRunState::g_BossLifeMarkerProtocolValue += 1800;
        break;

    case TH09_ECL_OPCODE_INTERRUPT_MAIN_ANM:
        Th09EclRunState::View(enemy)->primaryVm0008.pendingInterrupt =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 0));
        break;

    case TH09_ECL_OPCODE_INTERRUPT_SECONDARY_ANM:
        stateIndex = Th09EclRunControl::RawInt(instruction, 0);
        Th09EclRunState::View(enemy)->secondaryVms02AC[stateIndex].
            pendingInterrupt = Th09EclRunState::RawShort(instruction, 4);
        break;

    case TH09_ECL_OPCODE_SET_CALL_STACK_DISABLED:
        Th09EclRunState::AssignFlagField(
            Th09EclRunState::View(enemy)->primaryFlags337C,
            Th09EclRunState::ENEMY_STATE_CALL_STACK_DISABLED,
            static_cast<unsigned int>(
                Th09EclRunState::RawByte(instruction, 0)) << 23);
        break;

    case TH09_ECL_OPCODE_SET_BULLET_RANK_INFLUENCE:
        Th09EclRunState::View(enemy)->bulletRankSpeedLow2E38 =
            Th09EclRunControl::ReadFloat(enemy, instruction, 0);
        Th09EclRunState::View(enemy)->bulletRankSpeedHigh2E3C =
            Th09EclRunControl::ReadFloat(enemy, instruction, 1);
        Th09EclRunState::View(enemy)->bulletRankCount1Low2E40 =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 2));
        Th09EclRunState::View(enemy)->bulletRankCount1High2E42 =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 3));
        Th09EclRunState::View(enemy)->bulletRankCount2Low2E44 =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 4));
        Th09EclRunState::View(enemy)->bulletRankCount2High2E46 =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 5));
        break;

    case TH09_ECL_OPCODE_RESET_BOSS_TIMER_CALLBACK:
        stateCallback =
            Th09EclRunState::View(enemy)->deathCallbackSubId2D2E;
    th09_ecl_install_timer_callback:
        Th09EclRunState::View(enemy)->timerCallbackSubId33D4 = stateCallback;
        Th09EclRunState::View(enemy)->bossTimer2E64 = 0;
        break;

    case TH09_ECL_OPCODE_CLEAR_LASER_SLOTS:
        for (int i = 0; i < 0x20; ++i)
            Th09EclRunState::View(enemy)->laserSlots32D8[i] = 0;
        break;

    case TH09_ECL_OPCODE_SET_TIMEOUT_SPELL:
        Th09EclRunState::AssignFlagField(
            Th09EclRunState::View(enemy)->primaryFlags337C,
            Th09EclRunState::ENEMY_STATE_TIMEOUT_SPELL,
            static_cast<unsigned int>(
                Th09EclRunState::RawByte(instruction, 0)) << 24);
        break;

    case TH09_ECL_OPCODE_SET_SPECIAL_INTERACTION:
        Th09EclRunState::AssignFlagField(
            Th09EclRunState::View(enemy)->primaryFlags337C,
            Th09EclRunState::ENEMY_STATE_SPECIAL_INTERACTION,
            static_cast<unsigned int>(
                Th09EclRunState::RawByte(instruction, 0)) << 7);
        Th09EclRunState::View(enemy)->drawGroup3387 = 2;
        break;

    case TH09_ECL_OPCODE_SET_TRAIL:
        Th09EclRunState::View(enemy)->trailFlags53A0 =
            Th09EclRunState::RawByte(instruction, 0);
        Th09EclRunState::View(enemy)->trailHistoryLength53A2 =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 1));
        Th09EclRunState::View(enemy)->trailCollisionLength53A4 =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 2));
        Th09EclRunState::View(enemy)->trailSampleStride53A6 =
            static_cast<short>(
                Th09EclRunControl::ReadInt(enemy, instruction, 3));
        if ((Th09EclRunState::View(enemy)->trailFlags53A0 &
             Th09EclRunState::TRAIL_BUILD_GEOMETRY) != 0)
        {
            g_AnmManager->InitializeHorizontalTextureStrip(
                &Th09EclRunState::View(enemy)->primaryVm0008,
                reinterpret_cast<VertexTex1DiffuseXyzrhw *>(
                    Th09EclRunState::View(enemy)->trailVertices3E68),
                2 * (Th09EclRunState::View(enemy)->trailHistoryLength53A2 /
                     Th09EclRunState::View(enemy)->trailSampleStride53A6));
        }
        break;

#endif // TH09_ECL_RUN_STATE_BODY
