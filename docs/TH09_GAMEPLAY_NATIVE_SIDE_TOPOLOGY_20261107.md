# TH09 gameplay setup: playable sides and third shared-effect record — 2026-11-07

This checkpoint is bound to the verified Japanese TH09 1.50a original PE,
SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
It edits the complete source of the still-unmatched GameplaySetupThread as
well as the exact OnUpdate, DeletedCallback, and CleanupGameplayState owners.

## Two playable sides inside three physical records

The native `AsciiGameManagerView` has **two 0x38-byte side records** at
+0x00 and +0x38. The real `GameConfiguration::sideFocusMode` has **two
bytes**, one per playable side. The setup/teardown view occupies the same
native manager allocation but spans **three 0x38-byte physical records**;
record 2 begins at +0x70, and its shared `EffectManager *` at +0x7C is
the observed third runtime Effect subsystem. Its remaining fields are not
asserted to behave like an additional playable side.

`GameManagerSetupView.hpp` now derives the physical array as the native
playable count plus one. `GameplaySetupNativeSides.hpp` independently
checks both native source arrays, the configuration side-mode extent, the
third-record address, and its adjacency to the shared gameplay subsystem
at +0xA8. The full `GameManagerSetup.cpp` now uses the derived playable
count in its four loops and in DeletedCallback's two-side release, but
uses the dedicated third-record index solely for the shared EffectManager
create/reset/release/clear. Existing failure jumps and resource cleanup
order are not changed. These source names are reconstruction views, not
claims of historic C++ field spellings.

## Scoped original-bound results

Pinned VC7.1 source-cold replays of the changed setup header's actual
includers cover two GameManagerSetup compiler profiles plus SupervisorUpdate.
All **12 complete exact functions** matched the SHA-pinned original,
**3,987 complete physical bytes and 335 true relocation fields**. The
full 1,689-byte original GameplaySetupThread has a 1,682-byte compiled
candidate, 172 independently bound fields, 35 ordered calls, 694 unequal
overlapping byte positions, and seven missing target bytes. This remains
**nonexact**, with no newly claimed authored exact byte credit.

The fresh SupervisorUpdate object exposed 40 compiler-private symbol-name
changes across its original 996-byte physical owner (including the code
and table tail). A field-by-field independent audit verified **92 actual
relocation fields**, each same-section symbol offset, expected PE operand,
original VA, type and addend. Every one of those 40 changes affected
only the private name, not a destination; the strict comparator was left
unchanged and all 12 functions replayed. Ignored evidence:
`.analysis/gpt-web-setup-native-sides-20261107/` including
`supervisor-label-proof.json`, independent build logs and boss diagnostics.

No whole-product cold replay, runtime/semantic acceptance, remote publication,
or Factory exactness certification is asserted. The overall ledger remains
1,034 source-present / 1,021 exact / 13 nonexact.
