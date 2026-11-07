# TH09 Effect Type35 uses genuine GameManager side and native Player

Original target: Japanese TH09 1.50a PE with SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a source-facing ABI/owner recovery, not new byte-exact coverage or
runtime acceptance.

## One physical side allocation, not an Effect-local side class

Original-bound 327-byte `EffectManager::Create` stores the actual
GameManager gameplay-side pointer in `EffectManager::sideState`. The native
`AsciiGameManagerSideView` is exactly 0x38 bytes, with a true
`PlayerLifecycleView *player04` at offset +0x04. Earlier Effect Type35
sources still independently declared an `EffectShotSideView` with an
opaque four-byte prefix and a fake PlayerSpawnShotsView member, even after
the 190-byte shot method had been correctly migrated onto the native
Player allocation.

Both Type35 callbacks now cast the published `void *sideState` to the
actual `PlayerSideStateView` alias for that GameManager side rather than
to an invented second class. Both call through its genuine Player at +0x04.
The 72-byte exact initializer reads the real Player header's
`header24.anmFile98` at native Player +0xBC; the 38-byte exact updater
calls the actual `PlayerLifecycleView::SpawnShots` at original 0x0041F4C0.
The original `EffectManager::Create` storage contract, original animation
VM layout, attack flags and effect/shot argument chronology are unchanged.

## Focused original physical bytes and source closure

The pinned VC7.1 complete original-bound comparison reproduces both
Type35 functions exactly: **72/72 and 38/38 bytes**, including their
ordered original call fields. Native `PlayerSideStateView` has the same
physical +0x04 Player offset and 0x38-byte stride, and no fabricated
third playable side is introduced. The separately exact 190-byte
SpawnShots producer and 327-byte EffectManager::Create provide an
original-bound producer/callee closure for the two original effect owners.
All source type changes are ordinary C++, with no register hints,
assembly, padding, profile changes, or target byte patching.

The actual three affected source carriers, `EffectManager.cpp`,
`EffectType35.cpp`, and `PlayerSpawnShots.cpp`, were rebuilt independently
with the pinned VC7.1 profiles and passed strict complete original-bound
comparison of **18 exact functions / 2,830 physical bytes / 104 fields**.
A separate focused Effect-manager diagnostic retained the open 51-byte
AddedCallback at its original eight-byte residual, protecting the exact
source siblings. Private receipts: `.analysis/gpt-web-effect35-native-side-20261107/`.

This work continues the actual 1,210-byte ChargeAttack call chain, now
owned by native Player. It does **not** promote the still-nonexact charge
boss or the 51-byte Effect AddedCallback frontier to exactness. Scoped
source-cold receipts and focused tests protect the related owners;
whole-program cold replay, Windows product linking, runtime equivalence,
Factory acceptance and remote Git push are outside this checkpoint.
Global status remains **1,034 source-present / 1,021 exact / 13 nonexact**.
