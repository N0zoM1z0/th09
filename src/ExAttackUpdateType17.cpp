#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"
#include "PlayerLifecycleView.hpp"
#include "RngRuntimeLeaves.hpp"

struct ExAttackType17UpdateExtra
{
    int state00;
    Float3 motion04;
    Float3 point10;
    Float3 point1C;
    Float3 control28;
    Float3 control34;
    Float3 spawn40;
    short variant4C;
    unsigned char unknown4E[2];
};
typedef char ExAttackType17UpdateExtraSizeIs50[
    (sizeof(ExAttackType17UpdateExtra) == 0x50) ? 1 : -1];

struct ExAttackType17UpdateRecord
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
    ExAttackType17UpdateExtra *extra34;
};

struct ExAttackType17InterruptView
{
    void SetInterrupt(short interrupt);
};

struct ExAttackType17CircleAppendView
{
    PlayerLifecycleView *player00;
    PlayerCollisionQueryRecordView records[128];
    int count1804;

    void AppendCircleRecord(
        const PlayerPositionView *position,
        float radius,
        int unknown);
};

struct ExAttackType17PlayerView
{
    unsigned char unknown000[0xBC];
    AnmLoaded *anmFileBC;
    unsigned char unknown0C0[0x36C - 0xC0];
    ExAttackType17CircleAppendView collisionQuery36C;

    int CheckBulletCollision(
        PlayerPositionView *position,
        PlayerPositionView *collisionSize,
        Bullet *bullet);
};

struct ExAttackType17SideView
{
    unsigned char unknown00[0x04];
    ExAttackType17PlayerView *player04;
    unsigned char unknown08[0x30];
};

struct ExAttackType17GameManagerView
{
    ExAttackType17SideView sides[2];
};

extern ExAttackType17GameManagerView g_GameManager;
extern RngRuntimeView g_ReplayRng;

int __fastcall ExAttackUpdateCallbackType17(ExAttackRecord *base)
{
    ExAttackType17UpdateRecord *record =
        reinterpret_cast<ExAttackType17UpdateRecord *>(base);
    ExAttackType17UpdateExtra *extra = record->extra34;
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
            if (g_GameManager.sides[record->opponentSide04]
                    .player04->CheckBulletCollision(
                    reinterpret_cast<PlayerPositionView *>(&record->position20),
                    reinterpret_cast<PlayerPositionView *>(&collisionSize),
                    0) == 2)
            {
                extra->state00 = 2;
                record->timer10 = 0;
                reinterpret_cast<ExAttackType17InterruptView *>(
                    record->dynamicData1C)->SetInterrupt(1);
                return 0;
            }

            record->position20 += extra->motion04;

            if (extra->variant4C < 2)
            {
                g_GameManager.sides[record->opponentSide04]
                    .player04->collisionQuery36C.AppendCircleRecord(
                        reinterpret_cast<PlayerPositionView *>(&record->position20),
                        28.0f,
                        0);
            }
            else
            {
                g_GameManager.sides[record->opponentSide04]
                    .player04->collisionQuery36C.AppendCircleRecord(
                        reinterpret_cast<PlayerPositionView *>(&record->position20),
                        18.0f,
                        0);
            }

            if (record->position20.y >= 512.0f)
                return 1;
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
            extra->variant4C =
                static_cast<short>(g_ReplayRng.GetRandomU16InRange(4));
            g_GameManager.sides[record->side08]
                .player04->anmFileBC->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C),
                    extra->variant4C + 9);
            ++extra->state00;
            record->position20 = extra->spawn40;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;
        }
        break;
    }

    return 0;
}
