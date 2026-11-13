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
| **RunEcl** | 0x004086C0 | 15,564/15,564 physical bytes; 598 fields; **30 unlike** | Opcode 4 timer registers (7), opcode 155 timeout flag (23). Owned switch tables intact. `--owner runecl`; `src/EclManager.cpp` with `EclRun*.inl`. |
| **Enemy OnUpdate** | 0x00410730 | 3,900/3,900; 97 fields; 69 calls; **13 unlike** | Draw-group / bullet-descriptor register lifetimes. `--owner enemy`, `src/EnemyManagerCore.inl` included by EclManager. |
| **Player movement** | 0x0041C170 | 1,900/1,900; 66 fields; 23 calls; **122 unlike** | Option/history, input and Float3 scheduling. `--owner movement`, `src/PlayerMovement.cpp`. |
| **Player ChargeAttack** | 0x0041F810 | 1,210/1,210; 70 fields; 58 calls; **235 unlike** | Mode byte / input receiver scheduling; 53-block CFG retained. `--owner charge`, `src/PlayerChargeAttackUpdate.cpp`. |
| **Title OnUpdateResult** | 0x004266B5 | 484 candidate / 485 original; 34 fields; 9 calls; **218 unlike** and 1 missing | Character byte load/sign extension. Dedicated `inspect-title-update-result.py` after building `title-screen-setup-thread`; **not** `--owner resultdraw`. |
| **Title DrawReplayMenu** | 0x004234F6 | 1,280/1,280; 57 fields; 13 calls; **29 unlike** | Selected-record EDX/ECX lifetime. `--owner replaymenu`, `src/TitleScreenReplayMenuDraw.cpp`. |
| **Player CheckBulletCollision** | 0x0041DFF0 | 862/862 physical bytes; 275/277 instructions; 34/33 CFG blocks; 26/26 fields; 12/12 calls; **729 unlike**, 0 missing/extra | Still NONEXACT; loop continuation, x87/AABB branches, reward register order and final hit-count exits remain. `--owner player`, `src/PlayerCollisionHelpers.cpp`. |
| **Replay SaveReplay** | 0x00420C60 | 1,238/1,238; 53 fields; 23 calls; **2 unlike** | Header-copy EDI/ECX register encoding at +0xB7/+0xBB. `--owner replaysave`, `src/ReplayInput.cpp`. |
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
2. Pick **one nonexact boss** from the table. SaveReplay has only two
   ordinary register bytes left; RunEcl and Enemy OnUpdate have larger
   exact-sibling impact. Build its carrier and inspect its **whole physical
   function**; a `--skip-exact-siblings` diagnostic alone does not certify
   its existing exact neighbors after an edit.
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
