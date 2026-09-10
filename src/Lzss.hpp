#pragma once

class Lzss
{
  public:
    static void InitTree(int root);
    static void InitEncoderState();
    static unsigned char *Decode(unsigned char *in, int inSize,
                                 unsigned char *out, int outSize);
};
