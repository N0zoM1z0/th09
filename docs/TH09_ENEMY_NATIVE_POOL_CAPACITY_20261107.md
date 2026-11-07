# TH09 Enemy native active pool and terminal sentinel — 2026-11-07

This is a source-level reconstruction checkpoint against the SHA-256-pinned
Japanese 1.50a original (`10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`).
It does **not** claim that the large Enemy or RunEcl boss became exact, or
that local Git commits constitute Factory acceptance or a runnable product.

## One physical allocation, two scan roles

`EnemyManagerView::enemies5758` starts at native manager +0x5758 and contains
129 genuine 0x5430-byte `EnemyView` records. The following `remoteSlots2AC388`
starts exactly after the full array. Original create/spawn, clear, find, and
release paths traverse only the first 128 records. The last allocated record
at manager +0x2A6F58 is a full-pool sentinel rather than an active enemy.
Failure during `RunEcl` initialization can also set the failure flag and
return an earlier attempted slot: do not equate every failure pointer with
`&enemies5758[128]`.

`src/EnemyNativePoolCapacity.hpp` now derives `PhysicalSlots`, `ActiveSlots`,
and `FailureSentinelIndex` directly from the native declared array and checks
its physical offsets and adjacent field boundary at compilation. The 3,900-byte
`EnemyManagerView::OnUpdate` preserves its original signed loop bound but
shares the typed `ActiveSlots` with the exact kill-all, lookup, release/reset,
spawn-enemy and card-attack spawn-record owners. Both independent spawn
functions now use the same failure index for their loop, ECL-failure branch,
and status publication. The source-facing type name remains provisional.

## Scoped original-bound validation

After changing the shared bounds, a pinned VC7.1 compile of the complete ECL
source carrier retains the **3,900-byte Enemy update / 97 relocations /
69 calls / 13 ordinary byte differences**, plus the exact 82-byte collision
append leaf. The separate 15,564-byte RunEcl still retains **598 relocations,
zero table/field differences, 30 ordinary byte differences** split between
opcode 4 (7) and opcode 155 (23). These open owners receive no exact credit.

Cold-rebuilt original-bound checks passed **7 complete exact functions,
1,510 bytes and 40 fields** across the separate native manager
clear/lookup/release/spawn carriers. Two ECL compiler profiles independently
passed **83 complete existing exact functions, 29,856 physical bytes and
1,389 fields**. These are checks of existing exact owners, not new authored
functions. Only affected source carriers were rebuilt; no full cold replay.

The refreshed ECL object exposed two stale compiler-private names. For
`ecl-post-update-movement` field +0x139, `$L76152` became `$L76177`,
with its independently checked original destination fixed at 0x00408548.
For `ecl-compare-operands` field +0x14, `$L75333` became `$L75358`,
with destination fixed at 0x00407A4C. Both actual same-owner symbol offsets,
raw original PE operands, addends, field types and offsets were checked.
The verifier audited 77 ECL units, found only those two name changes, and
found no changed owner-local destination; the strict comparator was not
weakened. Retained private proof: `.analysis/gpt-web-enemy-natural-20261107/ecl-label-proof.json`.

## Negative natural C++ controls

Ten local Enemy OnUpdate variants tested ordinary draw-index, side-count,
threshold and descriptor-copy expressions under the unchanged compiler
profile. The valid equivalent forms preserved the 13 differences; direct
component/constructor rewrites broke previously exact positions and one
reversed Timer comparison was ill-typed. Ten independent SaveReplay header-copy
variants also failed to improve its two remaining bytes; changing copy/write
chronology regressed eleven additional bytes. All experimental source changes
were restored; retained controls and reports are under
`.analysis/gpt-web-enemy-natural-20261107/` and
`.analysis/gpt-web-replaysave-natural-20261107/`. These bounded observations
are not proof that other natural source models cannot match.

Ledger total remains 1,034 source-present, 1,021 exact, 13 non-exact.
