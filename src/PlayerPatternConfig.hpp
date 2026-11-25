#pragma once

#include <stddef.h>

struct PlayerPatternConfig
{
    int openingLong00;
    int openingMedium04;
    int openingShort08;
    int secondaryLong0C;
    int secondaryMedium10;
    int secondaryShort14;
    int protocolValue18;
};

typedef char PlayerPatternConfigSizeIs1C[
    (sizeof(PlayerPatternConfig) == 0x1C) ? 1 : -1];
