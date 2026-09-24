struct EffectPolarScaleView
{
    float x;
    float y;
    void SetFromAngleAxes(float angle, float xScale, float yScale);
};

void EffectPolarScaleView::SetFromAngleAxes(
    float angle, float xScale, float yScale)
{
    __asm
    {
        mov eax, this
        fld angle
        fsincos
        fmul [xScale]
        fstp [eax]
        fmul [yScale]
        fstp [eax + 4]
    }
}
