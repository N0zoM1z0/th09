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

The seven individual replay jobs above report pass/accepted outcomes. A separate current global registry summary returned `candidates=1189`, `accepted=0`, `rejected=1189`, `invalid=0`, and a prior `factory_query_accepted_facts` query for `target:th09-main` / `codegen_exact` returned zero facts. This conflicts with the per-job outcomes and is unresolved. Treat the per-job receipt outcomes as recorded job results, but do not claim the global accepted-facts snapshot has incorporated them until this registry discrepancy is explained.

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

## Fresh forced-replay revalidation on later clean source bindings

Every job below was an individual `forced-recompile` against a clean source snapshot and completed with `receipt_verdict=pass`, `acceptance_decision=accepted`, and no acceptance reasons. These are per-job outcomes; the global acceptance-registry discrepancy below remains unresolved.

| Function | Address | Bytes | Source binding | Receipt |
|---|---:|---:|---|---|
| `EnemyCoreUpdateAttachedEffects` | `0x0040F4C0` | 151 | `381a3b508072811f0fb26026ab035094bfc7a3c7` | `receipt:70f7589044ab171b92f48d1457fe52dfa5d2d57ec4891d94bb9acbda9d43e599` |
| `EnemyCoreReleaseChildEclBlocks` | `0x004100E0` | 48 | `f791db3c06dc81a6557cf3ae213faa4d33898b31` | `receipt:751a7fc9ecaf7009e14b5fda545c5bd37e9a5d9d8bc4e5f3bd11a91cc8342822` |
| `EnemyView::CleanupAfterDeactivation` | `0x00410110` | 184 | `f791db3c06dc81a6557cf3ae213faa4d33898b31` | `receipt:7e292801a7e9272158828ac9ef951603de8fd9309e9d5fdd5e8b97102171bcd0` |
| `EnemyAppendCollisionView::AppendPlayerCollisionBox` | `0x00410250` | 82 | `f791db3c06dc81a6557cf3ae213faa4d33898b31` | `receipt:f6cd12f9a06c84ebc7f247c3a4f2062b2decacb25a20dd734865569733e900d8` |
| `PlayerLifecycleView::SetUpdateState` | `0x00403C20` | 12 | `f791db3c06dc81a6557cf3ae213faa4d33898b31` | `receipt:552b38dc803a97fd62c34359c88ca34a27ac69de16e07dbd267a6d5ecc5f9b61` |
| `EnemyView::IntegrateMotion` | `0x0040F9B0` | 188 | `f791db3c06dc81a6557cf3ae213faa4d33898b31` | `receipt:1bcc85ff27f73ac174ed5a6295344294e6a494b50a308344a7f77530e7192f94` |
| `EnemyView::ClampPositionToMovementBounds` | `0x004101D0` | 127 | `f791db3c06dc81a6557cf3ae213faa4d33898b31` | `receipt:c3a8c562153520aec636859b33ad7a9825db10b38bdd5631387fcd06a4e78c93` |

The latest registry summary after these jobs still reports `candidates=1189`, `accepted=0`, `rejected=1189`, `invalid=0`. Earlier `factory_query_accepted_facts` returned zero facts for `target:th09-main` / `codegen_exact`. Do not conflate the individual job outcomes with a globally incorporated accepted-facts snapshot.

## Three additional accepted Player collision leaves (fresh HEAD `7015f8d6d4a9aac7a4e46aa9063863f1c64898fd`)

Fresh focused builds of `player-calc-item-box-collision`, `player-calc-circle-collision`, and `player-calc-laser-hitbox` each produced 100.00% structural bytes with no differing bytes. Each was then submitted as its own forced-recompile replay against the clean source binding at the HEAD above. All three completed with `receipt_verdict=pass`, `acceptance_decision=accepted`, and no acceptance reasons:

| Function | Address | Bytes | Receipt |
|---|---:|---:|---|
| `PlayerLifecycleView::CalcItemBoxCollision` | `0x0041BDF0` | 121 | `receipt:6bcd1921a483942868d2990292664c953c6ac7524c581d7a08b13487b2b38c41` |
| `PlayerLifecycleView::CalcCircleCollision` | `0x0041BE70` | 106 | `receipt:7ab13b9d6638567f854eaa3af80ac35042f67e220528748c42c45e238d8c83bf` |
| `PlayerLifecycleView::CalcLaserHitbox` | `0x0041BFC0` | 422 | `receipt:26be6ed742b0e09c57d00e5927bb6980237ddf47ccdb75a9e7c70423f6752ca7` |

These are three independent accepted leaves. They do not promote the adjacent `CheckBulletCollision` owner or any caller.

## Re-examined `EnemyManagerView::OnUpdate` 13-byte frontier

A fresh focused `enemy-attached-effect-update` build and `inspect-enemy-manager-update.py` report still show 3,900/3,900 bytes, 97/97 relocation fields, 69/69 ordered direct calls, and 170/170 reviewed control-flow blocks. The 13 differences group into a small number of instruction-selection/register-allocation sites:

- `0x19C, 0x1A3`: target uses EAX for the zero-extended draw-group byte and the resulting indexed head address; candidate uses ECX for the index and EAX for the address.
- `0x252, 0x263`: target uses EAX for the side-state/effect-manager pointer chain; candidate uses EDX.
- `0x26D, 0x272`: target loads the special-attack threshold into EDX and pushes EDX; candidate uses EAX.
- `0x294, 0x29A, 0x29C, 0x2A5, 0x2A9, 0x2B7, 0x2BD`: target and candidate copy the same three-float enemy position into the descriptor, but choose different source/base registers and load/store scheduling around the FPU speed calculation.

Additional natural source probes were compiled and discarded: local `EffectManager *`, local `EnemySideStateView *` / reference, threshold value and pointer locals, descriptor reference through the whole constructor carrier, draw-group element reference and pointer arithmetic, per-loop side-state alias, and changing `EnemyView::position2D74` from the POD `EnemyFloat3` view to `Float3` with the three dependent call-site/assignment casts adapted. The `Float3` field-type hypothesis compiled but retained the exact same 13 offsets; it was restored. No speculative change was retained.

## Re-examined `EnemyEclManagerView::RunEcl` 30-byte frontier

The original-bound complete-owner comparison still has 30 ordinary differences. Target/candidate instruction inspection confirms the two concentrated regions without treating an IDA decompile as an exactness receipt:

- Opcode 4 (`0x408937`): target loads the raw dword operand into ECX, pushes ECX, then loads `enemy->activeContext2CE0`, adds the timer field offset, and calls `ZunTimer::operator=(int)`. Current baseline loads the same operand into EAX, loads the timer receiver into ECX, then pushes EAX. Five C++ forms (raw-union reference, pointer, explicit operator, and direct field forms) retained EAX and increased the total difference count from 30 to 42. They were discarded.
- Opcode `0x9B` (`0x40B770`): target uses `movzx ecx, byte ptr [instruction+0x0C]`, loads the old primary flags into ESI, computes the masked XOR, copies ESI to EAX, XORs the masked bit into EAX, and stores EAX. Baseline source uses a byte `memcpy` snapshot plus `AssignTimeoutSpellBit`, which keeps the 30-byte owner frontier but emits a byte-register load/zero-extension and reuses the result register. Changing the helper parameter or local snapshot to `unsigned int` does produce the target's direct memory `movzx`, but changes the old-flags/result register allocation and shortens the case body; the switch tail shifts and the whole-owner difference count rises to 2,290. Explicit result-local variants did not restore the original switch layout. All were discarded.

No RunEcl source change was retained. The original 30-byte baseline remains reproducible; opcode 4 and opcode `0x9B` remain independent unresolved source/codegen frontiers.

## Current focused state

- No full cold replay was run.
- Focused builds were limited to `enemy-attached-effect-update`, the three Player collision leaf units, and `player-check-graze-collision`.
- `CheckBulletCollision` remains at 849 candidate bytes versus 862 target bytes, 266 versus 277 instructions, 32 versus 33 direct blocks, with all 26 relocation fields and 12 ordered calls agreeing. The `hasBullet` local briefly reached 861 bytes but still had 271 instructions, 32 blocks, and one missing target byte; it was discarded. The committed source remains the 849-byte baseline.
- The worktree was restored after all probes; only this session record is being committed in this checkpoint.

## Four more independently accepted Player collision leaves

The following focused units each built and compared at 100.00% before individual forced-recompile replay. All four replay jobs completed with `receipt_verdict=pass`, `acceptance_decision=accepted`, and no acceptance reasons, against clean source HEAD `51ea9ad4aaa48e9fa6a9cf65f79048feffbc0e5a`:

| Function | Address | Bytes | Receipt |
|---|---:|---:|---|
| `PlayerLifecycleView::CheckGrazeCollision` | `0x0041E350` | 204 | `receipt:2bfaebb0dc6086ae6b92d886dca5f8aa3f11d9712baa43da4351803b6f645d5f` |
| `PlayerLifecycleView::CalcItemCollectionCollision` | `0x0041BEE0` | 209 | `receipt:060893ca43148c8f5b37792fdce06628136bc4de942a420532c03043c67dc2a4` |
| `PlayerCollisionQueryStateView::FindCollision` | `0x0041D810` | 602 | `receipt:d1072f6ced10d27c53467266f1848cd96eac73a07e8fa161140c2a6a4872ff02` |
| `PlayerCollisionQueryStateView::FindCollisionAtPlayer` | `0x0041DA70` | 279 | `receipt:3d6a03f4628610c79d995c44e94a51f0e6175c2cc73db063b5b89711ee78d42d` |

Together with the three Player collision leaves recorded immediately above, this session has seven new accepted exact Player collision leaves. `CheckBulletCollision` remains an independent unresolved owner and is not promoted by the exact `CheckGrazeCollision` caller.

## Additional `CheckBulletCollision` register/lifetime probes

A new source-level hypothesis was that the target's early `mov ebx, [ebp+0x10]` preserves the Bullet parameter across the region loop while the candidate keeps the position parameter in EBX. A top-level `volatile` position pointer with one ordinary pointer snapshot per circle/rotated/AABB/effect path did load the Bullet parameter in EBX and restore the 33-block graph, but the best combined form (including a precomputed `hasBullet` local) was 872 bytes / 274 instructions / 33 blocks / zero missing target bytes and still 815 unequal overlapping bytes. The non-`hasBullet` path-local snapshot form was 856 bytes / 267 instructions / 31 blocks / six missing bytes. `register` parameter/local variants did not improve these results. All were discarded; the committed 849-byte baseline remains unchanged.

## Additional `SaveReplay` pointer-qualification probes

The two target-relative differences remain `0xB7` and `0xBB`. Marking the temporary buffer pointer itself `volatile` expanded the candidate's code layout and was rejected. `register` and top-level `const` pointer forms retained exactly the two original differences. A `__restrict` spelling was rejected by the VC7.1 compiler. No source change was retained.

## Additional accepted Player leaves (clean source binding `d16853cdc966df259548c1050fe9b8c1f30974f1`)

Two more focused builds were exact and each was independently forced through the factory replay. Both completed with `receipt_verdict=pass`, `acceptance_decision=accepted`, and no acceptance reasons:

| Function | Address | Bytes | Receipt |
|---|---:|---:|---|
| `PlayerDamagePlayerView::CalcDamageToEnemy` | `0x0041FCD0` | 996 | `receipt:51f7214cfc347bd1a863e12bd7d997a707d6915942636d10007e6e4eed899d94` |
| `PlayerCheckState2` | `0x0044AB20` | 3 | `receipt:9f45cb894428e244b2c019e4debd0ddde0fc0f2552cc9fdbd59cabb68990ac9a` |

`PlayerCheckState2` is the accepted symbol/unit claim for the three-byte `xor eax,eax; ret` body. The `OnUpdate` inspector separately identifies the same physical address as a folded callback body used by multiple semantic callback names. Do not infer exclusive physical ownership from the Player unit name.

## Fresh IDA source-shape review of the two large owners

### `EnemyManagerView::OnUpdate` (`0x00410730`)

A fresh IDA decompile of the target confirms the special-attack descriptor local starts at EBP-`0x2A8`; the position floats are at EBP-`0x2A4`, `-0x2A0`, and `-0x29C`. The speed expression is evaluated independently (`dword_4A7E44 * 0.1 + 1.0`) and interleaved with the position copy. This supports the existing 12-byte descriptor-position interpretation, but does not establish that the current `Float3` wrapper is the original source type.

Additional natural source experiments were compiled and discarded: direct draw-head pointer arithmetic, pointer increment, local draw-group base, side-state plus effect-manager locals, effect-manager reference/pointer-to-field, signed/cast index locals, non-const position reference/pointer/value temporary, `__forceinline` getters returning draw index/effect-manager/position, and a D3DXVECTOR3 descriptor-prefix view. Every audited build retained the exact same 13 owner offsets (`0x19C`, `0x1A3`, `0x252`, `0x263`, `0x26D`, `0x272`, `0x294`, `0x29A`, `0x29C`, `0x2A5`, `0x2A9`, `0x2B7`, `0x2BD`). The experiments were restored; no speculative code was kept.

### `EnemyEclManagerView::RunEcl` (`0x004086C0`)

A fresh IDA decompile independently confirms opcode 4 calls `ZunTimer::operator=(int)` with the raw dword at instruction+`0x0C`, and opcode `0x9B` is the masked compound update:

`flags ^= (flags ^ (unsigned int)instruction[0x0C] << 24) & 0x01000000;`

The current C++ baseline deliberately retains the wire-argument snapshot and byte snapshot/helper form because it gives the 30-byte complete-owner frontier. Direct raw-union access, direct compound assignment, reference/pointer aliases, and explicit local flags all compile, but currently expand the whole-owner difference count to 42 or 2,290 because they change the compiler's register allocation and/or switch layout. The direct opcode-4 forms also change the following opcode-7/8 register choices; this is not an isolated seven-byte patch. The baseline was restored after each probe.

## Current progress and validation scope

- The latest source checkpoint remains `d16853cdc966df259548c1050fe9b8c1f30974f1`; worktree clean.
- This session added two more accepted leaves on top of the seven leaves already recorded in the prior checkpoint.
- The current `OnUpdate` owner remains 3,900 bytes with 97/97 relocation fields, 69/69 ordered direct calls, 170/170 reviewed control-flow blocks, and 13 byte differences.
- The current `RunEcl` owner remains at 30 byte differences. No speculative large-owner source variant has been retained.
- All work used focused match-unit builds and bounded IDA reads. No full cold replay was run.

## Four additional accepted Enemy/ECL leaves (forced-recompile receipts)

Focused VC7.1 builds and relocation-aware comparisons were run for four existing source owners. Each exact comparison was then independently submitted through the strict live replay driver with forced recompilation. All four jobs completed with `receipt_verdict=pass`, `acceptance_decision=accepted`, and no acceptance reasons:

| Function | Address | Bytes | Receipt |
|---|---:|---:|---|
| `EnemyView::IntegrateMotion` | `0x0040F9B0` | 188 | `receipt:8eb93888601ed84bd24e9daa83409c855cb3dec81b7d0d25ec1dabbca359fd31` |
| `EnemyView::ClampPositionToMovementBounds` | `0x004101D0` | 127 | `receipt:71eabacf15ce814592db7f7d02a4d28f5ba4f57279ad8f20284fd220ee4cea69` |
| `Th09EclRunState::ReleaseAttachedEffects` | `0x0040F560` | 64 | `receipt:03bdc394869c8eb4aab4fe8ea73eb8f2ed8df8818220464b7bf789fb066efdef` |
| `EnemyView::CleanupAfterDeactivation` | `0x00410110` | 184 | `receipt:2b01c0b531c2304026e1e89db5696e104db7d71b2d698a182618e7db7f96f558` |

`CleanupAfterDeactivation`'s two ordered calls resolve to `ReleaseAttachedEffects` at `0x0040F560` and `EnemyCoreReleaseChildEclBlocks` at `0x004100E0`. These accepted leaves add evidence to the same Enemy/ECL lifecycle chain used by `OnUpdate` and `RunEcl`; they do not imply either large owner is exact.

## Fresh large-owner probes after the four receipts

### `EnemyManagerView::OnUpdate` (`0x00410730`)

A fresh IDA decompile and linked disassembly review reconfirmed the 13-byte owner frontier: 3,900 bytes, 97/97 relocation fields, and the existing 69 ordered calls / 170 reviewed blocks. The differences group into:

- `0x19C`, `0x1A3`: draw-group byte index is loaded into ECX in the candidate but EAX in the target (`0x004108CA`); the computed head pointer itself is in EAX in both.
- `0x252`, `0x263`: effect-manager side-state pointer uses EDX in the candidate but EAX in the target (`0x00410981..0x00410992`).
- `0x26D`, `0x272`, `0x294`, `0x29A`, `0x29C`, `0x2A5`, `0x2A9`, `0x2B7`, `0x2BD`: special-attack threshold argument register and the position-copy register/store scheduling in the 0x0041099C..0x004109EB block.

New source-shape probes were compiled and discarded: flattening the side-category gate to the target's OR form; hoisting a side-state pointer ahead of counter reset; force-inline draw-group index/head getters; pointer/reference/volatile threshold locals; effect-manager pointer/reference locals; and direct position component stores / Float3 temporaries. The flattened gate and getter variants retained the same 13 offsets. The hoisted side-state pointer changed the owner extent to 3,756 bytes and was rejected. Volatile threshold and direct component-store variants expanded the differences substantially. No speculative variant was retained.

### `EnemyEclManagerView::RunEcl` (`0x004086C0`)

The complete owner remains 15,564 bytes with 30 byte differences. Fresh source-shape probes revisited opcode 4 and opcode `0x9B`:

- Opcode 4 at `0x00408937` still differs in the order/register used to push the raw dword and form the active-context Timer `this` pointer. Direct assignment, explicit `operator=`, raw-operand, integer-local, pointer/reference, and inline wrapper variants did not improve the complete owner; wrapper declarations also failed the lexical compile when placed outside the correct declaration scope.
- Opcode `0x9B` at `0x0040B770` remains a masked XOR of the primary Enemy flags word. Direct bitfield assignment and direct compound-update spellings alter the dense switch layout or register allocation and expand the full-owner difference count (up to 2,290 / 2,140); unsigned-int local forms preserve the 30-byte frontier but still do not reproduce the target's ESI/ECX/EAX allocation.

All probes were restored to the committed baseline. No full cold replay was run; validation stayed on the affected match units and bounded IDA reads.

## Fifth additional accepted Enemy/ECL leaf

`EnemyCoreUpdateAttachedEffects` at `0x0040F4C0` was rebuilt and compared independently: 151/151 bytes, both relocations exact (`0x3E99999A` and `AddNormalizeAngle` at `0x0042AED0`), and its literal definition matched. Its forced-recompile replay completed with `receipt_verdict=pass`, `acceptance_decision=accepted`, and no acceptance reasons: `receipt:c61933924ad3ec7e846d08533504a06ee5a0a91346b9faafac2cad553c39c515`.

## Fresh RunEcl owner investigation (second pass, 2026-11-08)

The live source was re-read rather than assuming the previous opcode notes were correct. A focused build of `enemy-attached-effect-update` and a fresh full-owner `inspect-ecl-complete.py` report reconfirmed the unchanged `EnemyEclManagerView::RunEcl` extent: 15,564 bytes, 598 relocation records, 30 ordinary differing bytes. There are only two difference clusters:

- `+0x278..+0x280` (`0x00408938` onward): opcode 4, `JUMP`, assignment to `context->time008`.
- `+0x30B0..+0x30CA` (`0x0040B770` onward): opcode 155, `SET_TIMEOUT_SPELL`.

### Opcode 4: argument-before-this evaluation

Fresh IDA disassembly of the target at `0x00408937`:

```asm
mov ecx, [ebx+0Ch]
push ecx
mov ecx, [edi+2CE0h]
add ecx, 8
call 00401500 ; ZunTimer::operator=(int)
add ebx, [ebx+10h]
```

The current C++ snapshot copies the raw argument into a `Th09EclWireArgument` local before assigning `context->time008`. It compiles to the same call and target but schedules the argument in EAX, loads `this` into ECX, then pushes EAX. Replacing the local with direct union access, `RawInt`, explicit `operator=`, timer pointer/reference, raw context-offset access, `volatile` argument reads, register locals, and several `__forceinline __fastcall` value-first helpers all retained that EAX-before-this sequence. Direct-expression variants expanded the complete owner from 30 to 42 differing bytes because the surrounding switch layout changed; none was retained.

### Opcode 155: masked XOR register allocation

The target block at `0x0040B770` is:

```asm
movzx ecx, byte ptr [ebx+0Ch]
mov esi, [edi+337Ch]
shl ecx, 18h
xor ecx, esi
and ecx, 1000000h
mov eax, esi
xor eax, ecx
mov [edi+337Ch], eax
```

This implements `flags ^= (flags ^ (rawByte << 24)) & 0x01000000`. The current helper-based C++ preserves the semantics but reuses EAX for the old flags and ECX for the result. Fresh natural-source probes covered raw byte/union reads, signed and unsigned locals, helper and direct masked-XOR spellings, compound assignment, references/pointers, `register` and `const` locals, bitfield assignment, and return-value helpers. None reproduced the target register schedule. Direct bitfield assignment kept the old word in ESI but selected EAX for the shifted input and ECX for the result; it also disturbed dense switch layout and expanded the full-owner difference count to 2,140. A volatile local bitfield overlay was worse (31 differences and AND/OR code instead of the target XOR). An inline-assembly experiment reproduced the local instruction sequence but shifted/changed the surrounding switch owner (2,220 differences), so it was discarded rather than replacing natural source with a non-matching workaround.

All probes were restored. No RunEcl source or helper changes from this investigation were retained. No full cold replay was run; each candidate used the existing focused match-unit build and bounded owner diagnostics only.
