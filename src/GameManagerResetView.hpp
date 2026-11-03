#pragma once

#include "GameManagerRuntimeView.hpp"
#include <stddef.h>

// The exact original reset and coordinate-initializer methods retain their
// decorated GameManagerResetView C++ receiver identity. Their 0x390 bytes of
// state are the same native GameManager allocation, never a second layout.
struct GameManagerResetView : AsciiGameManagerView
{
    void InitializeCoordinateBounds();
    void ResetForSupervisorAdded();
};

typedef char GameManagerResetFlagAt13F[
    (offsetof(GameManagerResetView, resetFlag13F) == 0x13F) ? 1 : -1];
typedef char GameManagerResetCoordinate350[
    (offsetof(GameManagerResetView, coordinateOffsetX) == 0x350 &&
     offsetof(GameManagerResetView, coordinateOffsetY) == 0x354) ? 1 : -1];
typedef char GameManagerResetCoordinate36C[
    (offsetof(GameManagerResetView, coordinate36C) == 0x36C) ? 1 : -1];
typedef char GameManagerResetValue378[
    (offsetof(GameManagerResetView, value378) == 0x378 &&
     offsetof(GameManagerResetView, value37C) == 0x37C) ? 1 : -1];
typedef char GameManagerResetSize390[
    (sizeof(GameManagerResetView) == sizeof(AsciiGameManagerView) &&
     sizeof(GameManagerResetView) == 0x390) ? 1 : -1];
