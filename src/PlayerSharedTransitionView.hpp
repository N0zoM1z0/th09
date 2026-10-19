#pragma once

struct PlayerSharedTransitionView
{
    unsigned char unknown0000[0x11EA8];
    int transitionBlock11EA8;

    int GetTransitionBlockFlag();
};
