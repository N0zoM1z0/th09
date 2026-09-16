#include "EclRunControl.inl"
#include "AsciiGameManagerView.hpp"

#include <stddef.h>

namespace
{

struct EclOperandContextView
{
    unsigned char unknown000[0x1C];
    int intVariables1C[8];
    float floatVariables3C[8];
    int intVariables5C[4];
    float floatVariables6C[2];
    int intVariables74[4];
    float floatVariables84[4];
};
typedef char EclOperandContextInt1C[
    (offsetof(EclOperandContextView, intVariables1C) == 0x1C) ? 1 : -1];
typedef char EclOperandContextFloat3C[
    (offsetof(EclOperandContextView, floatVariables3C) == 0x3C) ? 1 : -1];
typedef char EclOperandContextInt5C[
    (offsetof(EclOperandContextView, intVariables5C) == 0x5C) ? 1 : -1];
typedef char EclOperandContextFloat6C[
    (offsetof(EclOperandContextView, floatVariables6C) == 0x6C) ? 1 : -1];
typedef char EclOperandContextInt74[
    (offsetof(EclOperandContextView, intVariables74) == 0x74) ? 1 : -1];
typedef char EclOperandContextFloat84[
    (offsetof(EclOperandContextView, floatVariables84) == 0x84) ? 1 : -1];

struct EclOperandFloat3View
{
    float x;
    float y;
    float z;

    EclOperandFloat3View operator-(const EclOperandFloat3View &other) const;
    float Length() const;
};
typedef char EclOperandFloat3SizeIs0C[
    (sizeof(EclOperandFloat3View) == 0x0C) ? 1 : -1];

struct EclOperandPlayerView
{
    unsigned char unknown0000[0x00A0];
    int operandValueA0;
    int operandValueA4;
    unsigned char unknown00A8[0x1B88 - 0x00A8];
    EclOperandFloat3View position1B88;

    float AngleToPoint(EclOperandFloat3View *point);
};
typedef char EclOperandPlayerValueA0[
    (offsetof(EclOperandPlayerView, operandValueA0) == 0x00A0) ? 1 : -1];
typedef char EclOperandPlayerPositionAt1B88[
    (offsetof(EclOperandPlayerView, position1B88) == 0x1B88) ? 1 : -1];

struct EclOperandManagerView
{
    unsigned char unknown000[0x0168];
    int sharedInts168[4];
    float sharedFloats178[4];
    unsigned char unknown188[0x0320 - 0x0188];
    EnemySideStateView *sideState320;
    EnemySideStateView *opposingSideState324;
};
typedef char EclOperandManagerSharedIntsAt168[
    (offsetof(EclOperandManagerView, sharedInts168) == 0x0168) ? 1 : -1];
typedef char EclOperandManagerSharedFloatsAt178[
    (offsetof(EclOperandManagerView, sharedFloats178) == 0x0178) ? 1 : -1];
typedef char EclOperandManagerSideAt320[
    (offsetof(EclOperandManagerView, sideState320) == 0x0320) ? 1 : -1];

struct EclOperandEnemyView
{
    EclOperandManagerView *manager0000;
    unsigned char unknown0004[0x2CE0 - 0x0004];
    EclOperandContextView *activeContext2CE0;
    void *activeCallStack2CE4;
    int intVariables2CE8[8];
    float floatVariables2D08[8];
    unsigned char unknown2D28[0x2D74 - 0x2D28];
    EclOperandFloat3View position2D74;
    unsigned char unknown2D80[0x2DB0 - 0x2D80];
    float floatVariables2DB0[3];
    unsigned char unknown2DBC[0x2DD4 - 0x2DBC];
    EclOperandFloat3View worldPosition2DD4;
    float movementAngle2DE0;
    float angularVelocity2DE4;
    float orbitAngle2DE8;
    float orbitAngularVelocity2DEC;
    unsigned char unknown2DF0[0x2DF4 - 0x2DF0];
    float speed2DF4;
    float acceleration2DF8;
    float orbitRadius2DFC;
    unsigned char unknown2E00[0x2E10 - 0x2E00];
    float floatVariables2E10[3];
    EclOperandFloat3View movementOrigin2E1C;
    unsigned char unknown2E28[0x2E48 - 0x2E28];
    int life2E48;
    unsigned char unknown2E4C[0x2E54 - 0x2E4C];
    int operandValue2E54;
    unsigned char unknown2E58[0x2E6C - 0x2E58];
    int bossTimerCurrent2E6C;
    unsigned char unknown2E70[0x335C - 0x2E70];
    int itemDropType335C;
    unsigned char unknown3360[0x336B - 0x3360];
    unsigned char bossSlot336B;
    unsigned char unknown336C[0x3380 - 0x336C];
    unsigned int secondaryFlags3380;
    unsigned char unknown3384[0x33AC - 0x3384];
    int lastDamage33AC;
    int lifeCallbackThresholds33B0[4];
};
typedef char EclOperandEnemyActiveContextAt2CE0[
    (offsetof(EclOperandEnemyView, activeContext2CE0) == 0x2CE0) ? 1 : -1];
typedef char EclOperandEnemyIntsAt2CE8[
    (offsetof(EclOperandEnemyView, intVariables2CE8) == 0x2CE8) ? 1 : -1];
typedef char EclOperandEnemyFloatsAt2D08[
    (offsetof(EclOperandEnemyView, floatVariables2D08) == 0x2D08) ? 1 : -1];
typedef char EclOperandEnemyPositionAt2D74[
    (offsetof(EclOperandEnemyView, position2D74) == 0x2D74) ? 1 : -1];
typedef char EclOperandEnemyWorldAt2DD4[
    (offsetof(EclOperandEnemyView, worldPosition2DD4) == 0x2DD4) ? 1 : -1];
typedef char EclOperandEnemyMovementAt2DE0[
    (offsetof(EclOperandEnemyView, movementAngle2DE0) == 0x2DE0) ? 1 : -1];
typedef char EclOperandEnemyLifeAt2E48[
    (offsetof(EclOperandEnemyView, life2E48) == 0x2E48) ? 1 : -1];
typedef char EclOperandEnemyTimerCurrentAt2E6C[
    (offsetof(EclOperandEnemyView, bossTimerCurrent2E6C) == 0x2E6C) ? 1 : -1];
typedef char EclOperandEnemyBossSlotAt336B[
    (offsetof(EclOperandEnemyView, bossSlot336B) == 0x336B) ? 1 : -1];
typedef char EclOperandEnemyFlagsAt3380[
    (offsetof(EclOperandEnemyView, secondaryFlags3380) == 0x3380) ? 1 : -1];
typedef char EclOperandEnemyLastDamageAt33AC[
    (offsetof(EclOperandEnemyView, lastDamage33AC) == 0x33AC) ? 1 : -1];
typedef char EclOperandEnemyLifeThresholdsAt33B0[
    (offsetof(EclOperandEnemyView, lifeCallbackThresholds33B0) == 0x33B0) ? 1 : -1];

#define TH09_ECL_OPERAND_ENEMY(owner) \
    reinterpret_cast<EclOperandEnemyView *>(owner)
#define TH09_ECL_OPERAND_CONTEXT(owner) \
    (TH09_ECL_OPERAND_ENEMY(owner)->activeContext2CE0)
#define TH09_ECL_OPERAND_PLAYER(owner) \
    reinterpret_cast<EclOperandPlayerView *>( \
        TH09_ECL_OPERAND_ENEMY(owner)->manager0000->sideState320->player04)
#define TH09_ECL_OPERAND_OPPOSING_PLAYER(owner) \
    reinterpret_cast<EclOperandPlayerView *>( \
        TH09_ECL_OPERAND_ENEMY(owner)->manager0000->opposingSideState324->player04)
#define TH09_ECL_OPERAND_GAME_MANAGER_B4 \
    (*reinterpret_cast<int *>( \
        reinterpret_cast<unsigned char *>(&g_GameManager) + 0x00B4))

} // namespace

namespace Th09EclRunControl
{

int __fastcall ResolveInt(EnemyView *enemy, int rawValue)
{
    switch (rawValue)
    {
    case 0x2710: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[0];
    case 0x2711: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[1];
    case 0x2712: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[2];
    case 0x2713: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[3];
    case 0x2714: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[4];
    case 0x2715: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[5];
    case 0x2716: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[6];
    case 0x2717: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[7];

    case 0x2718: return TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[0];
    case 0x2719: return TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[1];
    case 0x271A: return TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[2];
    case 0x271B: return TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[3];
    case 0x271C: return TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[4];
    case 0x271D: return TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[5];
    case 0x271E: return TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[6];
    case 0x271F: return TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[7];

    case 0x2745: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables74[0];
    case 0x2746: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables74[1];
    case 0x2747: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables74[2];
    case 0x2748: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables74[3];

    case 0x2734: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables5C[0];
    case 0x2735: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables5C[1];
    case 0x2736: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables5C[2];
    case 0x2737: return TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables5C[3];

    case 0x2730: return static_cast<int>(g_Rng.GetRandomU32() & 0x7FFFFFFF);
    case 0x2731: return static_cast<int>(g_Rng.GetRandomF32());
    case 0x2732: return static_cast<int>(g_Rng.GetRandomU32());
    case 0x2733: return static_cast<int>(g_Rng.GetRandomF32Signed());

    case 0x2738: return g_GameManager.difficulty;
    case 0x2739: return TH09_ECL_OPERAND_GAME_MANAGER_B4;
    case 0x2741: return TH09_ECL_OPERAND_ENEMY(enemy)->bossTimerCurrent2E6C;
    case 0x2743: return TH09_ECL_OPERAND_ENEMY(enemy)->life2E48;
    case 0x2744: return TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sideState320->characterIndex20;

    case 0x276E: return static_cast<int>(TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables6C[0]);
    case 0x276F: return static_cast<int>(TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables6C[1]);

    case 0x2720: return static_cast<int>(TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[0]);
    case 0x2721: return static_cast<int>(TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[1]);
    case 0x2722: return static_cast<int>(TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[2]);
    case 0x2723: return static_cast<int>(TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[3]);
    case 0x2724: return static_cast<int>(TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[4]);
    case 0x2725: return static_cast<int>(TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[5]);
    case 0x2726: return static_cast<int>(TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[6]);
    case 0x2727: return static_cast<int>(TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[7]);

    case 0x2728: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[0]);
    case 0x2729: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[1]);
    case 0x272A: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[2]);
    case 0x272B: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[3]);
    case 0x272C: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[4]);
    case 0x272D: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[5]);
    case 0x272E: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[6]);
    case 0x272F: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[7]);

    case 0x2749: return static_cast<int>(TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables84[0]);
    case 0x274A: return static_cast<int>(TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables84[1]);
    case 0x274B: return static_cast<int>(TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables84[2]);
    case 0x274C: return static_cast<int>(TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables84[3]);

    case 0x274D: return TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedInts168[0];
    case 0x274E: return TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedInts168[1];
    case 0x274F: return TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedInts168[2];
    case 0x2750: return TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedInts168[3];
    case 0x2751: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedFloats178[0]);
    case 0x2752: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedFloats178[1]);
    case 0x2753: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedFloats178[2]);
    case 0x2754: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedFloats178[3]);

    case 0x273A: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->worldPosition2DD4.x);
    case 0x273B: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->worldPosition2DD4.y);
    case 0x273C: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->worldPosition2DD4.z);
    case 0x273D: return static_cast<int>(TH09_ECL_OPERAND_PLAYER(enemy)->position1B88.x);
    case 0x273E: return static_cast<int>(TH09_ECL_OPERAND_PLAYER(enemy)->position1B88.y);
    case 0x273F: return static_cast<int>(TH09_ECL_OPERAND_PLAYER(enemy)->position1B88.z);

    case 0x275A: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->movementOrigin2E1C.x);
    case 0x275B: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->movementOrigin2E1C.y);
    case 0x275C: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->movementOrigin2E1C.z);

    case 0x2765: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2DB0[0]);
    case 0x2766: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2DB0[1]);
    case 0x2767: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2DB0[2]);

    case 0x2768: return TH09_ECL_OPERAND_ENEMY(enemy)->lifeCallbackThresholds33B0[0];
    case 0x2769: return TH09_ECL_OPERAND_ENEMY(enemy)->lifeCallbackThresholds33B0[1];
    case 0x276A: return TH09_ECL_OPERAND_ENEMY(enemy)->lifeCallbackThresholds33B0[2];
    case 0x276B: return TH09_ECL_OPERAND_ENEMY(enemy)->lifeCallbackThresholds33B0[3];

    case 0x2755: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->movementAngle2DE0);
    case 0x2756: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->angularVelocity2DE4);
    case 0x2757: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->speed2DF4);
    case 0x2758: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->acceleration2DF8);
    case 0x2759: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->orbitRadius2DFC);
    case 0x275D: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->orbitAngle2DE8);
    case 0x275E: return static_cast<int>(TH09_ECL_OPERAND_ENEMY(enemy)->orbitAngularVelocity2DEC);

    case 0x2763: return TH09_ECL_OPERAND_ENEMY(enemy)->lastDamage33AC;
    case 0x2764: return TH09_ECL_OPERAND_ENEMY(enemy)->bossSlot336B;
    case 0x276C: return TH09_ECL_OPERAND_ENEMY(enemy)->itemDropType335C;
    case 0x276D: return TH09_ECL_OPERAND_ENEMY(enemy)->operandValue2E54;

    case 0x2775:
        return (TH09_ECL_OPERAND_ENEMY(enemy)->secondaryFlags3380 & 0x0C00) < 0x0C00
                   ? TH09_ECL_OPERAND_OPPOSING_PLAYER(enemy)->operandValueA0
                   : TH09_ECL_OPERAND_OPPOSING_PLAYER(enemy)->operandValueA4;

    case 0x2740:
        return static_cast<int>(TH09_ECL_OPERAND_PLAYER(enemy)->AngleToPoint(&TH09_ECL_OPERAND_ENEMY(enemy)->worldPosition2DD4));

    case 0x2742:
    {
        EclOperandFloat3View delta = TH09_ECL_OPERAND_PLAYER(enemy)->position1B88 - TH09_ECL_OPERAND_ENEMY(enemy)->worldPosition2DD4;
        return static_cast<int>(delta.Length());
    }

    default: return rawValue;
    }
}

int *__fastcall ResolveIntLValue(
    EnemyView *enemy,
    int *rawValue,
    unsigned short parameterMask,
    int operandIndex)
{
    if (operandIndex >= 0 && !(parameterMask & (1U << operandIndex)))
        return rawValue;

    switch (*rawValue)
    {
    case 0x2710: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[0];
    case 0x2711: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[1];
    case 0x2712: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[2];
    case 0x2713: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[3];
    case 0x2714: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[4];
    case 0x2715: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[5];
    case 0x2716: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[6];
    case 0x2717: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables1C[7];

    case 0x2718: return &TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[0];
    case 0x2719: return &TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[1];
    case 0x271A: return &TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[2];
    case 0x271B: return &TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[3];
    case 0x271C: return &TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[4];
    case 0x271D: return &TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[5];
    case 0x271E: return &TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[6];
    case 0x271F: return &TH09_ECL_OPERAND_ENEMY(enemy)->intVariables2CE8[7];

    case 0x2745: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables74[0];
    case 0x2746: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables74[1];
    case 0x2747: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables74[2];
    case 0x2748: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables74[3];

    case 0x2734: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables5C[0];
    case 0x2735: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables5C[1];
    case 0x2736: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables5C[2];
    case 0x2737: return &TH09_ECL_OPERAND_CONTEXT(enemy)->intVariables5C[3];

    case 0x2738: return &g_GameManager.difficulty;
    case 0x2739: return &TH09_ECL_OPERAND_GAME_MANAGER_B4;
    case 0x2741: return &TH09_ECL_OPERAND_ENEMY(enemy)->bossTimerCurrent2E6C;
    case 0x2743: return &TH09_ECL_OPERAND_ENEMY(enemy)->life2E48;
    case 0x276C: return &TH09_ECL_OPERAND_ENEMY(enemy)->itemDropType335C;
    case 0x276D: return &TH09_ECL_OPERAND_ENEMY(enemy)->operandValue2E54;

    case 0x274D: return &TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedInts168[0];
    case 0x274E: return &TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedInts168[1];
    case 0x274F: return &TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedInts168[2];
    case 0x2750: return &TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedInts168[3];

    default: return rawValue;
    }
}

float *__fastcall ResolveFloatLValue(
    EnemyView *enemy,
    float *rawValue,
    unsigned short parameterMask,
    int operandIndex)
{
    if (operandIndex >= 0 && !(parameterMask & (1U << operandIndex)))
        return rawValue;

    switch (static_cast<int>(*rawValue))
    {
    case 0x2720: return &TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[0];
    case 0x2721: return &TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[1];
    case 0x2722: return &TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[2];
    case 0x2723: return &TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[3];
    case 0x2724: return &TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[4];
    case 0x2725: return &TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[5];
    case 0x2726: return &TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[6];
    case 0x2727: return &TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables3C[7];

    case 0x2728: return &TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[0];
    case 0x2729: return &TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[1];
    case 0x272A: return &TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[2];
    case 0x272B: return &TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[3];
    case 0x272C: return &TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[4];
    case 0x272D: return &TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[5];
    case 0x272E: return &TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[6];
    case 0x272F: return &TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2D08[7];

    case 0x2749: return &TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables84[0];
    case 0x274A: return &TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables84[1];
    case 0x274B: return &TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables84[2];
    case 0x274C: return &TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables84[3];

    case 0x273A: return &TH09_ECL_OPERAND_ENEMY(enemy)->position2D74.x;
    case 0x273B: return &TH09_ECL_OPERAND_ENEMY(enemy)->position2D74.y;
    case 0x273C: return &TH09_ECL_OPERAND_ENEMY(enemy)->position2D74.z;
    case 0x273D: return &TH09_ECL_OPERAND_PLAYER(enemy)->position1B88.x;
    case 0x273E: return &TH09_ECL_OPERAND_PLAYER(enemy)->position1B88.y;
    case 0x273F: return &TH09_ECL_OPERAND_PLAYER(enemy)->position1B88.z;

    case 0x276E: return &TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables6C[0];
    case 0x276F: return &TH09_ECL_OPERAND_CONTEXT(enemy)->floatVariables6C[1];

    case 0x2751: return &TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedFloats178[0];
    case 0x2752: return &TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedFloats178[1];
    case 0x2753: return &TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedFloats178[2];
    case 0x2754: return &TH09_ECL_OPERAND_ENEMY(enemy)->manager0000->sharedFloats178[3];

    case 0x275A: return &TH09_ECL_OPERAND_ENEMY(enemy)->movementOrigin2E1C.x;
    case 0x275B: return &TH09_ECL_OPERAND_ENEMY(enemy)->movementOrigin2E1C.y;
    case 0x275C: return &TH09_ECL_OPERAND_ENEMY(enemy)->movementOrigin2E1C.z;

    case 0x275F: return &TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2E10[0];
    case 0x2760: return &TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2E10[1];
    case 0x2761: return &TH09_ECL_OPERAND_ENEMY(enemy)->floatVariables2E10[2];

    case 0x2755: return &TH09_ECL_OPERAND_ENEMY(enemy)->movementAngle2DE0;
    case 0x2756: return &TH09_ECL_OPERAND_ENEMY(enemy)->angularVelocity2DE4;
    case 0x2757: return &TH09_ECL_OPERAND_ENEMY(enemy)->speed2DF4;
    case 0x2758: return &TH09_ECL_OPERAND_ENEMY(enemy)->acceleration2DF8;
    case 0x2759: return &TH09_ECL_OPERAND_ENEMY(enemy)->orbitRadius2DFC;

    case 0x275D: return &TH09_ECL_OPERAND_ENEMY(enemy)->orbitAngle2DE8;
    case 0x275E: return &TH09_ECL_OPERAND_ENEMY(enemy)->orbitAngularVelocity2DEC;

    default: return rawValue;
    }
}

} // namespace Th09EclRunControl

float EnemyView::ResolveFloat(float rawValue)
{
    switch (static_cast<int>(rawValue))
    {
    case 0x2710: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables1C[0]);
    case 0x2711: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables1C[1]);
    case 0x2712: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables1C[2]);
    case 0x2713: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables1C[3]);
    case 0x2714: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables1C[4]);
    case 0x2715: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables1C[5]);
    case 0x2716: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables1C[6]);
    case 0x2717: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables1C[7]);

    case 0x2718: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->intVariables2CE8[0]);
    case 0x2719: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->intVariables2CE8[1]);
    case 0x271A: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->intVariables2CE8[2]);
    case 0x271B: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->intVariables2CE8[3]);
    case 0x271C: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->intVariables2CE8[4]);
    case 0x271D: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->intVariables2CE8[5]);
    case 0x271E: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->intVariables2CE8[6]);
    case 0x271F: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->intVariables2CE8[7]);

    case 0x2745: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables74[0]);
    case 0x2746: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables74[1]);
    case 0x2747: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables74[2]);
    case 0x2748: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables74[3]);

    case 0x2734: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables5C[0]);
    case 0x2735: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables5C[1]);
    case 0x2736: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables5C[2]);
    case 0x2737: return static_cast<float>(TH09_ECL_OPERAND_CONTEXT(this)->intVariables5C[3]);

    case 0x2730: return static_cast<float>(Th09EclRunControl::g_Rng.GetRandomU32() & 0x7FFFFFFF);
    case 0x2731: return Th09EclRunControl::g_Rng.GetRandomF32();
    case 0x2732: return static_cast<float>(static_cast<int>(Th09EclRunControl::g_Rng.GetRandomU32()));
    case 0x2733: return Th09EclRunControl::g_Rng.GetRandomF32Signed();
    case 0x2762: return Th09EclRunControl::g_Rng.GetRandomF32InRange(6.2831855f) - 3.1415927f;

    case 0x2738: return static_cast<float>(g_GameManager.difficulty);
    case 0x2739: return static_cast<float>(TH09_ECL_OPERAND_GAME_MANAGER_B4);
    case 0x2741: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->bossTimerCurrent2E6C);
    case 0x2743: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->life2E48);
    case 0x2744: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->manager0000->sideState320->characterIndex20);
    case 0x276C: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->itemDropType335C);
    case 0x276D: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->operandValue2E54);

    case 0x274D: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->manager0000->sharedInts168[0]);
    case 0x274E: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->manager0000->sharedInts168[1]);
    case 0x274F: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->manager0000->sharedInts168[2]);
    case 0x2750: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->manager0000->sharedInts168[3]);
    case 0x2751: return TH09_ECL_OPERAND_ENEMY(this)->manager0000->sharedFloats178[0];
    case 0x2752: return TH09_ECL_OPERAND_ENEMY(this)->manager0000->sharedFloats178[1];
    case 0x2753: return TH09_ECL_OPERAND_ENEMY(this)->manager0000->sharedFloats178[2];
    case 0x2754: return TH09_ECL_OPERAND_ENEMY(this)->manager0000->sharedFloats178[3];

    case 0x2720: return TH09_ECL_OPERAND_CONTEXT(this)->floatVariables3C[0];
    case 0x2721: return TH09_ECL_OPERAND_CONTEXT(this)->floatVariables3C[1];
    case 0x2722: return TH09_ECL_OPERAND_CONTEXT(this)->floatVariables3C[2];
    case 0x2723: return TH09_ECL_OPERAND_CONTEXT(this)->floatVariables3C[3];
    case 0x2724: return TH09_ECL_OPERAND_CONTEXT(this)->floatVariables3C[4];
    case 0x2725: return TH09_ECL_OPERAND_CONTEXT(this)->floatVariables3C[5];
    case 0x2726: return TH09_ECL_OPERAND_CONTEXT(this)->floatVariables3C[6];
    case 0x2727: return TH09_ECL_OPERAND_CONTEXT(this)->floatVariables3C[7];

    case 0x2728: return TH09_ECL_OPERAND_ENEMY(this)->floatVariables2D08[0];
    case 0x2729: return TH09_ECL_OPERAND_ENEMY(this)->floatVariables2D08[1];
    case 0x272A: return TH09_ECL_OPERAND_ENEMY(this)->floatVariables2D08[2];
    case 0x272B: return TH09_ECL_OPERAND_ENEMY(this)->floatVariables2D08[3];
    case 0x272C: return TH09_ECL_OPERAND_ENEMY(this)->floatVariables2D08[4];
    case 0x272D: return TH09_ECL_OPERAND_ENEMY(this)->floatVariables2D08[5];
    case 0x272E: return TH09_ECL_OPERAND_ENEMY(this)->floatVariables2D08[6];
    case 0x272F: return TH09_ECL_OPERAND_ENEMY(this)->floatVariables2D08[7];

    case 0x2749: return TH09_ECL_OPERAND_CONTEXT(this)->floatVariables84[0];
    case 0x274A: return TH09_ECL_OPERAND_CONTEXT(this)->floatVariables84[1];
    case 0x274B: return TH09_ECL_OPERAND_CONTEXT(this)->floatVariables84[2];
    case 0x274C: return TH09_ECL_OPERAND_CONTEXT(this)->floatVariables84[3];

    case 0x273A: return TH09_ECL_OPERAND_ENEMY(this)->worldPosition2DD4.x;
    case 0x273B: return TH09_ECL_OPERAND_ENEMY(this)->worldPosition2DD4.y;
    case 0x273C: return TH09_ECL_OPERAND_ENEMY(this)->worldPosition2DD4.z;
    case 0x273D: return TH09_ECL_OPERAND_PLAYER(this)->position1B88.x;
    case 0x273E: return TH09_ECL_OPERAND_PLAYER(this)->position1B88.y;
    case 0x273F: return TH09_ECL_OPERAND_PLAYER(this)->position1B88.z;

    case 0x276E: return TH09_ECL_OPERAND_CONTEXT(this)->floatVariables6C[0];
    case 0x276F: return TH09_ECL_OPERAND_CONTEXT(this)->floatVariables6C[1];

    case 0x275A: return TH09_ECL_OPERAND_ENEMY(this)->movementOrigin2E1C.x;
    case 0x275B: return TH09_ECL_OPERAND_ENEMY(this)->movementOrigin2E1C.y;
    case 0x275C: return TH09_ECL_OPERAND_ENEMY(this)->movementOrigin2E1C.z;

    case 0x275F: return TH09_ECL_OPERAND_ENEMY(this)->floatVariables2E10[0];
    case 0x2760: return TH09_ECL_OPERAND_ENEMY(this)->floatVariables2E10[1];
    case 0x2761: return TH09_ECL_OPERAND_ENEMY(this)->floatVariables2E10[2];

    case 0x2765: return TH09_ECL_OPERAND_ENEMY(this)->floatVariables2DB0[0];
    case 0x2766: return TH09_ECL_OPERAND_ENEMY(this)->floatVariables2DB0[1];
    case 0x2767: return TH09_ECL_OPERAND_ENEMY(this)->floatVariables2DB0[2];

    case 0x2768: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->lifeCallbackThresholds33B0[0]);
    case 0x2769: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->lifeCallbackThresholds33B0[1]);
    case 0x276A: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->lifeCallbackThresholds33B0[2]);
    case 0x276B: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->lifeCallbackThresholds33B0[3]);

    case 0x2740:
        return TH09_ECL_OPERAND_PLAYER(this)->AngleToPoint(&TH09_ECL_OPERAND_ENEMY(this)->worldPosition2DD4);

    case 0x2755: return TH09_ECL_OPERAND_ENEMY(this)->movementAngle2DE0;
    case 0x2756: return TH09_ECL_OPERAND_ENEMY(this)->angularVelocity2DE4;
    case 0x2757: return TH09_ECL_OPERAND_ENEMY(this)->speed2DF4;
    case 0x2758: return TH09_ECL_OPERAND_ENEMY(this)->acceleration2DF8;
    case 0x2759: return TH09_ECL_OPERAND_ENEMY(this)->orbitRadius2DFC;
    case 0x275D: return TH09_ECL_OPERAND_ENEMY(this)->orbitAngle2DE8;
    case 0x275E: return TH09_ECL_OPERAND_ENEMY(this)->orbitAngularVelocity2DEC;

    case 0x2775:
    {
        EclOperandPlayerView *opposingPlayer = TH09_ECL_OPERAND_OPPOSING_PLAYER(this);
        return (TH09_ECL_OPERAND_ENEMY(this)->secondaryFlags3380 & 0x0C00) < 0x0C00
                   ? static_cast<float>(opposingPlayer->operandValueA0)
                   : static_cast<float>(opposingPlayer->operandValueA4);
    }

    case 0x2764: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->bossSlot336B);
    case 0x2763: return static_cast<float>(TH09_ECL_OPERAND_ENEMY(this)->lastDamage33AC);

    case 0x2742:
    {
        EclOperandFloat3View delta = TH09_ECL_OPERAND_PLAYER(this)->position1B88 - TH09_ECL_OPERAND_ENEMY(this)->worldPosition2DD4;
        return delta.Length();
    }

    default: return rawValue;
    }
}

#undef TH09_ECL_OPERAND_GAME_MANAGER_B4
#undef TH09_ECL_OPERAND_OPPOSING_PLAYER
#undef TH09_ECL_OPERAND_PLAYER
#undef TH09_ECL_OPERAND_CONTEXT
#undef TH09_ECL_OPERAND_ENEMY
