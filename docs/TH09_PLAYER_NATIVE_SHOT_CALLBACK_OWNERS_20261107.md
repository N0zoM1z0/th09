# Native Player shot and SHT descriptors across three exact callbacks

Original target: Japanese TH09 v1.50a, SHA-256 verified as
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
The original C++ nominal types, TU partition and linker folding are not
uniquely established by source-facing names. This checkpoint makes real
ordinary C++ ownership and runtime field offsets explicit while protecting
existing complete original-exact functions.

## Three independent original callbacks, one native shot allocation

The original complete 471-byte `PlayerShotUpdateCallbackType1`
(0x004415E0), 248-byte `PlayerShotUpdateCallbackType3` (0x00443150),
and 139-byte `PlayerShotSpawnCallbackType7` (0x0044AB90) use **the same
native PlayerShotCtorView** with its 0x484-byte stride, position at +0x2A4,
velocity x/y at +0x43C/+0x440, speed at +0x44C, angle at +0x450,
ZunTimer at +0x454 and state at +0x462. Their previous source files
contained independent shot prefix structs, two private timer classes and
a duplicate three-float position class. These shadow declarations are
removed; the real Player shot constructor and update/draw leaf owners all
use the same existing native record.

The two update callbacks call the real `ZunTimer::operator int()` with
its reviewed original **0x00435F00** target. This original four-byte
folded getter has several independently exact source-facing identities,
so the new source spelling is not evidence of unique historic ownership.
Update Type1 now borrows the existing `PlayerPositionView::operator float*`
at **0x004343D0**, replacing its duplicate Vec3 member signature. Neither
callee target/address, x87 argument flow nor the exact normalization and
tracking logic changed.

The native 0x38-byte `PlayerShtDescriptorView` now explicitly declares its
+0x14 float angle and +0x18 float speed, in addition to its existing
+0x00 signed interval and +0x28..+0x34 callback words. Spawn Type7 now
accepts a real native SHT descriptor and shot, checks the same interval,
performs the original exact forward-angle call, and publishes velocity
and angle on the genuine shot. The original callback tables still identify
these functions by their original addresses; C++ function-symbol changes
alone do not justify retargeting the SHT table.

## Complete immutable PE/COFF proof, including large owner closure

Before adoption, **three independent fixed-profile VC7.1 natural C++
experiments** compiled the full functions and compared their raw bytes
against the previous exact implementations: **471/471, 248/248, 139/139
bytes unchanged**. All real COFF relocation positions, types and addends
were identical. The only semantic symbol spelling differences in the
update callbacks were the **two exact timer conversions** and the one
exact Player position conversion, at fixed original target addresses.

After adoption, strict original PE-bound comparison passes all three
complete functions and their original literals/fields. Source-cold scoped
replay of the associated EclManager, PlayerRuntime, three callback and
PlayerSpawnShots carriers passes **108 exact functions / 36,295 complete
physical bytes / 1,711 relocation fields**. The header's visibility
shifted **two ECL and 17 PlayerRuntime compiler-private `$L` names**;
actual same-section COFF label definitions and immutable original PE
operand targets were checked before rebinding only those names in the
canonical manifest. No original function target, field addend, compiler
profile or comparator strictness changed. Original-bound evidence lives
under `.analysis/gpt-web-native-shot-callbacks-20261107/`.

The 3,900-byte Enemy OnUpdate boss still has its original 97 fields,
69 calls and 13 ordinary unlike bytes; the 1,210-byte ChargeAttack boss
remains nonexact. This checkpoint grants **no new exact function credit**.
The overall ledger remains **1,034 source-present / 1,021 exact / 13
nonexact**, with no full cold replay, Windows product link, runtime
semantic acceptance, Factory receipt or remote push claimed.
