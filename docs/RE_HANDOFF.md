# TH09 exact reconstruction — agent handoff (verified 2026-11-13)

**Start:** [AGENTS.md](../AGENTS.md) · [workflow](RE_WORKFLOW.md) ·
[original/VC7.1 Oracles](ORACLES.md) ·
[historical experiments and target layouts](KNOWLEDGE_BASE.md).
This is the **current restart checklist**, not an append-only session log.
Old source/commit histories remain accessible through Git, but ignored scratch
reports and compiled objects are disposable and must not be used as current
exactness evidence. **Start with `git status`, recover any dirty changes,
then regenerate the current ledger counts and needed compiler objects.**

## Verified target, counts and evidence boundaries

- **Target:** original Japanese TH09 **v1.50a**, Windows i386, PE image base
  `0x00400000`, entry `0x0047D45F`. Ignored, protected
  `resources/th09.exe` SHA-256:
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`.
  Do not patch, rename, commit or delete the target.
- **Checked 2026-11-13 against the canonical ledgers:**
  `config/functions.csv` has 2,248 candidates,
  **1,037 source-present authored mappings**, **1,025 exact**,
  **12 nonexact / 28,145 canonical function bytes**, and
  **35 reviewed unresolved origins**. The 1,025/1,037 figure is a
  **98.84% function count**, NOT overall reconstruction completeness:
  exact authored bytes are **263,786 / 291,931 = 90.3590%**.
  Complete original-code inventory is **not** proven. Generated
  [progress](PROGRESS.md) and the command below reflect ledgers;
  counts quoted in old experiment notes are historical. The branch may be
  synchronized with the remote; do not assume an ahead/behind count or a
  particular HEAD from a previous agent message.
- **Important size distinction:** the 28,145-byte nonexact total is the sum of
  `functions.csv` **logical sizes**, *not* the sum of the larger physical
  original-bound comparison extents. In particular:
  RunEcl = **14,792 logical / 15,564 physical** (includes owned tables/tails);
  Enemy OnUpdate = **3,883 / 3,900**; Player movement = **1,835 / 1,900**.
  Those physical extensions add **854** bytes across the three owners.
  Neither number is a new partial-exactness credit.
- **Phase:** exact source reconstruction is **unfinished**. A whole Windows
  i386 compile/link/runtime product, dual-Oracle semantic tests and port are
  **not** proven. `GameplaySetupThread` (1,689 bytes) and **Title DrawResult**
  (1,939 bytes) *are exact*; do not confuse DrawResult with the nonexact
  Title `OnUpdateResult` below.
- **Proof hierarchy:** original disk PE + attested local Ghidra for target
  analysis; pinned VC7.1 COFF for candidate source; canonical
  `config/matches.csv`, `config/functions.csv` and
  `config/match-units.toml` for exact claims. A diagnostic that preserves
  bytes, fields or CFG but has residual differences is **NONEXACT**.
  Verify complete owned code, tails, switch tables and every relocation.
  Private `$L` label names may be rebound only after proving *actual*
  same-section COFF definitions and original PE destinations, not by
  renaming strings blindly.

## Remaining source-present nonexact owners

Rows below are **full original PE comparison extents**, not logical ledger
sizes or partial exact claims. Except for the refreshed CheckBulletCollision
row, these are last-recorded diagnostics and must be rebuilt before relying
on them after related source changes. The `--owner` options refer to
`scripts/check-boss-frontiers.py` except where a dedicated inspector is given.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| **RunEcl** | 0x004086C0 | 15,564/15,564 physical bytes; 598 fields; **30 unlike** | Opcode 4 JUMP value/receiver register order (7), opcode 155 timeout flag (23); switch tables intact. Direct `RawInt`/DWORD reads and explicit or wrapped timer assignment did not improve JUMP; widening the timeout-byte local changed unrelated handler code. `--owner runecl`; `src/EclManager.cpp` with `EclRun*.inl`. |
| **Enemy OnUpdate** | 0x00410730 | 3,900/3,900; 97 fields; 69 calls; **13 unlike** | Draw-group / bullet-descriptor register lifetimes. A standalone TU, side-state getter, direct descriptor-member access and widened draw-group helper retained the same 13-byte frontier. `--owner enemy`, `src/EnemyManagerCore.inl` included by EclManager. |
| **Player movement** | 0x0041C170 | 1,900/1,900; 66 fields; 23 calls; **8 unlike** | Only `+0x127-0x12E` remain: `g_GameConfiguration` / `sideIndex` load order before focus-mode comparison. `--owner movement`, `src/PlayerMovement.cpp`. |
| **Player ChargeAttack** | 0x0041F810 | 1,210/1,210; 70 fields; 58 calls; **235 unlike** | Mode byte / input receiver scheduling; 53-block CFG retained. `--owner charge`, `src/PlayerChargeAttackUpdate.cpp`. |
| **Title OnUpdateResult** | 0x004266B5 | 484 candidate / 485 original; 34 fields; 9 calls; **218 unlike** and 1 missing | Character byte load/sign extension. Dedicated `inspect-title-update-result.py` after building `title-screen-setup-thread`; **not** `--owner resultdraw`. |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280; 57 fields; 13 calls; **29 unlike** | Selected-record EDX/ECX lifetime. `--owner replaymenu`, `src/TitleScreenReplayMenuDraw.cpp`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | 862/862 physical bytes; 275/277 instructions; 34/33 CFG blocks; 26/26 fields; 12/12 calls; **729 unlike**, 0 missing/extra | Still NONEXACT; loop continuation, x87/AABB branches, reward register order and final hit-count exits remain. `--owner player`, `src/PlayerCollisionHelpers.cpp`. |
| **Replay SaveReplay** | 0x00420C60 | 1,238/1,238; 53 fields; 23 calls; **2 unlike** | Header-copy EDI/ECX register encoding at +0xB7/+0xBB. Moving `currentOffset` initialization before buffer allocation did not change the pair. `--owner replaysave`, `src/ReplayInput.cpp`. |
| **Replay BeginPlaybackStage** | 0x00420840 | 538 candidate / 540 original; 32 fields; 5 calls; **39 unlike** and 2 missing | RNG seed-load and input-delay tail. `--owner replay`, `src/ReplayInput.cpp`. |
| **ScreenEffect shake** | 0x004227A0 | 336/336; 20 fields; 8 calls; **26 unlike** | ESI/EDI timer/receiver lifetimes. `--owner screen`, `src/ScreenEffect.cpp`. |
| **Supervisor network service** | 0x00430AA0 | 1,627 candidate / 1,633 original; 106 fields; **1,381 unlike** and 6 missing | Waiting/prediction block layout. Compile `src/SupervisorNetwork.cpp` with pinned profile and run `inspect-supervisor-service.py`. `--owner network` checks *different* functions. |
| **Effect AddedCallback** | 0x0040D1C0 | 51/51; 4 fields; 2 calls; **8 unlike** | ANM owner/manager EDI/ESI register selection. `--owner effect`, `src/EffectManager.cpp`; protect its exact Create caller. |

## Reproduce *only* the changed compiler carriers

`build/` objects, PDBs and comparison JSON are **ignored disposable build
caches**, removed in the storage cleanup. Missing objects are expected, not
missing sources or original target data; each selected command regenerates
only the necessary carriers.
Do **not** start with `--owner all`, a project-wide cold replay, or
`ci.py --full` just to repopulate caches.

```bash
git status --short --branch             # inspect AND recover dirty work
git diff --check
git log -7 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py

# Choose ONE open boss. Diagnostic-only: it cannot claim exactness.
nice -n 12 python3 -B scripts/check-boss-frontiers.py --owner runecl --skip-exact-siblings

# Preserve all genuinely affected exact siblings after modifying the owner:
python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl --dry-run
nice -n 12 python3 -B scripts/replay-scoped-exact.py --changed-file src/EclRunState.inl
# Or use --source src/ReplayInput.cpp for a single source-owned cohort.

# One exact function when needed; builds the selected original-bound carrier:
nice -n 12 python3 -B scripts/build-match-unit.py --unit ecl-pop-context
python3 -B scripts/compare-coff-function.py --unit ecl-pop-context --json

# A separate Title owner (do not substitute exact DrawResult):
nice -n 12 python3 -B scripts/build-match-unit.py --unit title-screen-setup-thread
python3 -B scripts/inspect-title-update-result.py build/matching/TitleScreen.obj

# The standalone, nonexact network service has NO canonical exact sibling:
mkdir -p build/matching
nice -n 12 scripts/compile-probe.sh src/SupervisorNetwork.cpp \
  build/matching/SupervisorNetwork.obj \
  /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob1 /Oy- /I src
python3 -B scripts/inspect-supervisor-service.py build/matching/SupervisorNetwork.obj

# For documentation/ledger edits (no game compiler necessary):
python3 -B scripts/validate-docs.py
python3 -B scripts/ci.py
```

`replay-scoped-exact.py` only checks previously **exact** canonical functions,
not the nonexact boss in the same TU. Editing a shared `.inl` can change
private symbol names in several VC7.1 carriers. Compile and check every
affected physical owner *before* changing manifest symbol spellings. Avoid
compiler register-forcing hints, artificial padding and target-patching.

For new target analysis use attested `scripts/ghidra-analysis.py`, for example
`python3 -B scripts/ghidra-analysis.py --function 0x004086C0`. This validates
the imported Program and original mapped PE bytes. The earlier Ghidra
pseudocode under ignored `.analysis/ghidra-direct/` is a **hypothesis**, not
a byte-exact source receipt; a fresh import can be necessary after a Ghidra
tool upgrade.

## Immediate next-agent steps

1. Verify the original SHA-256, ledger status and clean/dirty worktree via the
   commands above. A clean tree is **not** a prerequisite for continuing:
   inspect and integrate any interrupted edits before starting new ones.
2. Pick **one nonexact boss** from the table. Player movement now has only
   eight load-order bytes left at `+0x127-0x12E`; SaveReplay has two register
   bytes left; RunEcl and Enemy OnUpdate have larger exact-sibling impact.
   Build the carrier and inspect its **whole physical function**; a
   `--skip-exact-siblings` diagnostic alone does not certify existing exact
   neighbors after an edit.
3. Keep a natural source improvement only after comparing original code,
   relocation destinations/addends, calls and relevant exact siblings. Commit
   coherent advances with `gpt-web: ...`; never turn partial similarity
   into a canonical match. The live `docs/PROGRESS.md` must be regenerated
   from ledgers, not edited as a hand-counted score.

## Suggested attack order / experiments already rejected

1. **Replay SaveReplay:** only two instruction register bytes remain.
   Natural `memcpy` casts, constness, buffer pointer locals, `register`,
   `DWORD`/`size_t`, copy and loop-order variants have not changed +0xB7/+0xBB.
   Investigate real value lifetime and original optimization constraints, not
   artificial register directives.
2. **Enemy OnUpdate / RunEcl:** keep the full 3,900-/15,564-byte graph,
   literals, relocations and original jump tables. Draw-descriptor forms still
   differ by 13 register bytes; opcode 4/155 by 7+23. Many bitfield/XOR,
   timer, pointer-alias and source-order variants have failed or worsened
   unrelated exact siblings. Detailed hypotheses in [knowledge base](KNOWLEDGE_BASE.md).
3. **Player movement/charge/collision:** preserve original ABI, calls and
   NaN-aware float control flow. Collision now spans the full physical extent,
   but its CFG is still 34 vs 33 blocks. Do not add new padding or cosmetic
   gotos; the one existing loop-entry alignment shim is documented below.
4. **Other nonexact callbacks:** Title Result vs DrawResult, ReplayMenu,
   ScreenEffect, Effect AddedCallback and network-service timing all have
   separate inspectors. Consult the experiments before repeating rejected
   variants. Research large bosses **and** leaf functions.

Adjacent TH08/TH095 and `_reference/Cirno-Labs-th09` source are read-only
hypothesis material, **never** proof about TH09 original bytes.

## Storage / protected inputs / commit discipline

**Keep** `resources/th09.exe`, the ignored `.tools/wine/` VC7.1/Wine
prefix, the factory-provisioned tools, matched `.analysis/ghidra-direct/`
database and target-attested Ghidra exports, original manifests, `_reference/`,
and the small `.analysis/gpt-web/` private proof scripts and experiment
summaries. Some old scratch output belongs to failed probes, so review it
rather than treating it as current evidence. `build/`, Python bytecode
caches and superseded Ghidra tool-version projects are re-creatable;
they are not canonical source proof. A fresh build may recreate `build/`.

Check `git status` on entry and **do not skip dirty changes**; only this
worktree's current contents are authoritative. Commit actual improvements
as `gpt-web: <what changed>`, save multiple coherent checkpoints when earned,
and never silently push. Do not infer a fixed ahead/behind count from old notes.

## 2026-11-13 current CheckBulletCollision boss frontier

Latest maintained source checkpoint: 9656ccd0
(gpt-web: pin collision AABB scratch offsets), following 3e25037d
(gpt-web: place collision cursor in scratch home).

Focused pinned VC7.1 /O2 /Ob1 /Oy- comparison of
src/PlayerCollisionHelpers.cpp:

| Measure | Candidate | Original |
| --- | ---: | ---: |
| Physical extent | 862 bytes | 862 bytes |
| Instructions | 275 | 277 |
| Direct CFG blocks | 34 | 33 |
| Relocation fields | 26 bound | 26 |
| Ordered calls | 12 agree | 12 |
| Missing / extra physical bytes | 0 / 0 | — |
| Differing overlapping linked bytes | 729 | — |

The boss is NONEXACT: 729 overlapping bytes differ and the direct CFGs
disagree. Matching the physical extent, all relocation fields and all calls
does not make it exact. CheckGrazeCollision at 0x0041E350 remains exact at
204/204 bytes under its focused comparison.

Current source-shape facts:

- The entry matches retail through the aligned loop header: 0x34 stack frame,
  EBX Bullet argument, ESI active-region cursor, [ebp-4] slot home, short
  jump, seven-byte NOP, and first mov ecx,[esi] / test ecx,ecx.
- CollisionScratch overlays geometry vectors, reward effect/velocity vectors
  and CursorHome. CursorHome reserves 48 bytes before the slot pointer.
  The volatile scratchHome view is used for the initial and final slot-home
  accesses; intermediate cursor refreshes use the union member directly.
- Rotated-rectangle delta uses the direct position parameter. AABB temporaries
  now occupy target homes [ebp-24h], [ebp-34h] and [ebp-30h], and the
  x-maximum x87 add uses the target operand order.
- Remaining differences include an extra six-byte lea ebx,[ebx] before
  region-type dispatch, a bottom-of-loop next-slot null test, x87
  comparison/branch scheduling, AABB comparison register/branch layout,
  reward-effect payload register order and final hit-count exit layout.
- positionArgument and bulletParameter remain aliases used by geometry and
  reward paths. collisionSize is an unused ABI stack argument.
- The existing byte-emission shim is a narrow loop-entry alignment workaround,
  not a general recommendation or exactness proof. Do not add more assembly
  or padding to force a match.

Focused reproduction (rebuilds only this translation unit):

    mkdir -p build/matching
    scripts/compile-probe.sh src/PlayerCollisionHelpers.cpp       build/matching/PlayerCollisionHelpers.obj       /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /O2 /Ob1 /Oy- /I src
    python3 -B scripts/inspect-player-bullet-collision.py       build/matching/PlayerCollisionHelpers.obj --json
    python3 -B scripts/compare-coff-function.py       --unit player-check-graze-collision --json

The boss inspector is diagnostic only. The graze comparison protects the
exact leaf. After further source/shared-input edits, rebuild every affected
physical owner and exact sibling. No full cold replay was run for this
checkpoint; require a complete accepted Oracle receipt before any promotion.

## Storage cleanup performed

The ignored build directory (74 MB of probe objects/PDBs) and Python bytecode
cache were removed. Exact duplicate Factory IDA outputs and superseded
Ghidra export directories were also removed after hash/content comparison.
The original target, Wine prefix/toolchain, factory tools, saved Ghidra
database, reference source and useful private proof scripts were preserved.
build/ and __pycache__ may be absent; recreate only the selected object.

## 2026-11-13 GPT-web session update

- `EnemyManagerCore.inl`: `NativeEnemyDrawHead` now takes `unsigned char` for the native byte-sized draw-group selector. The full `--owner enemy` check retained all 77 exact sibling proofs. The remaining 13-byte `EnemyManagerView::OnUpdate` frontier is still grouped around draw-group, side/threshold, and descriptor-copy register allocation; see the matching session note in `KNOWLEDGE_BASE.md`.
- `CheckBulletCollision` received another target/IDA/Ghidra pass and multiple compile-probe variants. The direct/volatile pointer experiments did not improve whole-function exactness; preserve the recorded measurements and pursue slot-home lifetime/aliasing rather than repeating them.
- `SaveReplay` and `RunEcl` were reviewed but not modified because their known residuals already have broad, documented failed-variant coverage.
- No full cold replay was run. Continue with focused owner checks and make a local commit after each genuinely improved source or evidence checkpoint.

## 2026-11-13 continued focused session update

- `CheckBulletCollision` now has a precise remaining CFG hypothesis: retail tests the current slot at loop entry and unconditionally advances/jumps; the candidate has a bottom-of-loop next-slot null test. Removing `register` hints did nothing; volatile slot reads and region-type switch/range rewrites regressed the complete 862-byte owner. Start from the documented original/candidate disassembly, not another volatile alias.
- `EnemyManager::OnUpdate` remains 3,900 bytes/97 fields/69 calls/170 blocks with 13 residual bytes. The original 28-byte Enemy bullet descriptor constructor was verified, but alternate stack descriptor types, explicit field writes, speed-order changes, and draw/side pointer aliases did not improve the full owner.
- `BeginPlaybackStage` remains 538/540 bytes with 39 differences. The two missing bytes are in the original `xor edx,edx; mov dx,[eax+4]` seed argument schedule; zeroed locals, memcpy, union and RNG receiver variants did not help.
- `SaveReplay` remains limited to +0xB7/+0xBB; byte-buffer pointer variants did not help. `EffectManager::AddedCallback` remains at its existing eight-byte frontier.
- Small leaf review identified `0x00412780` as an external x87 `fsincos` helper and kept several constructor/folded aliases unclassified rather than inventing authored ownership.
- No full cold replay was run. Continue with focused owners and commit only a real source/evidence advance.


## 2026-11-13 GPT-web constructor-chain checkpoint

- Added six exact Enemy/ECL constructor match units to `config/match-units.toml`, `config/functions.csv`, and `config/matches.csv`: `enemy-ecl-manager-constructor`, `enemy-ecl-timeline-state-constructor`, `enemy-schedule-runtime-constructor`, `enemy-ecl-interpolation-slot-constructor`, `enemy-ecl-context-constructor`, and `enemy-view-constructor`.
- Each focused unit was rebuilt and compared; all 6/6 have zero differing bytes and all 28 relocation fields bind to reviewed target addresses. Original explicit-vs-implicit special-member origin remains unknown, so the six functions remain unclassified with unknown/review origin and are intentionally absent from `matches.csv`/`reccmp-functions.csv`; do not upgrade unique-owner or authored exact claims.
- This work uses only `EnemyManagerLifecycle.cpp`'s focused `/O2 /Ob0` object. No full cold replay was run. Continue with the 13-byte `EnemyManagerView::OnUpdate` frontier and the larger `CheckBulletCollision` boss.

## 2026-11-13 GPT-web collision loop follow-up

- New `CheckBulletCollision` `for`/shared-tail probes reach 33 CFG blocks, matching the target count, but remain 876-879 bytes with 14-17 extra bytes and 785-797 overlapping differences. They still generate a bottom-of-loop next-slot null test rather than retail's unconditional `add esi,4; jmp header`. The existing entry alignment shim is still required; removing it drops the CFG to 30 blocks.
- Volatile slot-element reads do not remove the bottom test; a combined volatile pointer/element variant grows to 992 bytes and fails the original-bound extent guard. See the detailed experiment table in `KNOWLEDGE_BASE.md` under “collision loop CFG continuation probes”. No candidate was retained.
- Repeated `EnemyManagerView::OnUpdate` draw-index, side receiver, threshold alias and descriptor-position view variants all retained the same 13-byte residual. A temporary `Float3` position-member experiment was fully restored and also retained all 13. Continue from the exact register windows already documented; do not repeat these aliases.

## 2026-11-13 GPT-web AddedCallback register follow-up

- `EffectManager::AddedCallback` remains at 8 bytes: target saves `g_AnmManager` directly to EDI then context ECX to ESI; candidate saves context to ESI then loads global through ECX and moves it to EDI. Fallback, owner store, all 4 fields, calls and CFG agree.
- New `register`, const, pointer/reference, volatile-pointee, aggregate-layout/order, comma-expression and local/global `GetAnm` variants did not improve the baseline; some regress to 11 bytes. Keep the current source. Detailed byte-level evidence is in `KNOWLEDGE_BASE.md` under “EffectManager AddedCallback receiver/register follow-up”.

## 2026-11-13 GPT-web Bullet/Laser constructor profile

- Added three canonical `/O2 /Ob0` match units for `BulletTypeSprites`, `Bullet`, and `Laser` constructors. Focused comparisons pass 56/56 (5 relocs), 58/58 (5 relocs), and 34/34 (3 relocs). The complete-owner inspector also passes implicit `BulletExState` 12/12 and all four relocation inventories.
- `/Ob1` is not the original Bullet ctor profile: it expands the seven 0x2C state elements into a loop (67 bytes); `/Ob0` emits the target's 0x00401470 EH vector helper call (58 bytes). No manual helper call was added.
- Keep 0x00414FA0/0x00414FE0/0x00415020 `unclassified` and origin `unknown`: the implicit-vs-authored special-member origin remains unresolved, and `BulletExState` shares the 0x41EE20 timer-prefix constructor body with PlayerState. No exactness credit is claimed. See KNOWLEDGE_BASE “Bullet/Laser constructor `/Ob0` closure”.

## 2026-11-13 GPT-web OnUpdate register follow-up

- New local declaration order, local scope, function-scope pointer/value, side-state alias/type, flags qualifier, draw-index return helper and direct-index variants do not move the 13 residual offsets in `EnemyManagerView::OnUpdate @ 0x410730`.
- The accepted candidates all retain `+0x19C,+0x1A3,+0x252,+0x263,+0x26D,+0x272,+0x294,+0x29A,+0x29C,+0x2A5,+0x2A9,+0x2B7,+0x2BD`. A broad distinct-side-type variant changes owner extent/relocations; an inline-assembly append experiment expands the owner to 5,832 bytes/111 relocations. Neither is retained.
- `/Ob0` is not the EnemyManager boss profile; continue with `/O2 /Ob1 /Oy-`. Full experiment notes are in `KNOWLEDGE_BASE.md` under “EnemyManager::OnUpdate register-allocation probes”.

## 2026-11-13 GPT-web PlayerMovement eight-byte frontier

- `PlayerLifecycleView::UpdateMovementAndOptions` (`0x41C170`) improved from 122 to **8** original-bound physical differences. The fresh 1900-byte diagnostic retains 66 fields, 23 ordered calls and 489 instructions; only `0x127-0x12E` remain different.
- Use direct `player->movementFields1CCC` member accesses for angle, speed multipliers and velocity members, but retain the local `PlayerMovementFields &runtime` alias solely for `runtime.velocity1CCC.y` at the vertical position update. Removing that one alias changes X87 operand order and creates eight new differences.
- The eight residual bytes are the load order of `g_GameConfiguration` and `player->sideIndex` before the focus-mode test. No exactness/origin claim; `functions.csv` remains unclassified. The guard in `scripts/check-boss-frontiers.py` now protects the 114 newly converged positions and the final eight-byte residual. See KNOWLEDGE_BASE “PlayerMovement boss reduced to an eight-byte frontier”.


## 2026-11-13 GPT-web Player collision-query constructor leaf

- Reconstructed the small constructor chain around `PlayerCollisionQueryStateView`. Target `0x0041CEB0` is 28 bytes: it calls the native vector-constructor helper `0x00401470` for 128 elements of stride `0x30`, beginning at `this+4`, with element callback `0x004343D0`.
- `PlayerCollisionQueryStateView` now keeps its default constructor implicit. The out-of-line `PlayerCollisionQueryRecordView` constructor and `PlayerConstructedVector3View` constructor live in `VectorPointerViews.cpp`; both compile to `8B C1 C3`, matching the shared three-byte target body. The state constructor's COFF relocations are `DIR32` to the record constructor and `REL32` to `??_H@YGXPAXIHP6EPAX0@Z@Z`; after binding them to `0x004343D0` and `0x00401470`, the 28-byte sequence matches the target.
- The explicit-vs-implicit origin of the retail state constructor is still not provable from bytes. The implicit source shape is retained because it reproduces the observed helper call; no `functions.csv`/`matches.csv` promotion or authored exactness claim was made.
- Focused exact siblings passed: `player-constructor` 243/243, `player-on-update` 522/522, `player-update-stage-c` 74/74, and `player-calc-item-box-collision` 121/121. No full cold replay was run.
- The `PlayerMovement` boss remains nonexact at eight bytes (`+0x127-0x12E`). Direct array indexing, config/side locals, inline config accessor, materialized focus byte, `this->sideIndex`, branch inversion and raw config-byte indexing did not improve it; reusing the earlier input-side local regressed badly. Avoid repeating those forms without a new lifetime/visibility hypothesis.
- Preserve the 30 pre-existing untracked `_CL_*` compiler scratch files; no cleanup or reset was performed.
