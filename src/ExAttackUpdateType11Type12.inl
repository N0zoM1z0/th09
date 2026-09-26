#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "BulletManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"
#include "ExAttackType8GameManagerView.hpp"
#include "PlayerLifecycleView.hpp"

// These callbacks share the EclManager translation unit with the maintained
// Float3::FromAngleMagnitude definition. The target keeps its receiver in ECX
// across that call; an external declaration alone produces a different body.

struct ExAttackType1112UpdateExtra
{
    int state00;
    int toggle04;
    float angle08;
    float angle0C;
    float unknown10;
    Float3 motion14;
    Float3 point20;
    Float3 point2C;
    Float3 control38;
    Float3 control44;
    Float3 spawn50;
};
typedef char ExAttackType1112UpdateExtraSizeIs5C[
    (sizeof(ExAttackType1112UpdateExtra) == 0x5C) ? 1 : -1];

struct ExAttackType1112UpdateRecord
{
    int unknown00;
    int opponentSide04;
    int side08;
    int active0C;
    ZunTimer timer10;
    void *dynamicData1C;
    Float3 position20;
    int vmCount2C;
    int unknown30;
    ExAttackType1112UpdateExtra *extra34;
    int parameter38;
    float rotation3C;
};

struct ExAttackType1112PlayerAnmView
{
    unsigned char unknown000[0xBC];
    AnmLoaded *anmFileBC;
};

struct ExAttackType1112BulletDescriptorStorage
{
    unsigned char storage[0x214];
    ExAttackType1112BulletDescriptorStorage();
};
typedef char ExAttackType1112BulletDescriptorStorageSizeIs214[
    (sizeof(ExAttackType1112BulletDescriptorStorage) == 0x214) ? 1 : -1];

float __stdcall AddNormalizeAngle(float angle, float delta);

int __fastcall ExAttackUpdateCallbackType11(ExAttackRecord *base)
{
    ExAttackType1112UpdateRecord *record =
        reinterpret_cast<ExAttackType1112UpdateRecord *>(base);
    ExAttackType1112UpdateExtra *extra = record->extra34;
    Float3 collisionSize(24.0f, 24.0f, 0.0f);

    switch (extra->state00)
    {
    case 2:
        if (record->timer10 > 20)
            return 1;
        break;

    case 1:
        if (record->timer10 > 20)
        {
            if (record->timer10.HasTickedEvery(10))
            {
                float bulletAngle = AddNormalizeAngle(
                    extra->angle0C, 0.10471976f);
                extra->angle0C = bulletAngle;

                ExAttackType1112BulletDescriptorStorage storage;
                BulletSpawnDescriptor *descriptor =
                    reinterpret_cast<BulletSpawnDescriptor *>(&storage);
                descriptor->position = record->position20;
                descriptor->bulletType = extra->toggle04 != 0 ? 6 : 0;
                descriptor->aimMode = 5;
                descriptor->count1 = 1;
                descriptor->count2 = 1;
                descriptor->color = 2;
                descriptor->angle = bulletAngle;
                descriptor->speed1 = 0.0f;

                descriptor->InstallWaitTransform(0, 0, 80);
                descriptor->InstallVectorAccelerationTransform(
                    1, 0, 120,
                    (g_GameManager.sides[1 - record->opponentSide04]
                             .player04->header24.state80 *
                         0.05f +
                     2.0f) *
                        0.0083333338f,
                    -999.0f);

                descriptor->angleStep = 0.0f;
                descriptor->speed2 = 0.0f;
                descriptor->unknown1FA = 0;
                descriptor->transformFlags = 0x00020014;
                g_GameManager.sides[record->opponentSide04]
                    .etama08->SpawnBulletPatternPrimary(descriptor);
                extra->toggle04 = 1 - extra->toggle04;
            }

            if (g_GameManager.sides[record->opponentSide04]
                    .player04->CheckBulletCollision(
                    reinterpret_cast<PlayerPositionView *>(
                        &record->position20),
                    reinterpret_cast<PlayerPositionView *>(
                        &collisionSize),
                    0) == 2)
            {
                extra->state00 = 2;
                record->timer10 = 0;
                reinterpret_cast<AnmVm *>(
                    record->dynamicData1C)->SetInterrupt(1);
                record->rotation3C = extra->angle08;
                return 0;
            }

            extra->motion14.FromAngleMagnitude(extra->angle08, 1.5f);
            record->position20 += extra->motion14;

            if (record->position20.y >= 480.0f ||
                record->position20.y <= -32.0f ||
                record->position20.x >= 176.0f ||
                record->position20.x <= -176.0f)
            {
                return 1;
            }
        }

        record->rotation3C = extra->angle08;
        break;

    case 0:
        ExAttackInterpolate2D(
            &record->position20.x,
            &extra->point2C.x,
            &extra->point20.x,
            &extra->control44.x,
            &extra->control38.x,
            record->timer10,
            90.0f);

        if (record->timer10 > 90)
        {
            reinterpret_cast<ExAttackType1112PlayerAnmView *>(
                g_GameManager.sides[record->side08].player04)
                ->anmFileBC->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C), 9);
            ++extra->state00;
            record->position20 = extra->spawn50;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;
        }
        break;
    }

    return 0;
}

int __fastcall ExAttackUpdateCallbackType12(ExAttackRecord *base)
{
    ExAttackType1112UpdateRecord *record =
        reinterpret_cast<ExAttackType1112UpdateRecord *>(base);
    ExAttackType1112UpdateExtra *extra = record->extra34;
    Float3 collisionSize(24.0f, 24.0f, 0.0f);

    switch (extra->state00)
    {
    case 2:
        if (record->timer10 > 20)
            return 1;
        break;

    case 1:
        if (record->timer10 > 20)
        {
            if (record->timer10.HasTickedEvery(10))
            {
                extra->angle0C = AddNormalizeAngle(
                    extra->angle0C, 0.10471976f);

                ExAttackType1112BulletDescriptorStorage storage;
                BulletSpawnDescriptor *descriptor =
                    reinterpret_cast<BulletSpawnDescriptor *>(&storage);
                descriptor->position = record->position20;
                descriptor->bulletType = extra->toggle04 != 0 ? 6 : 0;
                descriptor->aimMode = 5;
                descriptor->count1 = 1;
                descriptor->count2 = 1;
                descriptor->color = 2;
                descriptor->angle = extra->angle08;
                descriptor->speed1 = 0.0f;

                descriptor->InstallWaitTransform(0, 0, 80);
                descriptor->InstallVectorAccelerationTransform(
                    1, 0, 120,
                    (g_GameManager.sides[1 - record->opponentSide04]
                             .player04->header24.state80 *
                         0.05f +
                     2.0f) *
                        0.0083333338f,
                    -999.0f);

                descriptor->angleStep = 0.0f;
                descriptor->speed2 = 0.0f;
                descriptor->unknown1FA = 0;
                descriptor->transformFlags = 0x00020014;
                g_GameManager.sides[record->opponentSide04]
                    .etama08->SpawnBulletPatternPrimary(descriptor);
                extra->toggle04 = 1 - extra->toggle04;
            }

            if (g_GameManager.sides[record->opponentSide04]
                    .player04->CheckBulletCollision(
                    reinterpret_cast<PlayerPositionView *>(
                        &record->position20),
                    reinterpret_cast<PlayerPositionView *>(
                        &collisionSize),
                    0) == 2)
            {
                extra->state00 = 2;
                record->timer10 = 0;
                reinterpret_cast<AnmVm *>(
                    record->dynamicData1C)->SetInterrupt(1);
                record->rotation3C = extra->angle08;
                return 0;
            }

            extra->motion14.FromAngleMagnitude(extra->angle08, 1.5f);
            record->position20 += extra->motion14;

            if (record->position20.y >= 480.0f ||
                record->position20.y <= -32.0f ||
                record->position20.x >= 176.0f ||
                record->position20.x <= -176.0f)
            {
                return 1;
            }
        }

        record->rotation3C = extra->angle08;
        break;

    case 0:
        ExAttackInterpolate2D(
            &record->position20.x,
            &extra->point2C.x,
            &extra->point20.x,
            &extra->control44.x,
            &extra->control38.x,
            record->timer10,
            90.0f);

        if (record->timer10 > 90)
        {
            reinterpret_cast<ExAttackType1112PlayerAnmView *>(
                g_GameManager.sides[record->side08].player04)
                ->anmFileBC->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C), 9);
            ++extra->state00;
            record->position20 = extra->spawn50;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;
        }
        break;
    }

    return 0;
}
