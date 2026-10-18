// Included once by EclManager.cpp after the existing Float3 helper definition.
#include "AnmManager.hpp"
#include "RuntimeAngleMath.hpp"
#include "BulletManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"
#include "ExAttackType8GameManagerView.hpp"
#include "ExAttackType21ExtraView.hpp"

#include <stddef.h>

// Keep the exact Type21 ring cursor's adjacent U/V scalar assumption explicit.
typedef char ExAttackType21UpdateAdjacentUvFloats[
    (offsetof(ExAttackType21UpdateVertex, v) ==
     offsetof(ExAttackType21UpdateVertex, u) + sizeof(float)) ? 1 : -1];

struct ExAttackType21UpdateVmView
{
    unsigned char unknown000[0x1F8];
    unsigned int flags1F8;
};

struct ExAttackType21UpdatePlayerView
{
    unsigned char unknown000[0xBC];
    AnmLoaded *anmFileBC;
};

struct ExAttackType21BulletDescriptorStorage
{
    // Declaration-only constructor view; original class/definition unknown.
    // Both emission paths access the initialized bytes as this real member.
    BulletSpawnDescriptor descriptor;
    ExAttackType21BulletDescriptorStorage();
};

typedef char ExAttackType21BulletDescriptorStorageSizeIs214[
    (sizeof(ExAttackType21BulletDescriptorStorage) == 0x214) ? 1 : -1];
typedef char ExAttackType21BulletDescriptorAtZero[
    (offsetof(ExAttackType21BulletDescriptorStorage, descriptor) == 0) ? 1 : -1];



int __fastcall ExAttackUpdateCallbackType21(ExAttackRecord *base)
{
    ExAttackType21UpdateRecord *record =
        reinterpret_cast<ExAttackType21UpdateRecord *>(base);
    ExAttackType21UpdateExtra *extra = record->extra34;

    // The target reuses this real vector for ring UVs and both bullet deltas.
    Float3 workspace;

    switch (extra->state00)
    {
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

            reinterpret_cast<ExAttackType21UpdatePlayerView *>(
                g_GameManager.sides[record->side08].player04)
                ->anmFileBC->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C),
                    11);

            reinterpret_cast<ExAttackType21UpdateVmView *>(
                record->dynamicData1C)->flags1F8 &= ~2u;
        }
        break;

    case 1:
    {
        // The target advances through V offsets of rim vertices 1..32.
        // Anchor this cursor at the enclosing object's byte representation,
        // not at a standalone float subobject. Crossing scalar/member and
        // vertex boundaries using float* arithmetic would be undefined C++.
        unsigned char *vertexV = reinterpret_cast<unsigned char *>(extra) +
            offsetof(ExAttackType21UpdateExtra, vertices4C) +
            sizeof(ExAttackType21UpdateVertex) +
            offsetof(ExAttackType21UpdateVertex, v);
        Float3 *history = &extra->history3E8[1];
        // One ordinal addresses the two separately owned 32-element arrays.
        int sample = 0;
        float historyAngle = -3.1415927f;
        float uvAngle = extra->angle688;

        do
        {
            Float3 *uv = &workspace;
            uv->FromAngleMagnitude(uvAngle, 0.5f);
            *reinterpret_cast<float *>(vertexV - sizeof(float)) =
                uv->x + 0.5f;
            *reinterpret_cast<float *>(vertexV) = uv->y + 0.5f;
            uvAngle = AddNormalizeAngle(uvAngle, 0.2026834f);

            extra->magnitudes578[sample] += extra->angularSteps5FC[sample];
            history->FromAngleMagnitude(historyAngle, extra->magnitudes578[sample]);
            *history += extra->spawn40;

            vertexV += sizeof(ExAttackType21UpdateVertex);
            ++sample;
            ++history;
            historyAngle += 0.2026834f;
        }
        while (sample != 32);

        extra->angle688 =
            AddNormalizeAngle(extra->angle688, 0.052359879f);

        Bullet *bullet =
            &g_GameManager.sides[record->opponentSide04]
                .etama08->primaryBullets[0];
        for (int i = 0; i < 175; ++i, ++bullet)
        {
            if (bullet->state != 0 &&
                bullet->unknown10BE == 0 &&
                bullet->sprites.unknownD44 != 330)
            {
                Float3 *delta = &workspace;
                *delta = bullet->position - record->position20;
                if (delta->x * delta->x + delta->y * delta->y <= 1024.0f)
                {
                    bullet->unknown10BE = 1;

                    ExAttackType21BulletDescriptorStorage storage;
                    BulletSpawnDescriptor *descriptor =
                        &storage.descriptor;
                    descriptor->position = bullet->position;
                    descriptor->bulletType = 22;
                    descriptor->aimMode = 1;
                    descriptor->count1 = 1;
                    descriptor->count2 = 1;
                    descriptor->color = 0;
                    descriptor->angle = bullet->angle;
                    descriptor->angleStep = 0.0f;
                    descriptor->speed1 = bullet->speed + 1.0f;
                    descriptor->speed2 = 0.0f;
                    descriptor->unknown1FA = 0;
                    descriptor->transformFlags = 0;

                    Bullet *spawned =
                        g_GameManager.sides[record->opponentSide04]
                            .etama08->SpawnBulletPatternPrimary(descriptor);
                    spawned->unknown10BE = 1;
                }
            }
        }

        bullet =
            &g_GameManager.sides[record->opponentSide04]
                .etama08->secondaryBullets[0];
        for (int i = 0; i < 360; ++i, ++bullet)
        {
            if (bullet->state != 0 &&
                bullet->unknown10BE == 0 &&
                bullet->sprites.unknownD44 != 330)
            {
                Float3 *delta = &workspace;
                *delta = bullet->position - record->position20;
                if (delta->x * delta->x + delta->y * delta->y <= 1024.0f)
                {
                    bullet->unknown10BE = 1;

                    ExAttackType21BulletDescriptorStorage storage;
                    BulletSpawnDescriptor *descriptor =
                        &storage.descriptor;
                    descriptor->position = bullet->position;
                    descriptor->bulletType = 22;
                    descriptor->aimMode = 1;
                    descriptor->count1 = 1;
                    descriptor->count2 = 1;
                    descriptor->color = 0;
                    descriptor->angle = bullet->angle;
                    descriptor->angleStep = 0.0f;
                    descriptor->speed1 = bullet->speed + 0.69999999f;
                    descriptor->speed2 = 0.0f;
                    descriptor->unknown1FA = 0;
                    descriptor->transformFlags = 0;

                    Bullet *spawned =
                        g_GameManager.sides[record->opponentSide04]
                            .etama08->SpawnBulletPatternSecondary(descriptor);
                    spawned->unknown10BE = 1;
                }
            }
        }

        if ((reinterpret_cast<ExAttackType21UpdateVmView *>(
                 record->dynamicData1C)->flags1F8 & 1u) == 0)
            return 1;
        break;
    }

    case 2:
        if (record->timer10 > 20)
            return 1;
        break;

    default:
        break;
    }

    return 0;
}
