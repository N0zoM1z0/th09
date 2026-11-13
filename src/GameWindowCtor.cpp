#include "GameWindow.hpp"
#include <string.h>

// The process startup zeroes its actual native 0x44-byte GameWindow object.
GameWindow::GameWindow()
{
    memset(this, 0, sizeof(*this));
}
