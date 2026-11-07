# TH09 complete ChargeAttack is a native Player member, not a derived overlay

Original: SHA-256-pinned Japanese TH09 v1.50a retail PE
`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
This is a natural C++ source and actual ABI reconstruction, not byte-exact
credit for an unmatched whole function, Factory acceptance, or a runtime
port of TH09.

## Recover the full charge boss ownership and complete call chain

The full **1,210-byte** original `UpdateChargeAttack` at **0x0041F810**
operates directly on the one actual `PlayerLifecycleView` allocation.
Earlier maintained code declared the method on a zero-storage
`PlayerChargeAttackView : PlayerSpawnShotsView`, then cast the actual
Player inside the complete exact 522-byte Player OnUpdate caller to invoke
it. This was a reconstruction interface, not evidence for a physically
separate charge Player object.

After separately reconstructing the real 190-byte `SpawnShots`, 51-byte
resource clamp and three 91/91/100-byte State3 Effect members on the
native Player, there is no longer a legitimate reason to expose the large
charge method through another fake subclass. `PlayerLifecycleView` now
owns its declaration and complete implementation, and `PlayerRuntime.cpp`
calls `player->UpdateChargeAttack()` as an ordinary C++ method. The
redundant `PlayerChargeAttackView.hpp` is deleted, the charge source no
longer includes its redundant zero-storage method-facade headers, and
all source-facing call sites and tracking/diagnostic symbol spellings now
share the genuine native Player C++ member. Charge attack decisions,
press/hold/release chronology, timed shot spawns, meter effect logic,
resource reductions and native GameConfiguration/Supervisor fields retain
their previous source expressions.

## Fixed compiler and independent target checks

The changed receiver was first tested in a bounded, restored-source
experiment without any compiler-profile or byte-patching modifications.
The entire **1,197-byte candidate charge function** remained byte-for-byte
identical after the C++ method identity change, including its **70 actual
relocation positions and addends**. The independent 522-byte exact Player
OnUpdate caller likewise retained all raw bytes and 40 actual fields; its
one call at function offset +0x1BE simply uses the genuine native Player
member symbol while still resolving original target 0x0041F810.

Fresh source-cold original comparison confirms that the maintained large
charge boss remains **1,210 target / 1,197 candidate bytes, 70 fully bound
fields, 58 original ordered calls, 1,160 unlike overlapping bytes and
13 absent target bytes**. It still is **NONEXACT**; the C++ member
reconstruction does not bridge the missing source chronology or justify
any fractional match credit. The strict noncrediting inspector still
protects all previously equal ordinary bytes and the original field/
literal/call graph contracts.

The scoped exact source replay independently protects **28 existing
complete exact functions, 6,024 physical bytes and 308 fields** across
PlayerRuntime, PlayerSpawnShots, State3 effects, resource clamp and Effect35.
Two independent 852-byte and 572-byte Player helper owners exposed 17
fresh VC7.1 compiler-private `$L` names. Actual same-owner COFF symbol
offsets, original PE field values, original VAs and complete physical
bytes were audited independently before updating only those names in the
strict canonical manifest. Nothing was retargeted or relaxed. Evidence:
`.analysis/gpt-web-charge-native-boss-20261107/` including the initial
restored-source experiment, `player-label-proof.json`, full scoped exact
report and complete Charge original-bound diagnostic.

Only the involved source carriers were recompiled. Whole-product Windows
linking, semantic/runtime execution, full cold replay, remote Git push
and Factory acceptance remain open. Source-present/exact/nonexact totals
remain **1,034 / 1,021 / 13**.
