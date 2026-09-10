#pragma once

class Lzss
{
  public:
    static void InitTree(int root);
    static void InitEncoderState();
    static int AddString(int newNode, int *matchPosition);
    static void DeleteString(int p);
    static void ContractNode(int oldNode, int newNode);
    static void ReplaceNode(int oldNode, int newNode);
    static int FindNextNode(int node);
    static unsigned char *Encode(unsigned char *in, int inSize, int *outSize);
    static unsigned char *Decode(unsigned char *in, int inSize,
                                 unsigned char *out, int outSize);
};
