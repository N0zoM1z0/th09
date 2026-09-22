#include "BulletManager.hpp"

#include <string.h>

enum BulletAdvanceTransformKind
{
    ADVANCE_TRANSFORM_NONE = 0,
    ADVANCE_TRANSFORM_DECELERATE = 0x00000001,
    ADVANCE_TRANSFORM_ACCELERATE_VECTOR = 0x00000010,
    ADVANCE_TRANSFORM_ACCELERATE_POLAR = 0x00000020,
    ADVANCE_TRANSFORM_CHANGE_DIRECTION_RELATIVE = 0x00000040,
    ADVANCE_TRANSFORM_CHANGE_DIRECTION_AIMED = 0x00000080,
    ADVANCE_TRANSFORM_CHANGE_DIRECTION_ABSOLUTE = 0x00000100,
    ADVANCE_TRANSFORM_BOUNCE_ALL_EDGES = 0x00000400,
    ADVANCE_TRANSFORM_BOUNCE_EXCEPT_BOTTOM = 0x00000800,
    ADVANCE_TRANSFORM_SET_CULL_DELAY = 0x00002000,
    ADVANCE_TRANSFORM_SET_SPRITE = 0x00004000,
    ADVANCE_TRANSFORM_WAIT = 0x00020000,
    ADVANCE_TRANSFORM_DESPAWN = 0x00040000,
    ADVANCE_TRANSFORM_PLAY_SOUND = 0x00080000,
    ADVANCE_TRANSFORM_WRAP_X = 0x00400000,
    ADVANCE_TRANSFORM_WRAP_Y = 0x00800000,
    ADVANCE_TRANSFORM_SPAWN_CHILD_PATTERN = 0x01000000,
};

enum BulletAdvanceState
{
    ADVANCE_BULLET_STATE_DESPAWNING = 5,
};

struct BulletAdvanceSoundPlayerView
{
    void PlaySoundByIdx(int soundIndex, int pan);
    void PlaySoundPositionedByIdx(int soundIndex, float positionX);
};
extern BulletAdvanceSoundPlayerView g_SoundPlayer;

struct BulletAdvanceSupervisorView
{
    unsigned char unknown000[0x5B8];
    float framerateMultiplier;
};
extern BulletAdvanceSupervisorView g_Supervisor;

struct BulletSpawnDescriptorAdvanceView
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

    BulletSpawnDescriptorAdvanceView();
};
typedef char BulletAdvanceDescriptorViewSizeIs214[
    (sizeof(BulletSpawnDescriptorAdvanceView) == 0x214) ? 1 : -1];

void Bullet::AdvanceTransformProgram()
{
    BulletTransformRecord *record;
    int index;

nextRecord:
    if (this->transformIndex >= 18)
        return;

    index = this->transformIndex;
    record = &this->transforms[index];
    if (record->kind == ADVANCE_TRANSFORM_NONE)
        return;

    if (record->allowWhileActive == 0 &&
        this->activeTransformFlags != 0)
    {
        return;
    }

    if ((this->transformFlags & record->kind) == 0)
    {
        this->transformIndex = index + 1;
        goto nextRecord;
    }

    switch (record->kind)
    {
    case ADVANCE_TRANSFORM_DECELERATE:
        this->activeTransformFlags |= ADVANCE_TRANSFORM_DECELERATE;
        this->exStates[0].timer = 0;
        *reinterpret_cast<int *>(&this->exStates[0].vector.z) = 0;
        break;

    case ADVANCE_TRANSFORM_ACCELERATE_VECTOR:
        this->activeTransformFlags |= ADVANCE_TRANSFORM_ACCELERATE_VECTOR;
        this->exStates[1].accelerationMagnitude =
            record->payload.vectorAcceleration.magnitude;
        this->exStates[1].accelerationAngle =
            record->payload.vectorAcceleration.angle > -990.0f
                ? record->payload.vectorAcceleration.angle
                : this->angle;
        this->exStates[1].timer = 0;
        this->exStates[1].durationFrames =
            record->payload.vectorAcceleration.durationFrames;
        this->exStates[1].vector.FromAngleMagnitude(
            this->exStates[1].accelerationAngle,
            g_Supervisor.framerateMultiplier *
                this->exStates[1].accelerationMagnitude);

        if (this->transformIndex != 0 &&
            this->transformSound >= 0)
        {
            g_SoundPlayer.PlaySoundByIdx(this->transformSound, 0);
        }
        break;

    case ADVANCE_TRANSFORM_ACCELERATE_POLAR:
        this->activeTransformFlags |= ADVANCE_TRANSFORM_ACCELERATE_POLAR;
        this->exStates[2].speedDelta =
            record->payload.polarAcceleration.speedDelta;
        this->exStates[2].angleDelta =
            record->payload.polarAcceleration.angleDelta;
        this->exStates[2].timer = 0;
        this->exStates[2].durationFrames =
            record->payload.polarAcceleration.durationFrames;

        if (this->transformIndex != 0 &&
            this->transformSound >= 0)
        {
            g_SoundPlayer.PlaySoundByIdx(this->transformSound, 0);
        }
        break;

    case ADVANCE_TRANSFORM_CHANGE_DIRECTION_RELATIVE:
    case ADVANCE_TRANSFORM_CHANGE_DIRECTION_AIMED:
    case ADVANCE_TRANSFORM_CHANGE_DIRECTION_ABSOLUTE:
        this->activeTransformFlags |= record->kind;
        this->exStates[3].directionChangeAngle =
            record->payload.directionChange.angle;
        this->exStates[3].directionChangeSpeed =
            record->payload.directionChange.speed > -999.0f
                ? record->payload.directionChange.speed
                : this->speed;
        this->exStates[3].timer = 0;
        this->exStates[3].directionChangeIntervalFrames =
            record->payload.directionChange.intervalFrames;
        this->exStates[3].directionChangeRepeatCount =
            record->payload.directionChange.repeatCount;
        this->exStates[3].directionChangesCompleted = 0;
        break;

    case ADVANCE_TRANSFORM_BOUNCE_ALL_EDGES:
    case ADVANCE_TRANSFORM_BOUNCE_EXCEPT_BOTTOM:
        this->activeTransformFlags |= record->kind;
        if (record->payload.boundaryBounce.speed >= 0.0f)
            this->exStates[4].bounceSpeed =
                record->payload.boundaryBounce.speed;
        else
            this->exStates[4].bounceSpeed = this->speed;
        this->exStates[4].bounceLimit =
            record->payload.boundaryBounce.bounceLimit;
        this->exStates[4].bouncesCompleted = 0;
        break;

    case ADVANCE_TRANSFORM_SET_CULL_DELAY:
        this->offscreenCullDelayFrames =
            record->payload.raw.int0;
        ++this->transformIndex;
        goto nextRecord;

    case ADVANCE_TRANSFORM_SET_SPRITE:
        this->sprites =
            this->controller->bulletTypeSprites[
                record->payload.raw.int0];
        this->controller->bulletAnm->SetSprite(
                &this->sprites.bulletVm,
                this->sprites.bulletVm.activeSpriteIndex +
                    record->payload.raw.int1);
        ++this->transformIndex;
        goto nextRecord;

    case ADVANCE_TRANSFORM_WAIT:
        this->activeTransformFlags |= record->kind;
        this->exStates[5].timer = record->payload.raw.int0;
        break;

    case ADVANCE_TRANSFORM_DESPAWN:
        this->state = ADVANCE_BULLET_STATE_DESPAWNING;
        break;

    case ADVANCE_TRANSFORM_PLAY_SOUND:
        g_SoundPlayer.PlaySoundPositionedByIdx(
            record->payload.raw.int0, this->position.x);
        ++this->transformIndex;
        goto nextRecord;

    case ADVANCE_TRANSFORM_WRAP_X:
        this->activeTransformFlags |= record->kind;
        this->exStates[6].timer = record->payload.raw.int0;
        break;

    case ADVANCE_TRANSFORM_WRAP_Y:
        this->activeTransformFlags |= record->kind;
        this->exStates[6].timer = record->payload.raw.int0;
        break;

    case ADVANCE_TRANSFORM_SPAWN_CHILD_PATTERN:
        {
            BulletSpawnDescriptorAdvanceView pattern;
            pattern.position = this->position;
            unsigned int fadeParent =
                static_cast<unsigned int>(record->payload.raw.int0) &
                0x80000000u;
            pattern.aimMode = static_cast<unsigned short>(
                (static_cast<unsigned int>(record->payload.raw.int0) &
                 0x7F000000u) >> 24);
            pattern.bulletType = static_cast<short>(
                reinterpret_cast<unsigned char *>(
                    &record->payload.raw.int0)[2]);
            pattern.color = static_cast<short>(
                reinterpret_cast<unsigned char *>(
                    &record->payload.raw.int0)[1]);
            pattern.transformStartIndex =
                static_cast<unsigned int>(record->payload.raw.int0) & 0xFF;
            pattern.count1 =
                static_cast<short>(record->payload.raw.int1);
            pattern.speed1 = record->payload.raw.float0;
            pattern.speed2 = record->payload.raw.float1;

            ++record;
            ++index;
            this->transformIndex = index;
            pattern.count2 =
                static_cast<short>(record->payload.raw.int0);
            pattern.transformFlags =
                record->payload.raw.int1;
            pattern.angle = record->payload.raw.float0;
            pattern.angleStep = record->payload.raw.float1;

            memcpy(
                pattern.transforms,
                this->transforms,
                sizeof(pattern.transforms));

            this->controller->SpawnBulletPatternPrimary(
                reinterpret_cast<BulletSpawnDescriptor *>(&pattern));
            ++this->transformIndex;

            if (fadeParent != 0)
            {
                this->state = ADVANCE_BULLET_STATE_DESPAWNING;
                break;
            }

            goto nextRecord;
        }

    default:
        break;
    }

    ++this->transformIndex;
}
