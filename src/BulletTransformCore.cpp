#include "BulletManager.hpp"

BulletTransformRecord *Bullet::InstallTransformRecord(
    int index, int allowWhileActive, unsigned int kind)
{
    BulletTransformRecord *record = &this->transforms[index];
    record->kind = kind;
    record->allowWhileActive = allowWhileActive;
    this->transformFlags |= kind;
    this->transformIndex = 0;
    return record;
}

BulletTransformRecord *BulletSpawnDescriptor::InstallTransformRecord(
    int index, int allowWhileActive, unsigned int kind)
{
    BulletTransformRecord *record = &this->transforms[index];
    record->kind = kind;
    record->allowWhileActive = allowWhileActive;
    this->transformFlags |= kind;
    return record;
}
