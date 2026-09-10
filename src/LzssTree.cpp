#include "Lzss.hpp"

#define LZSS_DICTSIZE 0x2000
#define LZSS_DICTSIZE_MASK 0x1FFF
#define LZSS_LOOKAHEAD_MAX 18
#define LZSS_DICTPOS_MOD(pos, amount) (((pos) + (amount)) & LZSS_DICTSIZE_MASK)

struct LzssTreeNode
{
    int parent;
    int left;
    int right;
};

extern LzssTreeNode g_LzssTree[0x2001];
extern unsigned char g_LzssDictionary[0x2000];

void Lzss::ContractNode(int oldNode, int newNode)
{
    g_LzssTree[newNode].parent = g_LzssTree[oldNode].parent;

    if (g_LzssTree[g_LzssTree[oldNode].parent].right == oldNode)
        g_LzssTree[g_LzssTree[oldNode].parent].right = newNode;
    else
        g_LzssTree[g_LzssTree[oldNode].parent].left = newNode;

    g_LzssTree[oldNode].parent = 0;
}

void Lzss::ReplaceNode(int oldNode, int newNode)
{
    int parent = g_LzssTree[oldNode].parent;

    if (g_LzssTree[parent].left == oldNode)
        g_LzssTree[parent].left = newNode;
    else
        g_LzssTree[parent].right = newNode;

    g_LzssTree[newNode] = g_LzssTree[oldNode];
    g_LzssTree[g_LzssTree[newNode].left].parent = newNode;
    g_LzssTree[g_LzssTree[newNode].right].parent = newNode;
    g_LzssTree[oldNode].parent = 0;
}

int Lzss::FindNextNode(int node)
{
    int next = g_LzssTree[node].left;

    while (g_LzssTree[next].right != 0)
        next = g_LzssTree[next].right;

    return next;
}

int Lzss::AddString(int newNode, int *matchPosition)
{
    int i;
    int *child;
    int delta;

    if (newNode == 0)
        return 0;

    int testNode = g_LzssTree[LZSS_DICTSIZE].right;
    int matchLength = 0;

    for (;;)
    {
        for (i = 0; i < LZSS_LOOKAHEAD_MAX; i++)
        {
            delta = g_LzssDictionary[LZSS_DICTPOS_MOD(newNode, i)] -
                    g_LzssDictionary[LZSS_DICTPOS_MOD(testNode, i)];
            if (delta != 0)
                break;
        }

        if (i >= matchLength)
        {
            matchLength = i;
            *matchPosition = testNode;
            if (matchLength >= LZSS_LOOKAHEAD_MAX)
            {
                ReplaceNode(testNode, newNode);
                return matchLength;
            }
        }

        if (delta >= 0)
            child = &g_LzssTree[testNode].right;
        else
            child = &g_LzssTree[testNode].left;

        if (*child == 0)
        {
            *child = newNode;
            g_LzssTree[newNode].parent = testNode;
            g_LzssTree[newNode].right = 0;
            g_LzssTree[newNode].left = 0;
            return matchLength;
        }

        testNode = *child;
    }
}

void Lzss::DeleteString(int p)
{
    if (g_LzssTree[p].parent == 0)
        return;

    if (g_LzssTree[p].right == 0)
        ContractNode(p, g_LzssTree[p].left);
    else if (g_LzssTree[p].left == 0)
        ContractNode(p, g_LzssTree[p].right);
    else
    {
        int replacement = FindNextNode(p);
        DeleteString(replacement);
        ReplaceNode(p, replacement);
    }
}
