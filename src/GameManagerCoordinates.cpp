#include "AsciiGameManagerView.hpp"
#include "Supervisor.hpp"

float AsciiGameManagerView::TransformPopupX(float value)
{
    return (float)g_Supervisor.renderCoordinateOrigin348->x + value - this->coordinateOffsetX;
}

float AsciiGameManagerView::TransformPopupY(float value)
{
    return (float)g_Supervisor.renderCoordinateOrigin348->y + value - this->coordinateOffsetY;
}

float AsciiGameManagerView::InverseTransformPopupX(float value)
{
    return value - (float)g_Supervisor.renderCoordinateOrigin348->x + this->coordinateOffsetX;
}

float AsciiGameManagerView::InverseTransformPopupY(float value)
{
    return value - (float)g_Supervisor.renderCoordinateOrigin348->y + this->coordinateOffsetY;
}
