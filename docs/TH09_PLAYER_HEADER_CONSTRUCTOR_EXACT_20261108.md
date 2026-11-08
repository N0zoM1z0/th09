# TH09 newly source-bound exact 28-byte Player Header constructor

The immutable target is Japanese TH09 v1.50a, original retail i386 PE
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This document reports a **newly tracked exact constructor**, not a
whole-game executable, runtime acceptance or retroactive proof of the
original programmer's explicit versus implicit special-member syntax.

## Original 0x0041EE00 is a three-timer class constructor

The complete 28-byte target function at 0x0041EE00 is called at offset
+0x07 by the independently exact **243-byte Player constructor** at
0x0041F070. It receives the real `PlayerHeaderStateView` in ECX, saves
its receiver, calls the independently exact `ZunTimer::ZunTimer`
**three times** on the native header +0x00, +0x5C and +0x68 subobjects,
returns `this`, and has an ordinary terminal `ret` at 0x0041EE1B.
The target has three REL32 fields at offsets **+0x04/+0x0C/+0x14**,
all pointing to the original exact timer constructor **0x00403390**.
There are no jump tables or unexplained tail bytes in this owner.

The real `PlayerHeaderStateView` already contains all three native
`ZunTimer` values, its flags at +0x74, header self-link at +0x78 and
unchanged 0x9C-byte physical extent. With those actual constructed
members, a natural standalone C++ source defining an empty out-of-line
`PlayerHeaderStateView::PlayerHeaderStateView()` automatically produces
the complete original constructor under the pinned retail VC7.1
**`/O2 /Ob0 /Oy- /Gr`** profile; no assembly, byte injection, dummy
register state, volatile operations or compiler-hint tricks are used.
A separate copy-on-write `/Ob1` probe independently matched the same
28 bytes, confirming this is ordinary natural C++ constructor codegen
rather than fragile source forcing.

## New canonical source mapping and exactness scope

`src/PlayerHeaderConstructor.cpp` is now the independently maintained
source for **`player-header-constructor`** in `config/match-units.toml`.
The unit is reproducible by `scripts/build-match-unit.py --unit
player-header-constructor` and the strict original-bound
`compare-coff-function.py` unit. Both return **28/28 exact original
physical bytes and three precisely bound relocation fields**. The
source presence, exact match, reccmp and game-specific origin ledgers
are now explicitly mapped to original 0x0041EE00 and the unique
complete 28-byte extent. Existing Timer constructor and Player caller
units stay unchanged.

Original-source provenance remains **ambiguous**: the retail executable
cannot distinguish whether the game's C++ explicitly declared an empty
out-of-line Header constructor or obtained identical code from the
compiler's implicit special-member generation. We classify the
*game-specific reconstructed C++ class owner* as authored-game for
tracking, not the *human-written provenance* of those machine bytes.
The special-member provenance uncertainty is retained in the function
ledger. Generic CRT/compiler library stubs are not reclassified.

A fresh, four-source-carrier original-bound replay of the new 28-byte
constructor, the full PlayerRuntime owners, the exact 29-byte header
Reset, the 572-byte physical initializer and the independent ZunTimer
constructor passes **27 complete exact functions, 6,096 physical
bytes and 322 relocations**. The larger original-bound Player/ECL
source closure from the prior structural checkpoint remains **121
preexisting exact owners, 40,996 bytes and 2,068 fields**, without a
full cold replay.

This checkpoint increases the tracked source-present total from
**1,034 to 1,035**, complete byte-exact count from **1,021 to 1,022**;
the **13 nonexact** large functions and their **29,834 physical bytes**
remain unchanged. The distinct 3,900-byte Enemy OnUpdate, 862-byte
Player collision and 15,564-byte RunEcl residuals remain open.

Private reproducible source-cold objects and independent dual-profile
constructor evidence:
`.analysis/gpt-web-player-header-three-timers-20261108/`.
No Windows product link, full-project cold replay, runtime acceptance,
Factory receipt or remote push is claimed.
