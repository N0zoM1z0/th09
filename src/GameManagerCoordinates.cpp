#include "AsciiGameManagerView.hpp"

struct RenderCoordinateOriginView
{
    unsigned char unknown000[0xCC];
    unsigned int x;
    unsigned int y;
};

extern RenderCoordinateOriginView *g_RenderCoordinateOrigin;

float AsciiGameManagerView::TransformPopupX(float value)
{
    return (float)g_RenderCoordinateOrigin->x + value - this->coordinateOffsetX;
}

float AsciiGameManagerView::TransformPopupY(float value)
{
    return (float)g_RenderCoordinateOrigin->y + value - this->coordinateOffsetY;
}
