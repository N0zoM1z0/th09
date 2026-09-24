#include "EnemyManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "PlayerCollisionRegionCreate.hpp"
#include "RngRuntimeLeaves.hpp"
#include "SoundPlayer.hpp"
#include "Supervisor.hpp"

// Partial target-facing reconstruction of the TH09 EnemyManager update owner.
// This packet isolates the non-leaf death/reward dispatcher called from the
// 3,883-byte EnemyManager::OnUpdate.

struct EnemyRewardEffectView
{
    unsigned char unknown000[0xA0];
    float angularStepA0;
    unsigned short tierA4;
    unsigned short tierLimitA6;
    unsigned short sideA8;
};

extern RngRuntimeView g_ReplayRng;
extern EffectManager *g_SharedEffectManager;
extern float g_GameplayRegionWidth;

void EnemyView::HandleDeathRewards(int hitKind)
{
    int rewardMode = hitKind;

    if (rewardMode != 0)
        this->manager00->sideState320->player04->rewardAttack30410
            .ResetForEnemyReward();

    if ((this->rewardFlags3380 & 0x2000) != 0)
    {
        EnemyFloat3 *worldPosition = &this->worldPosition2DD4;
        g_SoundPlayer.PlaySoundPositionedByIdx(18, worldPosition->x);
        this->manager00->sideState320->player04->rewardAttack30410
            .QueueEnemyReward(worldPosition, 400, 0, 300, 1000);
        this->manager00->sideState320->player04->SpawnDefeatToken(
            this->defeatTokenType335C, &this->position2D74);
        return;
    }

    if ((this->rewardFlags3380 & 0x0C00) != 0)
    {
        g_SoundPlayer.PlaySoundPositionedByIdx(18, this->worldPosition2DD4.x);
        this->manager00->sideState320->player04->SpawnDefeatToken(
            g_ReplayRng.GetRandomU16InRange(4), &this->position2D74);
    }

    EnemyFloat3 *worldPosition = &this->worldPosition2DD4;
    g_SoundPlayer.PlaySoundPositionedByIdx(
        this->sequenceIndex2E58 % 2 + 2, worldPosition->x);

    if ((this->rewardFlags3380 & 0x01C0) != 0 && rewardMode != 0 &&
        (this->rewardFlags3380 & 0x1000) == 0)
    {
        this->manager00->sideState320->effectManager0C->SpawnEffect(
            5,
            reinterpret_cast<EffectFloat3 *>(worldPosition),
            1,
            static_cast<unsigned int>(-1));
    }
    else
    {
        this->manager00->sideState320->effectManager0C->SpawnEffect(
            this->deathEffectVariant3368 + 20,
            reinterpret_cast<EffectFloat3 *>(worldPosition),
            1,
            static_cast<unsigned int>(-1));

        if ((this->rewardFlags3380 & 0x01C0) == 0)
        {
            float effectScale;
            if (this->deathEffectVariant3368 == 0)
                effectScale = 5.0f;
            else if (this->deathEffectVariant3368 == 1)
                effectScale = 5.5f;
            else if (this->deathEffectVariant3368 == 2)
                effectScale = 6.0f;
            else
                effectScale = 6.25f;
            reinterpret_cast<PlayerCollisionRegionCreateView *>(
                this->manager00->sideState320->player04)
                ->CreateCircleType2(
                reinterpret_cast<const PlayerRegionPointView *>(worldPosition),
                32.0f,
                effectScale,
                3,
                this->deathEffectVariant3368 + 8,
                4);
        }
        else
        {
            float effectScale;
            if (this->deathEffectVariant3368 == 0)
                effectScale = 5.0f;
            else if (this->deathEffectVariant3368 == 1)
                effectScale = 5.5f;
            else if (this->deathEffectVariant3368 == 2)
                effectScale = 6.0f;
            else
                effectScale = 6.25f;
            reinterpret_cast<PlayerCollisionRegionCreateView *>(
                this->manager00->sideState320->player04)
                ->CreateCircleType2(
                reinterpret_cast<const PlayerRegionPointView *>(worldPosition),
                32.0f,
                effectScale,
                2,
                this->deathEffectVariant3368 + 8,
                4);
        }
    }

    int rewardTier = (this->rewardFlags3380 >> 6) & 7;
    int count;
    if (rewardTier == 0)
        count = 4;
    else if (rewardTier == 1)
        count = 10;
    else if (rewardTier == 2)
        count = 20;
    else if (rewardTier == 3)
        count = 30;
    else
        count = 50;

    EnemyRewardPlayerView *player = this->manager00->sideState320->player04;
    int rewardLevel = player->rewardAttack30410.rewardLevel38;
    int interval = rewardLevel <= 10 ? rewardLevel * 3 + 30 : 60;
    int spread;
    if (rewardTier != 0)
        spread = 0;
    else if (rewardLevel <= 24)
        spread = rewardLevel + 4;
    else
        spread = 28;

    int value;
    if (rewardTier != 0)
        value = rewardLevel <= 50 ? (rewardLevel + 5) * 200 : 11000;
    else
        value = rewardLevel <= 100 ? rewardLevel * 30 + 10 : 3000;
    if (rewardMode != 0)
        value *= 2;

    player->rewardAttack30410.QueueEnemyReward(
        worldPosition, interval, spread, count, value);

    float defeatValue = rewardLevel <= 128
        ? static_cast<float>(rewardLevel) * 0.0078125f + 1.0f
        : 2.0f;
    player->AccumulateDefeatValue(defeatValue);

    rewardTier = (this->rewardFlags3380 >> 6) & 7;
    if (rewardTier == 0 || rewardTier >= 3 || rewardMode >= 2)
        return;
    if (this->manager00->opposingSideState324->enemyManager10
            ->rewardEnemyCount2AC3B8 >= 25)
        return;

    EnemyFloat3 effectPosition;
    effectPosition.x = g_GameManager.TransformPopupX(this->position2D74.x);
    effectPosition.y = g_GameManager.TransformPopupY(this->position2D74.y);
    effectPosition.z = 0.0f;

    g_Supervisor.SelectSide(1 - this->manager00->sideIndex31C);

    EnemyFloat3 velocity;
    velocity.x = g_ReplayRng.GetRandomF32SignedInRange(
        g_GameplayRegionWidth * 0.5f - 8.0f);
    velocity.y = g_ReplayRng.GetRandomF32InRange(128.0f);
    velocity.z = 0.0f;

    Effect *effect = g_SharedEffectManager->SpawnEffectWithVelocity(
        this->manager00->sideIndex31C + 3,
        reinterpret_cast<EffectFloat3 *>(&effectPosition),
        reinterpret_cast<EffectFloat3 *>(&velocity),
        1,
        static_cast<unsigned int>(-1));

    g_Supervisor.SelectSide(this->manager00->sideIndex31C);

    EnemyRewardEffectView *rewardEffect =
        reinterpret_cast<EnemyRewardEffectView *>(effect);
    unsigned int effectTier = (this->rewardFlags3380 >> 6) & 7;
    rewardEffect->tierA4 =
        static_cast<unsigned short>(effectTier > 2 ? 2 : effectTier);
    effectTier = ((this->rewardFlags3380 >> 6) & 7) + 1;
    rewardEffect->tierLimitA6 =
        static_cast<unsigned short>(effectTier > 3 ? 3 : effectTier);
    rewardEffect->angularStepA0 =
        g_ReplayRng.GetRandomF32InRange(0.5f) * 0.016666668f;
    rewardEffect->sideA8 =
        static_cast<unsigned short>(this->manager00->sideIndex31C);
}
