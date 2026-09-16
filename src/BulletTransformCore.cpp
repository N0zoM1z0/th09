#include "BulletManager.hpp"

BulletTransformRecord *Bullet::InstallTransformRecord(
    int index, int allowWhileActive, unsigned int kind)
{
    this->transforms[index].allowWhileActive = allowWhileActive;
    BulletTransformRecord *record = &this->transforms[index];
    record->kind = kind;
    this->transformFlags |= kind;
    this->transformIndex = 0;
    return record;
}

BulletTransformRecord *BulletSpawnDescriptor::InstallTransformRecord(
    int index, int allowWhileActive, unsigned int kind)
{
    BulletTransformRecord *record = &this->transforms[index];
    record->allowWhileActive = allowWhileActive;
    record->kind = kind;
    this->transformFlags |= kind;
    return record;
}
