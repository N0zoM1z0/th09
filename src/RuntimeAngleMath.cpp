#include <math.h>

typedef float f32;
typedef int i32;

#define ZUN_PI ((f32)(3.14159265358979323846))
#define ZUN_2PI ((f32)(ZUN_PI * 2.0f))

namespace Th09EclRunControl
{
f32 __stdcall VectorAngle(f32 y, f32 x)
{
    return (f32)atan2(y, x);
}
}

f32 AddNormalizeAngle(f32 a, f32 b)
{
    i32 i;

    i = 0;
    a += b;
    while (a > ZUN_PI)
    {
        a -= ZUN_2PI;
        if (i++ > 16)
            break;
    }
    while (a < -ZUN_PI)
    {
        a += ZUN_2PI;
        if (i++ > 16)
            break;
    }
    return a;
}
