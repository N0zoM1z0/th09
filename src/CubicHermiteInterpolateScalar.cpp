float __stdcall CubicHermiteInterpolateScalar(
    float startValue,
    float endValue,
    float startTangent,
    float endTangent,
    float time)
{
    float startWeight =
        (time - 1.0f) * (time - 1.0f) * (2.0f * time + 1.0f);
    float endWeight = time * time * (3.0f - 2.0f * time);
    float startTangentWeight = (1.0f - time) * (1.0f - time) * time;
    float endTangentWeight = (time - 1.0f) * time * time;
    return startWeight * startValue + endWeight * endValue +
           startTangentWeight * startTangent +
           endTangentWeight * endTangent;
}
