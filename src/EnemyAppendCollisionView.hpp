#pragma once

#include <stddef.h>

struct EnemyManagerView;
struct Float3;

// TH09 original EnemyManager::OnUpdate calls this thiscall leaf twice with
// the real enemy as ECX and the position/size pointers on the stack. Its only
// stored member is the EnemyManager receiver at enemy offset 0x00.
// This is a shared 32-bit interface view, not original C++ class ownership.
struct EnemyAppendCollisionView
{
    EnemyManagerView *manager00;

    void AppendPlayerCollisionBox(
        const Float3 *position,
        const Float3 *size);
};
typedef char EnemyAppendCollisionViewSizeIs04[
    (sizeof(EnemyAppendCollisionView) == 0x04) ? 1 : -1];
typedef char EnemyAppendCollisionManagerAt00[
    (offsetof(EnemyAppendCollisionView, manager00) == 0) ? 1 : -1];
