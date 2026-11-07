#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "EffectManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"
#include "PlayerLifecycleView.hpp"
#include "GameManagerRuntimeView.hpp"

struct ExAttackType13UpdateExtra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 control28;
    Float3 control34;
    Float3 spawn40;
};
typedef char ExAttackType13UpdateExtraSizeIs4C[
    (sizeof(ExAttackType13UpdateExtra) == 0x4C) ? 1 : -1];

struct ExAttackType13UpdateRecord
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
    ExAttackType13UpdateExtra *extra34;
};

struct ExAttackType13InterruptView
{
    void SetInterrupt(short interrupt);
};



extern AsciiGameManagerView g_GameManager;

int __fastcall ExAttackUpdateCallbackType13(ExAttackRecord *base)
{
    ExAttackType13UpdateRecord *record =
        reinterpret_cast<ExAttackType13UpdateRecord *>(base);
    ExAttackType13UpdateExtra *extra = record->extra34;
    Float3 collisionSize(24.0f, 24.0f, 0.0f);

    switch (extra->state00)
    {
    case 2:
        if (record->timer10 > 20)
            return 1;
        break;

    case 1:
        if (record->timer10.HasTickedEvery(2))
        {
            g_GameManager.sides[record->opponentSide04]
                .effectManager0C->SpawnEffect(
                    9,
                    reinterpret_cast<EffectFloat3 *>(&record->position20),
                    1,
                    0xFFFFFFFFU);
        }

        if (record->timer10 > 20)
        {
            if (g_GameManager.sides[record->opponentSide04]
                    .player04->CheckBulletCollision(
                    reinterpret_cast<PlayerPositionView *>(&record->position20),
                    reinterpret_cast<PlayerPositionView *>(&collisionSize),
                    0) == 2)
            {
                extra->state00 = 2;
                record->timer10 = 0;
                reinterpret_cast<ExAttackType13InterruptView *>(
                    record->dynamicData1C)->SetInterrupt(1);
            }
            else
            {
                g_GameManager.sides[record->opponentSide04]
                    .player04->collisionQuery36C.AppendCircleRecord(
                        reinterpret_cast<PlayerPositionView *>(&record->position20),
                        14.0f,
                        0);
            }

            if (record->timer10 > 300)
            {
                extra->state00 = 2;
                record->timer10 = 0;
                reinterpret_cast<ExAttackType13InterruptView *>(
                    record->dynamicData1C)->SetInterrupt(1);
                return 0;
            }
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
            180.0f);

        if (record->timer10 > 180)
        {
            g_GameManager.sides[record->side08]
                .player04->header24.anmFile98->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C),
                    9);
            ++extra->state00;
            record->position20 = extra->spawn40;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;
        }
        break;
    }

    return 0;
}
