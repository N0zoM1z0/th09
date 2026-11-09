#include "FrontFactoryConstructView.hpp"

#include <string.h>

int FrontFactoryConstructView::Reset()
{
    memset(this, 0, sizeof(*this));
    return 0;
}
