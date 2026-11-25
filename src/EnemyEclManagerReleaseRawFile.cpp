#include "EnemyEclRawFileView.hpp"
#include "ZunMemory.hpp"

extern ZunMemory g_ZunMemory;

void EnemyEclManagerReleaseView::ReleaseRawFile()
{
    if (rawFile00 != 0)
        g_ZunMemory.Free(rawFile00);
    rawFile00 = 0;
}
