#include <math.h>
struct EffectPolarScaleView
{
    float x;
    float y;
    EffectPolarScaleView *SetFromAngleAxes(float angle, float xScale, float yScale);
};
EffectPolarScaleView *EffectPolarScaleView::SetFromAngleAxes(
    float angle, float xScale, float yScale)
{
    x = cosf(angle) * xScale;
    y = sinf(angle) * yScale;
    return this;
}
