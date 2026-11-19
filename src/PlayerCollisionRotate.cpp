#include "PlayerLifecycleView.hpp"

// The TH09 executable calls the compiler-provided single-precision math
// helpers at 0x401070 and 0x401060. Keep their real C/fastcall signatures
// visible here; including math.h would allow VC7.1 /Oi to inline FSIN/FCOS.
extern "C" float __fastcall sinf(float angle);
extern "C" float __fastcall cosf(float angle);

// Original callers pass output in ECX, input in EDX and angle on the stack.
void __fastcall RotatePlayerCollisionVector(
    PlayerPositionView *outVector,
    PlayerPositionView *point,
    float angle)
{
    float sinOut = sinf(angle);
    float cosOut = cosf(angle);
    PlayerPositionView &nativePoint = *point;

    outVector->x = cosOut * nativePoint.x - sinOut * nativePoint.y;
    float rotatedY = cosOut * nativePoint.y;
    outVector->y = rotatedY + sinOut * nativePoint.x;
}
