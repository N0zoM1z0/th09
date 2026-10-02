#include "EnemyManager.hpp"
#include "AsciiGameManagerView.hpp"
#include "PlayerCollisionRegionCreate.hpp"
#include "RngRuntimeLeaves.hpp"
#include "SoundPlayer.hpp"
#include "Supervisor.hpp"

// Target-facing death/reward dispatcher called by EnemyManager::OnUpdate.

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
    // Circle scale and the later defeat increment use the same scalar workspace.
    float rewardScalar;

    if (hitKind != 0)
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

    if ((this->rewardFlags3380 & 0x01C0) != 0 && hitKind != 0 &&
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
            if (this->deathEffectVariant3368 == 0)
                rewardScalar = 5.0f;
            else if (this->deathEffectVariant3368 == 1)
                rewardScalar = 5.5f;
            else if (this->deathEffectVariant3368 == 2)
                rewardScalar = 6.0f;
            else
                rewardScalar = 6.25f;
            reinterpret_cast<PlayerCollisionRegionCreateView *>(
                this->manager00->sideState320->player04)
                ->CreateCircleType2(
                reinterpret_cast<const PlayerRegionPointView *>(worldPosition),
                32.0f,
                rewardScalar,
                3,
                this->deathEffectVariant3368 + 8,
                4);
        }
        else
        {
            if (this->deathEffectVariant3368 == 0)
                rewardScalar = 5.0f;
            else if (this->deathEffectVariant3368 == 1)
                rewardScalar = 5.5f;
            else if (this->deathEffectVariant3368 == 2)
                rewardScalar = 6.0f;
            else
                rewardScalar = 6.25f;
            reinterpret_cast<PlayerCollisionRegionCreateView *>(
                this->manager00->sideState320->player04)
                ->CreateCircleType2(
                reinterpret_cast<const PlayerRegionPointView *>(worldPosition),
                32.0f,
                rewardScalar,
                2,
                this->deathEffectVariant3368 + 8,
                4);
        }
    }

    unsigned int rewardTier = (this->rewardFlags3380 >> 6) & 7;
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
    int interval = player->rewardAttack30410.rewardLevel38 > 10
        ? 60 : player->rewardAttack30410.rewardLevel38 * 3 + 30;
    int spread;
    if (((this->rewardFlags3380 >> 6) & 7u) != 0)
        spread = 0;
    else if (player->rewardAttack30410.rewardLevel38 > 24)
        spread = 28;
    else
        spread = player->rewardAttack30410.rewardLevel38 + 4;

    int value;
    if (((this->rewardFlags3380 >> 6) & 7u) == 0)
        value = player->rewardAttack30410.rewardLevel38 > 100
            ? 3000 : player->rewardAttack30410.rewardLevel38 * 30 + 10;
    else
        value = player->rewardAttack30410.rewardLevel38 > 50
            ? 11000 : (player->rewardAttack30410.rewardLevel38 + 5) * 200;
    if (hitKind != 0)
        value *= 2;

    player->rewardAttack30410.QueueEnemyReward(
        worldPosition, interval, spread, count, value);
    // The reward wrapper advances state38. Reacquire both the Player and level.
    player = this->manager00->sideState320->player04;
    int rewardLevel = player->rewardAttack30410.rewardLevel38;

    rewardScalar = rewardLevel > 128
        ? 2.0f
        : static_cast<float>(rewardLevel) * 0.0078125f + 1.0f;
    player->AccumulateDefeatValue(rewardScalar);

    rewardTier = (this->rewardFlags3380 >> 6) & 7;
    if (rewardTier == 0 || rewardTier >= 3 || hitKind >= 2)
        return;
    // Retain this owner across popup-coordinate conversion, as the target does.
    EnemyManagerView *manager = this->manager00;
    if (manager->opposingSideState324->enemyManager10
            ->rewardEnemyCount2AC3B8 >= 25)
        return;

    EnemyFloat3 effectPosition;
    effectPosition.x = g_GameManager.TransformPopupX(this->position2D74.x);
    effectPosition.y = g_GameManager.TransformPopupY(this->position2D74.y);
    effectPosition.z = 0.0f;

    g_Supervisor.SelectSide(1 - manager->sideIndex31C);

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
