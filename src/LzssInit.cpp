#include "Lzss.hpp"

struct LzssTreeNode
{
    int parent;
    int left;
    int right;
};

extern LzssTreeNode g_LzssTree[0x2001];
extern unsigned char g_LzssDictionary[0x2000];

void Lzss::InitTree(int root)
{
    g_LzssTree[0x2000].right = root;
    g_LzssTree[root].parent = 0x2000;
    g_LzssTree[root].right = 0;
    g_LzssTree[root].left = 0;
}

void Lzss::InitEncoderState()
{
    int i;

    for (i = 0; i < 0x2000; i++)
        g_LzssDictionary[i] = 0;

    for (i = 0; i < 0x2001; i++)
    {
        g_LzssTree[i].parent = 0;
        g_LzssTree[i].left = 0;
        g_LzssTree[i].right = 0;
    }
}
