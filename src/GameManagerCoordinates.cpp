#include "AsciiGameManagerView.hpp"

float AsciiGameManagerView::TransformPopupX(float value)
{
    return (float)g_RenderCoordinateOrigin->x + value - this->coordinateOffsetX;
}

float AsciiGameManagerView::TransformPopupY(float value)
{
    return (float)g_RenderCoordinateOrigin->y + value - this->coordinateOffsetY;
}
