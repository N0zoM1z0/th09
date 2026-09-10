#pragma once

class Lzss
{
  public:
    static unsigned char *Decode(unsigned char *in, int inSize,
                                 unsigned char *out, int outSize);
};
