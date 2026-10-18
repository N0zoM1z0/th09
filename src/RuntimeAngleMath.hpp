#pragma once

// The genuine TH09 v1.50a original 0x0042AED0 math runtime member:
// two float arguments on the stack, x87 return and callee ret 8.
// __fastcall on the reviewed /Gr VC7.1 profile retains those physical args.
float __fastcall AddNormalizeAngle(float angle, float delta);
