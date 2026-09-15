#pragma once

#include "AsciiManager.hpp"
#include "Chain.hpp"
#include "ZunTimer.hpp"

#include <stddef.h>

struct EtamaController;
struct BulletTypeSprites;
struct Bullet;

struct BulletTransformRawPayload
{
    float float0;
    float float1;
    int int0;
    int int1;
};

struct BulletDirectionChangePayload
{
    float angle;
    float speed;
    int intervalFrames;
    int repeatCount;
};

union BulletTransformPayload
{
    BulletTransformRawPayload raw;
    struct {
        float magnitude;
        float angle;
        int durationFrames;
        int reserved0C;
    } vectorAcceleration;
    struct {
        float speedDelta;
        float angleDelta;
        int durationFrames;
        int reserved0C;
    } polarAcceleration;
    BulletDirectionChangePayload directionChange;
    struct {
        float speed;
        float reserved04;
        int bounceLimit;
        int reserved0C;
    } boundaryBounce;
};

typedef char BulletTransformPayloadSizeIs10[(sizeof(BulletTransformPayload) == 0x10) ? 1 : -1];

struct BulletTransformRecord
{
    BulletTransformPayload payload;
    unsigned int kind;
    int allowWhileActive;
};

typedef char BulletTransformRecordSizeIs18[(sizeof(BulletTransformRecord) == 0x18) ? 1 : -1];

struct BulletSpawnDescriptor
{
    short bulletType;
    short color;
    Float3 position;
    float angle;
    float angleStep;
    float speed1;
    float speed2;
    BulletTransformRecord transforms[18];
    float laserStartOffset;
    float laserEndOffset;
    float laserStartLength;
    float laserWidth;
    int laserStartTime;
    int laserDuration;
    int laserDespawnDuration;
    int laserHitboxStartTime;
    int laserHitboxEndDelay;
    short count1;
    short count2;
    unsigned short aimMode;
    unsigned short unknown1FA;
    unsigned int transformFlags;
    int spawnSound;
    int transformSound;
    int transformStartIndex;
    unsigned char extraAttribute20C;
    unsigned char unknown20D[3];
    BulletTypeSprites *templateSprites;
};

typedef char BulletSpawnDescriptorSizeIs214[(sizeof(BulletSpawnDescriptor) == 0x214) ? 1 : -1];
typedef char BulletSpawnDescriptorTemplateAt210[(offsetof(BulletSpawnDescriptor, templateSprites) == 0x210) ? 1 : -1];

struct BulletTypeSprites
{
    AnmVm bulletVm;
    AnmVm spawnFastVm;
    AnmVm spawnNormalVm;
    AnmVm spawnSlowVm;
    AnmVm despawnVm;
    Float3 collisionSize;
    unsigned char unknownD40;
    unsigned char spriteHeightPx;
    unsigned char drawBucketIndex;
    unsigned char unknownD43;
    unsigned int unknownD44;
};

typedef char BulletTypeSpritesSizeIsD48[(sizeof(BulletTypeSprites) == 0xD48) ? 1 : -1];
typedef char BulletTypeSpritesCollisionAtD34[(offsetof(BulletTypeSprites, collisionSize) == 0xD34) ? 1 : -1];
typedef char BulletTypeSpritesExtraAtD44[(offsetof(BulletTypeSprites, unknownD44) == 0xD44) ? 1 : -1];

struct BulletExState
{
    ZunTimer timer;
    union {
        float float0;
        float accelerationMagnitude;
        float speedDelta;
        float directionChangeSpeed;
        float bounceSpeed;
    };
    union {
        float float1;
        float accelerationAngle;
        float angleDelta;
        float directionChangeAngle;
    };
    Float3 vector;
    union {
        int int0;
        int durationFrames;
        int directionChangeIntervalFrames;
        int bouncesCompleted;
    };
    union {
        int int1;
        int directionChangeRepeatCount;
        int bounceLimit;
    };
    union {
        int int2;
        int directionChangesCompleted;
    };
};

typedef char BulletExStateSizeIs2C[(sizeof(BulletExState) == 0x2C) ? 1 : -1];

struct Laser
{
    AnmVm bodyVm;
    AnmVm startCapVm;
    Float3 position;
    float angle;
    float startOffset;
    float endOffset;
    float startLength;
    float width;
    float currentWidth;
    float speed;
    int startTime;
    int hitboxStartTime;
    int duration;
    int despawnDuration;
    int hitboxEndDelay;
    int inUse;
    ZunTimer timer;
    unsigned short flags;
    short color;
    unsigned char state;
    unsigned char hideCapDuringStartup;
    unsigned char unknown59A[2];
};

typedef char LaserSizeIs59C[(sizeof(Laser) == 0x59C) ? 1 : -1];

struct Bullet
{
    EtamaController *controller;
    BulletTypeSprites sprites;
    Float3 position;
    Float3 velocity;
    Float3 unknownD64;
    float speed;
    unsigned char unknownD74[8];
    float angle;
    unsigned char unknownD80[8];
    ZunTimer stateTimer;
    ZunTimer activeTimer;
    unsigned char unknownDA0[0x10];
    int offscreenCullDelayFrames;
    unsigned int activeTransformFlags;
    unsigned int transformFlags;
    unsigned short unknownDBC;
    unsigned short state;
    unsigned short offscreenFrames;
    unsigned char spawnMarker;
    unsigned char isGrazed;
    unsigned char cancelledDuringSpawn;
    unsigned char unknownDC5;
    unsigned char unknownDC6[2];
    Bullet *nextInDrawBucket;
    int zoneTransitionCooldownFrames;
    int transformSound;
    int transformIndex;
    BulletTransformRecord transforms[18];
    BulletExState exStates[7];
    signed char collisionDisabled;
    unsigned char extraAttribute10BD;
    unsigned char unknown10BE;
    unsigned char unknown10BF;
    short bulletType;
    short color;

    void Deactivate();
    void AdvanceTransformProgram();
};

typedef char BulletSizeIs10C4[(sizeof(Bullet) == 0x10C4) ? 1 : -1];
typedef char BulletPositionAtD4C[(offsetof(Bullet, position) == 0xD4C) ? 1 : -1];
typedef char BulletVelocityAtD58[(offsetof(Bullet, velocity) == 0xD58) ? 1 : -1];
typedef char BulletSpeedAtD70[(offsetof(Bullet, speed) == 0xD70) ? 1 : -1];
typedef char BulletAngleAtD7C[(offsetof(Bullet, angle) == 0xD7C) ? 1 : -1];
typedef char BulletStateTimerAtD88[(offsetof(Bullet, stateTimer) == 0xD88) ? 1 : -1];
typedef char BulletActiveTimerAtD94[(offsetof(Bullet, activeTimer) == 0xD94) ? 1 : -1];
typedef char BulletOffscreenDelayAtDB0[(offsetof(Bullet, offscreenCullDelayFrames) == 0xDB0) ? 1 : -1];
typedef char BulletActiveFlagsAtDB4[(offsetof(Bullet, activeTransformFlags) == 0xDB4) ? 1 : -1];
typedef char BulletStateAtDBE[(offsetof(Bullet, state) == 0xDBE) ? 1 : -1];
typedef char BulletNextAtDC8[(offsetof(Bullet, nextInDrawBucket) == 0xDC8) ? 1 : -1];
typedef char BulletTransformsAtDD8[(offsetof(Bullet, transforms) == 0xDD8) ? 1 : -1];
typedef char BulletExStatesAtF88[(offsetof(Bullet, exStates) == 0xF88) ? 1 : -1];
typedef char BulletCollisionDisabledAt10BC[(offsetof(Bullet, collisionDisabled) == 0x10BC) ? 1 : -1];
typedef char BulletTypeAt10C0[(offsetof(Bullet, bulletType) == 0x10C0) ? 1 : -1];
typedef char BulletColorAt10C2[(offsetof(Bullet, color) == 0x10C2) ? 1 : -1];

struct BulletPlayerView;

struct BulletSideStateView
{
    void *unknown00;
    BulletPlayerView *player;
    unsigned char unknown08[0x2C];
    unsigned int flags;
};

typedef char BulletSideStateSizeIs38[(sizeof(BulletSideStateView) == 0x38) ? 1 : -1];

struct BulletAnmLoadedView
{
    void SetSprite(AnmVm *vm, int spriteIndex);
};

struct EtamaController
{
    BulletTypeSprites bulletTypeSprites[32];
    Bullet bullets[537];
    Laser lasers[48];
    int activePrimaryCount;
    int activeSecondaryCount;
    int activeTotalCount;
    int spawnSuppressionFrames;
    ZunTimer timer;
    int frameCounter;
    ChainElem *calcChain;
    ChainElem *drawChain;
    int sideIndex;
    BulletSideStateView *sideState;
    BulletSideStateView *opponentSideState;
    Bullet *drawBuckets[6];
    Bullet *primaryPoolStart;
    Bullet *secondaryPoolStart;
    int cancelItemType;
    BulletAnmLoadedView *bulletAnm;

    EtamaController();
    void Initialize();
    int ClearDrawBuckets();
    Bullet *SpawnSingleBullet(
        BulletSpawnDescriptor *descriptor, int index1, int index2,
        float angleToPlayer, int poolIndex);
    Bullet *SpawnBulletPatternPrimary(BulletSpawnDescriptor *descriptor);
    Bullet *SpawnBulletPatternSecondary(BulletSpawnDescriptor *descriptor);
    Laser *SpawnLaserPattern(BulletSpawnDescriptor *descriptor);
    int SelectBulletSprite(
        AnmVm *dst, const AnmVm *base, const AnmVm *sizeSource, int offset);

    static int OnUpdate(EtamaController *controller);
    static int OnDraw(EtamaController *controller);
    static int AddedCallback(EtamaController *controller);
    static EtamaController *Register(int sideIndex);
    static void Release(EtamaController *controller);
};

typedef char EtamaControllerSizeIs25E1C0[(sizeof(EtamaController) == 0x25E1C0) ? 1 : -1];
typedef char EtamaControllerPrimaryAt1A900[(offsetof(EtamaController, bullets) == 0x1A900) ? 1 : -1];
typedef char EtamaControllerLasersAt24D424[(offsetof(EtamaController, lasers) == 0x24D424) ? 1 : -1];
typedef char EtamaControllerCountsAt25E164[(offsetof(EtamaController, activePrimaryCount) == 0x25E164) ? 1 : -1];
typedef char EtamaControllerCalcAt25E184[(offsetof(EtamaController, calcChain) == 0x25E184) ? 1 : -1];
typedef char EtamaControllerSideAt25E190[(offsetof(EtamaController, sideState) == 0x25E190) ? 1 : -1];
typedef char EtamaControllerBucketsAt25E198[(offsetof(EtamaController, drawBuckets) == 0x25E198) ? 1 : -1];
typedef char EtamaControllerPrimaryStartAt25E1B0[(offsetof(EtamaController, primaryPoolStart) == 0x25E1B0) ? 1 : -1];
typedef char EtamaControllerAnmAt25E1BC[(offsetof(EtamaController, bulletAnm) == 0x25E1BC) ? 1 : -1];
