#include "EnemyManager.hpp"

int EnemyEclManagerView::GetSubroutineCount()
{
    return *reinterpret_cast<short *>(
        reinterpret_cast<unsigned char *>(rawFile00) + 0x06);
}

void *EnemyEclManagerView::GetSubroutine(int index)
{
    return *reinterpret_cast<void **>(
        reinterpret_cast<unsigned char *>(rawFile00) + 0x08 + index * 4);
}

void EnemyView::ResetBulletRankInfluence()
{
    unsigned char *self = reinterpret_cast<unsigned char *>(this);
    *reinterpret_cast<float *>(self + 0x2E38) = -0.5f;
    *reinterpret_cast<float *>(self + 0x2E3C) = 0.5f;
    *reinterpret_cast<short *>(self + 0x2E40) = 0;
    *reinterpret_cast<short *>(self + 0x2E42) = 0;
    *reinterpret_cast<short *>(self + 0x2E44) = 0;
    *reinterpret_cast<short *>(self + 0x2E46) = 0;
}
