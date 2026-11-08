# Exact 12-byte TH09 Player +0x3031C state constructor and native shot timer

Original Japanese TH09 v1.50a PE independently verified by SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint adds an independently reproducible source-exact owner. It
does not prove original historic C++ explicit versus implicit special-
member spelling, complete product linking or runtime scenario acceptance.

## Re-examine the original physical state construction

The original **12-byte subroutine at 0x0041EE20** is reached by the
already-exact **243-byte `PlayerLifecycleView` constructor at 0x41F070**,
via its original +0x7B REL32 field. It receives the native 0x1C-byte
Player state at **Player +0x3031C** in ECX, calls the original exact
`ZunTimer::ZunTimer @0x00403390` on the *receiver itself*, and returns
`this`. This is exactly one 12-byte natural VC7.1 special-member
constructor; target extent ends with `ret` at 0x41EE2B and contains no
owned post-return data.

Previous maintained source represented the first 0x10 bytes of
`PlayerState3031CView` as opaque padding despite a separate exact
151-byte `PlayerShotSpawnCallbackType1` source independently publishing
a timer value at state +0x00 and a shot pointer at state +0x0C.
The actual shared native Player subobject is now a
`ZunTimer timer00` at **+0x00**, a `PlayerShotCtorView *shot0C` at
**+0x0C**, state word +0x10, an unchanged 4-byte gap +0x14 and
transition counter +0x18. The enclosing state remains **0x1C bytes**
and Player remains **0x30F70 bytes**. The full Player OnUpdate still
uses the original +0x10/+0x18 fields; the exact shot callback continues
writing the original timer and shot-pointer positions. No extra timer
ctor or new allocation was introduced.

## Pinned compiler Oracle, original code and source ledger

With these genuine native members, an ordinary empty out-of-line
`PlayerState3031CView::PlayerState3031CView() {}` under the original
pinned VC7.1 `/O2 /Ob0 /Gr` source profile emits **12/12 original
machine bytes**, with the exact single REL32 field **+0x04 to
0x00403390**. The standalone source `src/PlayerState3031CConstructor.cpp`
is registered as canonical unit `player-state3031c-constructor`, with
all required source, origin, exact-match and reccmp tracking rows. The
full 243-byte outer Player ctor also remains original-exact and calls
the same `0x41EE20` target. No instruction patching, assembly, volatile,
register hints or arbitrary compiler-profile selection was used.

This is a known game-specific class constructor, classified as an
*authored-game source owner* for tracking, but the original binary cannot
distinguish an explicitly written empty constructor from the compiler's
identical implicit special member. We retain that historical provenance
unknown in the function ledger.

A focused full source-cold replay of native state ctor, PlayerRuntime,
PlayerShotSpawnCallback1, the exact AddedState and Reward owners, and
ECL exact siblings preserves **111 full exact functions / 37,615
physical bytes / 1,801 original-bound relocation fields**. The
larger Player and ECL translation units required **17 PlayerRuntime/
AddedState, six PlayerReward and two ECL compiler-private `$L` name
updates**. Actual same-section COFF definitions and immutable original
PE operands were verified before changing *only* label spellings in the
manifest. The separate original **151-byte shot callback** is exact.

Nonexact bosses remain separate: Player `CheckBulletCollision`
862 original/849 candidate with 13 missing bytes, Enemy OnUpdate
3,900 target/candidate with 13 ordinary unlike bytes, and RunEcl
15,564 bytes with 30 unlike opcode bytes. No open boss was falsely
promoted. The global ledger now has **1,037 source-present / 1,024
canonical exact / 13 nonexact** functions. All isolated source and
original-PE receipts are retained at
`.analysis/gpt-web-player-native-state3031c-ctor-20261108/`.

No whole-project cold replay, native Windows game link, semantic runtime
acceptance, Factory receipt or remote Git push is claimed.
