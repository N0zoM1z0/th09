#pragma once

#include <stddef.h>

typedef int (*ChainCallback)(void *);
typedef int (*ChainLifetimeCallback)(void *);

class ChainElem
{
  public:
    short priority;
    unsigned short isHeapAllocated : 1;
    ChainCallback callback;
    ChainLifetimeCallback addedCallback;
    ChainLifetimeCallback deletedCallback;
    ChainElem *prev;
    ChainElem *next;
    ChainElem *releaseTarget;
    void *arg;
};

typedef char ChainElemSizeIs20[(sizeof(ChainElem) == 0x20) ? 1 : -1];
typedef char ChainElemReleaseTargetAt18[
    (offsetof(ChainElem, releaseTarget) == 0x18) ? 1 : -1];

class Chain
{
  private:
    ChainElem calcChain;
    ChainElem drawChain;

  public:
    int AddToCalcChain(ChainElem *elem, int priority);
    int AddToDrawChain(ChainElem *elem, int priority);
};

typedef char ChainSizeIs40[(sizeof(Chain) == 0x40) ? 1 : -1];
