#include "ZunTimer.hpp"

#include <stddef.h>

struct PlayerOwnerStateResetView
{
    void *owner00;
    int state04;
    unsigned char unknown08[0x0C];
    ZunTimer timer14;
    ZunTimer timer20;
    unsigned char unknown2C[0x0C];
    int state38;
    int activeFrameCounter3C;
    void *callback40;

    void PrepareReset();
};

typedef char PlayerOwnerStateResetSizeIs44[
    (sizeof(PlayerOwnerStateResetView) == 0x44) ? 1 : -1];
typedef char PlayerOwnerStateResetTimer20At20[
    (offsetof(PlayerOwnerStateResetView, timer20) == 0x20) ? 1 : -1];
typedef char PlayerOwnerStateResetState38At38[
    (offsetof(PlayerOwnerStateResetView, state38) == 0x38) ? 1 : -1];

void PlayerOwnerStateResetView::PrepareReset()
{
    this->timer20 = 0;
    this->state04 = 0;
    this->state38 = 0;
}
