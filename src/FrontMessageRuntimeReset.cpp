#include "FrontMessageRuntimeView.hpp"

#include <string.h>

int FrontMessageRuntimeView::Reset()
{
    memset(this, 0, sizeof(*this));
    currentIndex08 = -1;
    return 0;
}
