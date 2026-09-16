#pragma once

#include <stddef.h>

struct GameManagerResetView
{
    unsigned char unknown000[0x13F];
    unsigned char flag13F;
    unsigned char unknown140[0x350 - 0x140];
    float coordinate350;
    float coordinate354;
    float coordinate358;
    float coordinate35C;
    float coordinate360;
    float coordinate364;
    float coordinate368;
    float coordinate36C;
    unsigned char unknown370[0x378 - 0x370];
    int value378;
    int value37C;
    unsigned char unknown380[0x390 - 0x380];

    void InitializeCoordinateBounds();
    void ResetForSupervisorAdded();
};

typedef char GameManagerResetFlagAt13F[
    (offsetof(GameManagerResetView, flag13F) == 0x13F) ? 1 : -1];
typedef char GameManagerResetCoordinate350[
    (offsetof(GameManagerResetView, coordinate350) == 0x350) ? 1 : -1];
typedef char GameManagerResetCoordinate36C[
    (offsetof(GameManagerResetView, coordinate36C) == 0x36C) ? 1 : -1];
typedef char GameManagerResetValue378[
    (offsetof(GameManagerResetView, value378) == 0x378) ? 1 : -1];
typedef char GameManagerResetSize390[
    (sizeof(GameManagerResetView) == 0x390) ? 1 : -1];
