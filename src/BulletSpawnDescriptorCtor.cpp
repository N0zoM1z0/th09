#include <stddef.h>
#include <string.h>

struct BulletSpawnDescriptorCtorView
{
    unsigned char unknown000[0x204];
    int transformSound204;
    unsigned char unknown208[0x0C];

    BulletSpawnDescriptorCtorView();
};

typedef char BulletSpawnDescriptorCtorViewSizeIs214[
    (sizeof(BulletSpawnDescriptorCtorView) == 0x214) ? 1 : -1];
typedef char BulletSpawnDescriptorCtorSoundAt204[
    (offsetof(BulletSpawnDescriptorCtorView, transformSound204) == 0x204) ? 1 : -1];

BulletSpawnDescriptorCtorView::BulletSpawnDescriptorCtorView()
{
    memset(this, 0, sizeof(*this));
    transformSound204 = -1;
}
