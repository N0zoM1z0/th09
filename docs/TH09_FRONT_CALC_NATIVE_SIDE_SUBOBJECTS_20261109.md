# FrontCalc uses constructed side subsystems rather than duplicate owners

The original TH09 v1.50a FrontCalc callback at `0x417630` (1,491 bytes)
accesses real per-side `Background`, `CardAttack`, `PlayerLifecycleView`,
and `FrontSide` subobjects at the established +0x00/+0x14/+0x04/+0x18
positions of the GameManager side record. Its previous local source
redeclared these as four disconnected prefix objects. The source now uses
the native classes and their actual C++ members:

- `Background::transitionMode` and `transitionFrames` at +0x1C/+0x20;
- `CardAttack::timer98` at +0x98;
- `PlayerLifecycleView::ownerState30410.Reset()` instead of an unnamed
  0x30410-byte prefix with a fake Reset method;
- `FrontSide::BeginAuxTransition()` directly on the side's HUD pointer.

The full pinned VC7.1 compile retains **1,491/1,491 original bytes** and
**107/107 original relocations**. The two changed member Reset symbols
at original-relative +0x39E/+0x3AF resolve to the same original PE
method destination `0x41D7E0` (whose separate original-bound owner is
already exact). All other callee symbols, code size and literal operands
remain unchanged. Forty-three scoped Front/Player/CardAttack tests pass.

This changes the C++ storage identity of existing exact code, not the
canonical authored exact count. The remaining local side-runtime view is
not yet promoted to the full original GameManager type.
