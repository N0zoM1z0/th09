#pragma once

struct PlayerLifecycleView;

// Shared current factory ABI, checked against its target-bound implementation.
// Original source spelling and translation-unit ownership remain unproven.
PlayerLifecycleView *__fastcall PlayerRegisterChain(
    unsigned char playerType, int sideIndex, int selector);
