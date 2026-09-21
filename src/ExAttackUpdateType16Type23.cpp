#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "ExAttackController.hpp"
#include "ExAttackInterpolation.hpp"
#include "PlayerLifecycleView.hpp"

struct ExAttackType16UpdateExtra
{
    int state00;
    Float3 motion04;
    float scalar10;
    Float3 point14;
    Float3 point20;
    Float3 control2C;
    Float3 control38;
    Float3 spawn44;
};
typedef char ExAttackType16UpdateExtraSizeIs50[
    (sizeof(ExAttackType16UpdateExtra) == 0x50) ? 1 : -1];

struct ExAttackType16UpdateRecord
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
    ExAttackType16UpdateExtra *extra34;
};

struct ExAttackType16InterruptView
{
    void SetInterrupt(short interrupt);
};

struct ExAttackType16CircleAppendView
{
    PlayerLifecycleView *player00;
    PlayerCollisionQueryRecordView records[128];
    int count1804;

    void AppendCircleRecord(
        const PlayerPositionView *position,
        float radius,
        int unknown);
};

struct ExAttackType16PlayerView
{
    unsigned char unknown000[0xBC];
    AnmLoaded *anmFileBC;
    unsigned char unknown0C0[0x36C - 0xC0];
    ExAttackType16CircleAppendView collisionQuery36C;

    int CheckBulletCollision(
        PlayerPositionView *position,
        PlayerPositionView *collisionSize,
        Bullet *bullet);
};

struct ExAttackType16SideView
{
    unsigned char unknown00[0x04];
    ExAttackType16PlayerView *player04;
    unsigned char unknown08[0x30];
};

struct ExAttackType16GameManagerView
{
    ExAttackType16SideView sides[2];
};

extern ExAttackType16GameManagerView g_GameManager;

int __fastcall ExAttackUpdateCallbackType16_23(ExAttackRecord *base)
{
    ExAttackType16UpdateRecord *record =
        reinterpret_cast<ExAttackType16UpdateRecord *>(base);
    ExAttackType16UpdateExtra *extra = record->extra34;
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
                reinterpret_cast<ExAttackType16InterruptView *>(
                    record->dynamicData1C)->SetInterrupt(1);
                return 0;
            }

            record->position20 += extra->motion04;
            extra->motion04.x += extra->scalar10;

            if (record->position20.x < -144.0f ||
                record->position20.x > 144.0f)
            {
                extra->motion04.x *= -1.0f;
            }

            if (record->position20.y >= 464.0f)
                return 1;

            g_GameManager.sides[record->opponentSide04]
                .player04->collisionQuery36C.AppendCircleRecord(
                    reinterpret_cast<PlayerPositionView *>(&record->position20),
                    13.0f,
                    0);
        }
        break;

    case 0:
        ExAttackInterpolate2D(
            &record->position20.x,
            &extra->point20.x,
            &extra->point14.x,
            &extra->control38.x,
            &extra->control2C.x,
            record->timer10,
            90.0f);

        if (record->timer10 > 90)
        {
            ++extra->state00;
            record->position20 = extra->spawn44;
            record->unknown00 = record->opponentSide04;
            record->timer10 = 0;

            if (record->position20.x < 0.0f)
            {
                g_GameManager.sides[record->side08]
                    .player04->anmFileBC->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C),
                    11);
                extra->scalar10 = -0.03f;
            }
            else
            {
                g_GameManager.sides[record->side08]
                    .player04->anmFileBC->ExecuteAnmIdx(
                    reinterpret_cast<AnmVm *>(record->dynamicData1C),
                    12);
                extra->scalar10 = 0.03f;
            }

            extra->motion04.y = 2.0f;
            extra->motion04.x = 0.0f;
        }
        break;
    }

    return 0;
}
