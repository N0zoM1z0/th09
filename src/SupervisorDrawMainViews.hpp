#pragma once

// Shared draw-state and alpha leaf declarations implemented in
// SupervisorDrawMainLeaves.cpp and consumed by SupervisorDrawMain and GameWindowRender.
struct SupervisorDrawMainAnmManagerView
{
    void ResetDrawState();
    void *GetSurface(int surfaceIndex);
};

struct SupervisorDrawMainAsciiManagerView
{
    void SetAlpha(int alpha);
};
