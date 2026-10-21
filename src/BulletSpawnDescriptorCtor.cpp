#include "BulletSpawnDescriptorCtorView.hpp"
#include <string.h>

BulletSpawnDescriptorCtorView::BulletSpawnDescriptorCtorView()
{
    memset(this, 0, sizeof(*this));
    descriptor.transformSound = -1;
}
