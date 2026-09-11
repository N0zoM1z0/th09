#include "Supervisor.hpp"

struct SupervisorTimerLayout
{
    unsigned char unknown_0000[0x5B8];
    float gameSpeed;
};

void Supervisor::TickTimer(int *current, float *subFrame)
{
    if (reinterpret_cast<SupervisorTimerLayout *>(this)->gameSpeed <= 0.99f) {
        *subFrame += reinterpret_cast<SupervisorTimerLayout *>(this)->gameSpeed;
        *current = (int)*subFrame;
    } else {
        *current = *current + 1;
        *subFrame += 1.0f;
    }
}
