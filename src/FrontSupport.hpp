#pragma once

struct Front
{
    unsigned char storage[0x1293C];

    int Reset();
};

typedef char FrontSizeIs1293C[(sizeof(Front) == 0x1293C) ? 1 : -1];
