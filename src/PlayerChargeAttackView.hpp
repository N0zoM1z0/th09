#pragma once

#include "PlayerLifecycleView.hpp"

// Shared interface for the complete charge-update owner at 0x0041F810.
// It adds no storage to the observed Player prefix. Original class identity
// and the original translation-unit partition remain unknown.
struct PlayerChargeAttackView : PlayerLifecycleView
{
    int UpdateChargeAttack();
};

typedef char PlayerChargeAttackHasNoExtraStorage[
    sizeof(PlayerChargeAttackView) == sizeof(PlayerLifecycleView) ? 1 : -1];
