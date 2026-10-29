#pragma once

#include "AnmManager.hpp"
#include "AsciiGameManagerView.hpp"

#include <stddef.h>

// ECL/ExAttack gateway to the shared native GameManager declaration.
// Side +0x0C is the existing EffectManager; its ANM member is +0x2D4.
typedef char ExAttackType8GameManagerSpeedAtEC[
    (offsetof(AsciiGameManagerView, speedEC) == 0xEC) ? 1 : -1];
typedef char ExAttackType8GameManagerDifficultyAt11C[
    (offsetof(AsciiGameManagerView, difficulty11C) == 0x11C) ? 1 : -1];
