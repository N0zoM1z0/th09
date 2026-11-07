# Shared Etama ANM slot across Effect, Etama and lifecycle (2026-11-07)

Target: original Japanese TH09 v1.50a, SHA-256
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.

## Recovered source ownership

Three game-source translation units independently use the same resource
identity: **slot 8, `etama.anm`**. Etama's complete original-exact
`EtamaController::AddedCallback` and the still-open 51-byte
`EffectManager::AddedCallback` both look up slot 8 via native
`AnmManager::GetAnm`, test the real `AnmLoaded::rawData04` resident pointer,
and conditionally call the genuine `PreloadAnm`. The separate exact Etama
startup and release leaves use the same slot for preload and release.

A small source-only `EtamaAnmResourceSlot` enum in
`src/EtamaAnmResourceSlot.hpp` now defines `ANM_FILE_SLOT_ETAMA = 8` for all
three owners. This **does not change the numeric target operand, call target,
ANM class layout, the resource-load branches, or any release semantics**. It
removes independent magic slot values from the three actual source paths.

The common `AnmManager.hpp` already declares ASCII and capture slots, but
was *not* edited: a private broader declaration of this same Etama enum
produced spurious VC7.1 compiler-local `$L...` name changes even inside
unrelated exact ECL functions. An original-bound ECL cold-build spot check
confirmed the risk. Isolating the Etama-only slot declaration protects
unaffected carrier profiles and their strict original symbol bindings.

## Scoped original-bound replay and label correction

- The 51-byte Effect AddedCallback remains explicitly **nonexact**:
  4 original-bound relocations, 19 instructions, 2 ordered calls, 3 direct
  blocks, **8 ordinary differing bytes** (all in the known entry schedule).
  Its 15 exact same-source siblings and the independent 41-byte ANM loaded
  callee remain exact: **16 comparisons, 2,571 physical bytes, 101 fields**.
- The complete Etama draw and lifecycle carriers were freshly rebuilt with
  their pinned compiler profiles. All **6 exact functions, 1,705 physical
  bytes and 78 relocation fields** match the original, including the
  **832-byte Etama AddedCallback** (not merely its smaller source size).
- The Etama carrier's two table-bearing exact owners initially failed their
  strict manifest comparison due to pre-existing VC7.1 `$L...` **names**.
  Exactly **15** names were corrected after separately checking each actual
  same-section symbol and section-relative value against the original PE
  operand at the same field offset. Original target addresses, relocation
  kind/addend, physical bytes and table destinations were untouched. An
  independent private-manifest replay of all six exact units succeeded
  before adopting the symbol spellings.

Ignored evidence: `.analysis/gptweb-effect-natural-20261107/` contains seven
natural callback codegen variants (six preserve eight entry differences and
one regresses to eleven); `.analysis/gptweb-etama-shared-slot-20261107/`
contains the exact localized field/name proof, two source-carrier build
receipts, and source-bound function results. These probes are diagnostic
only, not new exact-source acceptance.

Original-byte correctness for these bounded owners does **not** imply a
complete Windows executable build, runtime semantics, portability, Factory
acceptance, or remote publication. Global counts remain 1,034 source-present,
1,021 exact, 13 nonexact. No full cold replay was run.
