#include "FrontSupport.hpp"

#include <string.h>

int Front::Reset()
{
    memset(this, 0, sizeof(Front));
    return 0;
}
