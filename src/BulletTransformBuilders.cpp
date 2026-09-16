#include "BulletManager.hpp"

enum BulletTransformBuilderKind
{
    TRANSFORM_ACCELERATE_VECTOR = 0x10,
    TRANSFORM_ACCELERATE_POLAR = 0x20,
    TRANSFORM_SET_SPRITE = 0x4000,
    TRANSFORM_WAIT = 0x20000,
};

void Bullet::InstallVectorAccelerationTransform(
    int index, int allowWhileActive, int durationFrames,
    float magnitude, float angle)
{
    BulletTransformRecord *record =
        this->InstallTransformRecord(index, allowWhileActive, TRANSFORM_ACCELERATE_VECTOR);
    record->payload.vectorAcceleration.durationFrames = durationFrames;
    record->payload.vectorAcceleration.magnitude = magnitude;
    record->payload.vectorAcceleration.angle = angle;
}

void Bullet::InstallWaitTransform(
    int index, int allowWhileActive, int durationFrames)
{
    BulletTransformRecord *record =
        this->InstallTransformRecord(index, allowWhileActive, TRANSFORM_WAIT);
    record->payload.raw.int0 = durationFrames;
}

void Bullet::InstallSpriteTransform(
    int index, int allowWhileActive, int spriteIndex, int color)
{
    BulletTransformRecord *record =
        this->InstallTransformRecord(index, allowWhileActive, TRANSFORM_SET_SPRITE);
    record->payload.raw.int0 = spriteIndex;
    record->payload.raw.int1 = color;
}

void BulletSpawnDescriptor::InstallPolarAccelerationTransform(
    int index, int allowWhileActive, int durationFrames,
    float angleDelta, float speedDelta)
{
    BulletTransformRecord *record =
        this->InstallTransformRecord(index, allowWhileActive, TRANSFORM_ACCELERATE_POLAR);
    record->payload.polarAcceleration.durationFrames = durationFrames;
    record->payload.polarAcceleration.speedDelta = speedDelta;
    record->payload.polarAcceleration.angleDelta = angleDelta;
}

void BulletSpawnDescriptor::InstallVectorAccelerationTransform(
    int index, int allowWhileActive, int durationFrames,
    float magnitude, float angle)
{
    BulletTransformRecord *record =
        this->InstallTransformRecord(index, allowWhileActive, TRANSFORM_ACCELERATE_VECTOR);
    record->payload.vectorAcceleration.durationFrames = durationFrames;
    record->payload.vectorAcceleration.magnitude = magnitude;
    record->payload.vectorAcceleration.angle = angle;
}

void BulletSpawnDescriptor::InstallWaitTransform(
    int index, int allowWhileActive, int durationFrames)
{
    BulletTransformRecord *record =
        this->InstallTransformRecord(index, allowWhileActive, TRANSFORM_WAIT);
    record->payload.raw.int0 = durationFrames;
}
