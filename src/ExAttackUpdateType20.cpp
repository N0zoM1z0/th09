#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "BulletManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"

struct ExAttackType20Extra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 control28;
    Float3 control34;
    Float3 spawn40;
    float speed4C;
    float angle50;
    float angleStep54;
    int counter58;
};
typedef char ExAttackType20ExtraSizeIs5C[
    (sizeof(ExAttackType20Extra) == 0x5C) ? 1 : -1];

struct ExAttackType20Record
{
    int unknown00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    AnmVm *vm1C;
    Float3 position20;
    int vmCount2C;
    int unknown30;
    ExAttackType20Extra *extra34;
};

struct ExAttackType20SideView
{
    unsigned char unknown00[0x08];
    EtamaController *etama08;
    unsigned char unknown0C[0x2C];
};

struct ExAttackType20GameManagerView
{
    ExAttackType20SideView sides[2];
};

struct ExAttackType20BulletDescriptorStorage
{
    unsigned char storage[0x214];
    ExAttackType20BulletDescriptorStorage();
};
typedef char ExAttackType20BulletDescriptorStorageSizeIs214[
    (sizeof(ExAttackType20BulletDescriptorStorage) == 0x214) ? 1 : -1];

extern ExAttackType20GameManagerView g_GameManager;

int __fastcall ExAttackUpdateCallbackType20(ExAttackRecord *base)
{
    ExAttackType20Record *record =
        reinterpret_cast<ExAttackType20Record *>(base);
    ExAttackType20Extra *extra = record->extra34;

    switch (extra->state00)
    {
    case 2:
        if (record->timer10 > 20)
            return 1;
        break;

    case 1:
        if (!(record->timer10 < 30))
            return 1;

        if (record->timer10.HasTickedEvery(2))
        {
            ExAttackType20BulletDescriptorStorage storage;
            BulletSpawnDescriptor *descriptor =
                reinterpret_cast<BulletSpawnDescriptor *>(&storage);

            descriptor->position = record->position20;
            descriptor->color =
                static_cast<short>(extra->counter58 % 3);
            descriptor->speed1 = extra->speed4C;
            descriptor->bulletType = 21;
            descriptor->aimMode = 5;
            descriptor->count1 = 8;
            descriptor->count2 = 1;
            descriptor->angleStep = 0.0f;
            descriptor->speed2 = 0.0f;
            descriptor->unknown1FA = 0;
            descriptor->transformFlags = 0;
            descriptor->angle = extra->angle50;

            g_GameManager.sides[record->opponentSide04]
                .etama08->SpawnBulletPatternSecondary(descriptor);

            extra->angle50 += extra->angleStep54;
            ++extra->counter58;
            extra->speed4C += 0.2f;
            return 0;
        }
        break;

    case 0:
        ExAttackInterpolate2D(
            &record->position20.x,
            &extra->point1C.x,
            &extra->point10.x,
            &extra->control34.x,
            &extra->control28.x,
            record->timer10,
            90.0f);

        if (record->timer10 > 90)
        {
            ++extra->state00;
            record->position20 = extra->spawn40;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;
            record->vm1C->flagsWord &= ~2U;
        }
        break;
    }

    return 0;
}
