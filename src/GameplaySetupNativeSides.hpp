#pragma once

#include "GameManagerSetupView.hpp"
#include "GameConfiguration.hpp"
#include <stddef.h>

// Two playable GameManager sides share the first two physical 0x38 records.
// Setup/cleanup also use one third record, but only for shared EffectManager;
// it must never be visited by the playable-player controller loops.
struct GameplaySetupNativeSides
{
    enum
    {
        PlayableCount = sizeof(((AsciiGameManagerView *)0)->sides) /
                        sizeof(((AsciiGameManagerView *)0)->sides[0]),
        PhysicalCount = sizeof(((GameManagerSetupLayout *)0)->sides) /
                        sizeof(((GameManagerSetupLayout *)0)->sides[0]),
        SharedEffectIndex = PlayableCount,
        InputFocusCount = sizeof(((GameConfiguration *)0)->sideFocusMode) /
                          sizeof(((GameConfiguration *)0)->sideFocusMode[0])
    };
};

typedef char GameplaySetupNativeSidesLayout[
    (GameplaySetupNativeSides::PlayableCount == 2 &&
     GameplaySetupNativeSides::InputFocusCount ==
         GameplaySetupNativeSides::PlayableCount &&
     GameplaySetupNativeSides::PhysicalCount == 3 &&
     GameplaySetupNativeSides::SharedEffectIndex + 1 ==
         GameplaySetupNativeSides::PhysicalCount &&
     offsetof(GameManagerSetupLayout, sides) ==
         offsetof(AsciiGameManagerView, sides) &&
     sizeof(SetupSideState) == sizeof(AsciiGameManagerSideView) &&
     offsetof(GameManagerSetupLayout, sides) +
         GameplaySetupNativeSides::SharedEffectIndex * sizeof(SetupSideState) == 0x70 &&
     offsetof(GameManagerSetupLayout, sides) +
         GameplaySetupNativeSides::SharedEffectIndex * sizeof(SetupSideState) +
         offsetof(SetupSideState, effectManager0C) == 0x7C &&
     offsetof(GameManagerSetupLayout, gameSubsystem) ==
         offsetof(GameManagerSetupLayout, sides) +
         sizeof(((GameManagerSetupLayout *)0)->sides)) ? 1 : -1];
