# TH09 reconstruction — current agent handoff (2026-11-18)

This is the restart guide for the live TH09 repository. It records current
status and the next useful work, not a session diary. Read
[AGENTS.md](../AGENTS.md) first, then this file. Use
[PROGRESS.md](PROGRESS.md) for generated totals and
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) for historical target/compiler findings.
The canonical ledgers and current source always outrank dated prose.

## First five minutes

```bash
git status --short --branch
git log -8 --oneline
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/report-reconstruction-status.py
python3 -B scripts/ci.py
```

Recover dirty/untracked work before editing. Do not assume a clean worktree or
silently discard another agent's changes. For source work, inspect the exact
owner and shared consumers, change natural C/C++, then run only the affected
complete frontier/exact cohort. Do not run a full cold replay for routine work.
Commit coherent advances as `gpt-web: <English summary>`; a commit is a local
checkpoint, not proof of exactness.

## Target and phase

- **Only target:** original Japanese TH09 v1.50a, ignored
  `resources/th09.exe`, 685,056 bytes, SHA-256
  `10350095bcf95edb59e03bee9849a2dc8a7714b4927ad5909c569c550fce6822`,
  MD5 `cf634df46e05552e104fa97a971aaac0`, PE32 i386, image base
  `0x00400000`, entry `0x0047D45F`.
- **Current ledger:** 2,248 function candidates; boundary/origin review
  pending 0; 35 reviewed but origin-unresolved; 1,037 confirmed authored
  functions / 291,931 authored bytes; 1,026 exact / 278,578 exact bytes;
  11 source-present nonexact owners / 13,353 bytes. Exact share of reviewed
  authored bytes is 95.4260%. These are authored-byte statistics, not a
  whole-image exactness percentage. The complete candidate inventory remains
  unproven.
- **Phase:** exact reconstruction. `config/build.toml` is still a skeleton;
  whole Windows i386 build/link and runtime validation are open. Semantic
  dual-Oracle work and portability have not started.

Regenerate status from the ledgers with
`python3 -B scripts/report-reconstruction-status.py`; do not hand-edit the
counts in `PROGRESS.md`.

## Current nonexact owner queue

These are the eleven authored source-present nonexact owners in
`config/frontiers.toml`. Counts below are relocation-masked differing byte
positions, not differing instructions. The frontier file is authoritative
for exact residual offsets, profiles, sizes and target hashes.

| Owner | Target address | Current comparison | Remaining work |
| --- | --- | --- | --- |
| `SupervisorServiceUpdate` (`SupervisorNetwork.cpp`) | 0x00430AA0 | 1,629 / 1,633 bytes; 966 residual positions | Large host/network state-machine CFG, call order, stack homes and receiver lifetimes |
| `CheckBulletCollision` (`PlayerCollisionHelpers.cpp`) | 0x0041DFF0 | 842 / 862 bytes; 705 residual positions | Three nested retry levels, geometry/hit dispatch, cursor lifetime and native block order |
| `UpdateChargeAttack` (`PlayerChargeAttackUpdate.cpp`) | 0x0041F810 | 1,210 / 1,210 bytes; 183 residual positions | Native 32-bit side-index vs current 16-bit local; EDI/EBX input-pointer lifetime |
| `OnUpdateResult` (`TitleScreen.cpp`) | 0x004266B5 | 484 / 485 bytes; 167 residual positions | Result-state branches and selected-character load/call/store lifetime |
| `BeginPlaybackStage` (`ReplayInput.cpp`) | 0x00420840 | 540 / 540 bytes; 13 residual positions | Replay byte store, value load and side-0 runtime-pointer read order |
| `CalcShakeEnvelope` (`ScreenEffect.cpp`) | 0x004227A0 | 336 / 336 bytes; 26 residual positions | Receiver/timer register assignment and X87 value lifetimes |
| `DrawReplayMenu` (`TitleScreenReplayMenuDraw.cpp`) | 0x004234F6 | 1,280 / 1,280 bytes; 29 residual positions | Replay-record/menu-selection register and stack-home allocation |
| `UpdateMovementAndOptions` (`PlayerMovement.cpp`) | 0x0041C170 | 1,900 / 1,900 bytes; 4 residual positions | Config-pointer / live side-index load order at `+0x127..+0x12A` |
| `AddedCallback` (`EffectManager.cpp`) | 0x0040D1C0 | 51 / 51 bytes; 4 residual positions | `g_AnmManager` EDI load vs callback receiver ESI setup |
| `SaveReplay` (`ReplayInput.cpp`) | 0x00420C60 | 1,238 / 1,238 bytes; 2 residual positions | Header-copy source/destination register at `+0xB7/+0xBB` |
| `OnUpdate` (`EclManager.cpp`) | 0x00410730 | 3,900 / 3,900 bytes; 2 residual positions | Draw-group index uses ECX instead of retail EAX; residual bytes `+0x19C/+0x1A3` |

**Separate unclassified frontier:** `fsincos` in `EtamaControllerDraw.cpp`,
target `0x00412780`, candidate 36 / target 33 bytes, 25 differing positions.
It is not one of the eleven authored nonexact functions. Its origin remains
unclassified. Keep the maintained natural `cosf`/`sinf` C++; do not restore
inline assembly or an old exact claim.

## Latest source facts to preserve

- **Supervisor network PopFrame scratch:** commits `1c4cb7e7` and
  `bd4b8b07` recover the eight-byte native output window as
  `NetworkPopFrameScratch`. The prefill at `0x00430E66`/`0x00430E77` writes
  two seeds at frame offsets 0/2 and prediction at 4; the final frame at
  `0x0043108B`/`0x004310AC` reverses this to prediction at 0 and seeds at 4/6.
  All three typed union views have compile-time offset assertions. Commit
  `719acb53` also recovers the final frame's two consecutive 16-bit input
  results as a typed `NetworkFrameInputPair` with offsets 0/2 and size 4;
  the seed/prediction output pointers remain independent scalars pending
  VC7.1 lifetime recovery. Commit `32e1524f` additionally makes the
  target-backed connection phases 0/1/2/3, frame phases 0/1/2, and message
  kinds 0/2 explicit C++ enums; original identifier spellings are unknown.
  Commit `b79c9cf2` also names target-observed network timing thresholds
  (100-ms host poll, 4000-ms jitter, 4000/6000-ms retry bases, 20/60-second
  frame deadlines), 60-Hz frame conversion and -9998/+9998 prefill origin.
  These are native compile-time values, not new retry or safety behavior.
  The complete frontier remains 1,629/1,633 bytes, 966 residual positions;
  a shared function-scope output-object probe changed the local-frame size
  without yielding a ratchetable improvement. Common epilogue and global
  receiver-reload experiments also regressed; do not promote them on partial
  instruction similarity.
- **Replay capture/save native semantics:** commit `fb66d164` restores
  native 32-bit wrapping frame-counter increments in `RecordInputAndFps`
  without signed-overflow assumptions. Commit `9ad8f335` uses a complete
  typed `ReplayFrameDataStartView` aggregate assignment in `SaveReplay`,
  naturally emitting the retail eight-DWORD REP MOVSD. A focused replay of
  `ReplayInput.cpp` still proves 12 exact units over 4,093 physical bytes
  and 232 relocation fields. The nonexact `SaveReplay` and
  `BeginPlaybackStage` remain 1,238/1,238 with 2 residual positions and
  540/540 with 13, respectively. Neither commit claims an additional exact
  function.
- **Effect ANM publication:** commit `2ebbf0cd` keeps the natural
  `ResolveEffectAnm(AnmManager *, AnmLoaded *&)` helper. It resolves the
  resident ANM, optionally preloads it, then publishes the final handle to
  `effectAnm`. This preserves the delayed member-write boundary; a direct
  member-reference store before the preload was an incorrect earlier probe.
  The complete 51-byte callback still has four residual positions; this is
  not an exactness claim.
- **Enemy draw heads:** commit `09660746` removed the one-use synthetic
  draw-head helper. Both append sites use typed pointer arithmetic on the
  real four-entry `drawGroupHeads2AC410` array. The 3,900-byte Enemy manager
  boss remains at two residual bytes; do not reintroduce assembly, byte
  offsets or padding to force EAX allocation.
- **Player collision cursor and retry loop:** commits `4a6e16cb` and
  `8db39334` assert the true 514-entry `activeB100` extent. Commits
  `8613e62f` and `1bc46c9b` replace opaque collision/reward stack prefixes
  with typed 52-byte vector/cursor overlays (reward vectors at offsets 24/36,
  cursor pointer at 48) and spell out the original null-region return.
  Commits `aa841a4e` and `aa59e47b` restore a distinct inner type/delay
  retry and make the shared hit/cancel dispatch explicit in source.
  Commit `f8bd4309` further recovers the circle, rotated-rectangle and AABB
  checks as one natural three-way C++ dispatch feeding that common hit path.
  Commit `a4290c3c` snapshots the hit-region kind once before testing the
  two cancellation categories, matching the target's single kind load.
  VC7.1 still emits the maintained 842/862-byte, 705-difference body.
  Its five exact same-source functions pass a focused replay over 1,220
  physical bytes and 32 relocation fields. Target evidence carries
  Player in EDI, Bullet in EBX, the cursor in ESI, and region in ECX; the
  cursor home precedes the first region dereference. A complete three-nested-
  loop rewrite and a reordered rectangle branch fixed some bytes but also
  introduced new differences. The best angle-branch ordering probe reached
  845/862 bytes and 477 residual positions, but introduced 28 new positions
  (notably `+0xC9..+0xCD`, `+0x11A..+0x11C`, `+0x1B6..+0x1BF`); the original
  target executes rotated-rectangle geometry before AABB geometry and joins
  circle hits at the same dispatcher. Do not promote the diagnostic probe or
  widen the frontier based solely on its lower total mismatch count.
- **Player charge:** commit `ba55d762` names the two observed focus-input
  states: charge from held shot input (0) or held focus input (1), including
  the different held-key checks. The current `short side` and volatile mode
  snapshot are not claimed as final semantics. Widening/removing the side local, direct
  indexing and pointer/reference alternatives regressed the complete owner
  (some to 717+ differing positions). Preserve the 183-position frontier;
  do not ratchet based on a local instruction win.

- **New bounded compiler probes (2026-11-18):** ten `PlayerMovement`
  source-only inline focus-mode accessor variants all reproduced the same
  complete 1,900-byte function and four residual positions. Reversing the
  `ResolveEffectAnm` argument order and changing pointer/reference forms
  did not solve the 51-byte Effect callback's four-register-byte residual.
  A type-safe rewrite of the native Title increment macro and an explicit
  endpoint cast in `ScreenEffect` preserved their named nonexact frontiers,
  but changed compiler-private COFF labels in same-source exact units; both
  experiments were restored, with no ledger or private-label rewriting.
  A local `ShakeDirection` enum in the nonexact envelope also changed the
  original same-source `RegisterChain` private COFF relocation labels;
  the enum experiment was restored despite matching the 336-byte envelope.
  Keep the scoped exact cohorts mandatory for future shared-source edits.

Historical negative results are clues, not proof that every natural source
form is exhausted. Never use ASM, inert padding, fake returns, register hints,
compiler roulette or a widened frontier to manufacture progress.

## Next investigations

1. **Large boss — `SupervisorServiceUpdate`:** continue from the original
   `0x00430AA0` body and current `src/SupervisorNetwork.cpp`. Recover the
   state/host/network branches, native call order, result lifetime and frame
   processing as natural C++. Preserve the corrected prefill scratch layout.
   The current 966-position residual is substantial; the alias correction is
   not a boss solution.
2. **Large boss — `CheckBulletCollision`:** reconstruct the original nested
   filter/geometry/hit retry structure and cursor advances. Keep all affected
   exact Player siblings protected. Do not promote saved diagnostic C++ files
   just because their raw mismatch count is lower.
3. **Small, bounded blockers:** `EnemyManagerView::OnUpdate` (two draw-index
   bytes), `SaveReplay` (two header-copy bytes), then Effect callback,
   ScreenEffect, Replay playback and Title owners. Test complete owners, not
   just isolated expressions.
4. Resolve the 35 origin-unresolved candidates separately. Whole-product
   translation-unit partition, link order, data/resources and runtime closure
   remain later work; exact functions do not imply a runnable executable.

## Focused commands

```bash
# Recheck all eleven authored nonexact owners against their pinned profiles:
python3 -B scripts/check-frontier.py \
  supervisor-network-service player-bullet-collision player-charge-attack \
  title-update-result replay-begin-playback screen-shake-envelope \
  title-draw-replay-menu player-movement effect-added-callback \
  replay-save enemy-on-update --show
python3 -B scripts/check-frontier.py etama-fsincos --show

# Inspect exact consumers before changing shared source/header inputs:
python3 -B scripts/replay-scoped-exact.py --changed-file src/EnemyManagerCore.inl --dry-run
python3 -B scripts/replay-scoped-exact.py --source src/PlayerCollisionHelpers.cpp
python3 -B scripts/replay-scoped-exact.py --source src/EffectManager.cpp

# Target, tracking, docs and fast repository checks:
python3 -B scripts/verify-target.py
python3 -B scripts/validate-tracking.py --require-target
python3 -B scripts/validate-docs.py
python3 -B scripts/ci.py

git diff --check
```

A nonexact frontier pass means only that its recorded monotone baseline was
not worsened; it is not exact credit. Exact units require complete physical
extent and relocation proof with the pinned VC7.1 profile. For shared
`.hpp`/`.inl` edits, inspect the actual affected source dependents before
choosing scoped replay. Do not run a full cold replay for routine iteration.

## Private inputs and storage hygiene

Preserve these inputs; they are not disposable build output:

- `resources/th09.exe` (the original target; never patch, delete or commit it).
- `.tools/` (shared Ghidra/JDK/MSVC selectors and this game's Wine prefix).
- `_reference/` (reference checkout).
- `.analysis/ghidra-direct/243bffea8dcbe99a/` (saved Ghidra project) and
  `.analysis/factory-runtime/` (runtime lock state).
- `.analysis/c2-heap.bin`, `FUN_107844a4.decomp.txt`, the four
  `gdb-c2-*.cmd` recipes and the three small natural-C++ diagnostics retained
  under `.analysis/`.
- `c2-analysis/c2.dll`, `c2-analysis/project/`,
  `c2-analysis/allocator-project/`, `c2-analysis/scripts/` and the retained
  top-level C2 register/allocator/codegen reports. These support the still
  unresolved Enemy/Supervisor compiler-allocation questions.

`build/`, generated OBJ/PDB files, the 294 old `c2-analysis/probe*`
experiment project directories, duplicate C2 reports, old `.analysis/*.json`
replay snapshots, temporary IDA call-result captures and Ghidra export
snapshots are disposable and have been pruned. Recreate only the selected
compiler carrier when needed. Missing `build/frontier/*.obj` or
`build/matching/*.obj` after cleanup is expected. Do not run `git clean -fdx`:
it would also target protected ignored inputs. Never delete `.tools/`,
`resources/th09.exe`, `_reference/` or the saved analysis project while
cleaning caches.
