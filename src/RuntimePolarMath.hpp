#pragma once

// The native primitive writes only x/y. Its physical i386 inputs are ECX and
// two stack floats; original nominal names and source ownership remain unknown.
struct Float2;
void __fastcall SetPolar2D(Float2 *vector, float angle, float magnitude);
