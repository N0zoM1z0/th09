# Native Supervisor replaces PlayerRuntime's duplicate viewport/frame-rate owner

Original: SHA-256-pinned Japanese TH09 1.50a PE
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This checkpoint restores natural C++ receiver identity while retaining the
original compiler output, not newly earned byte-exact source credit.

## One actual 0x7B0-byte Supervisor, no second Player draw/frame-rate object

`PlayerRuntime.cpp` previously declared `PlayerSupervisorRuntimeView`,
which looked like a *separate* Supervisor method owner with a fabricated
0x5B8-byte padding span, a `framerateMultiplier5B8` field, three drawing
methods and a `g_PlayerSupervisorRuntime` global. All these source-level
references actually point to the real 0x7B0-byte `Supervisor` global at
**0x004B3100**, whose +0x5B8 frame-speed field is independently read by
BulletManager, ECL, PlayerMovement, GameManagerCutChain and ZunTimer.

The complete Player update, before-state dispatch, high/low-priority draw
and 251-byte shot update now use `g_Supervisor` directly. Their real exact
callee identities are `Supervisor::SelectSide` **0x00401440**,
`Supervisor::ConfigureGameplayViewport` **0x00401390** and
`Supervisor::ConfigureScreenViewport` **0x00403D90**. The old
`PreparePlayerDraw` spelling corresponded to that last exact *screen*
viewport method; the original target already has a genuine independent
71-byte implementation. Two UpdateShots references use the actual
**`g_Supervisor` base +0x5B8**, represented by the original COFF DIR32
symbol with its true `+0x5B8` addend. No new viewport, side state,
additional Supervisor allocation or script-compiled register hint exists.

## Whole-function and large Player caller closure

An isolated copy-on-write VC7.1 experiment compiled the complete
**522-byte Player OnUpdate, 852-byte physical UpdateBeforeState, 251-byte
UpdateShots, 166-byte OnDrawHighPrio, and 31-byte OnDrawLowPrio** functions.
Every unchanged original candidate byte and complete original COFF
field offset/type/addend survived the migration; only the reviewed native
external C++ symbol names changed, with original target VAs intact.

The adopted source was independently replayed under its actual original
compiler profiles: **21 complete exact functions / 5,391 physical bytes /
288 original-bound relocation fields** remain exact. The included full
Player update/initialization source owners caused **17 compiler-private
`$L` spellings** to shift. Actual same-section COFF definitions and their
owner-relative offsets were checked against the immutable original PE
operand destinations before only those labels were rebound in the strict
manifest. The three native Supervisor viewport/side method providers and
other viewport siblings were separately replayed: **eight complete exact
functions / 1,530 physical bytes / 106 fields**.

The separately open **862-byte `PlayerLifecycleView::CheckBulletCollision`
boss** is still a **849-byte candidate with 26 original-bound fields,
12 ordered calls, 799 unlike overlapping bytes and 13 missing bytes**;
this source improvement does not credit its partial bytes or alter its
control-flow deficit.

Original-PE and trial evidence lives in
`.analysis/gpt-web-player-native-supervisor-20261108/`. No full cold replay,
Windows product link, runtime gameplay verification, Factory exactness
receipt or remote Git push is claimed. Global ledger remains
**1,034 source-present / 1,021 exact / 13 nonexact**.
