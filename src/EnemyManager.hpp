#pragma once

#include "EffectManager.hpp"
#include "BulletManager.hpp"
#include "PlayerEnemyTrackingStateView.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

// Target-facing views for the TH09 enemy/ECL owner.  Names stop at behavior
// observed in th09.exe; opaque ranges deliberately do not import TH08 layout.

struct EnemyFloat3
{
    float x;
    float y;
    float z;
};
// Native 8-byte XY window shared by movement limits and hitbox dimensions.
struct EnemyFloat2
{
    float x;
    float y;
};
typedef char EnemyFloat3SizeIs0C[(sizeof(EnemyFloat3) == 0x0C) ? 1 : -1];
typedef char EnemyFloat2SizeIs08[(sizeof(EnemyFloat2) == 0x08) ? 1 : -1];

struct EnemyRewardAttackStateView
{
    unsigned char unknown00[0x38];
    int rewardLevel38;
    unsigned char unknown3C[0x08];

    void ResetForEnemyReward();
    void QueueEnemyReward(
        EnemyFloat3 *position,
        int interval,
        int spread,
        int count,
        int value);
};
typedef char EnemyRewardAttackStateSizeIs44[
    (sizeof(EnemyRewardAttackStateView) == 0x44) ? 1 : -1];

struct EnemyRewardPlayerView
{
    unsigned char unknown00000[0xA0];
    // PlayerAddedState initializes both words; the exact ECL operand
    // resolvers read the opposing Player's two native initial-state slots.
    int initialStateA0[2];
    unsigned char unknown000A8[0x364 - 0xA8];
    Effect *focusEffect364;
    unsigned char unknown00368[0x1B88 - 0x368];
    Float3 position1B88;
    unsigned char unknown1B94[0x30364 - 0x1B94];
    PlayerEnemyTrackingStateView tracking30364;
    unsigned char unknown30380[0x30388 - 0x30380];
    float accumulatedDefeatValue30388;
    unsigned char unknown3038C[0x303C8 - 0x3038C];
    ZunTimer stateTimer303C8;
    unsigned char unknown303D4[0x303F4 - 0x303D4];
    int tableValue303F4;
    unsigned char unknown303F8[0x30410 - 0x303F8];
    EnemyRewardAttackStateView rewardAttack30410;

    void AccumulateDefeatValue(float amount);
    void SpawnDefeatToken(unsigned int tokenType, EnemyFloat3 *position);
};
typedef char EnemyRewardPlayerNativeInitialStateAtA0[
    (offsetof(EnemyRewardPlayerView, initialStateA0) == 0xA0 &&
     sizeof(((EnemyRewardPlayerView *)0)->initialStateA0) == 8) ? 1 : -1];
typedef char EnemyRewardPlayerFocusAt364[
    (offsetof(EnemyRewardPlayerView, focusEffect364) == 0x364) ? 1 : -1];
typedef char EnemyRewardPlayerHomingAt3037C[
    (offsetof(EnemyRewardPlayerView, tracking30364) +
         offsetof(PlayerEnemyTrackingStateView, homingTarget18) == 0x3037C) ? 1 : -1];
typedef char EnemyRewardPlayerStateTimerAt303C8[
    (offsetof(EnemyRewardPlayerView, stateTimer303C8) == 0x303C8) ? 1 : -1];
typedef char EnemyRewardPlayerPositionAt1B88[
    (offsetof(EnemyRewardPlayerView, position1B88) == 0x1B88) ? 1 : -1];
typedef char EnemyRewardPlayerSideTableAt303F4[
    (offsetof(EnemyRewardPlayerView, tableValue303F4) == 0x303F4) ? 1 : -1];
typedef char EnemyRewardPlayerAttackAt30410[
    (offsetof(EnemyRewardPlayerView, rewardAttack30410) == 0x30410) ? 1 : -1];
typedef char EnemyRewardPlayerLevelAt30448[
    (offsetof(EnemyRewardPlayerView, rewardAttack30410) +
         offsetof(EnemyRewardAttackStateView, rewardLevel38) ==
     0x30448) ? 1 : -1];

class ChainElem;
class AnmLoaded;
class EtamaController;
struct Laser;
class Background;
struct EnemyCoreSideOwnerView;
struct EnemyManagerView;
struct EnemyView;

struct EnemySideStateView
{
    Background *background00;
    EnemyRewardPlayerView *player04;
    EtamaController *etama08;
    EffectManager *effectManager0C;
    EnemyManagerView *enemyManager10;
    EnemyCoreSideOwnerView *owner14;
    unsigned char unknown18[0x08];
    int characterIndex20;
    unsigned char unknown24[0x10];
    unsigned int flags34;
};
typedef char EnemySideStateSizeIs38[
    (sizeof(EnemySideStateView) == 0x38) ? 1 : -1];
typedef char EnemySideStateEtamaAt08[
    (offsetof(EnemySideStateView, etama08) == 0x08) ? 1 : -1];
typedef char EnemySideStateBackgroundAt00[
    (offsetof(EnemySideStateView, background00) == 0x00) ? 1 : -1];
typedef char EnemySideStateOwnerAt14[
    (offsetof(EnemySideStateView, owner14) == 0x14) ? 1 : -1];
typedef char EnemySideStateFlagsAt34[
    (offsetof(EnemySideStateView, flags34) == 0x34) ? 1 : -1];
typedef char EnemySideStateCharacterAt20[
    (offsetof(EnemySideStateView, characterIndex20) == 0x20) ? 1 : -1];

struct EnemyEclTimelineStateView
{
    // These words are shared by exact ECL operand resolvers and the
    // 0x20-byte RunEcl subroutine variable publication at +0x168.
    unsigned char storage000[0x160];
    int sharedInts160[4];
    float sharedFloats170[4];

    EnemyEclTimelineStateView();
};
typedef char EnemyEclTimelineNativeSharedStorage[
    (offsetof(EnemyEclTimelineStateView, sharedInts160) == 0x160 &&
     offsetof(EnemyEclTimelineStateView, sharedFloats170) == 0x170) ? 1 : -1];
typedef char EnemyEclTimelineStateViewSizeIs180[
    (sizeof(EnemyEclTimelineStateView) == 0x180) ? 1 : -1];

struct Th09EclContextView;

struct EnemyEclManagerView
{
    void *rawFile00;
    void *subroutineTable04;
    EnemyEclTimelineStateView timelineState08;

    EnemyEclManagerView();
    int Load(char *path);
    int GetSubroutineCount();
    void *GetSubroutine(int index);
    int RunEcl(EnemyView *enemy);
    int InitializeSubroutine(Th09EclContextView *context, short subroutineId);
    void ReleaseRawFile();
};
typedef char EnemyEclManagerViewSizeIs188[
    (sizeof(EnemyEclManagerView) == 0x188) ? 1 : -1];
typedef char EnemyEclManagerNativeSharedAt168[
    (offsetof(EnemyEclManagerView, timelineState08) +
         offsetof(EnemyEclTimelineStateView, sharedInts160) == 0x168 &&
     offsetof(EnemyEclManagerView, timelineState08) +
         offsetof(EnemyEclTimelineStateView, sharedFloats170) == 0x178) ? 1 : -1];
typedef char EnemyEclManagerTimelineAt08[
    (offsetof(EnemyEclManagerView, timelineState08) == 0x08) ? 1 : -1];

struct EnemyScheduleRuntimeView
{
    ZunTimer timer00;
    ZunTimer timer0C;

    EnemyScheduleRuntimeView();
    int Run();
};
typedef char EnemyScheduleRuntimeViewSizeIs18[
    (sizeof(EnemyScheduleRuntimeView) == 0x18) ? 1 : -1];
typedef char EnemyScheduleRuntimeTimer0C[
    (offsetof(EnemyScheduleRuntimeView, timer0C) == 0x0C) ? 1 : -1];

struct EnemyView
{
    EnemyManagerView *manager00;
    // The original Enemy/ECL activation pointer lives inside the common
    // Enemy allocation, not in a separate late-opcode-only byte view.
    unsigned char unknown0004[0x2CE0 - 0x0004];
    Th09EclContextView *activeContext2CE0;
    Th09EclContextView *activeCallStack2CE4;
    // The genuine per-Enemy operand banks are read and written by four
    // exact ECL resolvers at +0x2CE8 and +0x2D08.
    int intVariables2CE8[8];
    float floatVariables2D08[8];
    unsigned char unknown2D28[0x2D70 - 0x2D28];
    short pendingSubroutineId2D70;
    unsigned char unknown2D72[0x02];
    EnemyFloat3 position2D74;
    EnemyFloat3 positionOffset2D80;
    EnemyFloat3 velocity2D8C;
    unsigned char unknown2D98[0x2DB0 - 0x2D98];
    float floatVariables2DB0[3];
    EnemyFloat2 hitboxDimensions2DBC;
    unsigned char unknown2DC4[0x2DC8 - 0x2DC4];
    EnemyFloat2 secondaryHitboxDimensions2DC8;
    unsigned char unknown2DD0[0x2DD4 - 0x2DD0];
    EnemyFloat3 worldPosition2DD4;
    float movementAngle2DE0;
    float angularVelocity2DE4;
    float orbitAngle2DE8;
    float orbitAngularVelocity2DEC;
    unsigned char unknown2DF0[0x2DF4 - 0x2DF0];
    float speed2DF4;
    float acceleration2DF8;
    float orbitRadius2DFC;
    float radialVelocity2E00;
    EnemyFloat3 shootOffset2E04;
    union
    {
        float floatVariables2E10[3];
        EnemyFloat3 movementDelta2E10;
    };
    EnemyFloat3 movementOrigin2E1C;
    unsigned char movementTimer2E28[0x0C];
    int movementDuration2E34;
    unsigned char unknown2E38[0x2E48 - 0x2E38];
    int life2E48;
    unsigned char unknown2E4C[0x2E54 - 0x2E4C];
    int operandValue2E54;
    int sequenceIndex2E58;
    unsigned char unknown2E5C[0x2E6C - 0x2E5C];
    int bossTimerCurrent2E6C;
    unsigned char unknown2E70[0x2E74 - 0x2E70];
    // Original 0x214-byte bullet and laser descriptors are contiguous with
    // the 44-byte pending instruction, interval and timer region.
    BulletSpawnDescriptor bulletDescriptor2E74;
    unsigned char pendingShotInstruction3088[0x2C];
    int shootIntervalFrames30B4;
    ZunTimer shootIntervalTimer30B8;
    BulletSpawnDescriptor laserDescriptor30C4;
    Laser *laserSlots32D8[32];
    int selectedLaserSlot3358;
    // ECL reads the signed item-drop code; reward processing writes the
    // same 32-bit field through the original unsigned token API.
    union {
        unsigned int defeatTokenType335C;
        int itemDropType335C;
    };
    unsigned char unknown3360[0x3368 - 0x3360];
    signed char deathEffectVariant3368;
    unsigned char unknown3369[0x336B - 0x3369];
    unsigned char bossSlot336B;
    unsigned char unknown336C[0x337C - 0x336C];
    union
    {
        unsigned int flags337C;
        unsigned int primaryFlags337C;
    };
    union {
        unsigned int rewardFlags3380;
        unsigned int secondaryFlags3380;
    };
    unsigned char unknown3384[0x3386 - 0x3384];
    signed char anmDirection3386;
    unsigned char drawGroup3387;
    unsigned char difficultyOverride3388;
    unsigned char unknown3389[0x338A - 0x3389];
    short primaryAnmScript0338A;
    short primaryAnmScript3338C;
    short primaryAnmScript4338E;
    short primaryAnmScript13390;
    short primaryAnmScript23392;
    short specialAnmScript3394;
    unsigned char unknown3396[0x3398 - 0x3396];
    EnemyFloat2 movementLowerBounds3398;
    EnemyFloat2 movementUpperBounds33A0;
    float minimumPlayerDistanceSquared33A8;
    int lastDamage33AC;
    int lifeCallbackThresholds33B0[4];
    unsigned char unknown33C0[0x5430 - 0x33C0];

    EnemyView();
    float ResolveFloat(float rawValue);
    void IntegrateMotion();
    void HandleDeathRewards(int hitKind);
    void ResetBulletRankInfluence();
    void ClampPositionToMovementBounds();
    void CleanupAfterDeactivation();
};
typedef char EnemyViewSizeIs5430[(sizeof(EnemyView) == 0x5430) ? 1 : -1];
typedef char EnemyViewActiveContextAt2CE0[
    (offsetof(EnemyView, activeContext2CE0) == 0x2CE0) ? 1 : -1];
typedef char EnemyViewNativeOperandBanks[
    (offsetof(EnemyView, activeCallStack2CE4) == 0x2CE4 &&
     offsetof(EnemyView, intVariables2CE8) == 0x2CE8 &&
     offsetof(EnemyView, floatVariables2D08) == 0x2D08 &&
     sizeof(((EnemyView *)0)->intVariables2CE8) == 0x20 &&
     sizeof(((EnemyView *)0)->floatVariables2D08) == 0x20) ? 1 : -1];
typedef char EnemyViewPendingSubroutineAt2D70[
    (offsetof(EnemyView, pendingSubroutineId2D70) == 0x2D70) ? 1 : -1];
typedef char EnemyViewLifeAt2E48[
    (offsetof(EnemyView, life2E48) == 0x2E48) ? 1 : -1];
typedef char EnemyViewPositionAt2D74[
    (offsetof(EnemyView, position2D74) == 0x2D74) ? 1 : -1];
typedef char EnemyViewWorldPositionAt2DD4[
    (offsetof(EnemyView, worldPosition2DD4) == 0x2DD4) ? 1 : -1];
typedef char EnemyViewNativeMovementOperands[
    (offsetof(EnemyView, floatVariables2DB0) == 0x2DB0 &&
     offsetof(EnemyView, movementAngle2DE0) == 0x2DE0 &&
     offsetof(EnemyView, speed2DF4) == 0x2DF4 &&
     offsetof(EnemyView, floatVariables2E10) == 0x2E10 &&
     offsetof(EnemyView, movementOrigin2E1C) == 0x2E1C &&
     offsetof(EnemyView, operandValue2E54) == 0x2E54 &&
     offsetof(EnemyView, bossTimerCurrent2E6C) == 0x2E6C) ? 1 : -1];
// Full movement/ANM opcode geometry now uses this same native Enemy allocation.
typedef char EnemyViewMovementGeometryAt2DBC[
    (offsetof(EnemyView, velocity2D8C) == 0x2D8C &&
     offsetof(EnemyView, hitboxDimensions2DBC) == 0x2DBC &&
     offsetof(EnemyView, secondaryHitboxDimensions2DC8) == 0x2DC8 &&
     offsetof(EnemyView, radialVelocity2E00) == 0x2E00 &&
     offsetof(EnemyView, shootOffset2E04) == 0x2E04 &&
     offsetof(EnemyView, movementDelta2E10) == 0x2E10 &&
     offsetof(EnemyView, movementTimer2E28) == 0x2E28 &&
     offsetof(EnemyView, movementDuration2E34) == 0x2E34) ? 1 : -1];
typedef char EnemyViewMovementScriptsAndBoundsAt3386[
    (offsetof(EnemyView, primaryFlags337C) == 0x337C &&
     offsetof(EnemyView, anmDirection3386) == 0x3386 &&
     offsetof(EnemyView, specialAnmScript3394) == 0x3394 &&
     offsetof(EnemyView, movementLowerBounds3398) == 0x3398 &&
     offsetof(EnemyView, movementUpperBounds33A0) == 0x33A0 &&
     offsetof(EnemyView, minimumPlayerDistanceSquared33A8) == 0x33A8) ? 1 : -1];
typedef char EnemyViewBulletAndLaserDescriptors[
    (offsetof(EnemyView, bulletDescriptor2E74) == 0x2E74 &&
     offsetof(EnemyView, pendingShotInstruction3088) == 0x3088 &&
     offsetof(EnemyView, shootIntervalFrames30B4) == 0x30B4 &&
     offsetof(EnemyView, shootIntervalTimer30B8) == 0x30B8 &&
     offsetof(EnemyView, laserDescriptor30C4) == 0x30C4 &&
     sizeof(((EnemyView *)0)->bulletDescriptor2E74) == 0x214 &&
     sizeof(((EnemyView *)0)->laserDescriptor30C4) == 0x214) ? 1 : -1];
typedef char EnemyViewLaserSlotsAt32D8[
    (offsetof(EnemyView, laserSlots32D8) == 0x32D8 &&
     sizeof(((EnemyView *)0)->laserSlots32D8) == 0x80) ? 1 : -1];
typedef char EnemyViewSelectedLaserAt3358[
    (offsetof(EnemyView, selectedLaserSlot3358) == 0x3358) ? 1 : -1];
typedef char EnemyViewNativeEclRewardAndDamageFields[
    (offsetof(EnemyView, itemDropType335C) == 0x335C &&
     offsetof(EnemyView, bossSlot336B) == 0x336B &&
     offsetof(EnemyView, secondaryFlags3380) == 0x3380 &&
     offsetof(EnemyView, lastDamage33AC) == 0x33AC &&
     offsetof(EnemyView, lifeCallbackThresholds33B0) == 0x33B0) ? 1 : -1];
typedef char EnemyViewRewardFlagsAt3380[
    (offsetof(EnemyView, rewardFlags3380) == 0x3380) ? 1 : -1];

struct EnemyManagerView
{
    EnemyEclManagerView primaryEclManager000;
    EnemyEclManagerView opposingEclManager188;
    ChainElem *calcChain310;
    ChainElem *drawHighChain314;
    ChainElem *drawLowChain318;
    int sideIndex31C;
    EnemySideStateView *sideState320;
    EnemySideStateView *opposingSideState324;
    EnemyView spawnTemplate328;
    // Construction covers 129 records; update/spawn scan only the first 128.
    // A full spawn pool returns &enemies5758[128] (manager + 0x2A6F58).
    // The failure flag at +0x2AC42C also covers initial ECL failure, which can
    // return an earlier record. Original nominal array ownership is unproven.
    EnemyView enemies5758[129];
    EnemyView *remoteSlots2AC388[9];
    int activeEnemyCount2AC3AC;
    int normalEnemyCount2AC3B0;
    int specialEnemyCount2AC3B4;
    int rewardEnemyCount2AC3B8;
    unsigned char unknown2AC3BC[0x0C];
    ZunTimer sideTimer2AC3C8;
    unsigned char scheduleIndex2AC3D4;
    unsigned char unknown2AC3D5[0x2AC3DC - 0x2AC3D5];
    int protocolValue2AC3DC;
    EnemyScheduleRuntimeView scheduleRuntime2AC3E0;
    int unknown2AC3F8;
    EnemyManagerView *selfPointer2AC3FC;
    void *scheduledSubroutine2AC400;
    ZunTimer frameTimer2AC404;
    EnemyView *drawGroupHeads2AC410[4];
    AnmLoaded *enemyAnm2AC420;
    AnmLoaded *opposingEnemyAnm2AC424;
    unsigned char unknown2AC428[0x08];
    int timelineEventSlots2AC430[4];
    int timelineSpawnsSuppressed2AC440;
    EnemyView *priorityEnemy2AC444;
    EnemyView *firstActiveEnemy2AC448;
    int specialAttackThreshold2AC44C;

    void ResetForGameplayCleanup();

    EnemyManagerView();
    void Initialize();
    EnemyView *FindActiveEnemyBySideCategory(int sideCategory);
    static EnemyManagerView *__fastcall Create(int sideIndex);
    static int __fastcall AddedCallback(EnemyManagerView *enemyManager);
    static int __fastcall OnDrawHighPrio(EnemyManagerView *enemyManager);
    static int __fastcall OnDrawLowPrio(EnemyManagerView *enemyManager);

    EnemyView *SpawnEnemy(
        short eclSubroutineId,
        EnemyFloat3 *position,
        int life,
        signed char defeatTokenType,
        int score,
        int *initialVariables,
        int useOpposingEclManager);
    EnemyView *SpawnEnemyUsingOpposingEcl(
        int eclSubroutineId,
        EnemyFloat3 *position,
        int life,
        int defeatTokenType,
        int score,
        int *initialVariables);
    static int __fastcall OnUpdate(EnemyManagerView *enemyManager);
};
typedef char EnemyManagerViewSizeIs2AC450[
    (sizeof(EnemyManagerView) == 0x2AC450) ? 1 : -1];
typedef char EnemyManagerRemoteSlotsAt2AC388[
    (offsetof(EnemyManagerView, remoteSlots2AC388) == 0x2AC388) ? 1 : -1];
typedef char EnemyManagerPrimaryEclAt000[
    (offsetof(EnemyManagerView, primaryEclManager000) == 0x000) ? 1 : -1];
typedef char EnemyManagerOpposingEclAt188[
    (offsetof(EnemyManagerView, opposingEclManager188) == 0x188) ? 1 : -1];
typedef char EnemyManagerScheduleIndexAt2AC3D4[
    (offsetof(EnemyManagerView, scheduleIndex2AC3D4) == 0x2AC3D4) ? 1 : -1];
typedef char EnemyManagerScheduleRuntimeAt2AC3E0[
    (offsetof(EnemyManagerView, scheduleRuntime2AC3E0) == 0x2AC3E0) ? 1 : -1];
typedef char EnemyManagerScheduleTimerAt2AC3EC[
    (offsetof(EnemyManagerView, scheduleRuntime2AC3E0) +
         offsetof(EnemyScheduleRuntimeView, timer0C) ==
     0x2AC3EC) ? 1 : -1];
typedef char EnemyManagerEnemyAnmAt2AC420[
    (offsetof(EnemyManagerView, enemyAnm2AC420) == 0x2AC420) ? 1 : -1];
typedef char EnemyManagerTimelineSlotsAt2AC430[
    (offsetof(EnemyManagerView, timelineEventSlots2AC430) == 0x2AC430) ? 1 : -1];
typedef char EnemyManagerSpecialThresholdAt2AC44C[
    (offsetof(EnemyManagerView, specialAttackThreshold2AC44C) == 0x2AC44C) ? 1 : -1];
typedef char EnemyManagerCalcChainAt310[
    (offsetof(EnemyManagerView, calcChain310) == 0x310) ? 1 : -1];
typedef char EnemyManagerDrawHighChainAt314[
    (offsetof(EnemyManagerView, drawHighChain314) == 0x314) ? 1 : -1];
typedef char EnemyManagerDrawLowChainAt318[
    (offsetof(EnemyManagerView, drawLowChain318) == 0x318) ? 1 : -1];
typedef char EnemyManagerSideAt31C[
    (offsetof(EnemyManagerView, sideIndex31C) == 0x31C) ? 1 : -1];
typedef char EnemyManagerTemplateAt328[
    (offsetof(EnemyManagerView, spawnTemplate328) == 0x328) ? 1 : -1];
typedef char EnemyManagerEnemiesAt5758[
    (offsetof(EnemyManagerView, enemies5758) == 0x5758) ? 1 : -1];
typedef char EnemyManagerProtocolAt2AC3DC[
    (offsetof(EnemyManagerView, protocolValue2AC3DC) == 0x2AC3DC) ? 1 : -1];
typedef char EnemyManagerTimelineAt2AC440[
    (offsetof(EnemyManagerView, timelineSpawnsSuppressed2AC440) == 0x2AC440) ? 1 : -1];
typedef char EnemyManagerRewardCountAt2AC3B8[
    (offsetof(EnemyManagerView, rewardEnemyCount2AC3B8) == 0x2AC3B8) ? 1 : -1];
