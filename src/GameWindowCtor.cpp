#include <string.h>

struct GameWindowView
{
    unsigned char storage[0x44];

    GameWindowView();
};

GameWindowView::GameWindowView()
{
    memset(this, 0, sizeof(*this));
}
