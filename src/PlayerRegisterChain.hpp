#pragma once

struct PlayerLifecycleView;

// Shared lifecycle ABIs, checked against their target-bound implementations.
// Original source spelling and translation-unit ownership remain unproven.
PlayerLifecycleView *__fastcall PlayerRegisterChain(
    unsigned char playerType, int sideIndex, int selector);

void __fastcall PlayerRelease(PlayerLifecycleView *player);
