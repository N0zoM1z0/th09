#include "BulletManager.hpp"

// Original i386 treats the signed transform index as a native DWORD
// 0x18-byte record stride; there is no hidden C++ bounds check.
BulletTransformRecord *Bullet::InstallTransformRecord(
    int index, int allowWhileActive, unsigned int kind)
{
    BulletTransformRecord *record = reinterpret_cast<BulletTransformRecord *>(
        reinterpret_cast<unsigned int>(&this->transforms[0]) +
        static_cast<unsigned int>(index) * sizeof(this->transforms[0]));
    record->kind = kind;
    record->allowWhileActive = allowWhileActive;
    this->transformFlags |= kind;
    this->transformIndex = 0;
    return record;
}

BulletTransformRecord *BulletSpawnDescriptor::InstallTransformRecord(
    int index, int allowWhileActive, unsigned int kind)
{
    BulletTransformRecord *record = reinterpret_cast<BulletTransformRecord *>(
        reinterpret_cast<unsigned int>(&this->transforms[0]) +
        static_cast<unsigned int>(index) * sizeof(this->transforms[0]));
    record->kind = kind;
    record->allowWhileActive = allowWhileActive;
    this->transformFlags |= kind;
    return record;
}
