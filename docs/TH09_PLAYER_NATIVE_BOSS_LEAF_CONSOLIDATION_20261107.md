# TH09 native Player ChargeAttack: remove zero-storage source facades

Original retail target: SHA-256-pinned Japanese TH09 v1.50a
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is an ordinary C++ source/ABI reconstruction checkpoint, not a claim
of uniquely recovered original historic class or translation-unit spelling.

## One Player allocation and no derived empty stand-ins

The complete open 1,210-byte ChargeAttack boss at 0x0041F810, exact
190-byte SpawnShots, exact 51-byte resource clamp and three exact
91/91/100-byte State3 effects all operate on the same native
`PlayerLifecycleView` allocation. Previous stages recovered each
original method's actual C++ receiver and native shot/SHT/region fields.
Four historical helper headers remained as empty inheritance or
method-owner facades, despite contributing no object storage or now-visible
method declarations:

- `PlayerChargeAttackView.hpp` was removed in the previous boss-owner stage.
- This checkpoint removes `PlayerSpawnShotsView.hpp`,
  `PlayerResourceSubtractView.hpp`, and `PlayerStateEffectView.hpp`.

The three true source owners now include `PlayerLifecycleView.hpp` directly.
The shot producer declares its narrow native four-argument spawn callback
type locally at the actual indirect-call use site: original SHT descriptor
and Player shot fields are already declared in the genuine Player header.
There is no alternate 0x30388-byte Player, second charge ABI or fabricated
shot record. `SpawnShots`, the clamp, three State3 effects and the
`UpdateChargeAttack` big boss retain their existing call chronology.

The main Player header now independently checks the true native +0x30384
charge scalar and +0x30388 resource scalar offsets, replacing the former
empty derived `PlayerResourceAt30388` constraint. Source-facing tests now
check native members directly rather than insisting on the deleted empty
type names. Existing negative checks still prohibit duplicate receiver
casts, released-region substitutions and changes to exact callee addresses.

## Original PE source-cold proof and continued large-boss frontier

Before deletion an isolated natural C++ trial rebuilt all five exact leaf
methods and confirmed their **complete original physical bytes** unchanged:
190, 51, 91, 91, and 100 bytes. After adopting the native header and
retaining the real Player source, scoped pinned VC7.1 replays passed
**28 distinct complete exact functions / 6,024 physical bytes / 308 COFF
relocation fields** from the real PlayerRuntime, Effect35, SpawnShots,
resource and State3 source profiles. The source-header change again
renumbered 11+6 private `$L` symbol spellings in the complete 852-byte
Player UpdateBeforeState and 572-byte InitializeAddedState. Every actual
same-section COFF label location, original PE field kind, addend and target
address was checked prior to updating only those manifest symbol names;
no original destination, comparator or compiler profile was changed.
Private receipts: `.analysis/gpt-web-player-facade-cleanup-20261107/`.

The **1,210-byte ChargeAttack target** still compares against the existing
**1,197-byte candidate, 70 bound fields, 58 ordered calls, 1,160 unlike
overlapping byte positions and 13 missing bytes**. This is still one
nonexact boss; no fractional or new complete exact function is credited.
The other 12 nonexact source-present owners are untouched by this scope.

No whole-project cold replay, runtime Windows executable link, native/game
semantic acceptance, remote push or Factory acceptance is asserted.
Ledger remains **1,034 source-present / 1,021 exact / 13 nonexact**.
