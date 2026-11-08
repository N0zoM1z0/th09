# TH09 reconstruction session — 2026-11-08

This note records fresh live-worktree and target-attested observations from the 2026-11-08 session. It is not an acceptance claim by itself; Factory replay receipts are listed separately below.

## Reproducible baseline

- Repository: `th09`, branch `main`, source HEAD `85fb775d42c727d7c9c28cfd56f0958720ab2111`.
- Original target: TH09 1.50a Japanese PE, SHA-256 `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
- Native IDA provider `th09-ida` re-attested the target successfully.
- The live worktree was clean before and after the focused experiments. No pre-existing dirty or untracked user changes needed resolution.
- Builds were limited to the relevant EclManager translation unit and targeted function replays. No full cold replay was submitted.

## Large owner: `EnemyManagerView::OnUpdate`

Fresh build: `scripts/build-match-unit.py --unit enemy-attached-effect-update` followed by `scripts/inspect-enemy-manager-update.py build/matching/EclManager.obj --json`.

- Target and candidate extent: 3,900 bytes each; 3,883 code bytes.
- 97 relocation fields agree; 69 ordered direct calls agree; 170 reviewed direct control-flow blocks agree; switch destinations agree.
- 13 byte differences remain at owner-relative offsets:
  `0x19C, 0x1A3, 0x252, 0x263, 0x26D, 0x272, 0x294, 0x29A, 0x29C, 0x2A5, 0x2A9, 0x2B7, 0x2BD`.
- The inspector is diagnostic-only and grants no exactness credit.

New local source-shape probes were compiled and then discarded because none reduced the 13-byte frontier: draw-group pointer alias, unsigned and byte index locals, byte-offset pointer arithmetic, side-state pointer alias, direct side-state accesses, memberwise spawn-position copy, and a local `Float3` copy. No speculative variant was retained.

The current source still calls the separately reconstructed 15,564-byte `EnemyEclManagerView::RunEcl` owner. Its 30-byte frontier and the shared callback origin remain independent issues; the caller's structural agreement does not promote those callees.

## Large owner: `EnemyEclManagerView::RunEcl`

Fresh focused build: `scripts/build-match-unit.py --unit enemy-attached-effect-update`; comparison: `scripts/inspect-ecl-complete.py`.

- Physical owner: 15,564 bytes / 598 relocation fields.
- 30 ordinary byte differences remain, at offsets `0x278, 0x27A-0x27E, 0x280` and `0x30B0-0x30C2, 0x30C4-0x30C6, 0x30C8, 0x30CA`.
- Target-attested IDA disassembly independently confirms two concentrated regions:
  - ECL opcode 4 jump/timer assignment at `0x408937`: target loads the raw dword into ECX, pushes it, then loads the active timer receiver and calls `ZunTimer::operator=(int)`. The direct-union-member expression was tested; it increased the owner mismatch count from 30 to 42, so the original wire-argument snapshot was restored.
  - ECL opcode `0x9B` at `0x40B770`: target uses the byte at instruction `+0x0C`, reads the primary flag word at Enemy `+0x337C`, and performs the masked XOR update of bit 24. The bitfield-assignment and local-copy variants shifted the owner substantially (2,140+ or 2,290 differences); those variants were discarded. The original raw-byte snapshot plus `AssignTimeoutSpellBit` source remains the reproducible 30-difference baseline.
- The opcode-4 and opcode-`0x9B` target instruction sequences are captured in the session's target-attested analysis artifact; they are provisional semantic evidence, not exactness receipts.

## Accepted small Enemy leaves

Each claim below was submitted as an individual forced-recompile Factory replay against the clean source binding at HEAD `85fb775d42c727d7c9c28cfd56f0958720ab211`. Every job completed with `receipt_verdict=pass` and `acceptance_decision=accepted`:

| Function | Address | Bytes | Receipt |
|---|---:|---:|---|
| `EnemyCoreUpdateAttachedEffects` | `0x0040F4C0` | 151 | `receipt:00dcd2786f7d1e832f5f2dae9f2d6dd095c0adcc4d761f8dfe580cccb5b934a7` |
| `EnemyCoreReleaseChildEclBlocks` | `0x004100E0` | 48 | `receipt:fee8d5701ced8bd6585fa68ae67aabe081f9a6aeb733c755199a272e39b0ac49` |
| `EnemyView::CleanupAfterDeactivation` | `0x00410110` | 184 | `receipt:07d301aeccaa5f2bb886579284c057808a4b9f748700d7333fa613064d9e7da5` |
| `EnemyAppendCollisionView::AppendPlayerCollisionBox` | `0x00410250` | 82 | `receipt:6b6fda568ea5ac576e4d78a7ba34807cab85498a5347757a3f95d970274b5b3b` |

These are independent accepted leaves. They do not imply that `OnUpdate` or `RunEcl` is exact.

## Next work

1. Continue attacking `EnemyManagerView::OnUpdate` as a large owner; use the exact 13 offsets above as the current frontier, and seek a source-level explanation for the register and descriptor-store order rather than register steering.
2. Continue attacking `RunEcl` as a large owner; keep opcode 4 and opcode `0x9B` isolated so one experiment cannot shift the other 598 relocation fields.
3. Continue with additional small leaves and replay each exact claim independently.
4. Revalidate every adopted source variant against the live original target; imported notes and historical trials are leads, not proof.

## Additional accepted leaves and shared getter audit

Three further individual forced-recompile jobs were completed against source HEAD `4b4fd022be6fb803a72b204c8c64ce98c54c196c`. Each job's `outcome` reports `receipt_verdict=pass`, `acceptance_decision=accepted`, and no acceptance reasons:

| Function | Address | Bytes | Receipt |
|---|---:|---:|---|
| `PlayerLifecycleView::SetUpdateState` | `0x00403C20` | 12 | `receipt:239c12a4b66cd6c68cdc86a51de2bc1bf38aba358764ff4adb3979460a85b8cd` |
| `EnemyView::IntegrateMotion` | `0x0040F9B0` | 188 | `receipt:18fb8ea5d2fceea6afd1276c30ca173491318375080f1a063c87a9a1ea51cd1f` |
| `EnemyView::ClampPositionToMovementBounds` | `0x004101D0` | 127 | `receipt:620fb06abe32ed938ca6b5e7d1ea73a3c22c90882c2470fdf86ce5071def6561` |

A fresh native IDA decompile of `0x00435EC0` is a one-field getter (`return *(_DWORD *)this`) with 19 call sites. The source layout confirms three semantically distinct methods whose first fields are at offset zero: `PlayerLifecycleView::GetUpdateState`, `TextHelper::GetFormat`, and `SoundManagerProcessView::GetDirectSound`. Their identical three-byte body is therefore shared/folded code; no unique physical C++ owner is asserted. The function remains unclassified rather than assigning the same physical body to one arbitrary class.

## Additional large-owner probe: `ReplayManagerView::SaveReplay`

Fresh focused build: `scripts/build-match-unit.py --unit replay-manager-capture-frame-sync-state`; fresh compare: `scripts/inspect-replay-save.py build/matching/ReplayInput.obj --json`.

- Target and candidate extents: 1,238 bytes each; 53 relocation fields agree.
- Exactly two byte differences remain, at owner-relative offsets `0xB7` and `0xBB`.
- Target-attested IDA disassembly shows the difference is the base register used for the destination address: target loads the temporary-buffer pointer into EDI and forms `[EDI + EBX - 0xC0]`; candidate loads the same stack local into ECX and forms `[ECX + EBX - 0xC0]` before the same `rep movsd` copy.
- Tested natural source variants included a destination cursor local, add-then-sub pointer arithmetic, array-index address, non-const link pointer, `register` buffer/destination locals, and const buffer pointer. Every compiling variant retained the same two offsets. Void/char pointer variants without all required API casts failed compilation and were discarded. No source change was retained; the two-byte register-allocation frontier remains open.

## Acceptance-registry cross-check caveat

The seven individual replay jobs above report pass/accepted outcomes. A separate current global registry summary returned `accepted=0`, `rejected=1182`, and a `factory_query_accepted_facts` query for `target:th09-main` / `codegen_exact` returned zero facts. This conflicts with the per-job outcomes and is unresolved. Treat the per-job receipt outcomes as recorded job results, but do not claim the global accepted-facts snapshot has incorporated them until this registry discrepancy is explained.

## Additional `OnUpdate` / `RunEcl` probes

Further source-only experiments did not reduce either large owner:

- `OnUpdate`: pointer aliases at both draw-group assignments, byte/dword index locals, a reference to the draw-group head at the append label, shared side-state pointer in the special-attack block, threshold reference, non-const `Float3` reference, and pointer-dereference `Float3` copy. Variants that moved literal-field positions were rejected by the owner inspector; the baseline 13 differences remain.
- `RunEcl`: `RawInt` accessor for opcode 4, local-union bitfield and local-reference bitfield forms for opcode `0x9B`. None improved the 30-byte baseline; all source files were restored.

No source experiment from this section was retained. The next productive step is a new source-level hypothesis, not repeating these register-alias variants.

## Re-review correction: `PlayerLifecycleView::CheckBulletCollision`

The prior note claiming that this source had been changed to a pre-increment `active->count` scan was incorrect. A fresh source search and rebuild disproved it; no such source form exists in `src/PlayerCollisionHelpers.cpp`, and no code change was retained for this owner.

- Current source still walks the null-terminated pointer array with `for (; *slot != NULL; ++slot)`.
- The target-attested IDA disassembly at `0x0041DFF0` shows a null-terminated `activeB100` pointer walk: it initializes the slot pointer in ESI, jumps to the loop header at `0x0041E010`, loads/tests `[esi]`, and advances by four bytes before jumping back to that header. It does not compare a loop index against `active->count`.
- Fresh current-source baseline: target 862 bytes / 277 instructions / 33 direct blocks; candidate 849 bytes / 266 instructions / 32 direct blocks. All 26 relocation fields and 12 ordered calls agree; 799 overlapping bytes differ and 13 target bytes are missing. No exactness credit.
- The target prologue loads the Bullet argument from `[ebp+0x10]` into EBX and retains it across the loop; position is reloaded from `[ebp+8]` in the geometry paths. The current candidate instead keeps position in EBX and rotates the null check to the loop tail, preloading `[esi+4]`. This register-liveness and loop-scheduling difference is the real source frontier.
- A natural `const bool hasBullet = bullet != NULL` local moved the candidate to 861 bytes, but it still had only 271 instructions / 32 blocks, 818 overlapping byte differences, and one missing target byte. It is not a near-exact result and was discarded.
- Top-level volatile position, volatile slot, explicit goto, and `for (;;)` variants were also compiled. They produced 876–890 byte bodies, changed the return/loop layout, or failed the inspector's three-RET12 contract. None was retained.

The prior pre-increment scan conclusion is withdrawn. Do not use it as evidence or repeat it as an adopted fix.

## Further large-owner C++ probes

Additional variants were tested after the earlier frontier note; every source file was restored to HEAD afterwards.

- `EnemyManagerView::OnUpdate`: repeated both draw-group assignments using pointer aliases and byte/dword indices; moved the head reference to the append label; widened the side-state alias lifetime across the special-attack block; used threshold references; and tried non-const `Float3` references and pointer-dereference copies. None reduced the 13-byte frontier. A tail-reference form moved literal positions and was not accepted by the strict owner inspector.
- `EnemyEclManagerView::RunEcl`: integer locals, references, raw pointers, and explicit operator calls for opcode 4 all increased the difference count from 30 to 42. A copied-union `memcpy` form retained the 30-byte baseline. Direct byte/helper and flag-local variants for opcode `0x9B` shifted the owner/switch layout to 2,290 differences; those were discarded. The original wire-argument snapshot and `memcpy` byte snapshot remain the baseline.
- No speculative source variant from these probes was kept. The focused build was restored to the committed source state after each batch.

## Fresh replay revalidation at HEAD `381a3b508072811f0fb26026ab035094bfc7a3c7`

`EnemyCoreUpdateAttachedEffects` was independently forced-recompiled again against the later clean HEAD. The new job completed with `receipt_verdict=pass` and `acceptance_decision=accepted`:

- Address `0x0040F4C0`, 151 bytes.
- Receipt: `receipt:70f7589044ab171b92f48d1457fe52dfa5d2d57ec4891d94bb9acbda9d43e599`.

This revalidates that leaf against the newer source binding; it does not promote either large owner.
