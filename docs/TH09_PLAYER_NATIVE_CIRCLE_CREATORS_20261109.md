# Type-1 and type-4 collision circles belong to native Player

The original TH09 1.50a circle producers at 0x41CFE0 and 0x41D0D0
operate on the same native Player allocation as the state-3 effects and
FrontCalc's stage-transition setup. `PlayerLifecycleView` now owns the
natural C++ `CreateCircleType1` and `CreateCircleType4` methods, rather
than declaring those same two receivers on an independent prefix type.
The allocator remains a separately maintained prefix projection for now.

Both complete method bodies are original-exact (43 and 49 bytes, one
relocation each). The three state-3 effect functions (91, 91, 100 bytes)
and full 1,491-byte FrontCalc callback also remain exact, preserving all
five target-cold call sites in those functions: ten original REL32
operands were individually checked against the original PE destinations
0x41CFE0 / 0x41D0D0 before their C++ method symbols were rebound.

All 14 exact owners in the directly affected producer, state-3, FrontCalc
and Player selector source files match full original bytes/relocations.
Focused Player/region, native Player (55 cases), FrontMessage (31 cases),
and bullet-collision tests pass. These functions were already canonical
exact and this source ownership correction does not increase that total.
