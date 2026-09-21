#include "ExAttackInterpolation.hpp"

void __fastcall ExAttackInterpolate2D(
    float *out,
    float *endPoint,
    float *startPoint,
    float *endTangent,
    float *startTangent,
    float time,
    float duration)
{
    float t = time / duration;
    float startWeight =
        (t - 1.0f) * (t - 1.0f) * (2.0f * t + 1.0f);
    float endWeight = t * t * (3.0f - 2.0f * t);
    float startTangentWeight = (1.0f - t) * (1.0f - t) * t;
    float endTangentWeight = (t - 1.0f) * t * t;

    out[0] =
        endWeight * endPoint[0] + startWeight * startPoint[0] +
        endTangentWeight * endTangent[0] +
        startTangentWeight * startTangent[0];
    out[1] =
        endWeight * endPoint[1] + startWeight * startPoint[1] +
        endTangentWeight * endTangent[1] +
        startTangentWeight * startTangent[1];
}
