#include <string.h>
#include <stddef.h>

struct FrontMessageRuntimeResetView
{
    unsigned char storage000[0x08];
    int currentIndex008;
    unsigned char storage00C[0x1D70 - 0x0C];

    int Reset();
};

typedef char FrontMessageRuntimeResetSize1D70[
    (sizeof(FrontMessageRuntimeResetView) == 0x1D70) ? 1 : -1];
typedef char FrontMessageRuntimeCurrentAt008[
    (offsetof(FrontMessageRuntimeResetView, currentIndex008) == 0x08) ? 1 : -1];

int FrontMessageRuntimeResetView::Reset()
{
    memset(this, 0, sizeof(*this));
    currentIndex008 = -1;
    return 0;
}
